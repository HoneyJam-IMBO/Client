#pragma once
#include "DXObject.h"

#define STEMP_DATA_NUM 256*256
#define STEMP_WIDTH 256
#define STEMP_HEIGHT 256

struct Pixel24;
class CTexture;

class CStempManager;

class CStemp : public DXObject{
public:
	virtual bool Begin();
	virtual bool End();

	virtual void SetShaderState();
	virtual void CleanShaderState();

	void SetStempData(Pixel24* pStempData);
	void SetStempData(UINT index, byte data);
	void IncreaseTerrain(Pixel24* pHeightMapData, XMFLOAT2 picPos, float fRadius);
	void DecreaseTerrain(Pixel24* pHeightMapData, XMFLOAT2 picPos, float fRadius);
	void SetTerrain(Pixel24* pHeightMapData, XMFLOAT2 picPos, float fRadius);
	void SetStempManager(CStempManager* pStempManager) { m_pStempManager = pStempManager; }
	void SetStempTexture(shared_ptr<CTexture> pTexture) { m_pStempTexture = pTexture; }
	static CStemp* CreateStemp(wstring name, CStempManager* pStempManager);
private:
	Pixel24* m_pStempData{ nullptr };
	CStempManager* m_pStempManager{ nullptr };
	int m_nIndex{ 0 };

	shared_ptr<CTexture> m_pStempTexture{ nullptr };
public:
	CStemp();
	virtual ~CStemp();
};