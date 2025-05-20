#pragma once

#include <string>
#include <io.h>
#include <fstream>
#include <filesystem>
#include <codecvt>

class CUtils
{
public:
	CString GetAppPathTemp() const;
	std::string GetCurrentPath() const;
	BOOL FileExist(const CString& sFileName) const;
	std::string GetFileContent(const std::string& file) const;
	std::wstring GetFileContentW(const std::string& file) const;
	std::string ws2s(const std::wstring& wstr) const;
	std::wstring s2ws(const std::string& str) const;
	CStringA utf16_utf8(const CStringW& utf16) const;
	CStringW utf8_utf16(const CStringA& utf8) const;
	std::string ConvertWideToANSI(const std::wstring& wstr) const;
	std::wstring ConvertAnsiToWide(const std::string& str) const;
	std::string ConvertWideToUtf8(const std::wstring& wstr) const;
	std::wstring ConvertUtf8ToWide(const std::string& str) const;
};
