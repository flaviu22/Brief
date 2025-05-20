// TrendsLocationDlg.cpp : implementation file
//

#include "pch.h"
#include "Brief.h"
#include "TrendsLocationDlg.h"
#include "afxdialogex.h"

// CTrendsLocationDlg dialog

IMPLEMENT_DYNAMIC(CTrendsLocationDlg, CDialogEx)

CTrendsLocationDlg::CTrendsLocationDlg(CWnd* pParent/* = nullptr*/)
	: CDialogEx(IDD_DIALOG_GEO, pParent)
{
#ifndef _WIN32_WCE
	EnableActiveAccessibility();
#endif
}

void CTrendsLocationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_GEO, m_Combo);
}

BEGIN_MESSAGE_MAP(CTrendsLocationDlg, CDialogEx)
END_MESSAGE_MAP()

// CTrendsLocationDlg message handlers

BOOL CTrendsLocationDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO: Add extra initialization here

	m_hIcon = AfxGetApp()->LoadIcon(IDR_BriefTYPE);
	SetIcon(m_hIcon, FALSE);

	m_Combo.AdjustDroppedWidth();
	m_Combo.SetMode(CComboBoxExt::MODE_AUTOCOMPLETE);

	const auto geo_id = theApp.GetProfileString(_T("Settings"), _T("TrendsLocation"), _T("GB"));

	CTrends trends{};
	m_geo = trends.ReadGeos();
	for (const auto& it : m_geo)
	{
		const auto index = m_Combo.AddString(CString(it.first.c_str()));
		if (geo_id == it.second.c_str())
			m_Combo.SetCurSel(index);
	}

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void CTrendsLocationDlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	UpdateData();

	CString sGeo;
	m_Combo.GetWindowText(sGeo);

	if (sGeo.IsEmpty())
	{
		MessageBox(_T("Please choose an item!"), nullptr, MB_ICONERROR);
		m_Combo.ShowDropDown();
		return;
	}

	const auto found = m_geo.find(CW2A(sGeo.GetString(), CP_UTF8).m_psz);
	if (found == m_geo.end())
	{
		MessageBox(_T("Please choose an item from the list"), nullptr, MB_ICONERROR);
		m_Combo.ShowDropDown();
		return;
	}

	if (!found->second.empty())
	{
		theApp.WriteProfileString(_T("Settings"), _T("TrendsLocation"), CString(found->second.c_str()));
		theApp.WriteProfileString(_T("Settings"), _T("TrendsLocationName"), CString(found->first.c_str()));
	}

	CDialogEx::OnOK();
}
