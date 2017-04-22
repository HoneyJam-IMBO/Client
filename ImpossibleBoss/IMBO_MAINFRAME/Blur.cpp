#include "stdafx.h"
#include "Blur.h"

bool CBlur::Begin() {

	m_pHorBlurComputeShader = CComputeShader::CreateComputeShader(TEXT("CSHorizontalBlur.cso"));
	m_pResBuffer = CBuffer::CreateConstantBuffer(1, sizeof(stBlur), 0, BIND_CS);
	m_pVerBlurComputeShader = CComputeShader::CreateComputeShader(TEXT("CSVerticalBlur.cso"));
	return true;
}

bool CBlur::End() {
	ReleaseBuffer();
	m_pResBuffer->End();
	m_pResBuffer = nullptr;
	m_pVerBlurComputeShader->End();
	delete m_pVerBlurComputeShader;
	m_pVerBlurComputeShader = nullptr;

	m_pHorBlurComputeShader->End();
	delete m_pHorBlurComputeShader;
	m_pHorBlurComputeShader = nullptr;
	return true;
}

void CBlur::SetShaderState() {

}

void CBlur::CleanShaderState() {

}

void CBlur::UpdateShaderState() {

}
ID3D11ShaderResourceView* CBlur::Excute(ID3D11ShaderResourceView* pSRVDest) {
	stBlur* pBlurData = (stBlur*)m_pResBuffer->Map();
	pBlurData->m_nHeight = m_nHeight;
	pBlurData->m_nWidth = m_nWidth;
	m_pResBuffer->Unmap();
	m_pResBuffer->SetShaderState();
	//blur 1
	//set
	ID3D11ShaderResourceView* pBlurTargetSRVs[1] = { pSRVDest };
	GLOBALVALUEMGR->GetDeviceContext()->CSSetShaderResources(0, 1, pBlurTargetSRVs);
	ID3D11UnorderedAccessView *pTempHorBlurUAVs[1] = { m_pd3duavTempBlur };
	GLOBALVALUEMGR->GetDeviceContext()->CSSetUnorderedAccessViews(0, 1, pTempHorBlurUAVs, (UINT*)(&pTempHorBlurUAVs));
	//set cb 추가
	m_pHorBlurComputeShader->ExcuteShaderState((UINT)ceil((m_nWidth) / (128.0f - 12.0f)), (UINT)ceil(m_nHeight), 1);

	//clean
	ID3D11ShaderResourceView* pCleanSRVs[1] = { nullptr };
	GLOBALVALUEMGR->GetDeviceContext()->CSSetShaderResources(0, 1, pCleanSRVs);
	ID3D11UnorderedAccessView *pCleanUAVs[1] = { nullptr };
	GLOBALVALUEMGR->GetDeviceContext()->CSSetUnorderedAccessViews(0, 1, pCleanUAVs, (UINT*)(&pCleanUAVs));

	//blur2
	pBlurTargetSRVs[0] = { m_pd3dsrvTempBlur };
	GLOBALVALUEMGR->GetDeviceContext()->CSSetShaderResources(0, 1, pBlurTargetSRVs);
	ID3D11UnorderedAccessView* pBlurResultUAVs[1] = { m_pd3duavBloom };
	GLOBALVALUEMGR->GetDeviceContext()->CSSetUnorderedAccessViews(0, 1, pBlurResultUAVs, (UINT*)(&pBlurResultUAVs));

	m_pVerBlurComputeShader->ExcuteShaderState((UINT)ceil(m_nWidth), (UINT)ceil((m_nHeight) / (128.0f - 12.0f)), 1);

	//짜잔 이제 bloom blur이미지가 완성되었다.
	//test
	pCleanUAVs[0] = { nullptr };
	GLOBALVALUEMGR->GetDeviceContext()->CSSetUnorderedAccessViews(0, 1, pCleanUAVs, (UINT*)(&pCleanUAVs));
	pCleanSRVs[0] = { nullptr };
	GLOBALVALUEMGR->GetDeviceContext()->CSSetShaderResources(0, 1, pCleanSRVs);

	//DEBUGER->AddTexture(XMFLOAT2(100, 100), XMFLOAT2(250, 250), m_pd3dsrvTempBlur);
	//DEBUGER->AddTexture(XMFLOAT2(100, 250), XMFLOAT2(250, 400), m_pd3dsrvBloom);
	return m_pd3dsrvBloom;
}

