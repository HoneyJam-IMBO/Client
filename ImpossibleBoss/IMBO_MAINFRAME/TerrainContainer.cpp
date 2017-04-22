#include "stdafx.h"
#include "TerrainContainer.h"
#include "SpaceContainer.h"


#define TEXTURE_SIZE 257

void CTerrainContainer::Begin() {

	//terrain mesh create
	CreateTerrainMesh(m_pSpaceContainer);
	//terrain mesh create

	m_pSplattingInfoManager = CSplattingInfoManager::CreateSplattingInfoManager(this);
	//m_pSplattingInfoManager->CreateSplattingInfo(L"../slide.bmp", L"../../Assets/Detail_Texture_9.jpg");

	//global buffer 제작
	m_pGlobalTerrainBuffer = RESOURCEMGR->GetBuffer("TerrainCB");
	
	//global buf set
	m_pGlobalTerrainData = new TERRAIN_GLOBAL_VALUE;
	float fOneSpaceSize = (float)m_pSpaceContainer->GetOneSpaceSize();
	float fOneSideSpaceNum = (float)m_pSpaceContainer->GetOneSideSpaceNum();
	m_pGlobalTerrainData->OneSideSpaceNum = fOneSideSpaceNum;
	m_pGlobalTerrainData->OneSideSpaceNumRcp = 1.0f / fOneSideSpaceNum;
	m_pGlobalTerrainData->OneSpaceSizeRcp = 1.0f / fOneSpaceSize;
	m_pGlobalTerrainData->HeightScale = m_xmf3Scale.y;
	//global buf set

	//터레인 제작
	//te`rrain
	CTerrain* pTerrain = nullptr;
	for (UINT j = 0; j < m_pSpaceContainer->GetOneSideSpaceNum(); ++j) {
		for (UINT i = 0; i < m_pSpaceContainer->GetOneSideSpaceNum(); ++i) {
			pTerrain = CTerrain::CreateTerrain(this, i, j);
			m_vpTerrain.push_back(pTerrain);
		}
	}
	//terrain

	
	//RS
	D3D11_RASTERIZER_DESC descRasterizer;
	ZeroMemory(&descRasterizer, sizeof(D3D11_RASTERIZER_DESC));
	//descRasterizer.FillMode = D3D11_FILL_SOLID;
	descRasterizer.FillMode = D3D11_FILL_WIREFRAME;
	descRasterizer.CullMode = D3D11_CULL_NONE;
	GLOBALVALUEMGR->GetDevice()->CreateRasterizerState(&descRasterizer, &m_pd3dSpaceRSState);
	//RS

	//추가
	m_pTessFacterBuffer = RESOURCEMGR->CreateConstantBuffer("TerrainTessFacter", 1, sizeof(TERRAIN_TESS_FACTER), 1, BIND_HS);
	m_pTessFacterData = new TERRAIN_TESS_FACTER;

}

bool CTerrainContainer::End() {
	//TWBARMGR->DeleteBar("TerrainController");
	//TWBARMGR->DeleteVar("TOOL_MODE", "SetStemp");
	//TWBARMGR->DeleteVar("TOOL_MODE", "IndeStemp");
	//TWBARMGR->DeleteVar("TOOL_MODE", "TerrainHeightScale");

	if (m_pHeightData) delete[] m_pHeightData;
	m_pHeightData = nullptr;
	if (m_pNormalData) delete[] m_pNormalData;
	m_pNormalData = nullptr;
	if (m_pBaseTexture) m_pBaseTexture->End();
	m_pBaseTexture = nullptr;
	if (m_pHeightMapTexture) m_pHeightMapTexture->End();
	m_pHeightMapTexture = nullptr;
	if (m_pNormalTexture) m_pNormalTexture->End();
	m_pNormalTexture = nullptr;
	
	if (m_pGlobalTerrainBuffer) m_pGlobalTerrainBuffer = nullptr;
	if (m_pGlobalTerrainData) delete m_pGlobalTerrainData;
	m_pGlobalTerrainData = nullptr;
	
	if (m_pSplattingInfoManager) {
		m_pSplattingInfoManager->End();
		delete m_pSplattingInfoManager;
		m_pSplattingInfoManager = nullptr;
	}

	//terrain object remove
	for (auto pTerrain : m_vpTerrain) {
		m_pSpaceContainer->RemoveObject(pTerrain);
	}
	m_vpTerrain.clear();

	if(m_pd3dSpaceRSState)m_pd3dSpaceRSState->Release();
	if(m_pd3dTempRSState)m_pd3dTempRSState->Release();

	//추가
	m_pTessFacterBuffer = nullptr;

	return true;
}

