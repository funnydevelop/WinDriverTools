// RegisterDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "WinDriverTools.h"
#include "RegisterDlg.h"
#include "afxdialogex.h"
#include <list>

// RegisterDlg 对话框

IMPLEMENT_DYNAMIC(RegisterDlg, CDialogEx)

RegisterDlg::RegisterDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_ADD, pParent)
{

}

RegisterDlg::~RegisterDlg()
{
}

void RegisterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, selectList);
}


BEGIN_MESSAGE_MAP(RegisterDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &RegisterDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &RegisterDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// RegisterDlg 消息处理程序


void RegisterDlg::OnBnClickedOk()
{
	CWinDriverToolsDlg *parentWnd = (CWinDriverToolsDlg*)GetParent();
	parentWnd->GetDlgItem(IDC_STATIC_ERRMSG)->SetWindowTextW(_T("hasbeen"));
	// TODO: 在此添加控件通知处理程序代码
	CString path,key,value;
	this->GetDlgItem(IDC_EDIT_REG_PATH)->GetWindowText(path);
	this->GetDlgItem(IDC_EDIT_REG_KEY)->GetWindowText(key);
	this->GetDlgItem(IDC_EDIT_REG_VALUE)->GetWindowText(value);
	if (!validateRegisterKeyValue(path, key, value))
	{
		CDialogEx::OnOK();
	}
	else 
	{
		int row = parentWnd->regListCtl.InsertItem(parentWnd->regListCtlNum, path);
		parentWnd->regListCtl.SetItemText(row, 1, key);
		parentWnd->regListCtl.SetItemText(row, 2, value);
		parentWnd->regListCtlNum++;
	}
	CDialogEx::OnOK();
}
/**/
bool RegisterDlg::validateRegisterKeyValue(CString path,
										   CString key,
										   CString value)
{
	HKEY hKey;
	DWORD dwDisposition;
	/*At present ,The value just support DWORD type to insert Register List,
	.It will be extended to support multi data */
	wchar_t* temp =  value.GetBuffer(value.GetLength());
	wchar_t* pend;
	long numBuffer = wcstol(temp, &pend, 16);
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE,
		path,
		0, KEY_ALL_ACCESS, &hKey) != ERROR_SUCCESS)
	{
		LONG msg = RegCreateKeyEx(HKEY_LOCAL_MACHINE,
			path,
			0,
			NULL,
			REG_OPTION_VOLATILE,
			KEY_ALL_ACCESS, NULL,
			&hKey,
			&dwDisposition);
		if (msg != ERROR_SUCCESS)
		{
			printf("%d :RegCreateKey failed with Erro \n",msg);
			return false;
		}
	}
	else
	{
		printf("RegOpenKeyEx Error ! \n");
		return false;
	}
	
	if (RegSetValueEx(hKey, key, 0,
		REG_DWORD,(LPBYTE)&numBuffer,
		sizeof(DWORD))!= ERROR_SUCCESS)
	{
		printf("RegSetValueEx failed with Erro \n");
		return false;
	}
	if (RegCloseKey(hKey) != ERROR_SUCCESS)
	{
		printf("RegCloseKey failed with Erro \n");
		return false;
	}
	return true;
}


void RegisterDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}


BOOL RegisterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CString title("Add Register List");
	this->SetWindowTextW(title);
	// TODO:  在此添加额外的初始化
	initSelectList();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
void RegisterDlg::initSelectList()
{
	char* typeset[] = { "REG_NONE","REG_SZ","REG_EXPAND_SZ",
						"REG_BINARY","REG_DWORD", "REG_DWORD_LITTLE_ENDIAN", 
						"REG_DWORD_BIG_ENDIAN", "REG_LINK", "REG_MULTI_SZ",
						"REG_RESOURCE_LIST" , "REG_FULL_RESOURCE_DESCRIPTOR" ,"REG_RESOURCE_REQUIREMENTS_LIST",
						"REG_QWORD","REG_QWORD_LITTLE_ENDIAN"};
	for (int i = 0; i < sizeof(typeset) / sizeof(char*); i++)
	{
		wchar_t * wstr = charToWchar(typeset[i]);
		selectList.AddString(wstr);
		delete wstr;
		wstr = NULL;
	}
	//set default value was REG_DWORD , because the program just supports REG_DWORD in this version 
	selectList.SetCurSel(4);

}
wchar_t* RegisterDlg::charToWchar(char* str)
{
	int n = MultiByteToWideChar(0, 0, str, -1, NULL, 0);
	wchar_t * wstr = new wchar_t[n];
	MultiByteToWideChar(0, 0, str, -1, wstr, n);
	return wstr;
}
char* RegisterDlg::charToWchar(wchar_t * wstr)
{
	int n = WideCharToMultiByte(CP_OEMCP, NULL, wstr, -1, NULL, 0, NULL, FALSE);
	char * str = new char[n];
	WideCharToMultiByte(CP_OEMCP, NULL, wstr, -1, str, n, NULL, FALSE);
	return str;
}