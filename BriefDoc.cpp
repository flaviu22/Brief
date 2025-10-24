// BriefDoc.cpp : implementation of the CBriefDoc class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Brief.h"
#endif

#include "BriefDoc.h"
#include "ApiKeyDlg.h"
#include "WeatherLocationDlg.h"
#include "HtmlGenerator.h"
#include "MoveDlg.h"
#include "TimeZoneDlg.h"
#include "TrendsLocationDlg.h"
#include "SettingsDlg.h"
#include "WeatherHandler.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CBriefDoc

IMPLEMENT_DYNCREATE(CBriefDoc, CDocument)

BEGIN_MESSAGE_MAP(CBriefDoc, CDocument)
//	ON_COMMAND(ID_EDIT_APIKEY, &CBriefDoc::OnEditApikey)
	ON_COMMAND(ID_EDIT_MOVETIME, &CBriefDoc::OnEditMovetime)
	ON_COMMAND(ID_EDIT_TIMEZONE, &CBriefDoc::OnEditTimeZone)
	ON_COMMAND(ID_EDIT_TRENDSLOCATION, &CBriefDoc::OnEditTrendsLocation)
	ON_COMMAND(ID_EDIT_WEATHERLOCATION, &CBriefDoc::OnEditWeatherLocation)
	ON_COMMAND(ID_EDIT_SETTINGS, &CBriefDoc::OnEditSettings)
END_MESSAGE_MAP()

// CBriefDoc construction/destruction

BOOL CBriefDoc::OnNewDocument()
{
	if (! CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}

void CBriefDoc::OnCloseDocument()
{
	// TODO: Add your specialized code here and/or call the base class

	m_abort.store(true);
	// Wait for all threads to finish
	for (auto& t : m_threads)
	{
		if (t.joinable())
			t.join();
	}

	CDocument::OnCloseDocument();
}

// CBriefDoc serialization

void CBriefDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

// CBriefDoc diagnostics

#ifdef _DEBUG
void CBriefDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CBriefDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

// CBriefDoc commands
// not used, but kept for future use

void CBriefDoc::OnEditMovetime()
{
	// TODO: Add your command handler code here

	CDialogModalHelper<CMoveDlg> dlg(std::make_unique<CMoveDlg>(), m_bMove);
	dlg.DoModal();
}

void CBriefDoc::OnEditWeatherLocation()
{
	// TODO: Add your command handler code here

	CDialogModalHelper<CWeatherLocationDlg> dlg(std::make_unique<CWeatherLocationDlg>(this), m_bMove);
	dlg.DoModal();
}

void CBriefDoc::OnEditTimeZone()
{
	// TODO: Add your command handler code here

	CDialogModalHelper<CTimeZoneDlg> dlg(std::make_unique<CTimeZoneDlg>(), m_bMove);
	dlg.DoModal();
}

void CBriefDoc::OnEditTrendsLocation()
{
	// TODO: Add your command handler code here

	CDialogModalHelper<CTrendsLocationDlg> dlg(std::make_unique<CTrendsLocationDlg>(), m_bMove);
	dlg.DoModal();
}

void CBriefDoc::OnEditSettings()
{
	// TODO: Add your command handler code here

	CDialogModalHelper<CSettingsDlg> dlg(std::make_unique<CSettingsDlg>(), m_bMove);
	dlg.DoModal();
}

bool CBriefDoc::LoadingPageExists() const
{
	std::string file_path(CStringA(GetAppPathTemp()) + CStringA(_T("loading.html")));
	return std::filesystem::exists(file_path);
}

bool CBriefDoc::GenerateMessagePage(std::string message) const
{
	m_sError.Empty();

	if (!LoadingPageExists())
	{
		std::ofstream ofs(GetAppPathTemp() + _T("loading.html"));
		if (ofs.is_open())
		{
			CHtmlGenerator hg{};
			ofs << hg.GenerateHtmlMessage(std::move(message));
		}

		if (ofs.bad())
			m_sError.Format(_T("Error while handling messaging file."));
	}
	
	return m_sError.IsEmpty();
}

bool CBriefDoc::CreateIndexPage()
{
	m_sError.Empty();

	std::ofstream ofs(GetAppPathTemp() + _T("index.html"));
	if (ofs.is_open())
	{
		CHtmlGenerator hg{ GetCurrentPath() + "/icons/" };
		ofs << hg.GenerateHtml(m_bMove, std::move(m_weather), std::move(m_gtrends), std::move(m_ytrends), 
								theApp.GetProfileInt(_T("Settings"), _T("TimeZoneCityId"), 4254));
	}

	if (ofs.bad())
		m_sError.Format(_T("Error while handling index file."));

	return m_sError.IsEmpty();
}
// first is the error, second is the output
std::pair<std::string, std::string> CBriefDoc::GetEndpointResponse(
	const CString& sAddress, const time_seconds timeout/* = 10*/) const
{
	std::string error, data;
	CInternetSession session{};
	session.SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, TIME_SECOND * timeout);

	try
	{
		char buffer[4096];	// 4KB buffer
		std::unique_ptr<CStdioFile> file{ session.OpenURL(sAddress, 1,
			INTERNET_FLAG_TRANSFER_ASCII | INTERNET_FLAG_DONT_CACHE) };
		while (UINT nRead = file->Read(buffer, sizeof(buffer)))
		{
			data.append(buffer, nRead);
			if (m_abort.load())
				break;
		}
	}
	catch (CInternetException* pEx)
	{
		TCHAR szError[_MAX_PATH];
		pEx->GetErrorMessage(szError, _MAX_PATH);
		pEx->Delete();
#ifdef  _UNICODE
		error = CW2A(szError);
#else
		error = szError;
#endif
	}
	// Return a pair where the first element is the error message (if any),
	// and the second element is the data retrieved. If an error occurs,
	// the error message is prioritized, and the data may be incomplete.
	return std::make_pair(error, data);
}