void CTerrainContainer::PrepareRender(){
	if (m_bActive) {
		RENDERER->GetTerrainRenderContainer()->ClearVolatileResources();

		SetBufferInfo();
		RENDERER->GetTerrainRenderContainer()->AddVolatileTexture(m_pBaseTexture);
		RENDERER->GetTerrainRenderContainer()->AddVolatileTexture(m_pHeightMapTexture);
		RENDERER->GetTerrainRenderContainer()->AddVolatileTexture(m_pNormalTexture);
		RENDERER->GetTerrainRenderContainer()->AddVolatileBuffer(m_pGlobalTerrainBuffer);
		//추가
		RENDERER->GetTerrainRenderContainer()->AddVolatileBuffer(m_pTessFacterBuffer);

		m_pSplattingInfoManager->SetShaderState();//splatting
	}
}

float CTerrainContainer::GetHeight(XMVECTOR xmvPos){
	XMFLOAT3 xmf3Pos;
	XMStoreFloat3(&xmf3Pos, xmvPos);
	float fx = xmf3Pos.x;
	float fz = xmf3Pos.z;

	return GetHeight(XMFLOAT2(fx, fz));
}

float CTerrainContainer::GetHeight(XMFLOAT2 xmf2Pos){
	float fx = m_pSpaceContainer->GetSpaceSize() - xmf2Pos.x;
	float fz = m_pSpaceContainer->GetSpaceSize() - xmf2Pos.y;

	fx = fx / m_xmf3Scale.x;
	fz = fz / m_xmf3Scale.z;
	if ((fx < 0.0f) || (fz < 0.0f) || (fx >= m_nWidth) || (fz >= m_nLength)) return(0.0f);

	int x = (int)fx;
	int z = (int)fz;
	float fxPercent = fx - x;
	float fzPercent = fz - z;

	//float fBottomLeft = (float)m_pHeightMapImage[x + (z*m_nWidth)];
	//float fBottomRight = (float)m_pHeightMapImage[(x + 1) + (z*m_nWidth)];
	//float fTopLeft = (float)m_pHeightMapImage[x + ((z + 1)*m_nWidth)];
	//float fTopRight = (float)m_pHeightMapImage[(x + 1) + ((z + 1)*m_nWidth)];
	float fBottomLeft = (float)m_pHeightData[x + (z*m_nWidth)].r;
	float fBottomRight = (float)m_pHeightData[(x + 1) + (z*m_nWidth)].r;
	float fTopLeft = (float)m_pHeightData[x + ((z + 1)*m_nWidth)].r;
	float fTopRight = (float)m_pHeightData[(x + 1) + ((z + 1)*m_nWidth)].r;

	if (fzPercent < (1.0f - fxPercent))
		fTopRight = fTopLeft + (fBottomRight - fBottomLeft);
	else
		fBottomLeft = fTopLeft + (fBottomRight - fTopRight);

#ifdef _WITH_APPROXIMATE_OPPOSITE_CORNER
	if (bReverseQuad)
	{
		if (fzPercent >= fxPercent)
			fBottomRight = fBottomLeft + (fTopRight - fTopLeft);
		else
			fTopLeft = fTopRight + (fBottomLeft - fBottomRight);
	}
	else
	{
		if (fzPercent < (1.0f - fxPercent))
			fTopRight = fTopLeft + (fBottomRight - fBottomLeft);
		else
			fBottomLeft = fTopLeft + (fBottomRight - fTopRight);
	}
#endif
	float fTopHeight = fTopLeft * (1 - fxPercent) + fTopRight * fxPercent;
	float fBottomHeight = fBottomLeft * (1 - fxPercent) + fBottomRight * fxPercent;
	float fHeight = fBottomHeight * (1 - fzPercent) + fTopHeight * fzPercent;

	return(fHeight * m_xmf3Scale.y);
}

void CTerrainContainer::Update(shared_ptr<CCamera> pCamera) {
	//추가
	XMStoreFloat3(&m_pTessFacterData->camera_pos, pCamera->GetPosition());

	if (!pCamera) return;
	if (m_bActive) {
		PrepareRender();
		return;
	}
}

