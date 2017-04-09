#include "stdafx.h"
#include "Texture.h"

//array
bool CTexture::Begin(){
	return true;
}

bool CTexture::End() {
	//sampler
	//if (m_pSampler) {
	//	m_pSampler->End();
	//}
	//constantbuffer
	if (m_pConstantBuffer) {
		m_pConstantBuffer->End();
	}
	m_pConstantBuffer = nullptr;

	//texture
	if (m_pd3dsrvTexture) m_pd3dsrvTexture->Release();
	m_pd3dsrvTexture = nullptr;

	return true;
}

void CTexture::SetShaderState() {
	//constant buffer
	if(m_pConstantBuffer) m_pConstantBuffer->SetShaderState();

	//texture
		if (m_BindFlag & BIND_VS) {
			GLOBALVALUEMGR->GetDeviceContext()->VSSetShaderResources(m_TextureStartSlot, 1, &m_pd3dsrvTexture);
		}
		if (m_BindFlag & BIND_DS) {
			GLOBALVALUEMGR->GetDeviceContext()->DSSetShaderResources(m_TextureStartSlot, 1, &m_pd3dsrvTexture);
		}
		if (m_BindFlag & BIND_HS) {
			GLOBALVALUEMGR->GetDeviceContext()->HSSetShaderResources(m_TextureStartSlot, 1, &m_pd3dsrvTexture);
		}
		if (m_BindFlag & BIND_GS) {
			GLOBALVALUEMGR->GetDeviceContext()->GSSetShaderResources(m_TextureStartSlot, 1, &m_pd3dsrvTexture);
		}
		if (m_BindFlag & BIND_PS) {
			GLOBALVALUEMGR->GetDeviceContext()->PSSetShaderResources(m_TextureStartSlot, 1, &m_pd3dsrvTexture);
		}
		if (m_BindFlag & BIND_CS) {
			GLOBALVALUEMGR->GetDeviceContext()->CSSetShaderResources(m_TextureStartSlot, 1, &m_pd3dsrvTexture);
		}
}
void CTexture::CleanShaderState() {
	//constant buffer
	if(m_pConstantBuffer) m_pConstantBuffer->CleanShaderState();

	//texture
	ID3D11ShaderResourceView* pSRVs[1] = { nullptr };
		if (m_BindFlag & BIND_VS) {
			GLOBALVALUEMGR->GetDeviceContext()->VSSetShaderResources(m_TextureStartSlot, 1, pSRVs);
		}
		if (m_BindFlag & BIND_DS) {
			GLOBALVALUEMGR->GetDeviceContext()->DSSetShaderResources(m_TextureStartSlot, 1, pSRVs);
		}
		if (m_BindFlag & BIND_HS) {
			GLOBALVALUEMGR->GetDeviceContext()->HSSetShaderResources(m_TextureStartSlot, 1, pSRVs);
		}
		if (m_BindFlag & BIND_GS) {
			GLOBALVALUEMGR->GetDeviceContext()->GSSetShaderResources(m_TextureStartSlot, 1, pSRVs);
		}
		if (m_BindFlag & BIND_PS) {
			GLOBALVALUEMGR->GetDeviceContext()->PSSetShaderResources(m_TextureStartSlot, 1, pSRVs);
		}
		if (m_BindFlag & BIND_CS) {
			GLOBALVALUEMGR->GetDeviceContext()->CSSetShaderResources(m_TextureStartSlot, 1, pSRVs);
		}

}

void CTexture::UpdateShaderState() {
	//update texture matrix
	if (m_pConstantBuffer) m_pConstantBuffer->UpdateShaderState();
}

void CTexture::SetConstantBuffer(shared_ptr<CBuffer> pConstantBuffer) {
	if (!pConstantBuffer) return;
	m_pConstantBuffer = pConstantBuffer;
}

