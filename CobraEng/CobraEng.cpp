// CobraEng.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <Windows.h>
#include "jansson.h"
#include <Shlwapi.h>
#include <atlstr.h>


#pragma comment(lib, "Shlwapi.lib")


typedef int (__cdecl *lpscenter_1)(const char* lpScenterPath, const char* lpVirDbPath);
lpscenter_1 scenter_1 = NULL;

typedef BYTE* (__cdecl *lpscenter_10)(signed int a1, json_t* a2, LPVOID a3, LPVOID a4);
lpscenter_10 scenter_10 = NULL;

typedef json_t* (__cdecl *lpjson_pack)(const char *fmt, ...);
lpjson_pack myjson_pack  = NULL;

typedef char* (__cdecl *lpjson_dumps)(const json_t *json, size_t flags);
lpjson_dumps myjson_dumps = 0;

LPVOID lpscenter_10obj = NULL;
BYTE BB[2840];
int a = 0;


// ÿ���ļ�ɨ����ɺ����õ�����ص�����
int __cdecl EveryScanCall(int a1, char* a2, int a3)
{
	printf("�Ѿ�ɨ������� %s\r\n", a2);
	a++;
	return 1;
}


// ɨ���в����Ļ������������,û�����Ͳ������
signed int __cdecl VirusReportCallBack(int a1, json_t*  a2, int a3)
{

	char* abc = myjson_dumps(a2, JSON_COMPACT);
	// 
	printf("ɨ������ is %s\r\n", abc);
	return 1;
}

int _tmain(int argc, _TCHAR* argv[])
{
	HMODULE hCobra = ::LoadLibrary(L"scenter.dll");
    scenter_1 = (lpscenter_1)GetProcAddress(hCobra, (LPCSTR)0x1);
    scenter_10 = (lpscenter_10)GetProcAddress(hCobra, (LPCSTR)0xA);
	
	HMODULE hJansson = ::LoadLibrary(L"jansson.dll");
    myjson_pack = (lpjson_pack)GetProcAddress(hJansson, "json_pack");
    myjson_dumps = (lpjson_dumps)GetProcAddress(hJansson, "json_dumps");
    
	// ɨ���·��
	json_t*  valuePath = myjson_pack("[s]", "F:\\PyPo");
                        
	json_t * all = myjson_pack("{s:s,s:o,s:{s:b,s:b,s:b,s:b,s:s,s:i,s:b,s:s,s:s,s:b,s:b,s:b,s:b,s:b,s:i,s:i,s:i,s:i,s:b,s:b,s:b,s:b}}",
		"name", "demand", 
		"scl", valuePath,
		"conf",
		"decompo.installer", 1,
		"mon.installation", 1,
		"decompo.sfx", 1,
		"mon.exec", 1,
		"scan.exclusion.proc", "",
		"scan.threads", -1,
		"clean.quarantine", true,
		"scan.dbdir", "",
		"scan.exclusion.ext", "",
		"mon.open", true,
		"mon.creation", true,
		"tech.dtscan", true,
		"decompo.compr", true,
		"tech.advheur", true,
		"decompo.limit.count", 0,
		"decompo.limit.size", 0,
		"decompo.limit.level", 32,
		"decompo.limit.time", 30,
		"tech.smartopt", true,
		"decompo.compound", true,
		"tech.unpack", true,
		"scan.recursive", true
		);

	char* abc = myjson_dumps(all, JSON_COMPACT);
	
	char path[MAX_PATH];
	GetModuleFileNameA(NULL, path, MAX_PATH);

	::PathRemoveFileSpecA(path);

	CAtlStringA strScenterPath;
	CAtlStringA strVirDbPath;

	strScenterPath.Format("%s\\scenter", path);

	strVirDbPath.Format("%s\\virdb", path);


    int ret = scenter_1(strScenterPath.GetString(), strVirDbPath.GetString());
    
  
	if (ret == 0) // ���������óɹ�
	{
		printf("funxck xxxx\r\n");
	}
    
	DWORD as[5];
	memset(as, 0, sizeof(DWORD)*5); // Ӧ����һ���ص���������,���ĸ������ǲ�������ص�
	as[2] = (DWORD)EveryScanCall;
	as[3] = (DWORD)VirusReportCallBack;

	memset(BB, 0, 2840);
    lpscenter_10obj = scenter_10(2, all, as, 0);

	__asm
	{
		pushad
		
		mov eax, dword ptr[lpscenter_10obj]
		mov eax, dword ptr[eax+0x18]
		
		lea ebx, dword ptr[BB]
		push ebx

	    mov esi, dword ptr[lpscenter_10obj]
		push esi

        call eax
		add esp, 8
		
		popad
	}

    __asm
	{
		pushad
		
		mov eax, dword ptr[lpscenter_10obj]
		mov eax, dword ptr[eax+0x14]
		
		push  0
		mov esi, dword ptr[lpscenter_10obj]
		push esi
		
		call eax
		add  esp, 8
		
		popad
	}
    
	// ��ס
	getchar();
	return 0;
}