CGameObject * CTerrainContainer::PickObjects(XMVECTOR xmvWorldCameraStartPos, XMVECTOR xmvRayDir, float & distance){
	float fHitDistance = FLT_MAX;
	distance = fHitDistance;
	float fNearHitDistance = FLT_MAX;
	CGameObject* pObj = nullptr;
	//자신의 모든 객체에 대해서 검사
	for (auto pObject : m_vpTerrain) {
		if (pObject->CheckPickObject(xmvWorldCameraStartPos, xmvRayDir, fHitDistance)) {//ray와 충돌했다면
			if (fNearHitDistance > fHitDistance) {//이전의 가장 가까운 녀석과 비교
				distance = fHitDistance;//더 가까우면 가장 가까운 객체 변경
				return pObject;
			}

		}
	}
	//return pObj;
}

void CTerrainContainer::ReadyHeightMap(){
	m_pHeightMapTexture->End();
	m_pHeightMapTexture = nullptr;

	//height map data init
	WCHAR path[256];
	wsprintf(path, L"../%sHeightMap.bmp", m_wsSceneName.c_str());//name
	//EXPORTER->MakeBitmap24(path, m_pHeightData, m_nWidth, m_nLength);
	m_pHeightMapTexture = CTexture::CreateTexture(path, 1, BIND_DS);
}

void CTerrainContainer::SetBufferInfo(){
	TERRAIN_GLOBAL_VALUE* pData = (TERRAIN_GLOBAL_VALUE*)m_pGlobalTerrainBuffer->Map();
	pData->OneSideSpaceNum		= m_pGlobalTerrainData->OneSideSpaceNum;
	pData->OneSideSpaceNumRcp	= m_pGlobalTerrainData->OneSideSpaceNumRcp;
	pData->OneSpaceSizeRcp		= m_pGlobalTerrainData->OneSpaceSizeRcp;
	pData->HeightScale			= m_pGlobalTerrainData->HeightScale;
	m_pGlobalTerrainBuffer->Unmap();

	//추가
	SetTerrainTessFacter(m_pTessFacterData->zn, m_pTessFacterData->zf, m_pTessFacterData->space_lavel_pow, m_pTessFacterData->max_facter_value, m_pTessFacterData->min_facter_value, m_pTessFacterData->camera_pos);
}


void CTerrainContainer::SetBaseTexture(wstring path){
	if (m_pBaseTexture)m_pBaseTexture->End();
	m_pBaseTexture = nullptr;

	m_pBaseTexture = CTexture::CreateTexture(path, 0);
}

void CTerrainContainer::SetHeightMapTexture(wstring path){
	if (m_pHeightData) delete[]m_pHeightData;
	m_pHeightData = nullptr;
	if (m_pHeightMapTexture)m_pHeightMapTexture->End();
	m_pHeightMapTexture = nullptr;


}

void CTerrainContainer::CreateSplattingInfo(){
	//m_pSplattingInfoManager->CreateSplattingInfo(L"../outputdata/SplattingBlendInfo/BlendInfo0.bmp", L"../../Assets/Detail_Texture_9.jpg");
	m_pSplattingInfoManager->CreateSplattingInfo(L"../../Assets/Detail_Texture/Detail_Texture_Default.jpg");
	//m_pSplattingInfoManager->CreateSplattingInfo(L"../outputdata/SplattingBlendInfo/BlendInfo0.bmp", L"../../Assets/default.jpg");
}

