
#include "stdafx.h"
#include "MyMisc.h"


// �����ܰ�� �̷���� ������ ������丮���� �ѹ��� ���������ش�
BOOL CreateMultipleDirectory(CString dd)
{
	HANDLE		fFile;					// File Handle
	WIN32_FIND_DATA fileinfo;			// File Information Structure
	CStringArray	m_arr;				// CString Array to hold Directory Structures
	BOOL tt;							// BOOL used to test if Create Directory was successful
	int x1 = 0;							// Counter
	CString tem = _T("");					// Temporary CString Object

	// Before we go to a lot of work.  
	// Does the file exist

	fFile = FindFirstFile(dd,&fileinfo);

	// if the file exists and it is a directory
	if(fileinfo.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
	{
		//  Directory Exists close file and return
		FindClose(fFile);
		return TRUE;
	}

	m_arr.RemoveAll();					// Not really necessary - Just habit
	for(x1=0;x1<dd.GetLength();x1++)	// Parse the supplied CString Directory String
	{									
		if(dd.GetAt(x1) != '\\')		// if the Charachter is not a \ 
			tem += dd.GetAt(x1);		// else add character to Temp String
		else
		{
			m_arr.Add(tem);				// if the Character is a \ Add the Temp String to the CString Array
			tem += "\\";				// Now add the \ to the temp string
		}
		if(x1 == dd.GetLength()-1)		// If we reached the end of the file add the remaining string
			m_arr.Add(tem);
	}


	// Close the file
	FindClose(fFile);
	
	// Now lets cycle through the String Array and create each directory in turn
	for(x1 = 1;x1<m_arr.GetSize();x1++)
	{
		tem = m_arr.GetAt(x1);
		tt = CreateDirectory(tem, NULL);

		// If the Directory exists it will return a false
		// If we were successful we set the attributes to normal

		//if(tt) SetFileAttributes(tem, FILE_ATTRIBUTE_NORMAL);
	}
	m_arr.RemoveAll();
	//  Now lets see if the directory was successfully created
	fFile = FindFirstFile(dd,&fileinfo);

	// if the file exists and it is a directory
	if(fileinfo.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
	{
		//  Directory Exists close file and return
		FindClose(fFile);
		return TRUE;
	}
	else
	{
		FindClose(fFile);
		return FALSE;
	}
}


// ������ ���丮�� ���� ������ ������ ��� �ѹ��� �����ϴ� �Լ�
long RemoveDirectoryAll(CString cstrBasePath, BOOL bRecursive)
{
	CFileFind finder;
	BOOL bWorking;
	CString cstrDirTemp;
	CString cstrBasePathTemp;
	CString cstrFileTemp;

	char OldCurDir[_MAX_PATH+1];

	long lCount=0;

	// ������ ���� ���丮 ����
	GetCurrentDirectory(_MAX_PATH+1, OldCurDir);

	// ���� ���丮�� ����
	if(SetCurrentDirectory((LPCTSTR)cstrBasePath) == FALSE) return lCount;

	bWorking=finder.FindFile(_T("*.*"));

	if(bRecursive)	// ��� ȣ���̸�
	{
		while(bWorking)
		{
			bWorking=finder.FindNextFile();

			if(finder.IsDirectory())	// ���� ���丮�� ���ȣ��
			{
				cstrDirTemp=finder.GetFileName();

				if(cstrDirTemp.CompareNoCase(".")!=0 && cstrDirTemp.CompareNoCase("..")!=0)
				{
					cstrBasePathTemp=cstrBasePath+"\\"+cstrDirTemp;
					lCount+=RemoveDirectoryAll(cstrBasePathTemp, bRecursive);
				}
			}
			else						// �����̸� ����
			{
				lCount++;
				cstrFileTemp=finder.GetFileName();
				DeleteFile((LPCTSTR)cstrFileTemp);
			}
		}

		finder.Close();

		// ������ ���� ���丮�� ����
		SetCurrentDirectory(OldCurDir);

		// ���� ����
		RemoveDirectory((LPCTSTR)cstrBasePath);
		return lCount++;
	}
	else // ��� ȣ���� �ƴ�
	{
		while(bWorking)
		{
			bWorking=finder.FindNextFile();

			if(!finder.IsDirectory())
			{
				lCount++;
				cstrFileTemp=finder.GetFileName();
				DeleteFile((LPCTSTR)cstrFileTemp);
			}
		}

		finder.Close();

		// ������ ���� ���丮�� ����
		SetCurrentDirectory(OldCurDir);

		// ���� ����
		RemoveDirectory((LPCTSTR)cstrBasePath);
		return lCount++;
	}
}

