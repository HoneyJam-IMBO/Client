#pragma once
#include "ComputeShader.h"

class CCSHorizontalBlur : public CComputeShader {
public:
	//---------------------------------shader---------------------------
	virtual void UpdateShaderState();
	virtual void ResizeBuffer(UINT nWidth, UINT nHeight);
	//---------------------------------shader---------------------------

private:
	//begin func
	virtual bool CreateShaderValues();//texture, constbuffer etc.
	virtual bool CreateCS();
	//begin func

	//end func 
	virtual bool ReleaseShaderValues();
	//end func

	//set func
	virtual void SetShaderValues();
	//clean func
	virtual void CleanShaderValues();

	//bloom
	//2
	ID3D11Texture2D			 *m_pd3dtxtTempBloom{ nullptr };
	ID3D11ShaderResourceView *m_pd3dsrvTempBloom{ nullptr };
	ID3D11UnorderedAccessView*m_pd3duavTempBloom{ nullptr };

private:

public:
	CCSHorizontalBlur();
	virtual ~CCSHorizontalBlur();
};

/*
ù�ܰ迡����
�׸��� �� �׷��� srv�� �Է����� �޾Ƽ�
�װ��� 1024*16���� �ȼ��� 1024���� �����.
*/