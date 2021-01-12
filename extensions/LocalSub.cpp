#include "LocalSub.h"

#include <string>
#include "dialog.h"
#include "GeneralString.h"

//#include <Windows.h>
//#include <SDKDDKVer.h>
#include <atlstr.h>
#include <codecvt>
//#include <cstdint>
//#include <cstdlib>
#include <fstream>
//#include <functional>
//#include <hash_map>
#include <iostream>
//#include <locale>
//#include <malloc.h>
#include <map>
//#include <memory>
//#include <mutex>
//#include <stdio.h>
//#include <stdlib.h>
#include <string>
//#include <tchar.h>
//#include <vector>


Dialog dialog;

using namespace std;
std::hash<std::wstring> hash_encode;
map <size_t, LPCWSTR> meatdic;


static void make_console() {
    AllocConsole();
    freopen("CONOUT$", "w", stdout);
    freopen("CONIN$", "r", stdin);
    std::cout << "debug" << std::endl;
}
bool loadText()
{
    meatdic.clear();
    const std::locale empty_locale = std::locale::empty();
    typedef std::codecvt_utf8<wchar_t> converter_type;  //std::codecvt_utf16
    const converter_type* converter = new converter_type;
    const std::locale utf8_locale = std::locale(empty_locale, converter);

    if (!QFile::exists(dialog.jpfile)||!QFile::exists(dialog.zhfile)){
        return false;
    }
    std::wifstream fin1(dialog.jpfile.toLocal8Bit());  //input
    fin1.imbue(utf8_locale);
    std::wifstream fin2(dialog.zhfile.toLocal8Bit());  //input
    fin2.imbue(utf8_locale);
    std::wstring line1;
    std::wstring line2;

    int idx = 0;
    while (!fin1.eof() && !fin2.eof())
    {
        std::getline(fin1, line1);

        line1 = line1.append(L"\n");

        //CString str1(line1.c_str());
        std::getline(fin2, line2);
        CString str2(line2.c_str());

        LPCWSTR lpcwStr = str2.AllocSysString();
        meatdic.insert(pair<size_t, LPCWSTR>(hash_encode(line1), lpcwStr));
        //printf("%s\n", (LPCTSTR)str);
        idx++;
    }
    fin1.close();
    fin2.close();

    cout << "Load Texts:0x" << hex << idx << endl;
    return true;
}

BOOL WINAPI DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
        //MessageBoxW(NULL, L"Extension Loaded", L"Example", MB_OK);
        dialog.show();
        make_console();
        loadText();
		break;
	case DLL_PROCESS_DETACH:
        //MessageBoxW(NULL, L"Extension Removed", L"Example", MB_OK);
		break;
	}
	return TRUE;
}

//#define COPY_CLIPBOARD
//#define EXTRA_NEWLINES

/*
	Param sentence: sentence received by Textractor (UTF-16). Can be modified, Textractor will receive this modification only if true is returned.
	Param sentenceInfo: contains miscellaneous info about the sentence (see README).
	Return value: whether the sentence was modified.
	Textractor will display the sentence after all extensions have had a chance to process and/or modify it.
	The sentence will be destroyed if it is empty or if you call Skip().
	This function may be run concurrently with itself: please make sure it's thread safe.
	It will not be run concurrently with DllMain.
*/
bool ProcessSentence(std::wstring& sentence, SentenceInfo sentenceInfo)
{
    // Your code here...
    if (sentenceInfo["current select"] && sentenceInfo["process id"] != 0){
        LPCWSTR resl = meatdic[hash_encode(sentence)];
        if (resl != NULL) {
            output(resl);
            output("\n");
            //dialog.wigglyWidget->setWText(resl);
            //dialog.wigglyWidget->ReSize();
            dialog.inputUpdate(resl);
        }
        else{
            output(sentence);
            //output("\n");
            //dialog.wigglyWidget->setWText(sentence);
            //dialog.wigglyWidget->ReSize();
            dialog.inputUpdate(sentence);
        }

    }

    return true;
#ifdef COPY_CLIPBOARD
	// This example extension automatically copies sentences from the hook currently selected by the user into the clipboard.
	if (sentenceInfo["current select"])
	{
		HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, (sentence.size() + 2) * sizeof(wchar_t));
		memcpy(GlobalLock(hMem), sentence.c_str(), (sentence.size() + 2) * sizeof(wchar_t));
		GlobalUnlock(hMem);
		OpenClipboard(0);
		EmptyClipboard();
		SetClipboardData(CF_UNICODETEXT, hMem);
		CloseClipboard();
	}
	return false;
#endif // COPY_CLIPBOARD

#ifdef EXTRA_NEWLINES
	// This example extension adds extra newlines to all sentences.
	sentence += L"\r\n";
	return true;
#endif // EXTRA_NEWLINES
}
