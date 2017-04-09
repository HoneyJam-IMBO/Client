#include "stdafx.h"
#include "Layer.h"

CLayer::CLayer() : DXObject("layer") {
}
CLayer::~CLayer() {

}

void CLayer::Render(shared_ptr<CCamera> pCamera) {
	UpdateShaderState(pCamera);
	SetShaderState(pCamera);
	RenderExcute(pCamera);
	CleanShaderState(pCamera);
}
