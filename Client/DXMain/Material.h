#pragma once
#include "DXObject.h"
#include "Texture.h"

//재질은 texture와 재질 정보를 갖는 cbuffer를 가진다.
//texture - srv
//sampler
//material struct
struct MATERIAL_INFO {

	XMFLOAT4 diffuseColor;
	XMFLOAT4 specData;

};

class CMaterial : public DXObject {
public:
	bool Begin();
	virtual bool End();

	virtual void SetShaderState();
	virtual void CleanShaderState();

	virtual void UpdateShaderState();

	void SetMaterialBuffer(ID3D11Buffer* pMaterialBuffer) { m_pMaterialBuffer = pMaterialBuffer; }
	void SetMaterialInfo(MATERIAL_INFO infoMaterial) { m_infoMaterial = infoMaterial; }

	static shared_ptr<CMaterial> CreateMaterial(XMFLOAT4 color, float specExp, float specIntensity);
protected:
	ID3D11Buffer* m_pMaterialBuffer{ nullptr };

	MATERIAL_INFO m_infoMaterial{};

public:
	CMaterial();
	virtual ~CMaterial();

};
