#include "stdafx.h"
#include "ConstantBuffer.h"

bool CConstantBuffer::Begin(UINT Object, UINT BufferStride, UINT Slot, UINT BindFlag, UINT Offset) {

	m_Slot = Slot;
	m_BindFlag = BindFlag;
	m_nObject = Object;
	m_BufferStride = BufferStride;
	m_ByteWidth = m_nObject * m_BufferStride;
	m_Offset = Offset;

	//create buffer
	m_pd3dBuffer = CreateConstantBuffer(m_ByteWidth);

	return true;
}

bool CConstantBuffer::End() {

	return true;
}

void CConstantBuffer::SetShaderState() {

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
void CConstantBuffer::CleanShaderState() {
	
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

void CConstantBuffer::UpdateShaderState(){

}





ID3D11Buffer* CConstantBuffer::CreateConstantBuffer(UINT nByteWidth){
	
	ID3D11Buffer* pd3dBuffer;
	D3D11_BUFFER_DESC d3dBufferDesc;
	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	/*버퍼의 초기화 데이터가 없으면 동적 버퍼로 생성한다. 즉, 나중에 매핑을 하여 내용을 채우거나 변경한다.*/
	d3dBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	d3dBufferDesc.ByteWidth = nByteWidth;
	d3dBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	d3dBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	
	//초기화 data 없음!
	GLOBALVALUEMGR->GetDevice()->CreateBuffer(&d3dBufferDesc, NULL, &pd3dBuffer);

	return pd3dBuffer;
}


CConstantBuffer::CConstantBuffer() : CBuffer() { }
CConstantBuffer::~CConstantBuffer() { };

