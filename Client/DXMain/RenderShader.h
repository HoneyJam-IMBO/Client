#pragma once
#include "Shader.h"

enum eInputElement {
	IE_POSITION			= 0b0000000000000001,
	IE_NORMAL			= 0b0000000000000010,
	IE_TEXCOORD			= 0b0000000000000100,
	IE_TANGENT			= 0b0000000000001000,
	IE_BITANGENT		= 0b0000000000010000,
	IE_BONEWEIGHT		= 0b0000000000100000,
	IE_BONEINDEX		= 0b0000000001000000,
	IE_INSWORLDMTX		= 0b0000000010000000,
	IE_INSPOS			= 0b0000000100000000,
	IE_INSQUATERNION	= 0b0000001000000000,
	IE_INSFLOAT_B_A		= 0b0000010000000000,//float2
	IE_INSFLOAT_B_B		= 0b0000100000000000,
	IE_INSFLOAT_B_C		= 0b0001000000000000,
	IE_INSFLOAT_C_A		= 0b0010000000000000,//float3
	IE_INSFLOAT_C_B		= 0b0100000000000000,
	IE_INSFLOAT_C_C		= 0b1000000000000000
};
class CRenderShader : public CShader{
public:
	//----------------------------dxobject-----------------------------
	bool Begin();
	bool End();

	void SetShaderState();
	void CleanShaderState();
	//----------------------------dxobject-----------------------------

	static shared_ptr<CRenderShader> CreateRenderShader(LPCTSTR ShaderName, UINT InputElementFlag = 0, UINT BindFlag = BIND_VS | BIND_PS);
	static shared_ptr<CRenderShader> CreateRenderShader(LPCTSTR* ShaderNames, UINT InputElementFlag = 0, UINT BindFlag = BIND_VS | BIND_PS);
protected:
	//-------------------------------shaders-----------------------------
	ID3D11VertexShader* m_vertexShader{ nullptr };
	ID3D11InputLayout* m_inputLayout{ nullptr };

	ID3D11GeometryShader* m_geometryShader{ nullptr };
	ID3D11HullShader* m_hullShader{ nullptr };
	ID3D11DomainShader* m_domainShader{ nullptr };
	
	ID3D11PixelShader* m_pixelShader{ nullptr };
	//-------------------------------shaders-----------------------------
	
	void SetShaderData(LPCTSTR vsName, D3D11_INPUT_ELEMENT_DESC* vsDesc, UINT nElement, LPCTSTR gsName, LPCTSTR hsName,
		LPCTSTR dsName, LPCTSTR psName);
	//begin func
	bool CreateVS(LPCTSTR name, D3D11_INPUT_ELEMENT_DESC* vsDesc = nullptr, UINT nElement = 0);
	bool CreateVS(LPCTSTR name, UINT InputElementFlag);

	bool CreateGS(LPCTSTR name);

	bool CreateHS(LPCTSTR name);
	bool CreateDS(LPCTSTR name);

	bool CreatePS(LPCTSTR name);
	//begin func

public:
	CRenderShader();
	virtual ~CRenderShader();
};
