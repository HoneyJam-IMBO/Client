#include "stdafx.h"
#include "ImageUI.h"


CImageUI::CImageUI()
{
}


CImageUI::~CImageUI()
{
}

CImageUI * CImageUI::Create(XMVECTOR xyPos, XMVECTOR xySize, TCHAR * pTexName, float fRanderLayer)
{
	CImageUI*		pObject = new CImageUI();

	if (FAILED(pObject->Initialize()))
	{
		MSG_BOX("CImageUI Create Faild");
		//Safe_Delete(pObject);
		delete pObject;
		pObject = nullptr;
		return pObject;
	}
	pObject->SetInfo(xyPos, xySize, pTexName, fRanderLayer);

	return pObject;
}

void CImageUI::SetInfo(XMVECTOR xyPos, XMVECTOR xySize, TCHAR * pTexName, float fRanderLayer)
{
	memcpy(m_szTexture, pTexName, sizeof(TCHAR) * 64);
	m_pTexture = RESOURCEMGR->GetTexture(TCHARToString(m_szTexture));
	m_pUIRenderCont = RENDERER->GetUIRenderer();
	m_pMesh = RESOURCEMGR->GetMesh("UI").get();
	m_pCBuffer = CBuffer::CreateConstantBuffer(1, sizeof(tUImatVP), 4, BIND_VS, NULL);	//직교뷰*투영변환행렬 / 알파
	m_fRenderLayer = fRanderLayer;

	XMStoreFloat2(&m_f2XYPos, xyPos);
	XMStoreFloat2(&m_f2XYSize, xySize);
}

HRESULT CImageUI::Initialize()
{
	//x y 왼쪽 상단이 0, 0 오른쪽 하단이 WINSIZEX, WINSIZEY 임
	XMStoreFloat4x4(&m_f4x4View, XMMatrixIdentity());
	XMStoreFloat4x4(&m_f4x4Proj, XMMatrixOrthographicLH(WINSIZEX, WINSIZEY, 0.f, 1.f));

	return S_OK;
}

int CImageUI::Update(float fTimeElapsed)
{

	if (m_pUIRenderCont) m_pUIRenderCont->SetRenderContainer(m_fRenderLayer, this);
	return 0;
}

void CImageUI::Render()
{
	m_pTexture->UpdateShaderState();
	m_pTexture->SetShaderState();

	m_pCBuffer->UpdateShaderState();
	SetParameter();

	m_pMesh->Render(1);

	//해제
	m_pTexture->CleanShaderState();
	m_pCBuffer->CleanShaderState();
}

void CImageUI::Release()
{
}

void CImageUI::SetParameter()
{
	m_f4x4View._11 = m_f2XYSize.x;
	m_f4x4View._22 = m_f2XYSize.y;
	m_f4x4View._33 = 1.f;

	m_f4x4View._41 = m_f2XYPos.x - WINSIZEX * 0.5f;
	m_f4x4View._42 = -m_f2XYPos.y + WINSIZEY * 0.5f;

	tUImatVP* pdata = (tUImatVP*)m_pCBuffer->Map();

	XMStoreFloat4x4(&pdata->m_xmf4x4VP, XMMatrixTranspose(XMMatrixMultiply(XMLoadFloat4x4(&m_f4x4View), XMLoadFloat4x4(&m_f4x4Proj))));

	m_pCBuffer->Unmap();
	m_pCBuffer->SetShaderState();
}
