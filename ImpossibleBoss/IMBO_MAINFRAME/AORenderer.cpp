#include "stdafx.h"
#include "AORenderer.h"

bool CAORenderer::Begin() {
	//ao compute shader
	m_pCSAONormalDepthDownScale = CComputeShader::CreateComputeShader(TEXT("CSAONormalDepthDownScale.cso"));
	m_pCSSSAOCompute = CComputeShader::CreateComputeShader(TEXT("CSSSAOCompute.cso"));
	//ao compute shader

	return true;
}

bool CAORenderer::End() {
	ReleaseAmbientOcculutionViews();

	if (m_pCSAONormalDepthDownScale) {
		m_pCSAONormalDepthDownScale->End();
		delete m_pCSAONormalDepthDownScale;
	}
	m_pCSAONormalDepthDownScale = nullptr;

	if (m_pCSSSAOCompute) {
		m_pCSSSAOCompute->End();
		delete m_pCSSSAOCompute;
	}
	m_pCSSSAOCompute = nullptr;

	if (m_pTDoownscaleCB) delete m_pTDoownscaleCB;
	m_pTDoownscaleCB = nullptr;

	if (m_pd3dbufAOConstantBuffer)m_pd3dbufAOConstantBuffer->Release();
	m_pd3dbufAOConstantBuffer = nullptr;

	return true;
}

void CAORenderer::SetShaderState() {

}

void CAORenderer::CleanShaderState() {

}

void CAORenderer::UpdateShaderState() {

}

ID3D11ShaderResourceView* CAORenderer::Excute(shared_ptr<CCamera> pCamera, float fOffsetRadius, float fRadius) {
	UINT nWidth = GLOBALVALUEMGR->GetrcClient().right / 2;
	UINT nHeight = GLOBALVALUEMGR->GetrcClient().bottom / 2;

	//ambient occulution °úÁ¤
	//set
	// Constants
	D3D11_MAPPED_SUBRESOURCE MappedResource;
	GLOBALVALUEMGR->GetDeviceContext()->Map(m_pd3dbufAOConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);
	TDownscaleCB* pDownscale = (TDownscaleCB*)MappedResource.pData;
	pDownscale->nWidth = nWidth;
	pDownscale->nHeight = nHeight;
	pDownscale->fHorResRcp = 1.0f / (float)pDownscale->nWidth;
	pDownscale->fVerResRcp = 1.0f / (float)pDownscale->nHeight;
	const XMFLOAT4X4 pProj = pCamera->GetProjectionFloat4x4();
	pDownscale->ProjParams.x = 1.0f / pProj.m[0][0];
	pDownscale->ProjParams.y = 1.0f / pProj.m[1][1];
	float fQ = pCamera->GetFarClip() / (pCamera->GetFarClip() - pCamera->GetNearClip());
	pDownscale->ProjParams.z = -pCamera->GetNearClip() * fQ;
	pDownscale->ProjParams.w = -fQ;
	XMStoreFloat4x4(&pDownscale->ViewMtx, pCamera->GetViewMtx());
	pDownscale->fOffsetRadius = fOffsetRadius;
	pDownscale->fRadius = fRadius;
	pDownscale->fMaxDepth = pCamera->GetFarClip();
	GLOBALVALUEMGR->GetDeviceContext()->Unmap(m_pd3dbufAOConstantBuffer, 0);
	ID3D11Buffer* arrConstBuffers[1] = { m_pd3dbufAOConstantBuffer };
	GLOBALVALUEMGR->GetDeviceContext()->CSSetConstantBuffers(0, 1, arrConstBuffers);
	//buffer

	ID3D11UnorderedAccessView* pUAVs[1] = { m_pd3duavAOMiniNormalDepth };
	GLOBALVALUEMGR->GetDeviceContext()->CSSetUnorderedAccessViews(0, 1, pUAVs, (UINT*)(&pUAVs));

	m_pCSAONormalDepthDownScale->ExcuteShaderState((UINT)ceil((float)(nWidth * nHeight) / 1024.0f), 1, 1);
	//clear
	pUAVs[0] = { nullptr };
	GLOBALVALUEMGR->GetDeviceContext()->CSSetUnorderedAccessViews(0, 1, pUAVs, (UINT*)(&pUAVs));

	//set
	ID3D11ShaderResourceView* pSRVs[1] = { m_pd3dsrvAOMiniNormalDepth };
	GLOBALVALUEMGR->GetDeviceContext()->CSSetShaderResources(0, 1, pSRVs);
	pUAVs[0] = { m_pd3duavAmbientOcculution };
	GLOBALVALUEMGR->GetDeviceContext()->CSSetUnorderedAccessViews(0, 1, pUAVs, (UINT*)(&pUAVs));

	m_pCSSSAOCompute->ExcuteShaderState((UINT)ceil((float)(nWidth * nHeight) / 1024.0f), 1, 1);
	//clear
	pUAVs[0] = { nullptr };
	GLOBALVALUEMGR->GetDeviceContext()->CSSetUnorderedAccessViews(0, 1, pUAVs, (UINT*)(&pUAVs));

	return m_pd3dsrvAmbientOcculution;
}

