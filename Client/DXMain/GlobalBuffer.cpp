#include "stdafx.h"
#include "GlobalBuffer.h"

bool CGlobalBuffer::Begin(UINT BufferStride, UINT Slot, UINT BindFlag, UINT Offset) {
	CConstantBuffer::Begin(1, BufferStride, Slot, BindFlag, Offset);


	return true;
}

bool CGlobalBuffer::End() {

	return true;
}


CGlobalBuffer::CGlobalBuffer() : CConstantBuffer() {

}

CGlobalBuffer::~CGlobalBuffer()
{
}
