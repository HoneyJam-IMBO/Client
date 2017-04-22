#pragma once

template<typename Ty, size_t N>
constexpr size_t GetArraySize(Ty(&)[N]) noexcept
{
	return N;
}

template <typename T> void Safe_EndDelete(T& pointer)
{
	if (pointer)
	{
		pointer->End();
		delete pointer;
		pointer = nullptr;
	}
}

static std::string GetFileName(std::string path) {
	size_t size = path.size();
	char name[64];
	int index{ 0 };
	for (int i = size - 1; i > 1; i--) {
		if (path[i] == '\\' | path[i] == '/') {
			for (int j = i + 1; j < size; ++j) {
				if (path[j] == '.') break;
				name[index++] = path[j];
			}
			name[index] = '\0';
			break;
		}
	}
	return name;
}

static std::string TCHARToString(const TCHAR* ptsz)
{
	size_t len = wcslen((wchar_t*)ptsz);
	char* psz = new char[2 * len + 1];
	wcstombs(psz, (wchar_t*)ptsz, 2 * len + 1);
	std::string s = psz;
	delete[] psz;
	return s;
}

static TCHAR* StringToTCHAR(std::string& s)
{
	std::string tstr;
	const char* all = s.c_str();
	size_t len = 1 + strlen(all);
	wchar_t* t = new wchar_t[len];
	if (NULL == t) throw std::bad_alloc();
	mbstowcs(t, all, len);
	return (TCHAR*)t;
}
