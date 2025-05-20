#pragma once

#include "Data.h"
#include "BriefDoc.h"

#include "AnchorMapCtrl.h"

// CWeatherLocationDlg dialog

class CWeatherLocationDlg : public CDialogEx
{
	DECLARE_ANCHOR_MAP()
	DECLARE_DYNAMIC(CWeatherLocationDlg)

public:
	CWeatherLocationDlg(CBriefDoc* pDoc, CWnd* pParent = nullptr);   // standard constructor

protected:
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
	void PopulateList(std::vector<SWeatherLocation> locations);

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_LOCATION };
#endif

protected:
	HICON m_hIcon{};
	CBriefDoc* m_pDoc{};
	CListCtrl m_List;

private:
	CString m_sText;

protected:
	// Generated message map functions
	//{{AFX_MSG(CQuickListDlg)
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnEnChangeEditText();
    afx_msg void OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
