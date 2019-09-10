#include "stdafx.h"
#include <iostream>  
#include <algorithm>  
#include <cmath>  
#include <vector>  
#include <string>  
#include <cstring>
#include <atlbase.h>
#include <Windows.h>
#pragma warning(disable:4996)  
using namespace std;

#include <Aclapi.h> 
#pragma comment (lib,"Advapi32.lib")


DWORD AddAceToObjectsSecurityDescriptor(
	LPTSTR pszObjName,          // name of object
	SE_OBJECT_TYPE ObjectType,  // type of object
	LPTSTR pszTrustee,          // trustee for new ACE
	TRUSTEE_FORM TrusteeForm,   // format of trustee structure
	DWORD dwAccessRights,       // access mask for new ACE
	ACCESS_MODE AccessMode,     // type of ACE
	DWORD dwInheritance         // inheritance flags for new ACE
)
{
	DWORD dwRes = 0;
	PACL pOldDACL = NULL, pNewDACL = NULL;
	PSECURITY_DESCRIPTOR pSD = NULL;
	EXPLICIT_ACCESS ea;

	if (NULL == pszObjName)
		return ERROR_INVALID_PARAMETER;

	// Get a pointer to the existing DACL.

	dwRes = GetNamedSecurityInfo(pszObjName, ObjectType,
		DACL_SECURITY_INFORMATION,
		NULL, NULL, &pOldDACL, NULL, &pSD);
	if (ERROR_SUCCESS != dwRes) {
		printf("GetNamedSecurityInfo Error %u\n", dwRes);
		goto Cleanup;
	}

	// Initialize an EXPLICIT_ACCESS structure for the new ACE. 

	ZeroMemory(&ea, sizeof(EXPLICIT_ACCESS));
	ea.grfAccessPermissions = dwAccessRights;
	ea.grfAccessMode = AccessMode;
	ea.grfInheritance = dwInheritance;
	ea.Trustee.TrusteeForm = TrusteeForm;
	ea.Trustee.ptstrName = pszTrustee;

	// Create a new ACL that merges the new ACE
	// into the existing DACL.

	dwRes = SetEntriesInAcl(1, &ea, pOldDACL, &pNewDACL);
	if (ERROR_SUCCESS != dwRes) {
		printf("SetEntriesInAcl Error %u\n", dwRes);
		goto Cleanup;
	}

	// Attach the new ACL as the object's DACL.

	dwRes = SetNamedSecurityInfo(pszObjName, ObjectType,
		DACL_SECURITY_INFORMATION,
		NULL, NULL, pNewDACL, NULL);
	if (ERROR_SUCCESS != dwRes) {
		printf("SetNamedSecurityInfo Error %u\n", dwRes);
		goto Cleanup;
	}

Cleanup:

	if (pSD != NULL)
		LocalFree((HLOCAL)pSD);
	if (pNewDACL != NULL)
		LocalFree((HLOCAL)pNewDACL);

	return dwRes;
}

