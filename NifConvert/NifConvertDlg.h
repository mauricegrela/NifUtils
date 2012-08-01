// NifConvertDlg.h : header file
//

#pragma once

#include <set>

using namespace std;

// CNifConvertDlg dialog
class CNifConvertDlg : public CDialog
{
// Construction
public:
	CNifConvertDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_NIFCONVERT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON			m_hIcon;
	CString			m_fileNameAry[3];
	CString			m_texturePath;
	CRichEditCtrl	m_logView;
	CToolTipCtrl	m_toolTip;

  // Generated message map functions
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

  virtual void  parseDir(CString path, set<string>& directories, bool doDirs=true);

public:
	afx_msg void OnBnClickedButtonInput();
	afx_msg void OnBnClickedButtonOutput();
	afx_msg void OnBnClickedButtonTemplate();
	afx_msg void OnBnClickedButtonTexture();
	afx_msg void OnBnClickedOk();
	afx_msg void OnContextMenu(CWnd*, CPoint point);
	afx_msg void OnDefaultSavesettings();
	afx_msg void OnDefaultReloaddirectories();

	virtual void logMessage(const int type, const char* pMessage);
};
