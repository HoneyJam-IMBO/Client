#include "stdafx.h"
#include "LightLayer.h"
//#include "RenderContainerSeller.h"

//---------------------------dxobject---------------------------------
bool CLightLayer::Begin() {

	CreateShaderState();

	for (int i = object_id::OBJECT_END + 1; i < object_id::OBJECT_LIGHT_END; ++i) {
		object_id id = (object_id)i;
		m_mRenderContainer.insert(pairRenderContainer(id, RCSELLER->GetRenderContainer(id)));
	}

	return true;
}

void CLightLayer::CreateShaderState() {
	//depth stencil state
	D3D11_DEPTH_STENCIL_DESC descDepthStencil;
	ZeroMemory(&descDepthStencil, sizeof(D3D11_DEPTH_STENCIL_DESC));
	descDepthStencil.DepthEnable = true;
	descDepthStencil.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	descDepthStencil.DepthFunc = D3D11_COMPARISON_GREATER_EQUAL;

	GLOBALVALUEMGR->GetDevice()->CreateDepthStencilState(&descDepthStencil, &m_pLightDepthStencilState);
	//depth stencil state

	//rasterizer state
	D3D11_RASTERIZER_DESC descRasterizer;
	ZeroMemory(&descRasterizer, sizeof(D3D11_RASTERIZER_DESC));
	descRasterizer.FillMode = D3D11_FILL_SOLID;
	//descRasterizer.FillMode = D3D11_FILL_WIREFRAME;
	descRasterizer.CullMode = D3D11_CULL_FRONT;
	GLOBALVALUEMGR->GetDevice()->CreateRasterizerState(&descRasterizer, &m_pLightRasterizerState);
	//rasterizer state

	//blend state
	D3D11_BLEND_DESC descBlend;
	ZeroMemory(&descBlend, sizeof(D3D11_BLEND_DESC));
	descBlend.RenderTarget[0].BlendEnable = true;
	descBlend.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	descBlend.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	descBlend.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;

	descBlend.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	descBlend.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	descBlend.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	descBlend.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	//blend state
	GLOBALVALUEMGR->GetDevice()->CreateBlendState(&descBlend, &m_pLightBlendState);
}
bool CLightLayer::End() {

	return true;
}
//---------------------------dxobject---------------------------------

//--------------------------container---------------------------------
void CLightLayer::UpdateShaderState(shared_ptr<CCamera> pCamera) {

}
void CLightLayer::SetShaderState(shared_ptr<CCamera> pCamera) {
	//--------------------------------deferred lighting--------------------------------
	ID3D11Buffer* pGBufferUnpackingBuffer = pCamera->GetGBufferUnpackingBuffer();
	GLOBALVALUEMGR->GetDeviceContext()->PSSetConstantBuffers(PS_UNPACKING_SLOT, 1, &pGBufferUnpackingBuffer);

	ID3D11Buffer* pViewProjectionBuffer = pCamera->GetViewProjectionBuffer();
	GLOBALVALUEMGR->GetDeviceContext()->PSSetConstantBuffers(PS_CAMERA_DATA_SLOT, 1, &pViewProjectionBuffer);
}
void CLightLayer::RenderExcute(shared_ptr<CCamera> pCamera) {
	//이전 상태 저장
	GLOBALVALUEMGR->GetDeviceContext()->OMGetDepthStencilState(&m_pPreDepthStencilState, &m_PreStencilRef);
	GLOBALVALUEMGR->GetDeviceContext()->RSGetState(&m_pPreRasterizerState);
	GLOBALVALUEMGR->GetDeviceContext()->OMGetBlendState(&m_pPreBlendState, m_pPreBlendFactor, &m_PreSampleMask);

	object_id id = object_id::OBJECT_END;
	//scene의 모든 Part의 rendercontainer안에 part list Render!
	for (int i = object_id::OBJECT_END + 1; i < object_id::OBJECT_LIGHT_END; ++i) {
		id = (object_id)i;
		if (id == object_id::OBJECT_DIRECTIONAL_LIGHT) {
			//directional
			m_mRenderContainer[id]->Render(pCamera);

			GLOBALVALUEMGR->GetDeviceContext()->OMSetBlendState(m_pLightBlendState, nullptr, 0xffffffff);
			GLOBALVALUEMGR->GetDeviceContext()->OMSetDepthStencilState(m_pLightDepthStencilState, 0);
			GLOBALVALUEMGR->GetDeviceContext()->RSSetState(m_pLightRasterizerState);
		}
		else {
			m_mRenderContainer[id]->Render(pCamera);
		}

	}


}

void CLightLayer::CleanShaderState(shared_ptr<CCamera> pCamera) {
	//이전 상태로 되돌림
	GLOBALVALUEMGR->GetDeviceContext()->OMSetBlendState(m_pPreBlendState, m_pPreBlendFactor, m_PreSampleMask);
	GLOBALVALUEMGR->GetDeviceContext()->OMSetDepthStencilState(m_pPreDepthStencilState, m_PreStencilRef);
	GLOBALVALUEMGR->GetDeviceContext()->RSSetState(m_pPreRasterizerState);

	object_id id = object_id::OBJECT_END;
	//scene의 모든 Part의 rendercontainer안에 part list Clear!
	for (int i = object_id::OBJECT_END + 1; i < object_id::OBJECT_LIGHT_END; ++i) {
		id = (object_id)i;
		m_mRenderContainer[id]->ClearObjectList();
	}

}

CLightLayer::CLightLayer() : CLayer() {

}
CLightLayer::~CLightLayer() {

}