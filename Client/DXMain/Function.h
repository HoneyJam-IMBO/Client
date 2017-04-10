#pragma once


template <typename T> void Safe_Delete(T& pointer)
{
	if (nullptr != pointer)
	{
		pointer->End();
		delete pointer;
		pointer = nullptr;
	}
}