void SetRegPrivilege()
{
	DWORD dwRet;

	// ��������ַ�����ֵ�޸�Ϊ��Ҫ����Ȩ�޲�����ע����ע���ÿһ����Ȩ���ǲ�һ���ģ�������Ҫ�ܾ����ָ����ĳһ��
	LPTSTR SamName = _T("\\\\DESKTOP-AMF11P6\\MACHINE\\SOFTWARE\\FreeRDP\\FreeRDP\\Server");
	PSECURITY_DESCRIPTOR pSD = NULL;
	PACL pOldDacl = NULL;
	PACL pNewDacl = NULL;
	EXPLICIT_ACCESS ea;
	HKEY hKey = NULL;



	// ��ȡ������DACL 
	dwRet = GetNamedSecurityInfo(SamName, SE_REGISTRY_KEY, DACL_SECURITY_INFORMATION,
		NULL, NULL, &pOldDacl, NULL, &pSD);
	if (dwRet != ERROR_SUCCESS)
	{
		printf("GetNamedSecurityInfo Error: %d\n", dwRet);
		goto FreeAndExit;
	}

	// ����һ��ACE������Everyone��ȫ���ƶ��󣬲������Ӷ���̳д�Ȩ�� 
	ZeroMemory(&ea, sizeof(EXPLICIT_ACCESS));
	BuildExplicitAccessWithName(&ea, _T("Administrators"), KEY_ALL_ACCESS, SET_ACCESS,
		SUB_CONTAINERS_AND_OBJECTS_INHERIT);

	// ���µ�ACE����DACL 
	dwRet = SetEntriesInAcl(1, &ea, pOldDacl, &pNewDacl);
	if (dwRet != ERROR_SUCCESS)
	{
		printf("SetEntriesInAcl Error: %d\n", dwRet);
		goto FreeAndExit;
	}

	// ����SAM������DACL 
	dwRet = SetNamedSecurityInfo(SamName, SE_REGISTRY_KEY, DACL_SECURITY_INFORMATION,
		NULL, NULL, pNewDacl, NULL);
	if (dwRet != ERROR_SUCCESS)
	{
		printf("SetNamedSecurityInfo Error: %d\n", dwRet);
		goto FreeAndExit;
	}

	// ��SAM���Ӽ� 
	dwRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\FreeRDP\\FreeRDP\\Server"),
		0, KEY_ALL_ACCESS, &hKey);
	if (dwRet != ERROR_SUCCESS)
	{
		printf("RegOpenKeyEx Error: %d\n", dwRet);
		goto FreeAndExit;
	}

	printf("Open SAM Subkey Successfully.\n");

FreeAndExit:
	if (hKey) RegCloseKey(hKey);
	if (pNewDacl) LocalFree(pNewDacl);
	// ��ԭSAM������DACL 
	if (pOldDacl) SetNamedSecurityInfo(SamName, SE_REGISTRY_KEY, DACL_SECURITY_INFORMATION,
		NULL, NULL, pOldDacl, NULL);
	if (pSD) LocalFree(pSD);
	return;
}


//ע�������ǣ� 64λҪ�ӣ�KEY_WOW64_64KEYȨ��

void read_dword()//��ȡ������,������ΪDWORD
{
	HKEY hKEY;//�����йصļ����ڲ�ѯ����ʱ�ر�
			  //����·��data_Set��ص�hKEY

	LPCTSTR data_set = _T("SOFTWARE\\FreeRDP\\FreeRDP\\Server");

	//����ע���hKEY�򱣴�˺������򿪵ļ��ľ��
	if (ERROR_SUCCESS == ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, data_set, 0, KEY_READ | KEY_WOW64_64KEY, &hKEY))
	{
		DWORD dwValue;//���������ݣ�������ַ���������char����
		DWORD dwSize = sizeof(DWORD);
		DWORD dwType = REG_DWORD;

		if (::RegQueryValueEx(hKEY, _T("DefaultPort"), 0, &dwType, (LPBYTE)&dwValue, &dwSize) != ERROR_SUCCESS)
		{
			cout << "�����޷���ѯ�йص�ע�����Ϣ" << endl;
		}

		cout << "dwValue=" << dwValue << endl;
	}
	::RegCloseKey(hKEY);
}

void read_reg_sz()//��ȡ������,������ΪREG_SZ
{
	HKEY hkey;
	LPCTSTR data_set = _T("SOFTWARE\\FreeRDP\\FreeRDP\\Server");

	if (ERROR_SUCCESS == ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, data_set, 0, KEY_READ | KEY_WOW64_64KEY, &hkey))
	{
		char dwValue[256];
		DWORD dwSzType = REG_SZ;
		DWORD dwSize = sizeof(dwValue);
		if (::RegQueryValueEx(hkey, _T("qingcloud"), 0, &dwSzType, (LPBYTE)&dwValue, &dwSize) != ERROR_SUCCESS)
		{
			cout << "�޷���ѯ�йص�ע�����Ϣ" << endl;
		}
		cout << dwValue << endl;
	}
	::RegCloseKey(hkey);
}

