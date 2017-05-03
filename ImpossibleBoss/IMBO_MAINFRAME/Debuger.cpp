
#include "stdafx.h"
#include "Debuger.h"

bool CDebuger::Begin(){
	
	//test
	m_pDebugTextureObj = new CDebugTexture;
	m_pDebugTextureObj->Begin();
	
	//for (auto RenderContainer : RCSELLER->GetTagRenderContainer()[tag::TAG_DEBUG]) {
	//	m_mDebugRenderContainer[RenderContainer.first] = RenderContainer.second;
	//}
	CAtlMap<tag, mapRC>::CPair* pOutPair = RCSELLER->GetTagRenderContainer().Lookup(tag::TAG_DEBUG);
	CAtlMap<CString, CRenderContainer*>::CPair*	pInPair = NULL;
	if (pOutPair != nullptr) {
		POSITION pos = pOutPair->m_value.GetStartPosition();
		while (pos != NULL) {
			pInPair = pOutPair->m_value.GetNext(pos);
			m_mDebugRenderContainer[pInPair->m_key] = pInPair->m_value;
		}
	}

	
	//aabb객체 미리 할당
	m_ppBoundingBox = new CBoundingBox*[BOUNDINGBOX_NUM];
	for (int i = 0; i < BOUNDINGBOX_NUM; ++i) {
		m_ppBoundingBox[i] = new CBoundingBox;
	}
	//aabb객체 미리 할당

	//coordinatesys객체 미리 할당
	m_ppCoordinateSys = new CCoordinateSys*[COORD_NUM];
	for (int i = 0; i < COORD_NUM; ++i) {
		m_ppCoordinateSys[i] = new CCoordinateSys;
	}
	//coordinatesys객체 미리 할당

	//depth stencil state
	D3D11_DEPTH_STENCIL_DESC descDepthStencil;
	ZeroMemory(&descDepthStencil, sizeof(D3D11_DEPTH_STENCIL_DESC));
	descDepthStencil.DepthEnable = true;
	descDepthStencil.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	descDepthStencil.DepthFunc = D3D11_COMPARISON_GREATER_EQUAL;

	GLOBALVALUEMGR->GetDevice()->CreateDepthStencilState(&descDepthStencil, &m_pLightDepthStencilState);
	//depth stencil state

	//rasterizer state
	D3D11_RASTERIZER_DESC descRasterizer;
	ZeroMemory(&descRasterizer, sizeof(D3D11_RASTERIZER_DESC));
	//descRasterizer.FillMode = D3D11_FILL_SOLID;
	descRasterizer.FillMode = D3D11_FILL_WIREFRAME;
	descRasterizer.CullMode = D3D11_CULL_NONE;
	GLOBALVALUEMGR->GetDevice()->CreateRasterizerState(&descRasterizer, &m_pLightRasterizerState);
	//rasterizer state

	//blend state
	D3D11_BLEND_DESC descBlend;
	ZeroMemory(&descBlend, sizeof(D3D11_BLEND_DESC));
	descBlend.RenderTarget[0].BlendEnable = true;
	descBlend.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	descBlend.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	descBlend.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;

	descBlend.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	descBlend.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	descBlend.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	descBlend.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	//blend state
	GLOBALVALUEMGR->GetDevice()->CreateBlendState(&descBlend, &m_pLightBlendState);

	//font .
	if (FAILED(FW1CreateFactory(FW1_VERSION, &m_pFW1FontFactory)))
	{
		DebugMessageBox("폰트를 만들 수 없습니다", "폰트못만듬!");
		return false;
	}
	if (FAILED(m_pFW1FontFactory->CreateFontWrapper(GLOBALVALUEMGR->GetDevice(), L"나눔고딕", &m_pFW1Font)))
	{
		DebugMessageBox("폰트 로딩오류", "폰트없음");
		return false;
	}
	//font .

	return true;
}

