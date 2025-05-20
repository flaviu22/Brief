// MoveDlg.cpp : implementation file
//

#include "pch.h"
#include "Brief.h"
#include "MoveDlg.h"
#include "afxdialogex.h"

// CMoveDlg dialog

IMPLEMENT_DYNAMIC(CMoveDlg, CDialogEx)

CMoveDlg::CMoveDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_MOVE, pParent)
	, m_nMove(0)
{
}

void CMoveDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SPIN_MOVE, m_Spin);
	DDX_Text(pDX, IDC_EDIT_MOVE, m_nMove);
}

BEGIN_MESSAGE_MAP(CMoveDlg, CDialogEx)
END_MESSAGE_MAP()

// CMoveDlg message handlers

BOOL CMoveDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO: Add extra initialization here

	m_hIcon = AfxGetApp()->LoadIcon(IDR_BriefTYPE);
	SetIcon(m_hIcon, FALSE);

	m_Spin.SetRange(7, 900);
	m_Spin.SetPos(theApp.GetProfileInt(_T("Settings"), _T("MoveIntervalSeconds"), 77));
	m_nMove = m_Spin.GetPos();

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void CMoveDlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	UpdateData();

	if (m_Spin.GetPos() < 7 || m_Spin.GetPos() > 900)
	{
		MessageBox(_T("The time interval should be between 7 and 900"), nullptr, MB_ICONERROR);
		return;
	}

	theApp.WriteProfileInt(_T("Settings"), _T("MoveIntervalSeconds"), m_Spin.GetPos());

	CDialogEx::OnOK();
}
