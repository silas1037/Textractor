#ifndef GENERALSTRING_H
#define GENERALSTRING_H
#pragma once
#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <fstream>

#include <iostream>
#include <mutex>
#include "tchar.h"
#include <Windows.h>
#include <string>
#include <map>
#include <vector>
#include <atlstr.h>


#ifdef _UNICODE //这个宏 与工程字符集有关
#define COUT wcout
#else
#define COUT cout
#endif

using namespace std;

map <CString, LPCWSTR> mdic;



int output(wchar_t* str) {
        setlocale(LC_ALL, "chs");
        wprintf(L"%ls", str);
        //wprintf(L"wprintf\twchar_t* : %ls\r\n", str);
        //wcout << L"wcout\twchar_t* : " << str << endl;
        //printf("\n\n");
        return 0;
}

int output(LPCWSTR str) {
        setlocale(LC_ALL, "chs");
        //printf("LPCWSTR to wchar_t*\n");
        return output((wchar_t*)str);
}

int output(const char* str) {
        setlocale(LC_ALL, "chs");
        printf("%s", str);
        //cout << "cout\tconst char * : " << str << endl;

        //wprintf(L"wprintf\tchar_t* : %s\r\n", str);
        //wprintf(L"wprintf\tchar_t* : %ls\r\n", str);
        //wcout << L"wcout\tchar_t* : " << str << endl;

        //printf("\n\n");
        return 0;
}

int output(char* str) {
        setlocale(LC_ALL, "chs");
        printf("%s", str);
        //cout << "cout\tchar * : " << str << endl;
        //printf("\n\n");
        return 0;
}

int output(string str) {
        setlocale(LC_ALL, "chs");
        //setlocale(LC_ALL, "chs");
        //printf("string to const char*\n");
        return output(str.c_str());
}

int output(wstring str) {
        setlocale(LC_ALL, "chs");
        //printf("wstring to const wchar_t *\n");
        return output(str.c_str());
}

//int output(LPWSTR str) {
//	return output((const char*)str);
//}

enum cvttype {
        _p_char,
        _l_char,
        _p_wchar_t,
        _l_wchar_t,

        _p_TCHAR,
        _l_TCHAR,

        _CString,
        _CStringA,
        _string,
        _wstring,

        _LPSTR,
        _LPCSTR,
        _LPWSTR,
        _LPCWSTR
};
#define MAXLEN 256
char* str_p_char;
char str_l_char[MAXLEN];
wchar_t* str_p_wchar_t;
wchar_t str_l_wchar_t[MAXLEN];
TCHAR* str_p_TCHAR;
TCHAR str_l_TCHAR[MAXLEN];
CString str_CString;
CStringA str_CStringA;
string str_string;
wstring str_wstring;
LPSTR str_LPSTR;
LPCSTR str_LPCSTR;
LPWSTR str_LPWSTR;
LPCWSTR str_LPCWSTR;

auto convert(LPCWSTR str1, cvttype typea) {
        //printf(" === locate at LPCWSTR\n");
        switch (typea)
        {
        case _p_char:
                str_p_char = (LPSTR)(LPCSTR)(CStringA)str1;
                break;
        case _l_char:
                strcpy(str_l_char, (LPSTR)(LPCSTR)(CStringA)str1);
                break;
        case _p_wchar_t:
                str_p_wchar_t = (wchar_t*)str1;
                break;
        case _l_wchar_t:
                wcscpy(str_l_wchar_t, str1);
                break;


        case _p_TCHAR:
                str_p_TCHAR = (TCHAR*)str1;
                break;
        case _l_TCHAR:
                wcscpy((wchar_t*)str_l_TCHAR, (wchar_t*)str1);
                break;


        case _CString:
                str_CString = str1;
                break;
        case _CStringA:
                str_CStringA = (CStringA)str1;
                break;
        case _string:
                str_string = (LPCSTR)(CStringA)str1;
                break;
        case _wstring:
                str_wstring = str1;
                break;


        case _LPSTR:
                str_LPSTR = (LPSTR)(LPCSTR)(CStringA)str1;
                break;
        case _LPCSTR:
                str_LPCSTR = (LPCSTR)(CStringA)str1;
                break;
        case _LPWSTR:
                str_LPWSTR = (LPWSTR)str1;
                break;
        case _LPCWSTR:
                str_LPCWSTR = str1;
                break;
        default:
                break;
        }
        return typea;
}

auto convert(CString str1, cvttype typea) {
        //printf(" === locate at CString\n");
        switch (typea)
        {
        case _p_char:
                str_p_char = (LPSTR)(LPCSTR)(CStringA)str1;
                break;
        case _l_char:
                strcpy(str_l_char, (LPSTR)(LPCSTR)(CStringA)str1);
                break;
        case _p_wchar_t:
                str_p_wchar_t = (wchar_t*)(LPCWSTR)str1;
                break;
        case _l_wchar_t:
                wcscpy(str_l_wchar_t, str1);
                break;


        case _p_TCHAR:
                str_p_TCHAR = (TCHAR*)(LPCWSTR)str1;
                break;
        case _l_TCHAR:
                wcscpy((wchar_t*)str_l_TCHAR, (wchar_t*)(LPCWSTR)str1);
                break;


        case _CString:
                str_CString = str1;
                break;
        case _CStringA:
                str_CStringA = (CStringA)str1;
                break;
        case _string:
                str_string = (LPCSTR)(CStringA)str1;
                break;
        case _wstring:
                str_wstring = str1;
                break;


        case _LPSTR:
                str_LPSTR = (LPSTR)(LPCSTR)(CStringA)str1;
                break;
        case _LPCSTR:
                str_LPCSTR = (LPCSTR)(CStringA)str1;
                break;
        case _LPWSTR:
                str_LPWSTR = (LPWSTR)(LPCWSTR)str1;
                break;
        case _LPCWSTR:
                str_LPCWSTR = str1;
                break;
        default:
                break;
        }
        return typea;
}


auto convert(char * str1, cvttype typea) {
        //printf(" === locate at char_t*\n");
        str_CString = str1;
        return convert(str_CString, typea);
}

auto convert(wchar_t* str1, cvttype typea) {
        //printf(" === locate at wchar_t*\n");
        //str_CString = str1;
        CString str_CString(str1);
        //output(str_CString);
        return convert(str_CString, typea);
}

//auto convert(TCHAR* str1, cvttype typea) {
//	str_CString = str1;
//	return convert(str_CString, typea);
//}

//auto convert(CString str1, cvttype typea) {
//	str_CString = str1;
//	return typea;
//}


auto convert(CStringA str1, cvttype typea) {
        //printf(" === locate at CStringA\n");
        str_CString = str1;
        return convert(str_CString, typea);
}

auto convert(string str1, cvttype typea) {
        //printf(" === locate at string\n");
        str_CString = str1.c_str();
        return convert(str_CString, typea);
}

auto convert(wstring str1, cvttype typea) {
        //printf(" === locate at wstring\n");
        str_CString = str1.c_str();
        return convert(str_CString, typea);
}

//auto convert(LPSTR str1, cvttype typea) {
//	str_CString = str1;
//	return convert(str_CString, typea);
//}

auto convert(LPCSTR str1, cvttype typea) {
        //printf(" === locate at LPCSTR\n");
        str_CString = str1;
        return convert(str_CString, typea);
}

#endif // GENERALSTRING_H
