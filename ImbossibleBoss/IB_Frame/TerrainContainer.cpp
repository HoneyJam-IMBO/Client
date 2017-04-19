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

	//global buffer ����
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

	//�ͷ��� ����
	//te`rrain
	CTerrain* pTerrain = nullptr;
	for (int j = 0; j < m_pSpaceContainer->GetOneSideSpaceNum(); ++j) {
		for (int i = 0; i < m_pSpaceContainer->GetOneSideSpaceNum(); ++i) {
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
	//�ڽ��� ��� ��ü�� ���ؼ� �˻�
	for (auto pObject : m_vpTerrain) {
		if (pObject->CheckPickObject(xmvWorldCameraStartPos, xmvRayDir, fHitDistance)) {//ray�� �浹�ߴٸ�
			if (fNearHitDistance > fHitDistance) {//������ ���� ����� �༮�� ��
				distance = fHitDistance;//�� ������ ���� ����� ��ü ����
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
	//1. bitmap ��ǥ���� height��.
	//2. �׷��� world ��ǥ�� �� x z�� �ݴ�� �Ǵ� ����� �ҷ����δ�.
	//3. height�� �״���̴�. ���� ���� bitmap ���迡 �����ϱ� �׳� �о ��
	//4. normal�� ����Ѵ�.
	//��
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
		//splatting�� detail texture�� path�� �޴´�.
		wstring wsDetailTexturePath = IMPORTER->ReadWstring();
		
		//splatting�� blending info�� [scene name]�̸��� ���� �����Ѵ�.
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
	//texture ����
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
	RENDERER->GetTerrainRenderContainer()->ClearMesh();//������ mesh clear
	RENDERER->GetTerrainRenderContainer()->ClearBuffer();//������ buffer clear
	//resize terrain mesh
	RESOURCEMGR->CreateTerrainMesh(pSpaceContainer->GetOneSpaceSize(), name);//resource ������ resource mgr����
	//RESOURCEMGR->CreateInstancingBuffer(name, pSpaceContainer->GetSpaceNum(), sizeof(VS_VB_INSTANCE));
	//set resource
	RENDERER->GetTerrainRenderContainer()->AddMesh(RESOURCEMGR->GetMesh(name, 0));//���� mesh���
	RENDERER->GetTerrainRenderContainer()->AddInstanceBuffer(RESOURCEMGR->GetBuffer(name));
}

void CTerrainContainer::ChangeSpaceData(){
	//1. space���� ��� terrain����
	//End();

	for (auto pTerrain : m_vpTerrain) {
		m_pSpaceContainer->RemoveObject(pTerrain);
	}
	m_vpTerrain.clear();
	RENDERER->GetTerrainRenderContainer()->ClearObjectList();
	//2. mesh/ buffer���� ����
	//Begin();
	SetTerrainScale(XMFLOAT3(static_cast<float>(m_pSpaceContainer->GetSize() / (m_nWidth)),
		m_xmf3Scale.y, static_cast<float>(m_pSpaceContainer->GetSize() / (m_nLength))));
	CreateTerrainMesh(m_pSpaceContainer);
	
	//3. terrain data ���� ����
	float fOneSpaceSize = (float)m_pSpaceContainer->GetOneSpaceSize();
	float fOneSideSpaceNum = (float)m_pSpaceContainer->GetOneSideSpaceNum();
	m_pGlobalTerrainData->OneSideSpaceNum = fOneSideSpaceNum;
	m_pGlobalTerrainData->OneSideSpaceNumRcp = 1.0f / fOneSideSpaceNum;
	m_pGlobalTerrainData->OneSpaceSizeRcp = 1.0f / fOneSpaceSize;
	m_pGlobalTerrainData->HeightScale = m_xmf3Scale.y;
	
	//4. terrain ���� ����
	//�ͷ��� ����
	//terrain
	CTerrain* pTerrain = nullptr;
	for (int j = 0; j < m_pSpaceContainer->GetOneSideSpaceNum(); ++j) {
		for (int i = 0; i < m_pSpaceContainer->GetOneSideSpaceNum(); ++i) {
			pTerrain = CTerrain::CreateTerrain(this, i, j);
			m_vpTerrain.push_back(pTerrain);
		}
	}
	//terrain
}
void CTerrainContainer::SetActive(bool b){
	for (auto pTerrain : m_vpTerrain) {
		pTerrain->SetActive(b);
	}
	m_bActive = b;
}
CTerrainContainer::CTerrainContainer() : CObject("terraincontainer") {
}

CTerrainContainer::~CTerrainContainer()
{
}
