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