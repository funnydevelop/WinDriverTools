
// WinDriverToolsDlg.h: 头文件
//

#pragma once

#include<iostream>
#include<cstdlib>
#include<cstdio>
#include<string.h>
#include<windows.h>
#include"DriverFile.h"
#include"RegisterDlg.h"
#define ERROR 0
#define REPORT 1
using namespace std;
// CWinDriverToolsDlg 对话框
class CWinDriverToolsDlg : public CDialogEx
{
// 构造
public:
	CWinDriverToolsDlg(CWnd* pParent = nullptr);	// 标准构造函数
	~CWinDriverToolsDlg();
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_WINDRIVERTOOLS_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	DECLARE_MESSAGE_MAP()
public:
	static TCHAR errmsg[MAX_PATH];
	static int glo_set;
private:
	DriverFile* driverFile;
	COLORREF    fontColor;
	const int Install ,Start, Uninstall,Stop;

public:
	afx_msg void OnBnClickedButtonInstall();
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonStop();
	afx_msg void OnBnClickedButtonUninstall();
	afx_msg void OnBnClickedButtonClose();
	afx_msg void OnBnClickedButtonFile();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
private:
	BOOL CWinDriverToolsDlg::doServiceOperation(TCHAR *szFullPath, TCHAR *szName, int iType);
	TCHAR* getTitleName(TCHAR* filePath);
	void doReport(HWND hwnd, TCHAR* str, int type);
	bool OnNotiftyServiceAction(int operatorType);
public:
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonDel();
public:
	CListCtrl regListCtl;
	int regListCtlNum;
};