void CTerrainContainer::CreateNormalMap(){
	//1. bitmap 좌표계의 height다.
	//2. 그래서 world 좌표계 즉 x z가 반대로 되는 세계로 불러들인다.
	//3. height는 그대로이다. 높이 값은 bitmap 세계에 있으니까 그냥 읽어도 됨
	//4. normal을 계산한다.
	//끝
	m_pNormalTexture->End();
	m_pNormalTexture = nullptr;

	//calculate normal
	XMVECTOR xmvSumNormal = XMVectorSet(0.f,0.f,0.f,0.f);
	UINT nNormal{ 0 };
	for (int i = 0; i < m_nWidth; ++i) {
		for (int j = 0; j < m_nLength; ++j) {
			xmvSumNormal = XMVectorSet(0.f, 0.f, 0.f, 0.f);
			int xIndex = i;
			int yIndex = j;
			if ((xIndex -1) > 0 && (yIndex +1) < 255) {
				XMVECTOR p0 = XMVectorSet(256 - (xIndex), m_pHeightData[(xIndex) + (m_nLength * yIndex)].r * m_xmf3Scale.y, 256 - yIndex, 0.f);
				XMVECTOR p1 = XMVectorSet(256 - (xIndex - 1), m_pHeightData[(xIndex - 1) + (m_nLength * yIndex)].r* m_xmf3Scale.y, 256 - yIndex, 0.f);
				XMVECTOR p2 = XMVectorSet(256 - (xIndex), m_pHeightData[(xIndex)+(m_nLength * (yIndex + 1))].r* m_xmf3Scale.y, 256 - (yIndex + 1), 0.f);
				XMVECTOR edge1 = p1 - p0;
				XMVECTOR edge2 = p2 - p0;
				xmvSumNormal += XMVector3Normalize(XMVector3Cross(edge1, edge2));
				nNormal++;
			}
			if ((xIndex +1) < 255 && (yIndex +1) < 255) {
				XMVECTOR p0 = XMVectorSet(256 - (xIndex), m_pHeightData[(xIndex)+(m_nLength * yIndex)].r* m_xmf3Scale.y, 256 - yIndex, 0.f);
				XMVECTOR p1 = XMVectorSet(256 - (xIndex), m_pHeightData[(xIndex)+(m_nLength * (yIndex + 1))].r* m_xmf3Scale.y, 256 - (yIndex + 1), 0.f);
				XMVECTOR p2 = XMVectorSet(256 - (xIndex + 1), m_pHeightData[(xIndex + 1) + (m_nLength * yIndex)].r* m_xmf3Scale.y, 256 - yIndex, 0.f);
				XMVECTOR edge1 = p1 - p0;
				XMVECTOR edge2 = p2 - p0;
				xmvSumNormal += XMVector3Normalize(XMVector3Cross(edge1, edge2));
				nNormal++;
			}
			if (0 < (xIndex -1) && 0 < (yIndex -1)) {
				XMVECTOR p0 = XMVectorSet(256 - xIndex, m_pHeightData[(xIndex)+(m_nLength * yIndex)].r* m_xmf3Scale.y, 256 - yIndex, 0.f);
				XMVECTOR p1 = XMVectorSet(256 - xIndex, m_pHeightData[(xIndex)+(m_nLength * (yIndex - 1))].r* m_xmf3Scale.y, 256 - (yIndex - 1), 0.f);
				XMVECTOR p2 = XMVectorSet(256 - (xIndex - 1), m_pHeightData[(xIndex - 1) + (m_nLength * yIndex)].r* m_xmf3Scale.y, 256 - yIndex, 0.f);
				XMVECTOR edge1 = p1 - p0;
				XMVECTOR edge2 = p2 - p0;
				xmvSumNormal += XMVector3Normalize(XMVector3Cross(edge1, edge2));
				nNormal++;
			}
			if ((xIndex +1) < 255 && 0 < (yIndex -1)) {
				XMVECTOR p0 = XMVectorSet(256 - xIndex, m_pHeightData[(xIndex)+(m_nLength * yIndex)].r* m_xmf3Scale.y, 256 - yIndex, 0.f);
				XMVECTOR p1 = XMVectorSet(256 - (xIndex + 1), m_pHeightData[(xIndex + 1) + (m_nLength * yIndex)].r* m_xmf3Scale.y, 256 - yIndex, 0.f);
				XMVECTOR p2 = XMVectorSet(256 - xIndex, m_pHeightData[(xIndex)+(m_nLength * (yIndex - 1))].r* m_xmf3Scale.y, 256 - (yIndex - 1), 0.f);
				XMVECTOR edge1 = p1 - p0;
				XMVECTOR edge2 = p2 - p0;
				xmvSumNormal += XMVector3Normalize(XMVector3Cross(edge1, edge2));
				nNormal++;
			}
			XMVECTOR xmvAvgNormal = XMVector3Normalize(xmvSumNormal / nNormal);//-1 1
			xmvAvgNormal += XMVectorSet(1.f,1.f,1.f,0.f);//0 - 2
			xmvAvgNormal /= 2.f;//0-1

			xmvAvgNormal *= 255;//0-255
			XMFLOAT3 xmf3AvgNormal;
			XMStoreFloat3(&xmf3AvgNormal, xmvAvgNormal);
			m_pNormalData[xIndex + (yIndex * 256)].r = xmf3AvgNormal.x;
			m_pNormalData[xIndex + (yIndex * 256)].g = xmf3AvgNormal.y;
			m_pNormalData[xIndex + (yIndex * 256)].b = xmf3AvgNormal.z;
		}
	}
	//height map data init
	WCHAR path[256];
	wsprintf(path, L"../%sNormalMap.bmp", m_wsSceneName.c_str());//name
//	EXPORTER->MakeBitmap24(path, m_pNormalData, m_nWidth, m_nLength);
	m_pNormalTexture = CTexture::CreateTexture(path, PS_SLOT_NORMALMAP, BIND_PS);
}

