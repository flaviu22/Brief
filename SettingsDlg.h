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
	UINT m_nWeatherHours;
	UINT m_nWeatherDays;
	CSpinButtonCtrl m_SpinHours;
	CSpinButtonCtrl m_SpinDays;
	CToolTipCtrl m_ToolTip;

protected:
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
    afx_msg void OnEnChangeEditWeatherapikey();
    afx_msg void OnEnChangeEditTrendsaddress();
    afx_msg void OnEnChangeEditGoogleapiskey();
};