bool CDebuger::End() {
	for (int i = 0; i < BOUNDINGBOX_NUM; ++i) {
		delete m_ppBoundingBox[i];
	}
	for (int j = 0; j < COORD_NUM; ++j) {
		delete m_ppCoordinateSys[j];
	}

	//rendercontainer end는 seller의 역할이다.
	//font
	if (m_pFW1FontFactory) m_pFW1FontFactory->Release();
	m_pFW1FontFactory = nullptr;

	m_pFW1Font->Release();

	if (m_pFWTextRender)m_pFWTextRender->Release();
	m_pFWTextRender = nullptr;

	if (m_pFW1Glyphrovider) m_pFW1Glyphrovider->Release();
	m_pFW1Glyphrovider = nullptr;

	while (false == m_qDebugFontData.empty()) {
		m_qDebugFontData.pop();
	}
	while (false == m_qDebugTextureData.empty()) {
		m_qDebugTextureData.pop();
	}
	

	return true;
}
void CDebuger::RegistCoordinateSys(FXMMATRIX mtx) {
	if (false == INPUTMGR->GetDebugMode()) return;

	m_ppCoordinateSys[m_nCoordinateSys]->SetCoordinateSysInfo(mtx);
	m_mDebugRenderContainer[CA2CT("coordinatesys")]->AddObject(m_ppCoordinateSys[m_nCoordinateSys++]);
}

void CDebuger::RegistAABB(BoundingBox& aabb){
	if (false == INPUTMGR->GetDebugMode()) return;

	m_ppBoundingBox[m_nAABB]->SetBoundingBoxInfo(aabb);
	m_mDebugRenderContainer[CA2CT("aabb")]->AddObject(m_ppBoundingBox[m_nAABB++]);

}
void CDebuger::RegistOBB(BoundingOrientedBox & obb){
	if (false == INPUTMGR->GetDebugMode()) return;

	m_ppBoundingBox[m_nAABB]->SetBoundingBoxInfo(obb);
	m_mDebugRenderContainer[CA2CT("aabb")]->AddObject(m_ppBoundingBox[m_nAABB++]);
}

void CDebuger::RegistToDebugRenderContainer(CGameObject * pObject){
	if (false == INPUTMGR->GetDebugMode()) return;

	string name = pObject->GetName();

	if (name == "pointlight") {
		m_mDebugRenderContainer[CA2CT("debugpointlight")]->AddObject(pObject);
	}
	else if (name == "capsulelight") {
		m_mDebugRenderContainer[CA2CT("debugcapsulelight")]->AddObject(pObject);
	}
	else if (name == "spotlight") {
		m_mDebugRenderContainer[CA2CT("debugspotlight")]->AddObject(pObject);
	}
	

}

void CDebuger::DebugRender(shared_ptr<CCamera> pCamera){
	if (false == INPUTMGR->GetDebugMode()) return;

	RenderAABB(pCamera);
	RenderLightVolume(pCamera);
	RenderCoordinateSys(pCamera);

	//for (auto pRenderContaier : m_mDebugRenderContainer) {
	//	pRenderContaier.second->ClearObjectList();
	//}
	POSITION pos = m_mDebugRenderContainer.GetStartPosition();
	CAtlMap<CString, CRenderContainer*>::CPair*	pInPair = NULL;
	while (pos != NULL)
	{
		pInPair = m_mDebugRenderContainer.GetNext(pos);
		pInPair->m_value->ClearObjectList();
	}

	m_nAABB = 0;
	m_nCoordinateSys = 0;
	//ClearDebuger();
}