CTerrainContainer * CTerrainContainer::CreateTerrainContainer(LPCTSTR pTerrainName, int nWidth, int nLength, float fHeightScale, CSpaceContainer * pSpaceContainer, bool isTool){
	CTerrainContainer* pTerrainContainer = new CTerrainContainer;

	pTerrainContainer->SetSpaceContainer(pSpaceContainer);
	pTerrainContainer->SetTerrainWidth(nWidth);
	pTerrainContainer->SetTerrainLength(nLength);
	pTerrainContainer->SetTerrainScale(XMFLOAT3(static_cast<float>(pSpaceContainer->GetSize() / (nWidth)),
		fHeightScale, static_cast<float>(pSpaceContainer->GetSize() / (nLength))));
	pTerrainContainer->SetSceneName(pTerrainName);
	pTerrainContainer->SetIsTool(isTool);
	
	//terrain texture create
	wstring SceneName{ pTerrainName };
	string name{ "" }; name.assign(SceneName.begin(), SceneName.end());
	if (isTool) {
		//make texture and use
		pTerrainContainer->CreateResetTextures(pTerrainContainer->GetSceneName().c_str());
		RESOURCEMGR->CreateInstancingBuffer(name, 256, sizeof(VS_VB_INSTANCE));
	}
	else {
		//just read texture
		pTerrainContainer->CreateTerrainTextures(pTerrainContainer->GetSceneName().c_str());
		RESOURCEMGR->CreateInstancingBuffer(name, 256, sizeof(VS_VB_INSTANCE));
		//RESOURCEMGR->CreateInstancingBuffer(name, pSpaceContainer->GetSpaceNum(), sizeof(VS_VB_INSTANCE));
	}
	//terrain texture create


	pTerrainContainer->Begin();
	return pTerrainContainer;
}
CTerrainContainer * CTerrainContainer::CreateTerrainContainer(wstring wsOutputPath, wstring wsSceneName, int nWidth, int nLength, CSpaceContainer * pSpaceContainer, bool isTool){
	CTerrainContainer* pTerrainContainer = new CTerrainContainer;
	pTerrainContainer->SetSpaceContainer(pSpaceContainer);
	pTerrainContainer->SetTerrainWidth(256);
	pTerrainContainer->SetTerrainLength(256);
	pTerrainContainer->SetSceneName(wsSceneName);
	pTerrainContainer->SetIsTool(isTool);

	//base texture path
	XMFLOAT3 xmf3Scale;
	xmf3Scale.x = IMPORTER->ReadFloat();
	xmf3Scale.y = IMPORTER->ReadFloat();
	xmf3Scale.z = IMPORTER->ReadFloat();
	pTerrainContainer->SetTerrainScale(xmf3Scale);

	wstring wsBaseTexturePath = IMPORTER->ReadWstring();
	pTerrainContainer->SetBaseTexture(wsBaseTexturePath);
	//height map texture name
	wstring wsHeightDataName = wsOutputPath + wsSceneName + L"HeightMap.bmp";
	pTerrainContainer->SetHeightData(IMPORTER->ReadBitmap24(wsHeightDataName.c_str()));//heightmap
	pTerrainContainer->SetHeightMapTexture(CTexture::CreateTexture(wsHeightDataName.c_str(), 1, BIND_DS));

	//normal map texture name
	wstring wsNormalDataName = wsOutputPath + wsSceneName + L"NormalMap.bmp";
	pTerrainContainer->SetNormalData(IMPORTER->ReadBitmap24(wsNormalDataName.c_str()));//nomalmap
	pTerrainContainer->SetNormalMapTexture(CTexture::CreateTexture(wsNormalDataName.c_str(), PS_SLOT_NORMALMAP, BIND_PS));

	//create splatting info
	int nSplatting = IMPORTER->ReadInt();
	for (int i = 0; i<nSplatting; ++i) {
		//splatting의 detail texture는 path로 받는다.
		wstring wsDetailTexturePath = IMPORTER->ReadWstring();
		
		//splatting의 blending info는 [scene name]이름을 토대로 가공한다.
		//[scene name] + [splatting blending info] + [index]
		WCHAR wcBlendinfoPath[256];
		wsprintf(wcBlendinfoPath, L"%s%sBlendInfo%d.bmp", wsOutputPath.c_str(), wsSceneName.c_str(), i);
		
		pTerrainContainer->GetSplattingInfoManager()->CreateSplattingInfo(wsDetailTexturePath.c_str(), wcBlendinfoPath);
	}
	//create splatting info
	pTerrainContainer->Begin();
	return pTerrainContainer;
}
void CTerrainContainer::SetHeightScale(float scale){
	m_pGlobalTerrainData->HeightScale = m_xmf3Scale.y = scale;
}