void CBriefDoc::StartGettingWeather(const CBriefView* pView)
{
	m_threads[0] = std::thread(&CBriefDoc::GetWeather, this, pView);
}

void CBriefDoc::StartGettingGoogleTrends(const CBriefView* pView)
{
	m_threads[1] = std::thread(&CBriefDoc::GetGoogleTrends, this, pView);
}

void CBriefDoc::StartGettingYoutubeTrends(const CBriefView* pView)
{
	m_threads[2] = std::thread(&CBriefDoc::GetYoutubeTrends, this, pView);
}

void CBriefDoc::JointThread(const size_t pos)
{
	if (pos >= 0 && pos < m_threads.size() && m_threads[pos].joinable())
		m_threads[pos].join();
}
// method not used, but kept for future use
void CBriefDoc::GetExternalIp(const CBriefView* pView)
{
	auto ip = GetEndpointResponse(_T("https://api.ipify.org/"));
//	auto ip = GetEndpointResponse(_T("http://myexternalip.com/raw"));

	if (!m_abort.load())
	{
		if (ip.first.empty())
		{
			theApp.WriteProfileString(_T("Settings"), _T("ExternalIp"), CString(ip.second.c_str()));
			theApp.WriteProfileString(_T("Settings"), _T("ExternalIpError"), nullptr);
		}
		else
		{
			theApp.WriteProfileString(_T("Settings"), _T("ExternalIp"), nullptr);
			theApp.WriteProfileString(_T("Settings"), _T("ExternalIpError"), CString(ip.first.c_str()));
		}
	}

	::PostMessage(pView->GetSafeHwnd(), WMU_THREADDATA, MESSAGE_EXTERNALIP_DONE, 0);
}
// method not used, but kept for future use
void CBriefDoc::GetLocation(const CBriefView* pView, const CString& sIP)
{
	auto location = GetEndpointResponse(_T("http://ip-api.com/json/") + sIP);

	if (!m_abort.load())
	{
		if (location.first.empty())
		{
			CWeatherHandler wh{};
			auto decoded = wh.DecodeLocation(std::move(location.second));
			theApp.WriteProfileString(_T("Settings"), _T("LocationCountry"), CString(decoded.first.c_str()));
			theApp.WriteProfileString(_T("Settings"), _T("LocationCity"), CString(decoded.second.c_str()));
			theApp.WriteProfileString(_T("Settings"), _T("LocationError"), nullptr);
		}
		else
		{
			theApp.WriteProfileString(_T("Settings"), _T("LocationError"), CString(location.first.c_str()));
			theApp.WriteProfileString(_T("Settings"), _T("LocationCountry"), nullptr);
			theApp.WriteProfileString(_T("Settings"), _T("LocationCity"), nullptr);
		}
	}
	::PostMessage(pView->GetSafeHwnd(), WMU_THREADDATA, MESSAGE_GETLOCATION_DONE, 0);
}

void CBriefDoc::GetWeather(const CBriefView* pView)
{
	CString sAddress;
	sAddress.Format(_T("https://www.meteosource.com/api/v1/free/point?place_id=%s&sections=all&timezone=UTC&language=en&units=metric&key=%s"),
		theApp.GetProfileString(_T("Settings"), _T("WeatherLocationPlaceId")),
		theApp.GetProfileString(_T("Settings"), _T("WeatherApiKey")));
	auto output = GetEndpointResponse(sAddress);

	do
	{
		if (m_abort.load())
			break;
		m_errors[0] = std::move(output.first);
		if (!m_errors[0].empty())
			break;
		if (output.second.empty())
		{
			m_errors[0] = "No weather data received";
			break;
		}

		CWeatherHandler wh{};
		m_weather = wh.DecodeWeather(std::move(output.second));

		if (m_weather.icon.empty() && 0 == m_weather.icon_num && !m_weather.summary.empty())
		{
			m_errors[0] = m_weather.summary;
			break;
		}

		m_weather.location_name = theApp.GetProfileString(_T("Settings"), _T("WeatherLocationName"));
		m_weather.location_country = theApp.GetProfileString(_T("Settings"), _T("WeatherLocationCountry"));
	} while (FALSE);

	::PostMessage(pView->GetSafeHwnd(), WMU_THREADDATA, MESSAGE_GETWEATHER_DONE, 0);
}

