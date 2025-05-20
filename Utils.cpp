#include "pch.h"
#include "Utils.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

std::string CUtils::GetCurrentPath() const
{
	return std::filesystem::current_path().string();
}

CString CUtils::GetAppPathTemp() const
{
	CString sTempPath, sTemp;
	GetTempPath(_MAX_PATH, sTempPath.GetBuffer(_MAX_PATH));
	sTemp.Format(_T("%s%s\\"), sTempPath, AfxGetApp()->m_pszAppName);

	if (!PathIsDirectory(sTemp) &&
		!CreateDirectory(sTemp, NULL) &&
		ERROR_ALREADY_EXISTS != GetLastError())
		return sTempPath;

	return sTemp;
}

BOOL CUtils::FileExist(const CString& sFileName) const
{
	return (-1 != _access(CT2CA(sFileName), 0));
}

std::string CUtils::GetFileContent(const std::string& file) const
{
	std::ifstream ifs(file);
	const std::string out((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
	return out;
}

std::wstring CUtils::GetFileContentW(const std::string& file) const
{
	std::wifstream istream(file);
	std::wstring out((std::istreambuf_iterator<wchar_t>(istream)),
		std::istreambuf_iterator<wchar_t>());

	return out;
}

std::string CUtils::ws2s(const std::wstring& wstr) const
{
	using convert_typeX = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_typeX, wchar_t> converterX;
	return converterX.to_bytes(wstr);
}

std::wstring CUtils::s2ws(const std::string& str) const
{
	using convert_typeX = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_typeX, wchar_t> converterX;
	return converterX.from_bytes(str);
}

CStringA CUtils::utf16_utf8(const CStringW& utf16) const
{
	CStringA utf8;
	if (utf16.IsEmpty())
		return utf8;
	int cc = 0;

	if ((cc = WideCharToMultiByte(CP_UTF8, 0, utf16, -1, NULL, 0, 0, 0) - 1) > 0)
	{
		char* buf = utf8.GetBuffer(cc);
		if (buf)
			WideCharToMultiByte(CP_UTF8, 0, utf16, -1, buf, cc, 0, 0);
		utf8.ReleaseBuffer();
	}
	return utf8;
}

CStringW CUtils::utf8_utf16(const CStringA& utf8) const
{
	CStringW utf16;
	if (utf8.IsEmpty())
		return utf16;
	int cc = 0;

	if ((cc = MultiByteToWideChar(CP_UTF8, 0, utf8, -1, NULL, 0) - 1) > 0)
	{
		wchar_t* buf = utf16.GetBuffer(cc);
		if (buf)
			MultiByteToWideChar(CP_UTF8, 0, utf8, -1, buf, cc);
		utf16.ReleaseBuffer();
	}
	return utf16;
}

std::string CUtils::ConvertWideToANSI(const std::wstring& wstr) const
{
	auto count = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), static_cast<int>(wstr.length()), NULL, 0, NULL, NULL);
	std::string str(count, 0);
	WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, &str[0], count, NULL, NULL);
	return str;
}

std::wstring CUtils::ConvertAnsiToWide(const std::string& str) const
{
	const auto count = MultiByteToWideChar(CP_ACP, 0, str.c_str(), static_cast<int>(str.length()), NULL, 0);
	std::wstring wstr(count, 0);
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), static_cast<int>(str.length()), &wstr[0], count);
	return wstr;
}

std::string CUtils::ConvertWideToUtf8(const std::wstring& wstr) const
{
	const auto count = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), static_cast<int>(wstr.length()), NULL, 0, NULL, NULL);
	std::string str(count, 0);
	WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &str[0], count, NULL, NULL);
	return str;
}

std::wstring CUtils::ConvertUtf8ToWide(const std::string& str) const
{
	const auto count = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), static_cast<int>(str.length()), NULL, 0);
	std::wstring wstr(count, 0);
	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), static_cast<int>(str.length()), &wstr[0], count);
	return wstr;
}
