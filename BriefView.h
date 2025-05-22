// BriefView.h : interface of the CBriefView class
//

#pragma once

#include <afxinet.h>
#include <bitset>

class CBriefDoc;

class CBriefView : public CHtmlView
{
protected: // create from serialization only
	CBriefView() noexcept;
	DECLARE_DYNCREATE(CBriefView)

public:
#ifdef AFX_DESIGN_TIME
//	enum{ IDD = IDD_Brief_FORM };
#endif

// Attributes
public:
	CBriefDoc* GetDocument() const;
	void Refresh();

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnUpdate(CHtmlView* pSender, LPARAM lHint, CObject* pHint);
    virtual void OnDocumentComplete(LPCTSTR lpszURL);

// Implementation
public:
	~CBriefView() override;
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	void OnDraw(CDC* pDC);
	void DoPageRefresh();

private:
	BOOL m_bInit{ FALSE };
	std::bitset<3> m_InternetDataStatus{};

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnFilePrintPreview();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnFileMove();
	afx_msg void OnFileKeep();
	afx_msg void OnUpdateFileKeep(CCmdUI* pCmdUI);
	afx_msg void OnViewRefresh();
	afx_msg LRESULT OnThreadData(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPostInit(WPARAM wParam, LPARAM lParam);
};

#ifndef _DEBUG  // debug version in BriefView.cpp
inline CBriefDoc* CBriefView::GetDocument() const
   { return reinterpret_cast<CBriefDoc*>(m_pDocument); }
#endif