void CBriefDoc::GetGoogleTrends(const CBriefView* pView)
{
	auto output = GetEndpointResponse(
		theApp.GetProfileString(_T("Settings"), _T("GoogleTrendsAddress"), 
		_T("https://trends.google.com/trends/trendingsearches/daily/rss?geo=")) +
		theApp.GetProfileString(_T("Settings"), _T("TrendsLocation"), _T("GB")));

	do
	{
		if (m_abort.load())
			break;
		m_errors[1] = std::move(output.first);
		if (!m_errors[1].empty())
			break;
		if (output.second.empty())
		{
			m_errors[1] = "No google trends data received";
			break;
		}

		CTrends gt{};
		m_gtrends = gt.ReadGoogleTrends(std::move(output.second));
		m_errors[1] = gt.GetError();
	} while (FALSE);

	::PostMessage(pView->GetSafeHwnd(), WMU_THREADDATA, MESSAGE_GETGOOGLETRENDS_DONE, 0);
}

void CBriefDoc::GetYoutubeTrends(const CBriefView* pView)
{
	auto output = GetEndpointResponse(GetYoutubeTrendsAddress());

	do
	{
		if (m_abort.load())
			break;
		m_errors[2] = std::move(output.first);
		if (!m_errors[2].empty())
			break;
		if (output.second.empty())
		{
			m_errors[2] = "No youtube trends data received";
			break;
		}

		CTrends yt{};
		m_ytrends = yt.ReadYoutubeTrends(std::move(output.second));
		if (!yt.GetError().empty())
		{
			m_errors[2] = yt.GetError();
			break;
		}
		if (1 == m_ytrends.size() && 0 != m_ytrends[0].error_code)
		{
			m_errors[2] = m_ytrends[0].error + " Error code: " + std::to_string(m_ytrends[0].error_code);
			m_ytrends.clear();
			break;
		}
	} while (FALSE);

	::PostMessage(pView->GetSafeHwnd(), WMU_THREADDATA, MESSAGE_GETYOUTUBETRENDS_DONE, 0);
}

std::vector<SWeatherLocation> CBriefDoc::GetWeatherLoc(const CString& sText) const
{
	CString sAddress;
	sAddress.Format(_T("https://www.meteosource.com/api/v1/free/find_places?text=%s&key=%s"), 
		sText, CString(theApp.GetProfileString(_T("Settings"), _T("WeatherApiKey"))));

	CWeatherHandler wh{};
	return wh.DecodeWeatherLocation(GetEndpointResponse(sAddress).second);
}

CString CBriefDoc::GetYoutubeTrendsAddress() const
{
	CString sAddress;
/*	sAddress.Format(_T("https://www.googleapis.com/youtube/v3/search?key=%s")
						_T("&part=snippet,id&regionCode=%s")
						_T("&maxResults=%d")
						_T("&type=video")
						_T("&chart=mostPopular")
//						_T("&order=date")
						_T("&q=*")
						_T("&publishedAfter=%s")
						_T("&publishedBefore=%s"), 
					theApp.GetProfileString(_T("Settings"), _T("GoogleApisKey")),
					theApp.GetProfileString(_T("Settings"), _T("TrendsLocation"), _T("GB")),
					theApp.GetProfileInt(_T("Settings"), _T("YoutubeMaxResults"), 10),
					GetYesterday(),
					GetToday());*/

	sAddress.Format(_T("https://www.googleapis.com/youtube/v3/videos?key=%s")
		_T("&part=snippet,id,contentDetails,statistics&regionCode=%s")
		_T("&maxResults=%d")
		_T("&type=video")
		_T("&chart=mostPopular"),
		theApp.GetProfileString(_T("Settings"), _T("GoogleApisKey")),
		theApp.GetProfileString(_T("Settings"), _T("TrendsLocation"), _T("GB")),
		theApp.GetProfileInt(_T("Settings"), _T("YoutubeMaxResults"), 10));
	TRACE(_T("Youtube address: %s\n"), sAddress);
	return sAddress;
}

CString CBriefDoc::GetToday() const
{
	const auto local_time = std::chrono::current_zone()->to_local(std::chrono::system_clock::now());
	const auto today = std::format("{:%FT%XZ}", local_time);

	return CString(today.c_str());
}

CString CBriefDoc::GetYesterday() const
{
	const auto local_time = std::chrono::current_zone()->to_local(std::chrono::system_clock::now());
	const auto yesterday = std::format("{:%FT%XZ}", local_time - std::chrono::hours(24));

	return CString(yesterday.c_str());
}
// retrieve m_errors from threads and put it in m_sError
void CBriefDoc::RetrieveThreadsErrors()
{
	m_sError.Empty();
	for (size_t i = 0; i < m_errors.size(); ++i)
	{
		if (!m_errors[i].empty())
		{
			if (i > 0 && !m_sError.IsEmpty())
				m_sError += _T("|");
			m_sError += CString(m_errors[i].c_str());
			m_errors[i].clear();
		}
	}
}
