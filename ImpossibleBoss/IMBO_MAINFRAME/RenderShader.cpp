#include "stdafx.h"
#include "RenderShader.h"


bool CRenderShader::Begin() {
	
	return true;
}
bool CRenderShader::End() {
	if (m_vertexShader)			m_vertexShader->Release();
	if (m_geometryShader)		m_geometryShader->Release();
	if (m_hullShader)			m_hullShader->Release();
	if (m_domainShader)			m_domainShader->Release();
	if (m_pixelShader)			m_pixelShader->Release();

	if (m_inputLayout)			m_inputLayout->Release();

	return true;
}

void CRenderShader::SetShaderState() {
	// Input Layout
	GLOBALVALUEMGR->GetDeviceContext()->IASetInputLayout(m_inputLayout);

	// vs
	if (m_vertexShader) {
		GLOBALVALUEMGR->GetDeviceContext()->VSSetShader(m_vertexShader, nullptr, 0);
	}

	// gs
	if (m_geometryShader) {
		GLOBALVALUEMGR->GetDeviceContext()->GSSetShader(m_geometryShader, nullptr, 0);
	}

	// hs
	if (m_hullShader) {
		GLOBALVALUEMGR->GetDeviceContext()->HSSetShader(m_hullShader, nullptr, 0);
	}

	//ds 
	if (m_domainShader) {
		GLOBALVALUEMGR->GetDeviceContext()->DSSetShader(m_domainShader, nullptr, 0);
	}

	// ps
	if (m_pixelShader) {
		GLOBALVALUEMGR->GetDeviceContext()->PSSetShader(m_pixelShader, nullptr, 0);
	}
}
void CRenderShader::CleanShaderState() {
	// ps
	if (m_vertexShader) {
		GLOBALVALUEMGR->GetDeviceContext()->VSSetShader(nullptr, nullptr, 0);
	}

	// gs
	if (m_geometryShader) {
		GLOBALVALUEMGR->GetDeviceContext()->GSSetShader(nullptr, nullptr, 0);
	}

	// hs
	if (m_hullShader) {
		GLOBALVALUEMGR->GetDeviceContext()->HSSetShader(nullptr, nullptr, 0);
	}

	//ds
	if (m_domainShader) {
		GLOBALVALUEMGR->GetDeviceContext()->DSSetShader(nullptr, nullptr, 0);
	}

	// ps
	if (m_pixelShader) {
		GLOBALVALUEMGR->GetDeviceContext()->PSSetShader(nullptr, nullptr, 0);
	}
}


bool CRenderShader::CreateVS(LPCTSTR name, D3D11_INPUT_ELEMENT_DESC* vsDesc, UINT nElement){
	auto loadShader = ReadCSOFile(name);
	{
		GLOBALVALUEMGR->GetDevice()->CreateVertexShader(
			&(loadShader[0])
			, loadShader.size()
			, nullptr
			, &m_vertexShader
		);

		if (vsDesc) {
			GLOBALVALUEMGR->GetDevice()->CreateInputLayout(
				vsDesc
				, nElement
				, &(loadShader[0])
				, loadShader.size()
				, &m_inputLayout
			);
		}
	}

	if (m_vertexShader) return true;

	return false;
}