void CBlur::ResizeBuffer(UINT nScale) {
	ReleaseBuffer();

	m_nWidth = GLOBALVALUEMGR->GetrcClient().right / nScale;
	m_nHeight = GLOBALVALUEMGR->GetrcClient().bottom / nScale;
	m_nDownScaleGroup = (UINT)ceil((float)(m_nWidth * m_nHeight) / 1024);
	//TEMP TEXTURE
	//----------------------------------------UAV Desc----------------------------------------------//
	D3D11_UNORDERED_ACCESS_VIEW_DESC d3dUAVDesc;
	ZeroMemory(&d3dUAVDesc, sizeof(D3D11_UNORDERED_ACCESS_VIEW_DESC));
	d3dUAVDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
	d3dUAVDesc.Buffer.FirstElement = 0;
	d3dUAVDesc.Buffer.NumElements = m_nWidth*m_nHeight;
	//----------------------------------------UAV Desc----------------------------------------------//
	//----------------------------------------Resource Desc-----------------------------------------//
	D3D11_SHADER_RESOURCE_VIEW_DESC d3dSRVDesc;
	::ZeroMemory(&d3dSRVDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	d3dSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	d3dSRVDesc.Texture2D.MipLevels = 1;
	//d3dSRVDesc.Format = DXGI_FORMAT_R32_FLOAT;
	//----------------------------------------Resource Desc-----------------------------------------//
	//----------------------------------------TextUre Desc-----------------------------------------//
	D3D11_TEXTURE2D_DESC d3dTexture2DDesc;
	::ZeroMemory(&d3dTexture2DDesc, sizeof(D3D11_TEXTURE2D_DESC));
	d3dTexture2DDesc.Width = m_nWidth;
	d3dTexture2DDesc.Height = m_nHeight;
	d3dTexture2DDesc.MipLevels = 1;
	d3dTexture2DDesc.ArraySize = 1;
	d3dTexture2DDesc.SampleDesc.Count = 1;
	d3dTexture2DDesc.SampleDesc.Quality = 0;
	d3dTexture2DDesc.Usage = D3D11_USAGE_DEFAULT;
	d3dTexture2DDesc.CPUAccessFlags = 0;
	d3dTexture2DDesc.MiscFlags = 0;
	d3dTexture2DDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	//d3dTexture2DDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	//----------------------------------------TextUre Desc-----------------------------------------//
	d3dTexture2DDesc.Format = d3dSRVDesc.Format = d3dUAVDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;

	//
	GLOBALVALUEMGR->GetDevice()->CreateTexture2D(&d3dTexture2DDesc, nullptr, &m_pd3dtxtTempBlur);
	GLOBALVALUEMGR->GetDevice()->CreateShaderResourceView(m_pd3dtxtTempBlur, &d3dSRVDesc, &m_pd3dsrvTempBlur);
	GLOBALVALUEMGR->GetDevice()->CreateUnorderedAccessView(m_pd3dtxtTempBlur, &d3dUAVDesc, &m_pd3duavTempBlur);
	//TEMP TEXTURE

	//RESULT TEXTURE
	//----------------------------------------UAV Desc----------------------------------------------//
	ZeroMemory(&d3dUAVDesc, sizeof(D3D11_UNORDERED_ACCESS_VIEW_DESC));
	d3dUAVDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
	d3dUAVDesc.Buffer.FirstElement = 0;
	d3dUAVDesc.Buffer.NumElements = m_nWidth*m_nHeight;
	//----------------------------------------UAV Desc----------------------------------------------//
	//----------------------------------------Resource Desc-----------------------------------------//
	::ZeroMemory(&d3dSRVDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	d3dSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	d3dSRVDesc.Texture2D.MipLevels = 1;
	//d3dSRVDesc.Format = DXGI_FORMAT_R32_FLOAT;
	//----------------------------------------Resource Desc-----------------------------------------//
	//----------------------------------------TextUre Desc-----------------------------------------//
	::ZeroMemory(&d3dTexture2DDesc, sizeof(D3D11_TEXTURE2D_DESC));
	d3dTexture2DDesc.Width = m_nWidth;
	d3dTexture2DDesc.Height = m_nHeight;
	d3dTexture2DDesc.MipLevels = 1;
	d3dTexture2DDesc.ArraySize = 1;
	d3dTexture2DDesc.SampleDesc.Count = 1;
	d3dTexture2DDesc.SampleDesc.Quality = 0;
	d3dTexture2DDesc.Usage = D3D11_USAGE_DEFAULT;
	d3dTexture2DDesc.CPUAccessFlags = 0;
	d3dTexture2DDesc.MiscFlags = 0;
	d3dTexture2DDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	//d3dTexture2DDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	//----------------------------------------TextUre Desc-----------------------------------------//
	d3dTexture2DDesc.Format = d3dSRVDesc.Format = d3dUAVDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;

	//3.
	GLOBALVALUEMGR->GetDevice()->CreateTexture2D(&d3dTexture2DDesc, nullptr, &m_pd3dtxtBloom);
	GLOBALVALUEMGR->GetDevice()->CreateShaderResourceView(m_pd3dtxtBloom, &d3dSRVDesc, &m_pd3dsrvBloom);
	GLOBALVALUEMGR->GetDevice()->CreateUnorderedAccessView(m_pd3dtxtBloom, &d3dUAVDesc, &m_pd3duavBloom);
	//bloom texture 제작
	//RESULT TEXTURE
}

void CBlur::ReleaseBuffer() {
	if (m_pd3dtxtTempBlur) m_pd3dtxtTempBlur->Release();
	if (m_pd3dsrvTempBlur)m_pd3dsrvTempBlur->Release();
	if (m_pd3duavTempBlur)m_pd3duavTempBlur->Release();

	if (m_pd3dtxtBloom) m_pd3dtxtBloom->Release();
	if (m_pd3dsrvBloom)m_pd3dsrvBloom->Release();
	if (m_pd3duavBloom)m_pd3duavBloom->Release();

}


CBlur::CBlur() : DXObject("blur") {

}

CBlur::~CBlur() {

}
