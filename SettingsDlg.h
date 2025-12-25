#pragma once
#include "afxdialogex.h"
#include <bitset>

// CSettingsDlg dialog

class CSettingsDlg final : public CDialogEx
{
	DECLARE_DYNAMIC(CSettingsDlg)

public:
	CSettingsDlg(CWnd* pParent = nullptr);   // standard constructor
	~CSettingsDlg() override = default;

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SETTINGS };
#endif

protected:
	HICON m_hIcon{};
	CString m_sWeatherapiKey;
	CString m_sGoogleapisKey;
	CString m_sTrendsAddress;

protected:
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
    afx_msg void OnEnChangeEditWeatherapikey();
    afx_msg void OnEnChangeEditTrendsaddress();
    afx_msg void OnEnChangeEditGoogleapiskey();
};