void CAORenderer::ResizeBuffer() {
	ReleaseAmbientOcculutionViews();

	UINT nWidth = GLOBALVALUEMGR->GetrcClient().right / 2;
	UINT nHeight = GLOBALVALUEMGR->GetrcClient().bottom / 2;

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// Allocate SSAO
	D3D11_TEXTURE2D_DESC t2dDesc = {
		nWidth, //UINT Width;
		nHeight, //UINT Height;
		1, //UINT MipLevels;
		1, //UINT ArraySize;
		DXGI_FORMAT_R32_TYPELESS,//DXGI_FORMAT_R8_TYPELESS, //DXGI_FORMAT Format;
		1, //DXGI_SAMPLE_DESC SampleDesc;
		0,
		D3D11_USAGE_DEFAULT,//D3D11_USAGE Usage;
		D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE,//UINT BindFlags;
		0,//UINT CPUAccessFlags;
		0//UINT MiscFlags;    
	};
	GLOBALVALUEMGR->GetDevice()->CreateTexture2D(&t2dDesc, NULL, &m_pd3dtxtAmbientOcculution);

	// Create the UAVs
	D3D11_UNORDERED_ACCESS_VIEW_DESC UAVDesc;
	ZeroMemory(&UAVDesc, sizeof(UAVDesc));
	UAVDesc.Format = DXGI_FORMAT_R32_FLOAT;
	UAVDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
	GLOBALVALUEMGR->GetDevice()->CreateUnorderedAccessView(m_pd3dtxtAmbientOcculution, &UAVDesc, &m_pd3duavAmbientOcculution);

	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
	ZeroMemory(&SRVDesc, sizeof(SRVDesc));
	SRVDesc.Format = DXGI_FORMAT_R32_FLOAT;
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	SRVDesc.Texture2D.MipLevels = 1;
	GLOBALVALUEMGR->GetDevice()->CreateShaderResourceView(m_pd3dtxtAmbientOcculution, &SRVDesc, &m_pd3dsrvAmbientOcculution);

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// Allocate down scaled depth buffer
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	bufferDesc.StructureByteStride = 4 * sizeof(float);
	bufferDesc.ByteWidth = nWidth * nHeight * bufferDesc.StructureByteStride;
	bufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	GLOBALVALUEMGR->GetDevice()->CreateBuffer(&bufferDesc, NULL, &m_pd3dbufAOMiniNormalDepth);
	//t2dDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	//GLOBALVALUEMGR->GetDevice()->CreateTexture2D(&t2dDesc, NULL, &m_pd3dtxtAOMiniNormalDepth);

	ZeroMemory(&UAVDesc, sizeof(UAVDesc));
	UAVDesc.Format = DXGI_FORMAT_UNKNOWN;
	UAVDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	UAVDesc.Buffer.FirstElement = 0;
	UAVDesc.Buffer.NumElements = nWidth * nHeight;
	GLOBALVALUEMGR->GetDevice()->CreateUnorderedAccessView(m_pd3dbufAOMiniNormalDepth, &UAVDesc, &m_pd3duavAOMiniNormalDepth);

	//ZeroMemory(&SRVDesc, sizeof(SRVDesc));
	SRVDesc.Format = DXGI_FORMAT_UNKNOWN;
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	SRVDesc.Buffer.FirstElement = 0;
	SRVDesc.Buffer.NumElements = nWidth * nHeight;
	GLOBALVALUEMGR->GetDevice()->CreateShaderResourceView(m_pd3dbufAOMiniNormalDepth, &SRVDesc, &m_pd3dsrvAOMiniNormalDepth);

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// Allocate down scale depth constant buffer
	D3D11_BUFFER_DESC CBDesc;
	ZeroMemory(&CBDesc, sizeof(CBDesc));
	CBDesc.Usage = D3D11_USAGE_DYNAMIC;
	CBDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	CBDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	CBDesc.ByteWidth = sizeof(TDownscaleCB);
	GLOBALVALUEMGR->GetDevice()->CreateBuffer(&CBDesc, NULL, &m_pd3dbufAOConstantBuffer);

	m_pCSAONormalDepthDownScale->SetThreadGroup((UINT)ceil((float)(nWidth * nHeight) / 1024.0f), 1, 1);
	m_pCSSSAOCompute->SetThreadGroup((UINT)ceil((float)(nWidth * nHeight) / 1024.0f), 1, 1);
}

void CAORenderer::ReleaseAmbientOcculutionViews() {

	if (m_pd3dbufAOMiniNormalDepth)m_pd3dbufAOMiniNormalDepth->Release();//1/4scale texture
	m_pd3dbufAOMiniNormalDepth = nullptr;
	//if (m_pd3dtxtAOMiniNormalDepth)m_pd3dtxtAOMiniNormalDepth->Release();//1/4scale texture
	//m_pd3dtxtAOMiniNormalDepth = nullptr;
	if (m_pd3dsrvAOMiniNormalDepth) m_pd3dsrvAOMiniNormalDepth->Release();
	m_pd3dsrvAOMiniNormalDepth = nullptr;

	if (m_pd3duavAOMiniNormalDepth)m_pd3duavAOMiniNormalDepth->Release();
	m_pd3duavAOMiniNormalDepth = nullptr;

	if (m_pd3dtxtAmbientOcculution)m_pd3dtxtAmbientOcculution->Release();//1/4scale texture
	m_pd3dtxtAmbientOcculution = nullptr;
	if (m_pd3dsrvAmbientOcculution)m_pd3dsrvAmbientOcculution->Release();
	m_pd3dsrvAmbientOcculution = nullptr;
	if (m_pd3duavAmbientOcculution)m_pd3duavAmbientOcculution->Release();
	m_pd3duavAmbientOcculution = nullptr;
}

CAORenderer::CAORenderer() : DXObject("aorenderer") {

}

CAORenderer::~CAORenderer() {

}
