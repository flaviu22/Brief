// BriefDoc.h : interface of the CBriefDoc class
// https://trends.google.com/trends/trendingsearches/daily/rss?geo=
// https://trends.google.com/trending/rss?geo=

#pragma once

#include "Brief.h"
#include "Data.h"
#include "Utils.h"
#include "BriefView.h"

#include <array>
#include <vector>
#include <thread>
#include <chrono>

class CBriefDoc : public CDocument, public CUtils
{
protected: // create from serialization only
	CBriefDoc() noexcept = default;
	DECLARE_DYNCREATE(CBriefDoc)

// Attributes
public:
	BOOL m_bMove{ FALSE };
//	BOOL m_bKeep{ FALSE };
	mutable CString m_sError{};

public:
	void InitializeDownload() { m_abort.store(false); }
	bool IsDownloadAborted() const { return m_abort.load(); }
	void AbortDownload() { m_abort.store(true); }

// Operations
public:
	bool GenerateMessagePage(std::string message) const;
	bool CreateIndexPage();

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

public:
	void StartGettingWeather(const CBriefView* pView);
	void StartGettingGoogleTrends(const CBriefView* pView);
	void StartGettingYoutubeTrends(const CBriefView* pView);
	std::vector<SWeatherLocation> GetWeatherLoc(const CString& sText) const;
	std::array<std::string, 3> GetErrors() const { return m_errors; }
	void JointThread(const size_t pos);
	void RetrieveThreadsErrors();

private:
	SWeather m_weather{};
	std::vector<STrendItem> m_gtrends{};
	std::vector<STrendItem> m_ytrends{};
	std::atomic<bool> m_abort{ false };
	std::array<std::string, 3> m_errors{};	// errors from threads
	std::array<std::thread, 3> m_threads{};

private:
	void GetWeather(const CBriefView* pView);
	void GetGoogleTrends(const CBriefView* pView);
	void GetYoutubeTrends(const CBriefView* pView);
	void GetExternalIp(const CBriefView* pView);
	void GetLocation(const CBriefView* pView, const CString& sIP);
	std::pair<std::string, std::string> GetEndpointResponse(const CString& sAddress, const time_seconds timeout = 10) const;
	bool LoadingPageExists() const;
	CString GetYoutubeTrendsAddress() const;
	CString GetToday() const;
	CString GetYesterday() const;

private:
	template <typename T>
	class CDialogModalHelper
	{
	public:
		CDialogModalHelper(std::unique_ptr<T> pDialog, BOOL bMove, BOOL bRefresh = TRUE)
			: m_pDialog(std::move(pDialog))
			, m_bMove(bMove)
			, m_bRefresh(bRefresh)
		{
			AfxGetMainWnd()->KillTimer(ID_TIMER_MOVE);
		}

		void DoModal() const
		{
			m_nModalID = m_pDialog->DoModal();
		}

		~CDialogModalHelper()
		{
			if (m_bMove)
				AfxGetMainWnd()->SetTimer(ID_TIMER_MOVE, theApp.GetProfileInt(_T("Settings"), _T("MoveIntervalSeconds"), 77) * 1000, nullptr);

			if (IDOK == m_nModalID && m_bRefresh)
				::PostMessage(AfxGetMainWnd()->GetSafeHwnd(), WM_COMMAND, ID_VIEW_REFRESH, 0);
		}

		CDialogModalHelper(const CDialogModalHelper&) = delete;
		CDialogModalHelper& operator=(const CDialogModalHelper&) = delete;
		CDialogModalHelper(CDialogModalHelper&&) = delete;
		CDialogModalHelper& operator=(CDialogModalHelper&&) = delete;

	private:
		std::unique_ptr<T> m_pDialog{ nullptr };
		mutable UINT_PTR m_nModalID{ 0 };
		BOOL m_bMove{ FALSE };
		BOOL m_bRefresh{ TRUE };
	};

// Implementation
public:

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
	virtual void OnCloseDocument();
	afx_msg void OnEditMovetime();
	afx_msg void OnEditWeatherLocation();
	afx_msg void OnEditTimeZone();
	afx_msg void OnEditTrendsLocation();
	afx_msg void OnEditSettings();
};
