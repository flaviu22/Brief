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
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CSettingsDlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	UpdateData();

	theApp.WriteProfileString(_T("Settings"), _T("WeatherApiKey"), m_sWeatherapiKey);
	theApp.WriteProfileString(_T("Settings"), _T("GoogleApisKey"), m_sGoogleapisKey);
	theApp.WriteProfileString(_T("Settings"), _T("GoogleTrendsAddress"), m_sTrendsAddress);

	CDialogEx::OnOK();
}
