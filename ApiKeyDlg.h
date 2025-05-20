#pragma once

// CApiKeyDlg dialog

class CApiKeyDlg final : public CDialogEx
{
	DECLARE_DYNAMIC(CApiKeyDlg)

public:
	CApiKeyDlg(CWnd* pParent = nullptr);   // standard constructor
	~CApiKeyDlg() = default;

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_APIKEY };
#endif

protected:
	HICON m_hIcon;
	CString m_sKey;

protected:
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