void CTerrainContainer::CreateTerrainTextures(LPCTSTR pTerrainName){
	WCHAR path[256];
	wsprintf(path, L"../%sHeightMap.bmp", pTerrainName);//name
	m_pHeightData = IMPORTER->ReadBitmap24(path);//heightmap
	m_pHeightMapTexture = CTexture::CreateTexture(path, 1, BIND_DS);
	wsprintf(path, L"../%sNormalMap.bmp", pTerrainName);
	m_pNormalData = IMPORTER->ReadBitmap24(path);//nomalmap
	m_pNormalTexture = CTexture::CreateTexture(path, PS_SLOT_NORMALMAP, BIND_PS);
	m_pBaseTexture = CTexture::CreateTexture(L"../../Assets/default.jpg", 0);
}
void CTerrainContainer::SetHeightData(Pixel24 * pHeightData){
	if (m_pHeightData) delete[] m_pHeightData;
	m_pHeightData = nullptr;
	m_pHeightData = pHeightData;
}
void CTerrainContainer::SetNormalData(Pixel24 * pNormalData){
	if (m_pNormalData) delete[] m_pNormalData;
	m_pNormalData = nullptr;
	m_pNormalData = pNormalData;
}
void CTerrainContainer::SetHeightMapTexture(shared_ptr<CTexture> pHeightMapTexture){
	if (m_pHeightMapTexture) m_pHeightMapTexture->End();
	m_pHeightMapTexture = nullptr;
	m_pHeightMapTexture = pHeightMapTexture;
}
void CTerrainContainer::SetNormalMapTexture(shared_ptr<CTexture> pNormalMapTexture){
	if (m_pNormalTexture) m_pNormalTexture->End();
	m_pNormalTexture = nullptr;
	m_pNormalTexture = pNormalMapTexture;
}
void CTerrainContainer::CreateResetTextures(LPCTSTR pTerrainName) {
	//texture 제작
	//height map data init
	//WCHAR path[256];
	//wsprintf(path, L"../%sHeightMap.bmp", pTerrainName);//name
	//m_pHeightData = new Pixel24[(m_nWidth) * (m_nLength)];//pixel data
	//ZeroMemory(m_pHeightData, sizeof(Pixel24) * (m_nWidth) * (m_nLength));
	//EXPORTER->MakeBitmap24(path, m_pHeightData, m_nWidth, m_nLength);
	//m_pHeightMapTexture = CTexture::CreateTexture(path, 1, BIND_DS);
	//m_pBaseTexture = CTexture::CreateTexture(L"../../Assets/default.jpg", 0);
	////normal
	//wsprintf(path, L"../%sNormalMap.bmp", pTerrainName);
	//m_pNormalData = new Pixel24[(m_nWidth) * (m_nLength)];
	//ZeroMemory(m_pNormalData, sizeof(Pixel24) * m_nWidth * m_nLength);
	//EXPORTER->MakeBitmap24(path, m_pNormalData, m_nWidth, m_nLength);
	//m_pNormalTexture = CTexture::CreateTexture(path, PS_SLOT_NORMALMAP, BIND_PS);
	//CreateNormalMap();
}
void CTerrainContainer::CreateTerrainMesh(CSpaceContainer* pSpaceContainer){
	//convert wstring to string
	string name{ "" }; name.assign(m_wsSceneName.begin(), m_wsSceneName.end());

	//resize instancing buf
	RENDERER->GetTerrainRenderContainer()->ClearMesh();//기존의 mesh clear
	RENDERER->GetTerrainRenderContainer()->ClearBuffer();//기존의 buffer clear
	//resize terrain mesh
	RESOURCEMGR->CreateTerrainMesh(pSpaceContainer->GetOneSpaceSize(), name);//resource 제작은 resource mgr에게
	//RESOURCEMGR->CreateInstancingBuffer(name, pSpaceContainer->GetSpaceNum(), sizeof(VS_VB_INSTANCE));
	//set resource
	RENDERER->GetTerrainRenderContainer()->AddMesh(RESOURCEMGR->GetMesh(name, 0));//만든 mesh사용
	RENDERER->GetTerrainRenderContainer()->AddInstanceBuffer(RESOURCEMGR->GetBuffer(name));
}

