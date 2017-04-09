#pragma once
#include "ConstantBuffer.h"
#include "GlobalObject.h"
class CGlobalBuffer : public CConstantBuffer {
public:
	//object
	bool Begin(UINT BufferStride, UINT Slot = 0, UINT BindFlag = BIND_VS, UINT Offset = 0);
	virtual bool End();
	//object

private:

public:
	CGlobalBuffer();
	~CGlobalBuffer();

};