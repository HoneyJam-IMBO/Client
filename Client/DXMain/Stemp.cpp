#include "stdafx.h"
#include "Stemp.h"
#include "StempManager.h"

void CStemp::Begin(){
	m_nIndex = m_pStempManager->GetStemps().size();
}

bool CStemp::End(){
	m_pStempTexture->End();
	m_pStempTexture = nullptr;

	return true;
}

void CStemp::SetShaderState(){
	RENDERER->GetTerrainRenderContainer()->AddVolatileTexture(m_pStempTexture);
}

void CStemp::CleanShaderState(){
}

void CStemp::SetStempData(Pixel24* pStempData){
	m_pStempData = pStempData;
}

void CStemp::SetStempData(UINT index, byte data){

}

void CStemp::IncreaseTerrain(Pixel24* pHeightData, XMFLOAT2 pickPos, float fRadius){
	int x = (1 - pickPos.x) * (STEMP_WIDTH - 1);
	int y = (1 - pickPos.y) * (STEMP_HEIGHT - 1);

	int r = (fRadius * STEMP_WIDTH);
	//ZeroMemory(m_pHeightData, sizeof(Pixel) * (m_nWidth - 1) * (m_nLength - 1));
	for (int i = -r; i < r; ++i) {
		for (int j = -r; j < r; ++j) {
			int index = ((x + i) + (y + j)*STEMP_HEIGHT);
			if (index < 0) continue;
			if (index > (STEMP_WIDTH * STEMP_HEIGHT)) continue;

			int is = i + r;//0~ 2*r
			int js = j + r;
			int scale = (int)(STEMP_WIDTH/2) / r;//0-r 을 256size로 변환 시킬 scale
			int stempIndex = (is*scale) + (js*scale) * STEMP_HEIGHT;
			//m_pHeightData[index].r = m_pHeightData[index].r + 1;
			//m_pHeightData[index].g = m_pHeightData[index].g + 1;
			if ((pHeightData[index].r + m_pStempData[stempIndex].r) > 255) continue;
			pHeightData[index].r = pHeightData[index].r + m_pStempData[stempIndex].r;
		}
	}
}

void CStemp::DecreaseTerrain(Pixel24* pHeightData, XMFLOAT2 pickPos, float fRadius){
	int x = (1 - pickPos.x) * (STEMP_WIDTH - 1);
	int y = (1 - pickPos.y) * (STEMP_HEIGHT - 1);

	int r = (fRadius * STEMP_WIDTH);
	//ZeroMemory(m_pHeightData, sizeof(Pixel) * (m_nWidth - 1) * (m_nLength - 1));
	for (int i = -r; i < r; ++i) {
		for (int j = -r; j < r; ++j) {
			int index = ((x + i) + (y + j)*STEMP_HEIGHT);
			if (index < 0) continue;
			if (index >(STEMP_WIDTH * STEMP_HEIGHT)) continue;

			int is = i + r;//0~ 2*r
			int js = j + r;
			int scale = (int)(STEMP_WIDTH / 2) / r;//0-r 을 256size로 변환 시킬 scale
			int stempIndex = (is*scale) + (js*scale) * STEMP_HEIGHT;
			//m_pHeightData[index].r = m_pHeightData[index].r + 1;
			//m_pHeightData[index].g = m_pHeightData[index].g + 1;
			if ((pHeightData[index].r + m_pStempData[stempIndex].r) <= 0) continue;
			pHeightData[index].r = pHeightData[index].r - m_pStempData[stempIndex].r;
		}
	}
}

void CStemp::SetTerrain(Pixel24* pHeightData, XMFLOAT2 pickPos, float fRadius){
	int x = (1 - pickPos.x) * (STEMP_WIDTH - 1);
	int y = (1 - pickPos.y) * (STEMP_HEIGHT - 1);

	int r = (fRadius * STEMP_WIDTH);
	//ZeroMemory(m_pHeightData, sizeof(Pixel) * (m_nWidth - 1) * (m_nLength - 1));
	for (int i = -r; i < r; ++i) {
		for (int j = -r; j < r; ++j) {
			int index = ((x + i) + (y + j)*STEMP_HEIGHT);
			if (index < 0) continue;
			if (index >(STEMP_WIDTH * STEMP_HEIGHT)) continue;

			int is = i + r;//0~ 2*r
			int js = j + r;
			int scale = (int)(STEMP_WIDTH / 2) / r;//0-r 을 256size로 변환 시킬 scale
			int stempIndex = (is*scale) + (js*scale) * STEMP_HEIGHT;
			//m_pHeightData[index].r = m_pHeightData[index].r + 1;
			//m_pHeightData[index].g = m_pHeightData[index].g + 1;
			pHeightData[index].r = m_pStempData[stempIndex].r;
		}
	}
}

CStemp * CStemp::CreateStemp(wstring name, CStempManager* pStempManager){
	CStemp* pStemp = new CStemp;
	pStemp->SetStempManager(pStempManager);
	pStemp->SetStempTexture(CTexture::CreateTexture(name, 3, BIND_PS));
	
	Pixel24* pStempData = IMPORTER->ReadBitmap24(name.c_str());
	
	pStemp->SetStempData(pStempData);
	pStemp->Begin();
	return pStemp;
}

CStemp::CStemp() : DXObject("stemp"){
}

CStemp::~CStemp()
{
}
