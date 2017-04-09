#include "stdafx.h"
#include "StaticBuffer.h"

bool CStaticBuffer::Begin(UINT Object, UINT BufferStride, void* pData, UINT Slot, UINT BindFlag, UINT Offset) {

	m_Slot = Slot;
	m_BindFlag = BindFlag;
	m_nObject = Object;
	m_BufferStride = BufferStride;
	m_ByteWidth = m_nObject * m_BufferStride;
	m_Offset = Offset;

	//create buffer
	m_pd3dBuffer = CreateStaticBuffer(m_ByteWidth, pData);

	return true;
}

bool CStaticBuffer::End() {

	return true;
}

void CStaticBuffer::SetShaderState() {

	ID3D11Buffer* pBuffers[1] = { m_pd3dBuffer };

	if (m_BindFlag & BIND_VS) {
		GLOBALVALUEMGR->GetDeviceContext()->VSSetConstantBuffers(m_Slot, 1, pBuffers);
	}
	if (m_BindFlag & BIND_DS) {
		GLOBALVALUEMGR->GetDeviceContext()->DSSetConstantBuffers(m_Slot, 1, pBuffers);
	}
	if (m_BindFlag & BIND_HS) {
		GLOBALVALUEMGR->GetDeviceContext()->HSSetConstantBuffers(m_Slot, 1, pBuffers);
	}
	if (m_BindFlag & BIND_GS) {
		GLOBALVALUEMGR->GetDeviceContext()->GSSetConstantBuffers(m_Slot, 1, pBuffers);
	}
	if (m_BindFlag & BIND_PS) {
		GLOBALVALUEMGR->GetDeviceContext()->PSSetConstantBuffers(m_Slot, 1, pBuffers);
	}
	if (m_BindFlag & BIND_CS) {
		GLOBALVALUEMGR->GetDeviceContext()->CSSetConstantBuffers(m_Slot, 1, pBuffers);
	}
}
void CStaticBuffer::CleanShaderState() {

	ID3D11Buffer* pBuffers[1] = { nullptr };

	if (m_BindFlag & BIND_VS) {
		GLOBALVALUEMGR->GetDeviceContext()->VSSetConstantBuffers(m_Slot, 1, pBuffers);
	}
	if (m_BindFlag & BIND_DS) {
		GLOBALVALUEMGR->GetDeviceContext()->DSSetConstantBuffers(m_Slot, 1, pBuffers);
	}
	if (m_BindFlag & BIND_HS) {
		GLOBALVALUEMGR->GetDeviceContext()->HSSetConstantBuffers(m_Slot, 1, pBuffers);
	}
	if (m_BindFlag & BIND_GS) {
		GLOBALVALUEMGR->GetDeviceContext()->GSSetConstantBuffers(m_Slot, 1, pBuffers);
	}
	if (m_BindFlag & BIND_PS) {
		GLOBALVALUEMGR->GetDeviceContext()->PSSetConstantBuffers(m_Slot, 1, pBuffers);
	}
	if (m_BindFlag & BIND_CS) {
		GLOBALVALUEMGR->GetDeviceContext()->CSSetConstantBuffers(m_Slot, 1, pBuffers);
	}


}

void CStaticBuffer::UpdateShaderState() {

}


ID3D11Buffer * CStaticBuffer::CreateStaticBuffer(UINT nByteWidth, void * pData){

	ID3D11Buffer* pd3dBuffer;
	D3D11_BUFFER_DESC d3dBufferDesc;
	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	
	d3dBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	d3dBufferDesc.ByteWidth = nByteWidth;
	d3dBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	d3dBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	if (pData == nullptr) {
		GLOBALVALUEMGR->GetDevice()->CreateBuffer(&d3dBufferDesc, nullptr, &pd3dBuffer);
	}
	else {
		D3D11_SUBRESOURCE_DATA d3dBufferData;
		ZeroMemory(&d3dBufferData, sizeof(D3D11_SUBRESOURCE_DATA));
		d3dBufferData.pSysMem = pData;
		GLOBALVALUEMGR->GetDevice()->CreateBuffer(&d3dBufferDesc, &d3dBufferData, &pd3dBuffer);
	}
	return pd3dBuffer;

}





CStaticBuffer::CStaticBuffer() : CConstantBuffer() { }
CStaticBuffer::~CStaticBuffer() { };