bool CRenderShader::CreateVS(LPCTSTR name, UINT InputElementFlag){
	auto loadShader = ReadCSOFile(name);
	{
		GLOBALVALUEMGR->GetDevice()->CreateVertexShader(
			&(loadShader[0])
			, loadShader.size()
			, nullptr
			, &m_vertexShader
		);

		vector<D3D11_INPUT_ELEMENT_DESC> vDesc;
		UINT nElement{ 0 };
		UINT nInstanceElement{ 0 };
		if (InputElementFlag & IE_POSITION) {
			vDesc.push_back({ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, nElement++, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		}
		if (InputElementFlag& IE_NORMAL) {
			vDesc.push_back({ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, nElement++, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		}
		if (InputElementFlag& IE_TEXCOORD) {
			vDesc.push_back({ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, nElement++, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		}
		if (InputElementFlag& IE_TANGENT) {
			vDesc.push_back({ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, nElement++, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		}
		if (InputElementFlag& IE_BINORMAL) {
			vDesc.push_back({ "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, nElement++, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		}
		if (InputElementFlag& IE_BONEWEIGHT) {
			vDesc.push_back({ "BONE_WEIGHT", 0, DXGI_FORMAT_R32G32B32_FLOAT, nElement++, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		}
		if (InputElementFlag& IE_BONEINDEX) {
			vDesc.push_back({ "BONE_INDEX", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, nElement++, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		}
		if (InputElementFlag& IE_INSWORLDMTX) {
			vDesc.push_back({ "INSWORLDMTX", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, nElement, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 });
			nInstanceElement++;
			vDesc.push_back({ "INSWORLDMTX", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, nElement, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 });
			nInstanceElement++;
			vDesc.push_back({ "INSWORLDMTX", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, nElement, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 });
			nInstanceElement++;
			vDesc.push_back({ "INSWORLDMTX", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, nElement, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 });
			nInstanceElement++;
		}
		if (InputElementFlag& IE_INSPOS) {
			if (nInstanceElement == 0) {
				vDesc.push_back({ "INSPOS", 0, DXGI_FORMAT_R32G32B32_FLOAT, nElement, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 });
			}
			else {
				vDesc.push_back({ "INSPOS", 0, DXGI_FORMAT_R32G32B32_FLOAT, nElement, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 });
			}
			nInstanceElement++;
		}
		if (InputElementFlag& IE_INSQUATERNION) {
			if (nInstanceElement == 0) {
				vDesc.push_back({ "INSQUATERNION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, nElement, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 });
			}
			else {
				vDesc.push_back({ "INSQUATERNION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, nElement, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 });
			}
			nInstanceElement++;
		}
		if (InputElementFlag& IE_INSFLOAT_B_A) {
			if (nInstanceElement == 0) {
				vDesc.push_back({ "INSFLOAT_B_A", 0, DXGI_FORMAT_R32G32_FLOAT, nElement, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 });
			}
			else {
				vDesc.push_back({ "INSFLOAT_B_A", 0, DXGI_FORMAT_R32G32_FLOAT, nElement, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 });
			}
			nInstanceElement++;
		}
		if (InputElementFlag& IE_INSFLOAT_B_B) {
			if (nInstanceElement == 0) {
				vDesc.push_back({ "INSFLOAT_B_B", 0, DXGI_FORMAT_R32G32_FLOAT, nElement, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 });
			}
			else {
				vDesc.push_back({ "INSFLOAT_B_B", 0, DXGI_FORMAT_R32G32_FLOAT, nElement, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 });
			}
			nInstanceElement++;
		}
		if (InputElementFlag& IE_INSFLOAT_B_C) {
			if (nInstanceElement == 0) {
				vDesc.push_back({ "INSFLOAT_B_C", 0, DXGI_FORMAT_R32G32_FLOAT, nElement, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 });
			}
			else {
				vDesc.push_back({ "INSFLOAT_B_C", 0, DXGI_FORMAT_R32G32_FLOAT, nElement, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 });
			}
			nInstanceElement++;
		}
		if (InputElementFlag& IE_INSFLOAT_C_A) {
			if (nInstanceElement == 0) {
				vDesc.push_back({ "INSFLOAT_C_A", 0, DXGI_FORMAT_R32G32B32_FLOAT, nElement, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 });
			}
			else {
				vDesc.push_back({ "INSFLOAT_C_A", 0, DXGI_FORMAT_R32G32B32_FLOAT, nElement, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 });
			}
			nInstanceElement++;
		}
		if (InputElementFlag& IE_INSFLOAT_C_B) {
			if (nInstanceElement == 0) {
				vDesc.push_back({ "INSFLOAT_C_B", 0, DXGI_FORMAT_R32G32B32_FLOAT, nElement, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 });
			}
			else {
				vDesc.push_back({ "INSFLOAT_C_B", 0, DXGI_FORMAT_R32G32B32_FLOAT, nElement, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 });
			}
			nInstanceElement++;
		}
		if (InputElementFlag& IE_INSFLOAT_C_C) {
			if (nInstanceElement == 0) {
				vDesc.push_back({ "INSFLOAT_C_C", 0, DXGI_FORMAT_R32G32B32_FLOAT, nElement, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 });
			}
			else {
				vDesc.push_back({ "INSFLOAT_C_C", 0, DXGI_FORMAT_R32G32B32_FLOAT, nElement, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 });
			}
			nInstanceElement++;
		}

		D3D11_INPUT_ELEMENT_DESC* pDesc = new D3D11_INPUT_ELEMENT_DESC[vDesc.size()];
		for (int i = 0; i < vDesc.size(); ++i) {
			pDesc[i] = vDesc[i];
		}
		if (false == vDesc.empty()) {
			GLOBALVALUEMGR->GetDevice()->CreateInputLayout(
				pDesc
				, nElement + nInstanceElement
				, &(loadShader[0])
				, loadShader.size()
				, &m_inputLayout
			);
		}
		delete[] pDesc;
	}
	if (m_vertexShader) return true;

	return false;


	return false;
}

bool CRenderShader::CreateGS(LPCTSTR name){

	auto loadShader = ReadCSOFile(name);
	{
		GLOBALVALUEMGR->GetDevice()->CreateGeometryShader(
			&(loadShader[0])
			, loadShader.size()
			, nullptr
			, &m_geometryShader
		);

	}

	if (m_geometryShader) return true;

	return false;
}

bool CRenderShader::CreateHS(LPCTSTR name){
	auto loadShader = ReadCSOFile(name);
	{
		GLOBALVALUEMGR->GetDevice()->CreateHullShader(
			&(loadShader[0])
			, loadShader.size()
			, nullptr
			, &m_hullShader
		);

	}

	if (m_hullShader) return true;

	return false;
}

bool CRenderShader::CreateDS(LPCTSTR name){
	auto loadShader = ReadCSOFile(name);
	{
		GLOBALVALUEMGR->GetDevice()->CreateDomainShader(
			&(loadShader[0])
			, loadShader.size()
			, nullptr
			, &m_domainShader
		);

	}

	if (m_domainShader) return true;

	return false;
}

bool CRenderShader::CreatePS(LPCTSTR name){
	auto loadShader = ReadCSOFile(name);
	{
		GLOBALVALUEMGR->GetDevice()->CreatePixelShader(
			&(loadShader[0])
			, loadShader.size()
			, nullptr
			, &m_pixelShader
		);

	}

	if (m_pixelShader) return true;

	return false;
}

void CRenderShader::SetShaderData(LPCTSTR vsName, D3D11_INPUT_ELEMENT_DESC* vsDesc, UINT nElement, LPCTSTR gsName, LPCTSTR hsName, LPCTSTR dsName, LPCTSTR psName){
	if(vsName) CreateVS(vsName, vsDesc, nElement);
	if(gsName) CreateGS(gsName);
	if(dsName) CreateDS(dsName);
	if(hsName) CreateHS(hsName);
	if(psName) CreatePS(psName);
}

shared_ptr<CRenderShader> CRenderShader::CreateRenderShader(LPCTSTR ShaderName, UINT InputElementFlag, UINT BindFlag){
	shared_ptr<CRenderShader> pRenderShader = make_shared<CRenderShader>();
	WCHAR shaderName[256];
	if (BindFlag & BIND_VS) {
		wsprintf(shaderName, L"VS%s.cso", ShaderName);
		pRenderShader->CreateVS(shaderName, InputElementFlag);
	}
	if (BindFlag& BIND_HS) {
		wsprintf(shaderName, L"HS%s.cso", ShaderName);
		pRenderShader->CreateHS(shaderName);
	}
	if (BindFlag& BIND_DS) {
		wsprintf(shaderName, L"DS%s.cso", ShaderName);
		pRenderShader->CreateDS(shaderName);
	}
	if (BindFlag& BIND_GS) {
		wsprintf(shaderName, L"GS%s.cso", ShaderName);
		pRenderShader->CreateGS(shaderName);
	}
	if (BindFlag& BIND_PS) {
		wsprintf(shaderName, L"PS%s.cso", ShaderName);
		pRenderShader->CreatePS(shaderName);
	}

	return pRenderShader;
}

shared_ptr<CRenderShader> CRenderShader::CreateRenderShader(LPCTSTR * ShaderNames, UINT InputElementFlag, UINT BindFlag){
	shared_ptr<CRenderShader> pRenderShader = make_shared<CRenderShader>();
	WCHAR shaderName[256];
	UINT shaderIndex{ 0 };
	if (BindFlag & BIND_VS) {
		wsprintf(shaderName, L"%s", ShaderNames[shaderIndex++]);
		pRenderShader->CreateVS(shaderName, InputElementFlag);
	}
	if (BindFlag& BIND_HS) {
		wsprintf(shaderName, L"%s", ShaderNames[shaderIndex++]);
		pRenderShader->CreateHS(shaderName);
	}
	if (BindFlag& BIND_DS) {
		wsprintf(shaderName, L"%s", ShaderNames[shaderIndex++]);
		pRenderShader->CreateDS(shaderName);
	}
	if (BindFlag& BIND_GS) {
		wsprintf(shaderName, L"%s", ShaderNames[shaderIndex++]);
		pRenderShader->CreateGS(shaderName);
	}
	if (BindFlag& BIND_PS) {
		wsprintf(shaderName, L"%s", ShaderNames[shaderIndex++]);
		pRenderShader->CreatePS(shaderName);
	}

	return pRenderShader;
}

CRenderShader::CRenderShader() : CShader() {

}
CRenderShader::~CRenderShader() {

}