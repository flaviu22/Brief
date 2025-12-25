// SettingsDlg.cpp : implementation file
//

#include "pch.h"
#include "Brief.h"
#include "afxdialogex.h"
#include "SettingsDlg.h"

// CSettingsDlg dialog

IMPLEMENT_DYNAMIC(CSettingsDlg, CDialogEx)

CSettingsDlg::CSettingsDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_SETTINGS, pParent)
	, m_sWeatherapiKey(_T(""))
	, m_sTrendsAddress(_T(""))
	, m_sGoogleapisKey(_T(""))
	, m_hIcon(nullptr)
{
}

void CSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_WEATHERAPIKEY, m_sWeatherapiKey);
	DDX_Text(pDX, IDC_EDIT_TRENDSADDRESS, m_sTrendsAddress);
	DDX_Text(pDX, IDC_EDIT_GOOGLEAPISKEY, m_sGoogleapisKey);
}

BEGIN_MESSAGE_MAP(CSettingsDlg, CDialogEx)
	ON_EN_CHANGE(IDC_EDIT_WEATHERAPIKEY, &CSettingsDlg::OnEnChangeEditWeatherapikey)
	ON_EN_CHANGE(IDC_EDIT_TRENDSADDRESS, &CSettingsDlg::OnEnChangeEditTrendsaddress)
	ON_EN_CHANGE(IDC_EDIT_GOOGLEAPISKEY, &CSettingsDlg::OnEnChangeEditGoogleapiskey)
END_MESSAGE_MAP()

// CSettingsDlg message handlers

BOOL CSettingsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here

	m_hIcon = AfxGetApp()->LoadIcon(IDI_SETTINGS);
	SetIcon(m_hIcon, FALSE);

	m_sWeatherapiKey = theApp.GetProfileString(_T("Settings"), _T("WeatherApiKey"));
	m_sGoogleapisKey = theApp.GetProfileString(_T("Settings"), _T("GoogleApisKey"));
	m_sTrendsAddress = theApp.GetProfileString(_T("Settings"), _T("GoogleTrendsAddress"), _T("https://trends.google.com/trending/rss?geo=GB"));

	CheckDlgButton(IDC_CHECK_WEATHER, theApp.GetProfileInt(_T("Settings"), _T("StopWeather"), 0));
	CheckDlgButton(IDC_CHECK_GOOGLE, theApp.GetProfileInt(_T("Settings"), _T("StopGoogle"), 0));
	CheckDlgButton(IDC_CHECK_YOUTUBE, theApp.GetProfileInt(_T("Settings"), _T("StopYoutube"), 0));

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CSettingsDlg::OnEnChangeEditWeatherapikey()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here

	UpdateData();

	if (m_sWeatherapiKey.IsEmpty())
	{
		if (!IsDlgButtonChecked(IDC_CHECK_WEATHER))
			CheckDlgButton(IDC_CHECK_WEATHER, TRUE);
	}
	else
	{
		if (IsDlgButtonChecked(IDC_CHECK_WEATHER))
			CheckDlgButton(IDC_CHECK_WEATHER, FALSE);
	}
}

void CSettingsDlg::OnEnChangeEditTrendsaddress()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here

	UpdateData();

	if (m_sTrendsAddress.IsEmpty())
	{
		if (!IsDlgButtonChecked(IDC_CHECK_GOOGLE))
			CheckDlgButton(IDC_CHECK_GOOGLE, TRUE);
	}
	else
	{
		if (IsDlgButtonChecked(IDC_CHECK_GOOGLE))
			CheckDlgButton(IDC_CHECK_GOOGLE, FALSE);
	}
}

void CSettingsDlg::OnEnChangeEditGoogleapiskey()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here

	UpdateData();

	if (m_sGoogleapisKey.IsEmpty())
	{
		if (!IsDlgButtonChecked(IDC_CHECK_YOUTUBE))
			CheckDlgButton(IDC_CHECK_YOUTUBE, TRUE);
	}
	else
	{
		if (IsDlgButtonChecked(IDC_CHECK_YOUTUBE))
			CheckDlgButton(IDC_CHECK_YOUTUBE, FALSE);
	}
}

void CSettingsDlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	UpdateData();

	std::bitset<3> checks{
		static_cast<unsigned int>(m_sWeatherapiKey.IsEmpty() && !IsDlgButtonChecked(IDC_CHECK_WEATHER))			|
		static_cast<unsigned int>(m_sTrendsAddress.IsEmpty() && !IsDlgButtonChecked(IDC_CHECK_GOOGLE)) 	<< 1	|
		static_cast<unsigned int>(m_sGoogleapisKey.IsEmpty() && !IsDlgButtonChecked(IDC_CHECK_YOUTUBE)) << 2
	};

	if (checks[0])	// Weather API Key is empty but checkbox not checked
		CheckDlgButton(IDC_CHECK_WEATHER, TRUE);

	if (checks[1])	// Google Trends Address is empty but checkbox not checked
		CheckDlgButton(IDC_CHECK_GOOGLE, TRUE);

	if (checks[2])	// Google APIs (YouTube) Key is empty but checkbox not checked
		CheckDlgButton(IDC_CHECK_YOUTUBE, TRUE);

	if (checks.any())
		return;

	theApp.WriteProfileString(_T("Settings"), _T("WeatherApiKey"), m_sWeatherapiKey);
	theApp.WriteProfileString(_T("Settings"), _T("GoogleApisKey"), m_sGoogleapisKey);
	theApp.WriteProfileString(_T("Settings"), _T("GoogleTrendsAddress"), m_sTrendsAddress);

	theApp.WriteProfileInt(_T("Settings"), _T("StopWeather"), IsDlgButtonChecked(IDC_CHECK_WEATHER));
	theApp.WriteProfileInt(_T("Settings"), _T("StopGoogle"), IsDlgButtonChecked(IDC_CHECK_GOOGLE));
	theApp.WriteProfileInt(_T("Settings"), _T("StopYoutube"), IsDlgButtonChecked(IDC_CHECK_YOUTUBE));

	CDialogEx::OnOK();
}