void write_dword()//��\SOFTWARE\FreeRDP\FreeRDP\Server�ļ�����д��һ��test���Ӽ�������������ΪName����ֵΪ6
{
	HKEY hkey;//�����йص�hkey���ڲ�ѯ����ʱҪ�ر�
	HKEY hTempKey;

	DWORD dwValue = 6;
	DWORD dwSize = sizeof(DWORD);
	DWORD dwType = REG_DWORD;

	LPCTSTR data_set = _T("SOFTWARE\\FreeRDP\\FreeRDP\\Server");
	if (ERROR_SUCCESS == RegOpenKeyEx(HKEY_LOCAL_MACHINE, data_set, 0, KEY_SET_VALUE | KEY_WOW64_64KEY, &hkey))
	{
		if (ERROR_SUCCESS == ::RegCreateKey(hkey, _T("test"), &hTempKey))
		{
			if (ERROR_SUCCESS != ::RegSetValueEx(hTempKey, _T("Name"), 0, REG_DWORD, (CONST BYTE*)&dwValue, sizeof(DWORD)))
			{
				cout << "д��ע���ʧ��" << endl;
			}
		}
	}
	::RegCloseKey(hkey);
}

void write_reg_sz()
{
	HKEY hkey;
	HKEY hTempKey;
	char m_name_set[256] = "China";

	DWORD len = strlen(m_name_set) + 1;
	LPCTSTR data_set = _T("SOFTWARE\\FreeRDP\\FreeRDP\\Server");
	if (ERROR_SUCCESS == ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, data_set, 0, KEY_SET_VALUE | KEY_WOW64_64KEY, &hkey))
	{
		if (ERROR_SUCCESS == ::RegCreateKey(hkey, _T("test"), &hTempKey))
		{
			if (ERROR_SUCCESS != ::RegSetValueEx(hTempKey, _T("Name"), 0, REG_SZ, (const BYTE*)m_name_set, len))
			{
				cout << "д�����" << endl;
			}
		}
	}
	::RegCloseKey(hkey);
}

void write_binary()
{
	HKEY hkey;
	HKEY hTempKey;
	BYTE m_name[10];
	memset(m_name, 0, sizeof(m_name));
	m_name[0] = 0xff;
	m_name[1] = 0xac;
	m_name[2] = 0x05;
	m_name[3] = 0x4e;

	LPCTSTR data_set = _T("SOFTWARE\\FreeRDP\\FreeRDP\\Server");
	if (ERROR_SUCCESS == ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, data_set, 0, KEY_SET_VALUE | KEY_WOW64_64KEY, &hkey))
	{
		if (ERROR_SUCCESS == ::RegCreateKey(hkey, _T("test"), &hTempKey))
		{
			if (ERROR_SUCCESS != ::RegSetValueEx(hTempKey, _T("Name"), 0, REG_BINARY, (unsigned char *)m_name, 5))
			{
				cout << "д�����" << endl;
			}
		}
	}
	::RegCloseKey(hkey);
}

void delete_value()
{
	HKEY hkey;
	LPCTSTR data_set = _T("SOFTWARE\\FreeRDP\\FreeRDP\\Server\\test");

	if (ERROR_SUCCESS == ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, data_set, 0, KEY_SET_VALUE | KEY_WOW64_64KEY, &hkey))
	{
		if (ERROR_SUCCESS != ::RegDeleteValue(hkey, _T("Name")))
		{
			cout << "ɾ������" << endl;
		}
	}
	::RegCloseKey(hkey);
}

void delete_key()
{
	HKEY hkey;
	LPCTSTR data_set = _T("SOFTWARE\\FreeRDP\\FreeRDP\\Server");

	if (ERROR_SUCCESS == ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, data_set, 0, KEY_SET_VALUE | KEY_WOW64_64KEY, &hkey))
	{
		if (ERROR_SUCCESS != ::RegDeleteKey(hkey, _T("test")))
		{
			cout << "ɾ������" << endl;
		}
	}
	::RegCloseKey(hkey);
}

int main()
{
	read_dword();
	read_reg_sz();
	write_reg_sz();
	write_binary();
	delete_value();
	delete_key();
	system("pause");
	return 0;
}