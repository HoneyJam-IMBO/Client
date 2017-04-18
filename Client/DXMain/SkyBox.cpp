#include "stdafx.h"
#include "SkyBox.h"

bool CSkyBox::Begin() {
	//object_id set 
	//m_objectID = object_id::OBJECT_SKYBOX;
	CGameObject::Begin();
	return true;
}
bool CSkyBox::End() {

	return CGameObject::End();
}
void CSkyBox::Animate(float fTimeElapsed) {
	//모든 컴포넌트를 돌면서 Update실행
	if (m_pCamera) SetPosition(m_pCamera->GetPosition());
}

void CSkyBox::SetCamera(shared_ptr<CCamera> pCamera){
	m_pCamera = pCamera;
}

CSkyBox::CSkyBox() : CGameObject("skybox", tag::TAG_SPACE) {

}
CSkyBox::~CSkyBox() {

}