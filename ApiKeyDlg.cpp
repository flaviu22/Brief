// CApiKeyDlg.cpp : implementation file
//

#include "pch.h"
#include "Brief.h"
#include "ApiKeyDlg.h"
#include "afxdialogex.h"

// CApiKeyDlg dialog

IMPLEMENT_DYNAMIC(CApiKeyDlg, CDialogEx)

CApiKeyDlg::CApiKeyDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_APIKEY, pParent)
	, m_sKey(_T(""))
{

}

void CApiKeyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_KEY, m_sKey);
}

BEGIN_MESSAGE_MAP(CApiKeyDlg, CDialogEx)
END_MESSAGE_MAP()

// CApiKeyDlg message handlers

BOOL CApiKeyDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here

	m_hIcon = AfxGetApp()->LoadIcon(IDR_BriefTYPE);
	SetIcon(m_hIcon, FALSE);

	m_sKey = theApp.GetProfileString(_T("Settings"), _T("ApiKey"));
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void CApiKeyDlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	UpdateData();

	theApp.WriteProfileString(_T("Settings"), _T("ApiKey"), m_sKey);

	CDialogEx::OnOK();
}
