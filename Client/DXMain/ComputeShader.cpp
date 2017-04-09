#include "stdafx.h"
#include "ComputeShader.h"


bool CComputeShader::Begin() {
	if (false == CreateCS()) return false;
	
	if (false == CreateShaderValues()) return false;

	return true;
}
bool CComputeShader::End() {
	if (m_computeShader)		m_computeShader->Release();
	
	if(false == ReleaseShaderValues()) return false;

	return true;
}

CComputeShader* CComputeShader::CreateComputeShader(LPCTSTR path){
	CComputeShader* pComputeShader = new CComputeShader();
	ID3D11ComputeShader* pd3dComputeShader{ nullptr };;
	auto loadVS = ReadCSOFile(path);
	{
		GLOBALVALUEMGR->GetDevice()->CreateComputeShader(
			&(loadVS[0])
			, loadVS.size()
			, nullptr
			, &pd3dComputeShader
		);

	}
	pComputeShader->SetpComputeShader(pd3dComputeShader);
	if (pd3dComputeShader) return pComputeShader;

	delete pComputeShader;
	return pComputeShader;
}

void CComputeShader::SetShaderState() {
	// cs
	if (m_computeShader) {
		GLOBALVALUEMGR->GetDeviceContext()->CSSetShader(m_computeShader, nullptr, 0);
	}

	//virtual
	SetShaderValues();
}
void CComputeShader::CleanShaderState() {
	// cs
	if (m_computeShader) {
		GLOBALVALUEMGR->GetDeviceContext()->CSSetShader(nullptr, nullptr, 0);
	}
	
	//virtual
	CleanShaderValues();
}

void CComputeShader::UpdateShaderState() {

}

void CComputeShader::ExcuteShaderState(){
	UpdateShaderState();
	SetShaderState();
	GLOBALVALUEMGR->GetDeviceContext()->Dispatch(m_ThreadGroupX, m_ThreadGroupY, m_ThreadGroupZ);
	CleanShaderState();
}

void CComputeShader::ExcuteShaderState(UINT threadGroupX, UINT threadGroupY, UINT threadGroupZ){
	UpdateShaderState();
	SetShaderState();
	GLOBALVALUEMGR->GetDeviceContext()->Dispatch(threadGroupX, threadGroupY, threadGroupZ);
	CleanShaderState();
}

void CComputeShader::ResizeBuffer(UINT nWidth, UINT nHeight) {
	m_nBufferWidth = nWidth;
	m_nBufferHeight = nHeight;
}

void CComputeShader::SetShaderValues()
{
}

void CComputeShader::CleanShaderValues()
{
}

void CComputeShader::SetThreadGroup(UINT x, UINT y, UINT z)
{
	m_ThreadGroupX = x;
	m_ThreadGroupY = y;
	m_ThreadGroupZ = z;
}


CComputeShader::CComputeShader() : CShader() {

}
CComputeShader::~CComputeShader() {

}