void CDebuger::RenderAABB(shared_ptr<CCamera> pCamera){
	if (false == INPUTMGR->GetDebugMode()) return;
	//render aabb

	m_mDebugRenderContainer[CA2CT("aabb")]->Render(pCamera);
}
void CDebuger::RenderCoordinateSys(shared_ptr<CCamera> pCamera) {
	if (false == INPUTMGR->GetDebugMode()) return;
	//render coordinatesystem
	CString csName = CA2CT("coordinatesys");
	m_mDebugRenderContainer[csName]->Render(pCamera);
}
void CDebuger::RenderLightVolume(shared_ptr<CCamera> pCamera){

	//이전 상태 저장
	//m_pd3dDeviceContext->OMGetDepthStencilState(&m_pPreDepthStencilState, &m_PreStencilRef);
	//m_pd3dDeviceContext->OMGetBlendState(&m_pPreBlendState, m_pPreBlendFactor, &m_PreSampleMask);
	GLOBALVALUEMGR->GetDeviceContext()->RSGetState(&m_pPreRasterizerState);

	//m_pd3dDeviceContext->OMSetBlendState(m_pLightBlendState, nullptr, 0xffffffff);
	//m_pd3dDeviceContext->OMSetDepthStencilState(m_pLightDepthStencilState, 0);
	GLOBALVALUEMGR->GetDeviceContext()->RSSetState(m_pLightRasterizerState);

	//for (auto RenderContainer : m_mDebugRenderContainer) {
	//	RenderContainer.second->Render(pCamera);
	//}
	POSITION pos = m_mDebugRenderContainer.GetStartPosition();
	CAtlMap<CString, CRenderContainer*>::CPair*	pInPair = NULL;
	while (pos != NULL)
	{
		pInPair = m_mDebugRenderContainer.GetNext(pos);
		pInPair->m_value->Render(pCamera);
	}

	//이전 상태로 되돌림
	//m_pd3dDeviceContext->OMSetBlendState(m_pPreBlendState, m_pPreBlendFactor, m_PreSampleMask);
	//m_pd3dDeviceContext->OMSetDepthStencilState(m_pPreDepthStencilState, m_PreStencilRef);
	GLOBALVALUEMGR->GetDeviceContext()->RSSetState(m_pPreRasterizerState);

}

void CDebuger::ClearDebuger(){

//font
	while (false == m_qDebugFontData.empty()) {
		m_qDebugFontData.pop();
	}
//texture
	while (false == m_qDebugTextureData.empty()) {
		m_qDebugTextureData.pop();
	}
	while (false == m_qDebugDepthTextureData.empty()) {
		m_qDebugDepthTextureData.pop();
	}

	POSITION pos = m_mDebugRenderContainer.GetStartPosition();
	CAtlMap<CString, CRenderContainer*>::CPair*	pInPair = NULL;
	while (pos != NULL)
	{
		pInPair = m_mDebugRenderContainer.GetNext(pos);
		pInPair->m_value->ClearObjectList();
	}
	/*for (auto RenderContainer : m_mDebugRenderContainer) {
		RenderContainer.second->ClearObjectList();
	}*/
	
	m_nAABB = 0;
	m_nCoordinateSys = 0;
}

