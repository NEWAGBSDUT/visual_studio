#pragma once
#include <string>
#include <atlstr.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <locale.h>
using namespace std;

#define local_str_cm(s1,s2) ( (wcslen(s1)==wcslen(s2) && memcmp(s1,s2,wcslen(s1))==0) ? (1) : (0) )

// string �ַ��� ת��Ϊ wstring
wstring str2wstr(const string& s);
// string �ַ��� ת��Ϊ wchar_t
wchar_t* str2wchar(const string& s);

const char* tchar2cchat(wchar_t* s);