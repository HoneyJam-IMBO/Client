#include "stdafx.h"
#include "Material.h"

bool CMaterial::Begin() {
	
	return true; 
}
bool CMaterial::End() { 
	if (m_pMaterialBuffer) m_pMaterialBuffer->Release();

	return true; 
}

void CMaterial::SetShaderState() {
	GLOBALVALUEMGR->GetDeviceContext()->PSSetConstantBuffers(PS_CB_MATERIAL, 1, &m_pMaterialBuffer);
}

void CMaterial::CleanShaderState() {

}

void CMaterial::UpdateShaderState() {
	//m_pd3dDeviceContext->UpdateSubresource(m_pMaterialBuffer, 0, nullptr, &m_infoMaterial, 0, 0);
}

shared_ptr<CMaterial> CMaterial::CreateMaterial(XMFLOAT4 color, float specExp, float specIntensity){
	shared_ptr<CMaterial> pMaterial = make_shared<CMaterial>();
	MATERIAL_INFO info;
	info.diffuseColor = color;
	info.specData.x = specExp;
	info.specData.y = specIntensity;
	pMaterial->SetMaterialInfo(info);

	ID3D11Buffer* pMaterialBuffer = CreateBuffer(sizeof(MATERIAL_INFO), 1, &info, D3D11_BIND_CONSTANT_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
	pMaterial->SetMaterialBuffer(pMaterialBuffer);

	return pMaterial;
}


CMaterial::CMaterial() : DXObject("material"){ }
CMaterial::~CMaterial() { };

