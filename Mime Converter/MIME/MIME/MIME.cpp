#include <stdio.h>
#include <locale.h>
#include <Windows.h>

typedef void (*CFTOMIME)(const wchar_t*, int);
typedef void (*CMIMETOF)(const wchar_t*, int);

int main()
{
	setlocale(LC_ALL, "Russian");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	wchar_t dllPath[MAX_PATH];

	wprintf(L"              Примечание: пути к файлам вводятся без кавычек.\n");
	wprintf(L"                  Пример: C:\\my files\\data.bin\n");
	wprintf(L"------------------------\n");
	wprintf(L"Введите путь к файлу dll: ");
	fgetws(dllPath, MAX_PATH, stdin);

	size_t len = wcslen(dllPath);

	if (len > 0 && dllPath[len - 1] == L'\n')
		dllPath[len - 1] = L'\0';

	HINSTANCE hDLL = LoadLibrary(dllPath);

	if (NULL != hDLL)
	{
		CFTOMIME pConvertFileToMime = (CFTOMIME)GetProcAddress(hDLL, "convertFileToMime");
		CMIMETOF pConvertMimeToFile = (CMIMETOF)GetProcAddress(hDLL, "convertMimeToFile");

		if (NULL != pConvertFileToMime && NULL != pConvertMimeToFile)
		{
			wprintf(L"       Доступные команды: FileToMime -qp путь_к_файлу\n");
			wprintf(L"                          FileToMime -b64 путь_к_файлу\n");
			wprintf(L"                          MimeToFile -qp путь_к_файлу\n");
			wprintf(L"                          MimeToFile -b64 путь_к_файлу\n");
			wprintf(L"                          exit\n");

			wchar_t command[1024];

			while (1)
			{
				wprintf(L"\n> ");
				fgetws(command, 1024, stdin);

				len = wcslen(command);
				if (len > 0 && command[len - 1] == L'\n')
					command[len - 1] = L'\0';

				if (wcscmp(command, L"exit") == 0)
					break;
				else if (wcsstr(command, L"FileToMime -qp "))
					pConvertFileToMime(command + 15, 0);
				else if (wcsstr(command, L"FileToMime -b64 "))
					pConvertFileToMime(command + 16, 1);
				else if (wcsstr(command, L"MimeToFile -qp "))
					pConvertMimeToFile(command + 15, 0);
				else if (wcsstr(command, L"MimeToFile -b64 "))
					pConvertMimeToFile(command + 16, 1);
				else
					wprintf(L"Неизвестная команда\n");
			}
		}

		FreeLibrary(hDLL);
	}

	return 0;
}