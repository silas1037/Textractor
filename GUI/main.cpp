﻿#include "mainwindow.h"
#include "module.h"
#include <winhttp.h>
#include <QApplication>
#include "extenwindow.h"
extern const wchar_t* UPDATE_AVAILABLE;

int main(int argc, char *argv[])
{


	std::thread([]
	{
		if (!*VERSION) return;
		using InternetHandle = AutoHandle<Functor<WinHttpCloseHandle>>;
		// Queries GitHub releases API https://developer.github.com/v3/repos/releases/ and checks the last release tag to check if it's the same
		if (InternetHandle internet = WinHttpOpen(L"Mozilla/5.0 Textractor", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, NULL, NULL, 0))
			if (InternetHandle connection = WinHttpConnect(internet, L"api.github.com", INTERNET_DEFAULT_HTTPS_PORT, 0))
				if (InternetHandle request = WinHttpOpenRequest(connection, L"GET", L"/repos/Artikash/Textractor/releases", NULL, NULL, NULL, WINHTTP_FLAG_SECURE))
					if (WinHttpSendRequest(request, NULL, 0, NULL, 0, 0, NULL))
					{
						char buffer[1000] = {};
						WinHttpReceiveResponse(request, NULL);
						WinHttpReadData(request, buffer, 1000, DUMMY);
						if (abs(strstr(buffer, "/tag/") - strstr(buffer, VERSION)) > 10) TEXTRACTOR_MESSAGE(UPDATE_AVAILABLE);
					}
	}).detach();

	QDir::setCurrent(QFileInfo(S(GetModuleFilename().value())).absolutePath());

	QApplication app(argc, argv);
	app.setFont(QFont("MS Shell Dlg 2", 10));

    if (QFile::exists("normalStyle.css"))
    {
        // Load Qt style sheet
        QFile styleFile("normalStyle.css");
        if(styleFile.open(QIODevice::ReadOnly))
        {
            app.setStyleSheet(styleFile.readAll());
            styleFile.close();
        }

    }
//    return MainWindow().show(), app.exec();



    MainWindow a;
    a.show();
    if (QFile::exists("LocalSub.xdll")||QFile::exists("LocalSub.dll")){Load("LocalSub");}

    return app.exec();
}
