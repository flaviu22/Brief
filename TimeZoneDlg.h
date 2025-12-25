#pragma once
#include "afxdialogex.h"


// CTimeZoneDlg dialog

class CTimeZoneDlg final : public CDialogEx
{
	DECLARE_DYNAMIC(CTimeZoneDlg)

public:
	CTimeZoneDlg(CWnd* pParent = nullptr);   // standard constructor
	~CTimeZoneDlg() override = default;

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_TIMEZONE };
#endif

protected:
	HICON m_hIcon;
	// Combo that contain the time zone values
	CComboBox m_Combo;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual void OnOK();

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};
