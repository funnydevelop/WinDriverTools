#pragma once

#include "WinDriverToolsDlg.h"
// RegisterDlg 对话框

class RegisterDlg : public CDialogEx
{
	DECLARE_DYNAMIC(RegisterDlg)

public:
	RegisterDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~RegisterDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_ADD };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
private:
	bool validateRegisterKeyValue(CString path,CString key,CString value);
	void initSelectList();
	wchar_t* charToWchar(char* str);
	char* charToWchar(wchar_t * wstr);
public:
	virtual BOOL OnInitDialog();
	CComboBox selectList;
};
