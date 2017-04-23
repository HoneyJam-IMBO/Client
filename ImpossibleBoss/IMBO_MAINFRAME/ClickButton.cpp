#include "stdafx.h"
#include "ClickButton.h"


CClickButton::CClickButton()
{
}


CClickButton::~CClickButton()
{
}

CClickButton * CClickButton::Create(XMVECTOR xyPos, XMVECTOR xySize, TCHAR * pTexName)
{
	CClickButton*		pObject = new CClickButton();

	if (FAILED(pObject->Initialize()))
	{
		MSG_BOX("CClickButton Create Faild");
		//Safe_Delete(pObject);
		delete pObject;
		pObject = nullptr;
		return pObject;
	}
	pObject->SetInfo(xyPos, xySize, pTexName);

	return pObject;
}

void CClickButton::SetInfo(XMVECTOR xyPos, XMVECTOR xySize, TCHAR * pTexName)
{
	memcpy(m_szTexture, pTexName, sizeof(TCHAR) * 64);
	m_pTexture = RESOURCEMGR->GetTexture(TCHARToString(m_szTexture));
	m_pUIRenderCont = RENDERER->GetUIRenderer();
	m_pMesh = RESOURCEMGR->GetMesh("UI").get();
	m_pCBuffer = CBuffer::CreateConstantBuffer(1, sizeof(tUImatVP), 4, BIND_VS, NULL);	//직교뷰*투영변환행렬 / 알파

	XMStoreFloat2(&m_f2XYPos, xyPos);
	XMStoreFloat2(&m_f2XYSize, xySize);
}

HRESULT CClickButton::Initialize()
{
	XMStoreFloat4x4(&m_f4x4View, XMMatrixIdentity());
	XMStoreFloat4x4(&m_f4x4Proj, XMMatrixOrthographicLH(WINSIZEX, WINSIZEY, 0.f, 1.f));

	return S_OK;
}

int CClickButton::Update(float fTimeElapsed)
{
	if (m_pUIRenderCont) m_pUIRenderCont->SetRenderContainer(3.0f, this);
	return 0;
}

void CClickButton::Render()
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

void CClickButton::Release()
{
}

void CClickButton::SetParameter()
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
