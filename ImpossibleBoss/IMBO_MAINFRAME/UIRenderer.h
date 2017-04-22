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
};