void CTerrainContainer::ChangeSpaceData(){
	//1. space안의 모든 terrain제거
	//End();

	for (auto pTerrain : m_vpTerrain) {
		m_pSpaceContainer->RemoveObject(pTerrain);
	}
	m_vpTerrain.clear();
	RENDERER->GetTerrainRenderContainer()->ClearObjectList();
	//2. mesh/ buffer새로 제작
	//Begin();
	SetTerrainScale(XMFLOAT3(static_cast<float>(m_pSpaceContainer->GetSize() / (m_nWidth)),
		m_xmf3Scale.y, static_cast<float>(m_pSpaceContainer->GetSize() / (m_nLength))));
	CreateTerrainMesh(m_pSpaceContainer);
	
	//3. terrain data 새로 설정
	float fOneSpaceSize = (float)m_pSpaceContainer->GetOneSpaceSize();
	float fOneSideSpaceNum = (float)m_pSpaceContainer->GetOneSideSpaceNum();
	m_pGlobalTerrainData->OneSideSpaceNum = fOneSideSpaceNum;
	m_pGlobalTerrainData->OneSideSpaceNumRcp = 1.0f / fOneSideSpaceNum;
	m_pGlobalTerrainData->OneSpaceSizeRcp = 1.0f / fOneSpaceSize;
	m_pGlobalTerrainData->HeightScale = m_xmf3Scale.y;
	
	//4. terrain 새로 제작
	//터레인 제작
	//terrain
	CTerrain* pTerrain = nullptr;
	for (int j = 0; j < m_pSpaceContainer->GetOneSideSpaceNum(); ++j) {
		for (int i = 0; i < m_pSpaceContainer->GetOneSideSpaceNum(); ++i) {
			pTerrain = CTerrain::CreateTerrain(this, i, j);
			m_vpTerrain.push_back(pTerrain);
		}
	}
	//terrain

	//추가 
	int space_lavel_pow = 1;
	for (int i = 0; i < m_pSpaceContainer->GetSpaceLevel(); ++i) {
		space_lavel_pow *= 2;
	}
	SetTessFacterSLP(space_lavel_pow);

}
void CTerrainContainer::SetActive(bool b){
	for (auto pTerrain : m_vpTerrain) {
		pTerrain->SetActive(b);
	}
	m_bActive = b;
}
//추가 
void CTerrainContainer::SetTerrainTessFacter(float zn, float zf, float space_level_pow, float max_facter_value, float min_facter_value, XMFLOAT3 xmf3CameraPos) {
	TERRAIN_TESS_FACTER* pData = (TERRAIN_TESS_FACTER*)m_pTessFacterBuffer->Map();
	pData->camera_pos = xmf3CameraPos;
	pData->zn = zn;
	pData->zf = zf;
	pData->space_lavel_pow = space_level_pow;
	pData->max_facter_value = max_facter_value / space_level_pow;
	pData->min_facter_value = min_facter_value / space_level_pow;
	m_pTessFacterBuffer->Unmap();
}
CTerrainContainer::CTerrainContainer() : CObject("terraincontainer") {
}

CTerrainContainer::~CTerrainContainer()
{
}

