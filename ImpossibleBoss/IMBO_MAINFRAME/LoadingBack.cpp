#include "stdafx.h"
#include "LoadingBack.h"

CLoadingBack::CLoadingBack()
{
}
CLoadingBack::~CLoadingBack()
{
}

CLoadingBack * CLoadingBack::Create(XMVECTOR xyPos, XMVECTOR xySize, TCHAR* pTexName)
{
	CLoadingBack*		pObject = new CLoadingBack();

	if (FAILED(pObject->Initialize()))
	{
		MSG_BOX("CLoadingBack Create Faild");
		//Safe_Delete(pObject);
		delete pObject;
		pObject = nullptr;
		return pObject;
	}
	pObject->SetInfo(xyPos, xySize, pTexName);

	return pObject;
}

void CLoadingBack::SetInfo(XMVECTOR xyPos, XMVECTOR xySize, TCHAR* pTexName)
{
	memcpy(m_szTexture, pTexName, sizeof(TCHAR) * 64);
	m_pTexture = RESOURCEMGR->GetTexture(TCHARToString(m_szTexture));
	m_pUIRenderCont = RENDERER->GetUIRenderer();
	m_pMesh = RESOURCEMGR->GetMesh("UI").get();
	m_pCBuffer = CBuffer::CreateConstantBuffer(1, sizeof(tUImatVP), 4, BIND_VS , NULL);	//직교뷰*투영변환행렬 / 알파
	
	XMStoreFloat2(&m_f2XYPos, xyPos);
	XMStoreFloat2(&m_f2XYSize, xySize);
}

HRESULT CLoadingBack::Initialize()
{
	XMStoreFloat4x4(&m_f4x4View, XMMatrixIdentity());
	XMStoreFloat4x4(&m_f4x4Proj, XMMatrixOrthographicLH(WINSIZEX, WINSIZEY, 0.f, 1.f));

	m_iType = rand() % 3;
	return S_OK;
}

int CLoadingBack::Update(float fTimeElapsed)
{
	m_fAccTime += fTimeElapsed * 4.f;
	//m_f2XYPos.x += 2.f * cos(m_fAccTime);

	//x y 왼쪽 상단이 0, 0 오른쪽 하단이 WINSIZEX, WINSIZEY 임

	if (0 == m_iType)
	{
		m_f2XYPos.x = WINSIZEX * 0.5f - cos(m_fAccTime) * 200.f;
		m_f2XYPos.y = WINSIZEY * 0.5f - sin(m_fAccTime) * 200.f;
	}
	else if(1 == m_iType)
	{
		m_f2XYPos.x = WINSIZEX * 0.5f - cos(m_fAccTime) * 200.f;
		//m_f2XYPos.y = WINSIZEY * 0.5f - sin(m_fAccTime) * 200.f;
	}
	else if (2 == m_iType)
	{
		m_f2XYPos.x = WINSIZEX * 0.5f - cos(m_fAccTime) * 200.f;
		m_f2XYPos.y = WINSIZEY * 0.5f - cos(m_fAccTime) * 200.f;
	}

	if (m_pUIRenderCont) m_pUIRenderCont->SetRenderContainer(1.f, this);
	return 0;
}

void CLoadingBack::Render()
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
void CLoadingBack::Release()
{
}

void CLoadingBack::SetParameter()
{
	m_f4x4View._11 = m_f2XYSize.x;
	m_f4x4View._22 = m_f2XYSize.y;
	m_f4x4View._33 = 1.f;

	m_f4x4View._41 = m_f2XYPos.x - WINSIZEX * 0.5f;
	m_f4x4View._42 = -m_f2XYPos.y + WINSIZEY * 0.5f;

	tUImatVP* pdata = (tUImatVP*)m_pCBuffer->Map();

	XMStoreFloat4x4(&pdata->m_xmf4x4VP, XMMatrixTranspose(XMMatrixMultiply(XMLoadFloat4x4(&m_f4x4View), XMLoadFloat4x4(&m_f4x4Proj))));
//	pdata->m_fAlpha = 1.f /*임시*/;

	m_pCBuffer->Unmap();
	m_pCBuffer->SetShaderState();

}
