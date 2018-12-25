#pragma once

#include <tchar.h>
class DriverFile
{
	public:
		DriverFile();
		~DriverFile();

	private:
		TCHAR *fullPath;
		CString commonFile;
		TCHAR *driverTitle;
	public:
		TCHAR * getFullPath();
		CString getCommonFile();
		TCHAR * getDriverTitle();
		void setFullPath(TCHAR * fullPath);
		void setCommonFile(CString commonFile);
		void setDriverTitle(TCHAR* title);

};