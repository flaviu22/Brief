// TimeZoneDlg.cpp : implementation file
//

#include "pch.h"
#include "Brief.h"
#include "afxdialogex.h"
#include "TimeZoneDlg.h"
#include "HtmlData.h"

// CTimeZoneDlg dialog

IMPLEMENT_DYNAMIC(CTimeZoneDlg, CDialogEx)

CTimeZoneDlg::CTimeZoneDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_TIMEZONE, pParent)
{
#ifndef _WIN32_WCE
	EnableActiveAccessibility();
#endif
}

void CTimeZoneDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_TIMEZONE, m_Combo);
}

BEGIN_MESSAGE_MAP(CTimeZoneDlg, CDialogEx)
END_MESSAGE_MAP()

// CTimeZoneDlg message handlers

BOOL CTimeZoneDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here

	m_hIcon = AfxGetApp()->LoadIcon(IDR_BriefTYPE);
	SetIcon(m_hIcon, FALSE);

	const auto cityid = theApp.GetProfileInt(_T("Settings"), _T("TimeZoneCityId"), 4254);
	for (const auto& t : time_zones)
	{
		const auto index = m_Combo.AddString(CString(std::string(t.first).c_str()));
		m_Combo.SetItemData(index, t.second);
		if (cityid == t.second)
			m_Combo.SetCurSel(index);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void CTimeZoneDlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	UpdateData(FALSE);
	const auto index = m_Combo.GetCurSel();
	if (CB_ERR == index)
	{
		theApp.WriteProfileString(_T("Settings"), _T("TimeZoneCityId"), nullptr);
		MessageBox(_T("Please select a time zone!"), nullptr, MB_ICONERROR);
	}

	theApp.WriteProfileInt(_T("Settings"), _T("TimeZoneCityId"), static_cast<int>(m_Combo.GetItemData(index)));


	CDialogEx::OnOK();
}
