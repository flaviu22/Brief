// Brief.h : main header file for the Brief application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"						// main symbols
#include <string_view>

constexpr std::string_view google_trends_address = "https://trends.google.com/trending/rss?geo=";
constexpr UINT FORE_MAX_RETRY = 3;
constexpr UINT FORE_ELAPSE = 333;

// CBriefApp:
// See Brief.cpp for the implementation of this class
//

class CBriefApp : public CWinAppEx
{
public:
	CBriefApp() noexcept;

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CBriefApp theApp;
