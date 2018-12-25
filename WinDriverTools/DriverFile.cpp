#include "stdafx.h"
#include"DriverFile.h"

DriverFile::DriverFile()
{
	fullPath = new TCHAR[MAX_PATH];
	driverTitle = new TCHAR[MAX_PATH];
	commonFile = "";
}

DriverFile::~DriverFile()
{
	delete[] fullPath;
	fullPath = NULL;
	delete[] driverTitle;
	driverTitle = NULL;
}
TCHAR * DriverFile::getFullPath() 
{
	return fullPath;
}
CString DriverFile::getCommonFile()
{
	return commonFile;
}
void DriverFile::setFullPath(TCHAR * fullPath)
{
	if (this->fullPath != NULL)
	{
		delete[] this->fullPath;
		this->fullPath = NULL;
	}
	else
	{
		this->fullPath = new TCHAR[MAX_PATH];
		_tcscpy(this->fullPath, fullPath);
	}
}
void DriverFile::setCommonFile(CString commonFile)
{
	this->commonFile = commonFile;
}
TCHAR * DriverFile::getDriverTitle()
{
	return this->driverTitle;
}
void DriverFile::setDriverTitle(TCHAR * title)
{
	_tcscpy(this->driverTitle, title);
}