ID3D11ShaderResourceView* CTexture::CreateTexture2DArraySRV(_TCHAR(*ppstrFilePaths)[128], UINT nTextures){
	
	D3DX11_IMAGE_LOAD_INFO d3dxImageLoadInfo;
	d3dxImageLoadInfo.Width = D3DX11_FROM_FILE;
	d3dxImageLoadInfo.Height = D3DX11_FROM_FILE;
	d3dxImageLoadInfo.Depth = D3DX11_FROM_FILE;
	d3dxImageLoadInfo.FirstMipLevel = 0;
	d3dxImageLoadInfo.MipLevels = D3DX11_FROM_FILE;
	d3dxImageLoadInfo.Usage = D3D11_USAGE_STAGING;
	d3dxImageLoadInfo.BindFlags = 0;
	d3dxImageLoadInfo.CpuAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
	d3dxImageLoadInfo.MiscFlags = 0;
	d3dxImageLoadInfo.Format = DXGI_FORMAT_FROM_FILE; //DXGI_FORMAT_R8G8B8A8_UNORM
	d3dxImageLoadInfo.Filter = D3DX11_FILTER_NONE;
	d3dxImageLoadInfo.MipFilter = D3DX11_FILTER_LINEAR;
	d3dxImageLoadInfo.pSrcInfo = 0;

	ID3D11Texture2D **ppd3dTextures = new ID3D11Texture2D*[nTextures];
	for (UINT i = 0; i < nTextures; i++) D3DX11CreateTextureFromFile(GLOBALVALUEMGR->GetDevice(), ppstrFilePaths[i], &d3dxImageLoadInfo, 0, (ID3D11Resource **)&ppd3dTextures[i], 0);

	//test
	D3D11_TEXTURE2D_DESC* pd3dTexure2DDesc = new D3D11_TEXTURE2D_DESC[nTextures];
	for (UINT i = 0; i < nTextures; i++) {
		ppd3dTextures[i]->GetDesc(&pd3dTexure2DDesc[i]);
	}
	
	//test

	D3D11_TEXTURE2D_DESC d3dTexure2DDesc;
	ppd3dTextures[0]->GetDesc(&d3dTexure2DDesc);

	D3D11_TEXTURE2D_DESC d3dTexture2DArrayDesc;
	d3dTexture2DArrayDesc.Width = d3dTexure2DDesc.Width;
	d3dTexture2DArrayDesc.Height = d3dTexure2DDesc.Height;
	//d3dTexture2DArrayDesc.MipLevels = d3dTexure2DDesc.MipLevels;
	d3dTexture2DArrayDesc.MipLevels = 1;
	d3dTexture2DArrayDesc.ArraySize = nTextures;
	d3dTexture2DArrayDesc.Format = d3dTexure2DDesc.Format;
	d3dTexture2DArrayDesc.SampleDesc.Count = 1;
	d3dTexture2DArrayDesc.SampleDesc.Quality = 0;
	d3dTexture2DArrayDesc.Usage = D3D11_USAGE_DEFAULT;
	d3dTexture2DArrayDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	d3dTexture2DArrayDesc.CPUAccessFlags = 0;
	d3dTexture2DArrayDesc.MiscFlags = 0;

	ID3D11Texture2D *pd3dTexture2DArray;
	GLOBALVALUEMGR->GetDevice()->CreateTexture2D(&d3dTexture2DArrayDesc, 0, &pd3dTexture2DArray);

	ID3D11DeviceContext *pd3dDeviceContext;
	GLOBALVALUEMGR->GetDevice()->GetImmediateContext(&pd3dDeviceContext);

	D3D11_MAPPED_SUBRESOURCE d3dMappedTexture2D;
	for (UINT t = 0; t < nTextures; t++)
	{
		for (UINT m = 0; m < d3dTexure2DDesc.MipLevels; m++)
		{
			pd3dDeviceContext->Map(ppd3dTextures[t], m, D3D11_MAP_READ, 0, &d3dMappedTexture2D);
			pd3dDeviceContext->UpdateSubresource(	  pd3dTexture2DArray
													, D3D11CalcSubresource(m, t, d3dTexure2DDesc.MipLevels)
													, 0
													, d3dMappedTexture2D.pData
													, d3dMappedTexture2D.RowPitch
													, d3dMappedTexture2D.DepthPitch
			);
			pd3dDeviceContext->Unmap(ppd3dTextures[t], m);
		}
	}


	D3D11_SHADER_RESOURCE_VIEW_DESC d3dTextureSRVDesc;
	ZeroMemory(&d3dTextureSRVDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	d3dTextureSRVDesc.Format = d3dTexture2DArrayDesc.Format;
	d3dTextureSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	d3dTextureSRVDesc.Texture2DArray.MostDetailedMip = 0;
	d3dTextureSRVDesc.Texture2DArray.MipLevels = d3dTexture2DArrayDesc.MipLevels;
	d3dTextureSRVDesc.Texture2DArray.FirstArraySlice = 0;
	d3dTextureSRVDesc.Texture2DArray.ArraySize = nTextures;

	ID3D11ShaderResourceView *pd3dsrvTextureArray;
	GLOBALVALUEMGR->GetDevice()->CreateShaderResourceView(pd3dTexture2DArray, &d3dTextureSRVDesc, &pd3dsrvTextureArray);

	if (pd3dTexture2DArray) pd3dTexture2DArray->Release();

	for (UINT i = 0; i < nTextures; i++) if (ppd3dTextures[i]) ppd3dTextures[i]->Release();
	delete[] ppd3dTextures;
	//test
	delete[] pd3dTexure2DDesc;

	if (pd3dDeviceContext) pd3dDeviceContext->Release();

	return(pd3dsrvTextureArray);
}

ID3D11ShaderResourceView* CTexture::CreateTexture2DArraySRV(ID3D11Texture2D **ppd3dTextures, UINT nTextures) {

	D3DX11_IMAGE_LOAD_INFO d3dxImageLoadInfo;
	d3dxImageLoadInfo.Width = D3DX11_FROM_FILE;
	d3dxImageLoadInfo.Height = D3DX11_FROM_FILE;
	d3dxImageLoadInfo.Depth = D3DX11_FROM_FILE;
	d3dxImageLoadInfo.FirstMipLevel = 0;
	d3dxImageLoadInfo.MipLevels = D3DX11_FROM_FILE;
	d3dxImageLoadInfo.Usage = D3D11_USAGE_STAGING;
	d3dxImageLoadInfo.BindFlags = 0;
	d3dxImageLoadInfo.CpuAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
	d3dxImageLoadInfo.MiscFlags = 0;
	d3dxImageLoadInfo.Format = DXGI_FORMAT_FROM_FILE; //DXGI_FORMAT_R8G8B8A8_UNORM
	d3dxImageLoadInfo.Filter = D3DX11_FILTER_NONE;
	d3dxImageLoadInfo.MipFilter = D3DX11_FILTER_LINEAR;
	d3dxImageLoadInfo.pSrcInfo = 0;

	D3D11_TEXTURE2D_DESC d3dTexure2DDesc;
	ppd3dTextures[0]->GetDesc(&d3dTexure2DDesc);

	D3D11_TEXTURE2D_DESC d3dTexture2DArrayDesc;
	d3dTexture2DArrayDesc.Width = d3dTexure2DDesc.Width;
	d3dTexture2DArrayDesc.Height = d3dTexure2DDesc.Height;
	d3dTexture2DArrayDesc.MipLevels = d3dTexure2DDesc.MipLevels;
	d3dTexture2DArrayDesc.ArraySize = nTextures;
	d3dTexture2DArrayDesc.Format = d3dTexure2DDesc.Format;
	d3dTexture2DArrayDesc.SampleDesc.Count = 1;
	d3dTexture2DArrayDesc.SampleDesc.Quality = 0;
	d3dTexture2DArrayDesc.Usage = D3D11_USAGE_DEFAULT;
	d3dTexture2DArrayDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	d3dTexture2DArrayDesc.CPUAccessFlags = 0;
	d3dTexture2DArrayDesc.MiscFlags = 0;

	ID3D11Texture2D *pd3dTexture2DArray;
	GLOBALVALUEMGR->GetDevice()->CreateTexture2D(&d3dTexture2DArrayDesc, 0, &pd3dTexture2DArray);

	D3D11_MAPPED_SUBRESOURCE d3dMappedTexture2D;
	for (UINT t = 0; t < nTextures; t++)
	{
		for (UINT m = 0; m < d3dTexure2DDesc.MipLevels; m++)
		{
			//? map 왜 함? 병신임?
			GLOBALVALUEMGR->GetDeviceContext()->Map(ppd3dTextures[t], m, D3D11_MAP_READ, 0, &d3dMappedTexture2D);
			GLOBALVALUEMGR->GetDeviceContext()->UpdateSubresource(pd3dTexture2DArray, D3D11CalcSubresource(m, t, d3dTexure2DDesc.MipLevels), 0, d3dMappedTexture2D.pData, d3dMappedTexture2D.RowPitch, d3dMappedTexture2D.DepthPitch);
			GLOBALVALUEMGR->GetDeviceContext()->Unmap(ppd3dTextures[t], m);
		}
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC d3dTextureSRVDesc;
	ZeroMemory(&d3dTextureSRVDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	d3dTextureSRVDesc.Format = d3dTexture2DArrayDesc.Format;
	d3dTextureSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	d3dTextureSRVDesc.Texture2DArray.MostDetailedMip = 0;
	d3dTextureSRVDesc.Texture2DArray.MipLevels = d3dTexture2DArrayDesc.MipLevels;
	d3dTextureSRVDesc.Texture2DArray.FirstArraySlice = 0;
	d3dTextureSRVDesc.Texture2DArray.ArraySize = nTextures;

	ID3D11ShaderResourceView *pd3dsrvTextureArray;
	GLOBALVALUEMGR->GetDevice()->CreateShaderResourceView(pd3dTexture2DArray, &d3dTextureSRVDesc, &pd3dsrvTextureArray);

	if (pd3dTexture2DArray) pd3dTexture2DArray->Release();

	for (UINT i = 0; i < nTextures; i++) if (ppd3dTextures[i]) ppd3dTextures[i]->Release();
	delete[] ppd3dTextures;

	return(pd3dsrvTextureArray);
}

ID3D11ShaderResourceView * CTexture::CreateTexture2DArraySRV(ID3D11ShaderResourceView ** ppd3dSRVs, UINT nTextures){
	//make textures
	ID3D11Texture2D** ppd3dTextures = new ID3D11Texture2D*[nTextures];
	ID3D11Resource* pResource;
	for (int i = 0; i < nTextures; ++i) {
		ppd3dSRVs[i]->GetResource(&pResource);
		ppd3dTextures[i] = (ID3D11Texture2D*)(pResource);
	}
	//make textures

	D3DX11_IMAGE_LOAD_INFO d3dxImageLoadInfo;
	d3dxImageLoadInfo.Width = D3DX11_FROM_FILE;
	d3dxImageLoadInfo.Height = D3DX11_FROM_FILE;
	d3dxImageLoadInfo.Depth = D3DX11_FROM_FILE;
	d3dxImageLoadInfo.FirstMipLevel = 0;
	d3dxImageLoadInfo.MipLevels = D3DX11_FROM_FILE;
	d3dxImageLoadInfo.Usage = D3D11_USAGE_STAGING;
	d3dxImageLoadInfo.BindFlags = 0;
	d3dxImageLoadInfo.CpuAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
	d3dxImageLoadInfo.MiscFlags = 0;
	d3dxImageLoadInfo.Format = DXGI_FORMAT_FROM_FILE; //DXGI_FORMAT_R8G8B8A8_UNORM
	d3dxImageLoadInfo.Filter = D3DX11_FILTER_NONE;
	d3dxImageLoadInfo.MipFilter = D3DX11_FILTER_LINEAR;
	d3dxImageLoadInfo.pSrcInfo = 0;

	D3D11_TEXTURE2D_DESC d3dTexure2DDesc;
	ppd3dTextures[0]->GetDesc(&d3dTexure2DDesc);

	D3D11_TEXTURE2D_DESC d3dTexture2DArrayDesc;
	d3dTexture2DArrayDesc.Width = d3dTexure2DDesc.Width;
	d3dTexture2DArrayDesc.Height = d3dTexure2DDesc.Height;
	d3dTexture2DArrayDesc.MipLevels = d3dTexure2DDesc.MipLevels;
	d3dTexture2DArrayDesc.ArraySize = nTextures;
	d3dTexture2DArrayDesc.Format = d3dTexure2DDesc.Format;
	d3dTexture2DArrayDesc.SampleDesc.Count = 1;
	d3dTexture2DArrayDesc.SampleDesc.Quality = 0;
	d3dTexture2DArrayDesc.Usage = D3D11_USAGE_DEFAULT;
	d3dTexture2DArrayDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	d3dTexture2DArrayDesc.CPUAccessFlags = 0;
	d3dTexture2DArrayDesc.MiscFlags = 0;

	ID3D11Texture2D *pd3dTexture2DArray;
	GLOBALVALUEMGR->GetDevice()->CreateTexture2D(&d3dTexture2DArrayDesc, 0, &pd3dTexture2DArray);

	D3D11_MAPPED_SUBRESOURCE d3dMappedTexture2D;
	for (UINT t = 0; t < nTextures; t++)
	{
		for (UINT m = 0; m < d3dTexure2DDesc.MipLevels; m++)
		{
			//? map 왜 함? 병신임?
			//map을 하는데 기존의 texture를 mapped_subresource로 map함
			GLOBALVALUEMGR->GetDeviceContext()->Map(ppd3dTextures[t], m, D3D11_MAP_READ, 0, &d3dMappedTexture2D);
			GLOBALVALUEMGR->GetDeviceContext()->UpdateSubresource(
				pd3dTexture2DArray, D3D11CalcSubresource(m, t, d3dTexure2DDesc.MipLevels), 
				0, d3dMappedTexture2D.pData, d3dMappedTexture2D.RowPitch, d3dMappedTexture2D.DepthPitch);
			GLOBALVALUEMGR->GetDeviceContext()->Unmap(ppd3dTextures[t], m);
		}
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC d3dTextureSRVDesc;
	ZeroMemory(&d3dTextureSRVDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	d3dTextureSRVDesc.Format = d3dTexture2DArrayDesc.Format;
	d3dTextureSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	d3dTextureSRVDesc.Texture2DArray.MostDetailedMip = 0;
	d3dTextureSRVDesc.Texture2DArray.MipLevels = d3dTexture2DArrayDesc.MipLevels;
	d3dTextureSRVDesc.Texture2DArray.FirstArraySlice = 0;
	d3dTextureSRVDesc.Texture2DArray.ArraySize = nTextures;

	ID3D11ShaderResourceView *pd3dsrvTextureArray;
	GLOBALVALUEMGR->GetDevice()->CreateShaderResourceView(pd3dTexture2DArray, &d3dTextureSRVDesc, &pd3dsrvTextureArray);

	if (pd3dTexture2DArray) pd3dTexture2DArray->Release();

	for (UINT i = 0; i < nTextures; i++) if (ppd3dTextures[i]) ppd3dTextures[i]->Release();
	delete[] ppd3dTextures;

	return(pd3dsrvTextureArray);
}

shared_ptr<CTexture> CTexture::CreateTexture(UINT nTextures, _TCHAR(*ppstrFilePaths)[128], UINT Slot, UINT BindFlag, shared_ptr<CBuffer> pConstantBuffer){

	shared_ptr<CTexture> pTexture = make_shared<CTexture>();
	//constant buffer
	pTexture->SetConstantBuffer(pConstantBuffer);
	//texture
	pTexture->SetTextureSlot(Slot);
	pTexture->SetpTextureSRV(CreateTexture2DArraySRV(ppstrFilePaths, nTextures));
	pTexture->SetBindFlag(BindFlag);

	return pTexture;
}

//shared_ptr<CTexture> CTexture::CreateTexture(UINT nTextures, WCHAR(*ppstrFilePaths)[128], shared_ptr<CSampler> pSampler, UINT Slot, UINT BindFlag, shared_ptr<CBuffer> pConstantBuffer){
//	shared_ptr<CTexture> pTexture = make_shared<CTexture>();
//	//sampler
//	pTexture->SetSampler(pSampler);
//	//constant buffer
//	pTexture->SetConstantBuffer(pConstantBuffer);
//	//texture
//	pTexture->SetTextureSlot(Slot);
//	pTexture->SetpTextureSRV(CreateTexture2DArraySRV(ppstrFilePaths, nTextures));
//	pTexture->SetBindFlag(BindFlag);
//
//	return pTexture;
//}

shared_ptr<CTexture> CTexture::CreateTexture(UINT nTextures, ID3D11Texture2D ** ppd3dTextures, UINT Slot, UINT BindFlag, shared_ptr<CBuffer> pConstantBuffer){
	shared_ptr<CTexture> pTexture = make_shared<CTexture>();
	//constant buffer
	pTexture->SetConstantBuffer(pConstantBuffer);
	//texture
	pTexture->SetTextureSlot(Slot);
	pTexture->SetpTextureSRV(CreateTexture2DArraySRV(ppd3dTextures, nTextures));
	pTexture->SetBindFlag(BindFlag);

	return pTexture;
}

shared_ptr<CTexture> CTexture::CreateTexture(UINT nTextures, ID3D11ShaderResourceView ** ppd3dSRVs, UINT Slot, UINT BindFlag, shared_ptr<CBuffer> pConstantBuffer){
	shared_ptr<CTexture> pTexture = make_shared<CTexture>();
	//constant buffer
	pTexture->SetConstantBuffer(pConstantBuffer);
	//texture
	pTexture->SetTextureSlot(Slot);
	pTexture->SetpTextureSRV(CreateTexture2DArraySRV(ppd3dSRVs, nTextures));
	pTexture->SetBindFlag(BindFlag);

	return pTexture;
}

shared_ptr<CTexture> CTexture::CreateTexture(_TCHAR(pstrFilePath)[128], UINT Slot, UINT BindFlag, shared_ptr<CBuffer> pConstantBuffer){
	wstring wpath{ pstrFilePath };
	string path; path.assign(wpath.cbegin(), wpath.cend());

	shared_ptr<CTexture> pTexture = make_shared<CTexture>();
	pTexture->SetsPath(path);
	//constant buffer
	pTexture->SetConstantBuffer(pConstantBuffer);
	//texture
	pTexture->SetTextureSlot(Slot);
	ID3D11ShaderResourceView* pd3dsrvTexture{ nullptr };
	D3DX11CreateShaderResourceViewFromFile(GLOBALVALUEMGR->GetDevice(), pstrFilePath, NULL, NULL, &pd3dsrvTexture, NULL);
	if (nullptr == pd3dsrvTexture) return nullptr;
	pTexture->SetpTextureSRV(pd3dsrvTexture);
	pTexture->SetBindFlag(BindFlag);

	return pTexture;
}

shared_ptr<CTexture> CTexture::CreateTexture(wstring pstrFilePath, UINT Slot, UINT BindFlag, shared_ptr<CBuffer> pConstantBuffer){
	wstring wpath{ pstrFilePath };
	string path; path.assign(wpath.cbegin(), wpath.cend());

	shared_ptr<CTexture> pTexture = make_shared<CTexture>();
	pTexture->SetsPath(path);
	//constant buffer
	pTexture->SetConstantBuffer(pConstantBuffer);
	//texture
	pTexture->SetTextureSlot(Slot);
	ID3D11ShaderResourceView* pd3dsrvTexture{ nullptr };
	D3DX11CreateShaderResourceViewFromFile(GLOBALVALUEMGR->GetDevice(), pstrFilePath.c_str(), NULL, NULL, &pd3dsrvTexture, NULL);
	pTexture->SetpTextureSRV(pd3dsrvTexture);
	pTexture->SetBindFlag(BindFlag);

	return pTexture;
}

shared_ptr<CTexture> CTexture::CreateTexture(ID3D11ShaderResourceView * pShaderResourceView, UINT Slot, UINT BindFlag, shared_ptr<CBuffer> pConstantBuffer){
	shared_ptr<CTexture> pTexture = make_shared<CTexture>();
	//constant buffer
	pTexture->SetConstantBuffer(pConstantBuffer);
	//texture
	pTexture->SetTextureSlot(Slot);
	pTexture->SetpTextureSRV(pShaderResourceView);
	pTexture->SetBindFlag(BindFlag);

	return pTexture;
}


CTexture::CTexture() : DXObject("texture") { }
CTexture::~CTexture() { };

