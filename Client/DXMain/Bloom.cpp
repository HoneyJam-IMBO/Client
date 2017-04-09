#include "stdafx.h"
#include "Bloom.h"

bool CBloom::Begin() {
	m_pBloomComputeShader = CComputeShader::CreateComputeShader(TEXT("CSBloom.cso"));

	return true;
}

bool CBloom::End() {
	ReleaseBuffer();
	m_pBloomComputeShader->End();
	delete m_pBloomComputeShader;
	m_pBloomComputeShader = nullptr;
	return true;
}

void CBloom::SetShaderState() {

}

void CBloom::CleanShaderState() {

}

void CBloom::UpdateShaderState() {

}

ID3D11ShaderResourceView* CBloom::Excute(shared_ptr<CCamera> pCamera) {
	ID3D11UnorderedAccessView* pTempBloomUAVs[1] = { m_pd3duavTempBloom };
	//1번에는 0번 tmp texture를
	GLOBALVALUEMGR->GetDeviceContext()->CSSetUnorderedAccessViews(0, 1, pTempBloomUAVs, (UINT*)(&pTempBloomUAVs));

	m_pBloomComputeShader->ExcuteShaderState(m_nDownScaleGroup, 1, 1);


	ID3D11UnorderedAccessView* pCleanTempBloomUAVs[1] = { nullptr };
	GLOBALVALUEMGR->GetDeviceContext()->CSSetUnorderedAccessViews(0, 1, pCleanTempBloomUAVs, (UINT*)(&pCleanTempBloomUAVs));
	//bloom을 위한 텍스쳐 제작 끝.
	/*
	이제 추출한 밝은 부분에 블러를 입여준다.
	*/
	//blur 1
	//set
	return m_pd3dsrvTempBloom;
}

void CBloom::ResizeBuffer() {
	ReleaseBuffer();

	m_nWidth = GLOBALVALUEMGR->GetrcClient().right / 4;
	m_nHeight = GLOBALVALUEMGR->GetrcClient().bottom / 4;
	m_nDownScaleGroup = (UINT)ceil((float)(m_nWidth * m_nHeight) / 1024);

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

	//2.
	GLOBALVALUEMGR->GetDevice()->CreateTexture2D(&d3dTexture2DDesc, nullptr, &m_pd3dtxtTempBloom);
	GLOBALVALUEMGR->GetDevice()->CreateShaderResourceView(m_pd3dtxtTempBloom, &d3dSRVDesc, &m_pd3dsrvTempBloom);
	GLOBALVALUEMGR->GetDevice()->CreateUnorderedAccessView(m_pd3dtxtTempBloom, &d3dUAVDesc, &m_pd3duavTempBloom);
}

void CBloom::ReleaseBuffer() {
	if (m_pd3dtxtTempBloom) m_pd3dtxtTempBloom->Release();
	if (m_pd3dsrvTempBloom)m_pd3dsrvTempBloom->Release();
	if (m_pd3duavTempBloom)m_pd3duavTempBloom->Release();
}


CBloom::CBloom() : DXObject("bloom") {

}

CBloom::~CBloom() {

}
