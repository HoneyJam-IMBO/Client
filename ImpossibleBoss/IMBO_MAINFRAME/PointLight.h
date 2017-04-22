#pragma once

#include "Light.h"

struct POINT_LIGHT_DS_CB {
	XMMATRIX m_PointLightMtx;
};
struct POINT_LIGHT_PS_CB {
	XMFLOAT4 m_PointLightPos_Range;
	XMFLOAT4 m_PointLightColor_Intensity;
};

struct POINT_LIGHT {
	float fRange;
	XMFLOAT3 xmf3Color;
	//float fIntensity;
};


class CPointLight : public CLight {
public:
	bool Begin();
	virtual bool End();


	//instance buffer controll
	virtual void SetBufferInfo(void** ppMappedResources, int& nInstance, shared_ptr<CCamera> pCamera);

	//light info setter
	void SetPointLightData(POINT_LIGHT& data) { m_PointData = data; }
	virtual void SetLength(float len);
	virtual void SetRange(float outer, float inner = 0.0f);
	float GetRange() { return m_PointData.fRange; }
	virtual void SetColor(float r, float g, float b);//color
	virtual XMFLOAT3 GetColor();

	static CPointLight* CreatePointLight(float fRange, XMFLOAT3 xmf3Color);
	void PickingProc();

	virtual void LoadInfo();
private:
	//data
	POINT_LIGHT m_PointData{};
	float m_fRange{ 0.0 };
	float m_fRangeRcp{ 0.0 };
	XMMATRIX m_xmmtxScale;
	//data

public:
	CPointLight();
	virtual ~CPointLight();
};