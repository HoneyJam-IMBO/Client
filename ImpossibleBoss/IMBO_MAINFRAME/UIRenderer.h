#pragma once

class CUIObject;
class CUIRenderer
{
public:
	CUIRenderer();
	~CUIRenderer();

public:
	HRESULT Initialize();
	void	SetRenderContainer(float fLayer, CUIObject* pUI) { m_mapUI.insert(make_pair(fLayer, pUI)); }
	void	RenderUI();

	void	ClearData() { m_mapUI.clear(); }

private:
	multimap<float, CUIObject*> m_mapUI;

	CRenderShader*	m_pRenderShader{ nullptr };


	ID3D11BlendState*	m_pAlphaBlendState{ nullptr };
	ID3D11BlendState*	m_pPreBlendState{ nullptr };
	float* m_pPreBlendFactor{ nullptr };
	UINT m_PreSampleMask{ 0 };
};