//utill func
void CDebuger::AddText(float fontSize, float posX, float posY, UINT32 color, TCHAR * msg, ...) {
	va_list arg;
	va_start(arg, msg);
	TCHAR TEXT[500] = { NULL };
	vswprintf(TEXT, msg, arg);

	CDebugFontData Data(TEXT, fontSize, posX, posY, color, m_pivot);
	m_qDebugFontData.push(Data);
}
void CDebuger::RenderText(){

	if (false == m_pFW1Font) return;
	if (false == INPUTMGR->GetDebugMode()) return;

	//이전 상태 저장
	GLOBALVALUEMGR->GetDeviceContext()->OMGetDepthStencilState(&m_pPreDepthStencilState, &m_PreStencilRef);
	GLOBALVALUEMGR->GetDeviceContext()->OMGetBlendState(&m_pPreBlendState, m_pPreBlendFactor, &m_PreSampleMask);
	GLOBALVALUEMGR->GetDeviceContext()->RSGetState(&m_pPreRasterizerState);


	while (false == m_qDebugFontData.empty()) {
		//get
		CDebugFontData Data = m_qDebugFontData.front();

		//draw
		m_pFW1Font->DrawString
		(
			GLOBALVALUEMGR->GetDeviceContext(),
			Data.msg.c_str(),
			Data.fontSize,
			Data.x,
			Data.y,
			Data.color,
			Data.pivot
		);

		//pop
		m_qDebugFontData.pop();
	}
	
	//이전 상태로 되돌림
	GLOBALVALUEMGR->GetDeviceContext()->OMSetDepthStencilState(m_pPreDepthStencilState, m_PreStencilRef);
	GLOBALVALUEMGR->GetDeviceContext()->OMSetBlendState(m_pPreBlendState, m_pPreBlendFactor, m_PreSampleMask);
	GLOBALVALUEMGR->GetDeviceContext()->RSSetState(m_pPreRasterizerState);

}
void CDebuger::AddTexture(XMFLOAT2 lt, XMFLOAT2 rb, ID3D11ShaderResourceView* pSRV){
	if (false == INPUTMGR->GetDebugMode()) return;
	CDebugTextureData DebugTextureData(pSRV, lt, rb);
	m_qDebugTextureData.push(DebugTextureData);
}
void CDebuger::AddDepthTexture(XMFLOAT2 fLeftTop, XMFLOAT2 fRightBottom, ID3D11ShaderResourceView * pSRV){
	if (false == INPUTMGR->GetDebugMode()) return;
	CDebugTextureData DebugTextureData(pSRV, fLeftTop, fRightBottom);
	m_qDebugDepthTextureData.push(DebugTextureData);
}
void CDebuger::RenderTexture(){
	if (false == INPUTMGR->GetDebugMode()) return;
	while (false == m_qDebugTextureData.empty()) {

		CDebugTextureData DebugTextureData = m_qDebugTextureData.front();
		m_qDebugTextureData.pop();

		m_pDebugTexture = CTexture::CreateTexture(DebugTextureData.m_pSRV);

		m_pDebugTextureObj->SetTextureInfo(DebugTextureData.lt, DebugTextureData.rb);
		CString csName = CA2CT("debugtexture");
		m_mDebugRenderContainer[csName]->AddObject(m_pDebugTextureObj);
		m_mDebugRenderContainer[csName]->AddTexture(m_pDebugTexture);
		m_mDebugRenderContainer[csName]->Render(nullptr);
		m_mDebugRenderContainer[csName]->ClearObjectList();
		m_mDebugRenderContainer[csName]->ClearTextures();
	}

	//dpeh thexture
	while (false == m_qDebugDepthTextureData.empty()) {

		CDebugTextureData DebugTextureData = m_qDebugDepthTextureData.front();
		m_qDebugDepthTextureData.pop();

		m_pDebugTexture = CTexture::CreateTexture(DebugTextureData.m_pSRV);

		m_pDebugTextureObj->SetTextureInfo(DebugTextureData.lt, DebugTextureData.rb);
		CString csName = CA2CT("debugdepthtexture");
		m_mDebugRenderContainer[csName]->AddObject(m_pDebugTextureObj);
		m_mDebugRenderContainer[csName]->AddTexture(m_pDebugTexture);
		m_mDebugRenderContainer[csName]->Render(nullptr);
		m_mDebugRenderContainer[csName]->ClearObjectList();
		m_mDebugRenderContainer[csName]->ClearTextures();
	}
}
int CDebuger::DebugMessageBox(std::string _title, std::string _message)
{
	const char* msgChar = _message.c_str();
	int msgLen = static_cast<int>(1 + strlen(msgChar));
	TCHAR* message = new TCHAR[msgLen];
	mbstowcs(message, msgChar, msgLen);

	const char* titleChar = _title.c_str();
	int titleLen = static_cast<int>(1 + strlen(titleChar));
	TCHAR* title = new TCHAR[titleLen];
	mbstowcs(title, titleChar, titleLen);

	return MessageBox(nullptr, message, title, MB_OK);
}

int CDebuger::DebugGMessageBox(TCHAR* title, TCHAR* message, ...)
{
	va_list arg;
	va_start(arg, message);
	TCHAR TEXT[500] = { NULL };
	vswprintf(TEXT, message, arg);

	return MessageBox(nullptr, TEXT, title, MB_OK);
}


CDebuger::CDebuger() :CSingleTonBase<CDebuger>("debugersingleton"){

}

CDebuger::~CDebuger(){

}
