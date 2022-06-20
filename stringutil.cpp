#include "stringutil.h"

string& String_Replace_All(string& src, const string& old_value, const string& new_value) {
	// 每次重新定位起始位置，防止上轮替换后的字符串形成新的old_value
	for (string::size_type pos(0); pos != string::npos; pos += new_value.length()) {
		if ((pos = src.find(old_value, pos)) != string::npos) {
			src.replace(pos, old_value.length(), new_value);
		}
		else break;
	}
	return src;
}

string String_TO_UTF8(string str)
{

	int nwLen = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);
	wchar_t* pwBuf = new wchar_t[nwLen + 1];
	ZeroMemory(pwBuf, nwLen * 2 + 2);

	::MultiByteToWideChar(CP_ACP, 0, str.c_str(), (int)str.length(), pwBuf, nwLen);

	int nLen = ::WideCharToMultiByte(CP_UTF8, 0, pwBuf, -1, NULL, 0, NULL, NULL);

	char* pBuf = new char[nLen + 1];
	ZeroMemory(pBuf, nLen + 1);

	::WideCharToMultiByte(CP_UTF8, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);

	string retStr(pBuf);

	delete[]pwBuf;
	delete[]pBuf;

	pwBuf = NULL;
	pBuf = NULL;

	return retStr;

}

string UTF8_To_String(const string& s)
{
	if (s.empty())
	{
		return string();
	}

	wstring result;

	int n = MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, NULL, 0);
	wchar_t* buffer = new wchar_t[n];

	::MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, buffer, n);

	result = buffer;
	delete[] buffer;

	string result2;
	int len = WideCharToMultiByte(CP_ACP, 0, result.c_str(), (int)result.size(), NULL, 0, NULL, NULL);
	char* buffer2 = new char[len + 1];
	WideCharToMultiByte(CP_ACP, 0, result.c_str(), (int)result.size(), buffer2, len, NULL, NULL);
	buffer2[len] = '\0';
	result2.append(buffer2);
	delete[] buffer2;

	return result2;
}