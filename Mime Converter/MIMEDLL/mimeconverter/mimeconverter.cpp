#include "pch.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>
#include "mimeconverter.h"

void convertFileToMime(const wchar_t* wFileName, int em)
{
	getMimeContentTypeW(wFileName, em);
	getMimeContentTransferEncodingW(wFileName, em);
}
void convertMimeToFile(const wchar_t* wFileNameMIME, int em)
{
	HANDLE hFileNameMIME = CreateFile(wFileNameMIME, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);

	if (hFileNameMIME != INVALID_HANDLE_VALUE)
	{
		const unsigned sizeBuffer = 8268;
		char* bufferA = (char*)malloc(sizeBuffer);
		char* bufferB = (char*)malloc(sizeBuffer);

		if (bufferA && bufferB)
		{
			char** buffer[2] = { &bufferA, &bufferB };
			OVERLAPPED o = { 0 };

			// Предварительная обработка файла
			// ------------------------------------------------------------------------------------

			if (!ReadFile(hFileNameMIME, *buffer[0], 250, NULL, &o) && GetLastError() != ERROR_IO_PENDING)
			{
				printf("[%u] Ошибка чтения файла\n", GetLastError());
			}

			WaitForSingleObject(hFileNameMIME, INFINITE);

			wchar_t wFileName[MAX_PATH] = { L'\0' };
			char* fileName = strrchr(*buffer[0], '\"');

			if (fileName)
			{
				o.Offset = fileName - (*buffer[0]) + 5; // сместить указатель в файле на начало данных
				(*buffer[0])[fileName - (*buffer[0])] = '\0';
				fileName = strrchr(*buffer[0], '\"');

				if (fileName)
				{
					const wchar_t* wFilePath = wcsrchr(wFileNameMIME, L'\\');

					if (wFilePath)
					{
						wcsncpy_s(wFileName, MAX_PATH, wFileNameMIME, (wFilePath - wFileNameMIME + 1)); // "wFileName" + путь файла
					}

					const unsigned SIZE_FILENAMEW = MultiByteToWideChar(CP_ACP, 0, fileName + 1, -1, NULL, 0);
					wchar_t* fileNameW = (wchar_t*)malloc(SIZE_FILENAMEW * sizeof(wchar_t));

					if (fileNameW)
					{
						MultiByteToWideChar(CP_ACP, 0, fileName + 1, -1, fileNameW, SIZE_FILENAMEW);
						wcsncat_s(wFileName, MAX_PATH, fileNameW, SIZE_FILENAMEW); // "wFileName" + имя файла

						free(fileNameW);
					}
				}
			}

			// Декодирование [ Quoted-printable | Base64 ]
			// ------------------------------------------------------------------------------------

			HANDLE hFileName = CreateFile(wFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

			if (hFileName != INVALID_HANDLE_VALUE)
			{
				char* tmpBuffer = NULL;
				char* decodeBuffer = NULL;
				size_t decodeSize;
				DWORD bytesWritten;
				DWORD bytesRead;

				if (!ReadFile(hFileNameMIME, *buffer[0], sizeBuffer, NULL, &o) && GetLastError() != ERROR_IO_PENDING)
				{
					printf("[%u] Ошибка чтения файла\n", GetLastError());
				}

				WaitForSingleObject(hFileNameMIME, INFINITE);
				swapBuffers(buffer[0], buffer[1]);
				bytesRead = o.InternalHigh;
				o.Offset += bytesRead;

				size_t QPCharLen = 0;
				char QPChar[3] = { '\0' };

				while (bytesRead)
				{
					if (!ReadFile(hFileNameMIME, *buffer[0], sizeBuffer, NULL, &o) && GetLastError() != ERROR_IO_PENDING)
					{
						printf("[%u] Ошибка чтения файла\n", GetLastError());
					}

					tmpBuffer = *buffer[1];

					switch (em)
					{
					case 0:
					{
						char* iBuffer = *buffer[1];
						int QPLineLen = 0;

						while ((int)bytesRead > (iBuffer - *buffer[1]))
						{
							if (QPCharLen)
							{
								QPChar[QPCharLen++] = *iBuffer++;

								if (QPCharLen == 3)
								{
									if (QPChar[1] != '\r' && QPChar[2] != '\n')
									{
										quotedPrintDecode(QPChar, QPCharLen, &decodeBuffer, &decodeSize);

										if (decodeBuffer)
										{
											if (!WriteFile(hFileName, decodeBuffer, decodeSize, &bytesWritten, NULL))
											{
												printf("[%u] Ошибка записи в файл\n", GetLastError());
											}

											free(decodeBuffer);
										}
									}

									tmpBuffer += iBuffer - *buffer[1];
									QPCharLen = 0;
								}
							}
							else if (*iBuffer == '=')
							{
								if (*(iBuffer + 1) == '\r' && *(iBuffer + 2) == '\n')
								{
									if (QPLineLen)
									{
										quotedPrintDecode(tmpBuffer, QPLineLen, &decodeBuffer, &decodeSize);

										if (decodeBuffer)
										{
											if (!WriteFile(hFileName, decodeBuffer, decodeSize, &bytesWritten, NULL))
											{
												printf("[%u] Ошибка записи в файл\n", GetLastError());
											}

											free(decodeBuffer);
										}

										tmpBuffer += QPLineLen;
										QPLineLen = 0;
									}

									tmpBuffer += 3;
									iBuffer += 3;
								}
								else if (('0' <= *(iBuffer + 1) && *(iBuffer + 1) <= '9' || 'A' <= *(iBuffer + 1) && *(iBuffer + 1) <= 'F')
									&& ('0' <= *(iBuffer + 2) && *(iBuffer + 2) <= '9' || 'A' <= *(iBuffer + 2) && *(iBuffer + 2) <= 'F'))
								{
									QPLineLen += 3;
									iBuffer += 3;
								}
								else
								{
									QPChar[QPCharLen++] = *iBuffer++;
								}
							}
							else
							{
								QPLineLen++;
								iBuffer++;
							}
						}

						if (QPLineLen)
						{
							quotedPrintDecode(tmpBuffer, QPLineLen, &decodeBuffer, &decodeSize);

							if (decodeBuffer)
							{
								if (!WriteFile(hFileName, decodeBuffer, decodeSize, &bytesWritten, NULL))
								{
									printf("[%u] Ошибка записи в файл\n", GetLastError());
								}

								free(decodeBuffer);
							}
						}
					}
					break;
					case 1:
						while ((long)bytesRead > 0)
						{
							bytesRead < 78
								? base64Decode(tmpBuffer, bytesRead - 2, &decodeBuffer, &decodeSize)
								: base64Decode(tmpBuffer, 76, &decodeBuffer, &decodeSize);

							if (decodeBuffer)
							{
								if (!WriteFile(hFileName, decodeBuffer, decodeSize, &bytesWritten, NULL))
								{
									printf("[%u] Ошибка записи в файл\n", GetLastError());
								}

								free(decodeBuffer);
							}

							bytesRead -= 78;
							tmpBuffer += 78;
						}
						break;
					}

					WaitForSingleObject(hFileNameMIME, INFINITE);
					swapBuffers(buffer[0], buffer[1]);
					bytesRead = o.InternalHigh;
					o.Offset += bytesRead;
				}
			}

			CloseHandle(hFileName);

			free(bufferA);
			free(bufferB);
		}

		CloseHandle(hFileNameMIME);
	}
	else
	{
		printf("[%u] Не удалось открыть файл\n", GetLastError());
	}
}

void getMimeContentTypeW(const wchar_t* wFileName, int em)
{
	HANDLE hFileName = CreateFile(wFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFileName != INVALID_HANDLE_VALUE)
	{
		// Определить MIME тип по расширению файла
		// ------------------------------------------------------------------------------------

		char mimetype[100] = { '\0' };
		char* extension = getFileExtensionW(wFileName);

		if (extension)
		{
			for (size_t i = 0; i < SIZE_MIMETL; i++)
			{
				if (!strcmp(mimeTypesListing[i].extension, extension))
				{
					strcpy_s(mimetype, sizeof(mimetype), mimeTypesListing[i].mimetype);
					break;
				}
			}

			free(extension);
		}

		// Запись в файл поля Content-Type
		// ------------------------------------------------------------------------------------

		wchar_t wFileNameMIME[MAX_PATH];
		const wchar_t* wExtension = wcsrchr(wFileName, L'.');

		wExtension // "wFileNameMIME" + имя файла
			? wcsncpy_s(wFileNameMIME, MAX_PATH, wFileName, (wExtension - wFileName))
			: wcscpy_s(wFileNameMIME, MAX_PATH, wFileName);
		switch (em) // "wFileNameMIME" + расширение файла
		{
		case 0:
			wcscat_s(wFileNameMIME, MAX_PATH, L".qp");
			break;
		case 1:
			wcscat_s(wFileNameMIME, MAX_PATH, L".b64");
			break;
		}

		HANDLE hFileNameMIME = CreateFile(wFileNameMIME, GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

		if (hFileNameMIME != INVALID_HANDLE_VALUE)
		{
			char contentData[200] = "MIME-Version: 1.0\r\nContent-Type: ";
			DWORD bytesWritten;

			strlen(mimetype) // "contentData" + MIME тип
				? strcat_s(contentData, sizeof(contentData), mimetype)
				: strcat_s(contentData, sizeof(contentData), "application/octet-stream");

			// "contentData" + имя файла
			char* fileName = getFileNameW(wFileName);

			if (fileName)
			{
				strcat_s(contentData, sizeof(contentData), "; name=\"");
				strcat_s(contentData, sizeof(contentData), fileName);
				strcat_s(contentData, sizeof(contentData), "\"");

				free(fileName);
			}

			strcat_s(contentData, sizeof(contentData), "\r\n");
			SetFilePointerEx(hFileNameMIME, { 0 }, NULL, FILE_END); // установить указатель в конец файла

			if (!WriteFile(hFileNameMIME, contentData, strlen(contentData), &bytesWritten, NULL))
			{
				printf("[%u] Ошибка записи в файл\n", GetLastError());
			}

			CloseHandle(hFileNameMIME);
		}
		else
		{
			printf("[%u] Не удалось открыть файл\n", GetLastError());
		}

		CloseHandle(hFileName);
	}
	else
	{
		printf("[%u] Не удалось открыть файл\n", GetLastError());
	}
}
void getMimeContentTransferEncodingW(const wchar_t* wFileName, int em)
{
	HANDLE hFileName = CreateFile(wFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);

	if (hFileName != INVALID_HANDLE_VALUE)
	{
		// Запись в файл поля Content-Transfer-Encoding
		// ------------------------------------------------------------------------------------

		wchar_t wFileNameMIME[MAX_PATH];
		const wchar_t* wExtension = wcsrchr(wFileName, L'.');

		wExtension // "wFileNameMIME" + имя файла
			? wcsncpy_s(wFileNameMIME, MAX_PATH, wFileName, (wExtension - wFileName))
			: wcscpy_s(wFileNameMIME, MAX_PATH, wFileName);
		switch (em) // "wFileNameMIME" + расширение файла
		{
		case 0:
			wcscat_s(wFileNameMIME, MAX_PATH, L".qp");
			break;
		case 1:
			wcscat_s(wFileNameMIME, MAX_PATH, L".b64");
			break;
		}

		HANDLE hFileNameMIME = CreateFile(wFileNameMIME, GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

		if (hFileNameMIME != INVALID_HANDLE_VALUE)
		{
			char contentData[200] = "Content-Transfer-Encoding: ";
			char* fileName = getFileNameW(wFileName);
			DWORD bytesWritten;

			if (fileName)
			{
				switch (em)
				{
				case 0:
					strcat_s(contentData, sizeof(contentData), "quoted-printable\r\n");
					break;
				case 1:
					strcat_s(contentData, sizeof(contentData), "base64\r\n");
					break;
				}
				strcat_s(contentData, sizeof(contentData), "Content-Disposition: attachment; filename=\"");
				strcat_s(contentData, sizeof(contentData), fileName);
				strcat_s(contentData, sizeof(contentData), "\"");

				free(fileName);
			}

			strcat_s(contentData, sizeof(contentData), "\r\n\r\n");
			SetFilePointerEx(hFileNameMIME, { 0 }, NULL, FILE_END); // установить указатель в конец файла

			if (!WriteFile(hFileNameMIME, contentData, strlen(contentData), &bytesWritten, NULL))
			{
				printf("[%u] Ошибка записи в файл\n", GetLastError());
			}

			// Кодирование [ Quoted-printable | Base64 ]
			// ------------------------------------------------------------------------------------

			unsigned int sizeBuffer = 0;
			switch (em)
			{
			case 0:
				sizeBuffer = 8250;
				break;
			case 1:
				sizeBuffer = 8208;
				break;
			}
			char* bufferA = (char*)malloc(sizeBuffer);
			char* bufferB = (char*)malloc(sizeBuffer);

			if (bufferA && bufferB)
			{
				char** buffer[2] = { &bufferA, &bufferB };
				OVERLAPPED o = { 0 };
				DWORD bytesRead;

				if (!ReadFile(hFileName, *buffer[0], sizeBuffer, NULL, &o) && GetLastError() != ERROR_IO_PENDING)
				{
					printf("[%u] Ошибка чтения файла\n", GetLastError());
				}

				WaitForSingleObject(hFileName, INFINITE); // ожидание завершения асинхронной операции чтения
				swapBuffers(buffer[0], buffer[1]); // поменять адреса буферов местами
				bytesRead = o.InternalHigh;
				o.Offset += bytesRead; // сместить указатель в файле на количество прочитанных байт

				char* encodeBuffer = NULL;
				int encodedLineLenMax;
				size_t encodeSize;

				switch (em)
				{
				case 0:
					encodedLineLenMax = 0;
					break;
				case 1:
					encodedLineLenMax = 76;
					break;
				}

				while (bytesRead)
				{
					if (!ReadFile(hFileName, *buffer[0], sizeBuffer, NULL, &o) && GetLastError() != ERROR_IO_PENDING)
					{
						printf("[%u] Ошибка чтения файла\n", GetLastError());
					}

					switch (em)
					{
					case 0:
						quotedPrintEncode(*buffer[1], bytesRead, &encodeBuffer, &encodeSize);
						if (encodeBuffer)
						{
							char* eBuffer = encodeBuffer;
							int tmpELLM = 0;
							int charLen = 0;

							for (; (int)encodeSize > 0; encodeSize -= charLen)
							{
								if ((61 != *eBuffer) && ((32 < *eBuffer && *eBuffer < 127) || (9 == *eBuffer) || (32 == *eBuffer) || (13 == *eBuffer) || (0 == *eBuffer)))
								{
									charLen = 1;
								}
								else if (10 == *eBuffer)
								{
									if (!WriteFile(hFileNameMIME, eBuffer - tmpELLM, tmpELLM, &bytesWritten, NULL))
									{
										printf("[%u] Ошибка записи в файл\n", GetLastError());
									}

									encodedLineLenMax = 0;
									tmpELLM = 0;
									charLen = 1;
								}
								else
								{
									charLen = 3;
								}

								encodedLineLenMax += charLen;
								tmpELLM += charLen;

								if (encodedLineLenMax >= 76)
								{
									if (!WriteFile(hFileNameMIME, eBuffer - (tmpELLM - charLen), tmpELLM - charLen, &bytesWritten, NULL))
									{
										printf("[%u] Ошибка записи в файл\n", GetLastError());
									}
									if (!WriteFile(hFileNameMIME, "=", 1, &bytesWritten, NULL))
									{
										printf("[%u] Ошибка записи в файл\n", GetLastError());
									}
									if (!WriteFile(hFileNameMIME, "\r", 1, &bytesWritten, NULL))
									{
										printf("[%u] Ошибка записи в файл\n", GetLastError());
									}
									if (!WriteFile(hFileNameMIME, "\n", 1, &bytesWritten, NULL))
									{
										printf("[%u] Ошибка записи в файл\n", GetLastError());
									}

									encodedLineLenMax = charLen;
									tmpELLM = charLen;
								}

								eBuffer += charLen;
							}

							if (!WriteFile(hFileNameMIME, eBuffer - tmpELLM, tmpELLM, &bytesWritten, NULL))
							{
								printf("[%u] Ошибка записи в файл\n", GetLastError());
							}

							free(encodeBuffer);
						}
						break;
					case 1:
						base64Encode(*buffer[1], bytesRead, &encodeBuffer, &encodeSize);
						if (encodeBuffer)
						{
							char* eBuffer = encodeBuffer;

							while ((int)encodeSize > 0)
							{
								if ((int)encodeSize < encodedLineLenMax)
								{
									if (!WriteFile(hFileNameMIME, eBuffer, encodeSize, &bytesWritten, NULL))
									{
										printf("[%u] Ошибка записи в файл\n", GetLastError());
									}
								}
								else
								{
									if (!WriteFile(hFileNameMIME, eBuffer, encodedLineLenMax, &bytesWritten, NULL))
									{
										printf("[%u] Ошибка записи в файл\n", GetLastError());
									}
								}

								if (!WriteFile(hFileNameMIME, "\r", 1, &bytesWritten, NULL))
								{
									printf("[%u] Ошибка записи в файл\n", GetLastError());
								}
								if (!WriteFile(hFileNameMIME, "\n", 1, &bytesWritten, NULL))
								{
									printf("[%u] Ошибка записи в файл\n", GetLastError());
								}

								encodeSize -= encodedLineLenMax;
								eBuffer += encodedLineLenMax;
							}

							free(encodeBuffer);
						}
						break;
					}

					WaitForSingleObject(hFileName, INFINITE);
					swapBuffers(buffer[0], buffer[1]);
					bytesRead = o.InternalHigh;
					o.Offset += bytesRead;
				}

				if (em == 1 && !WriteFile(hFileNameMIME, "\r", 1, &bytesWritten, NULL))
				{
					printf("[%u] Ошибка записи в файл\n", GetLastError());
				}
				if (em == 1 && !WriteFile(hFileNameMIME, "\n", 1, &bytesWritten, NULL))
				{
					printf("[%u] Ошибка записи в файл\n", GetLastError());
				}

				free(bufferA);
				free(bufferB);
			}

			CloseHandle(hFileNameMIME);
		}
		else
		{
			printf("[%u] Не удалось открыть файл\n", GetLastError());
		}

		CloseHandle(hFileName);
	}
	else
	{
		printf("[%u] Не удалось открыть файл\n", GetLastError());
	}
}

void quotedPrintEncode(const char* dataIn, size_t sizeIn, char** dataOut, size_t* sizeOut)
{
	if (dataIn && sizeIn)
	{
		*dataOut = (char*)malloc(sizeIn * 3);

		if (*dataOut)
		{
			char table[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
			char* QPDataOut = *dataOut;
			int tmp;

			for (size_t i = 0; i < sizeIn; i++)
			{
				tmp = (int)dataIn[i];

				if ((61 != tmp) && ((32 < tmp && tmp < 127) || (9 == tmp) || (32 == tmp) || (13 == tmp) || (10 == tmp) || (0 == tmp)))
				{
					*QPDataOut++ = dataIn[i];
				}
				else
				{
					size_t i;

					*QPDataOut++ = '=';
					for (i = 0; (((tmp >> 0x4) & 0xF) ^ i) && (i < 16); i++);
					*QPDataOut++ = table[i];
					for (i = 0; ((tmp & 0xF) ^ i) && (i < 16); i++);
					*QPDataOut++ = table[i];
				}
			}

			*sizeOut = QPDataOut - *dataOut;
		}
	}
}
void quotedPrintDecode(const char* dataIn, size_t sizeIn, char** dataOut, size_t* sizeOut)
{
	if (dataIn && sizeIn)
	{
		*dataOut = (char*)malloc(sizeIn);

		if (*dataOut)
		{
			char table[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
			char* QPDataOut = *dataOut;
			int tmp;
			int i;

			for (size_t index = 0; index < sizeIn;)
			{
				if (dataIn[index] == '=')
				{
					tmp = 0;
					for (i = 0; (dataIn[index + 1] != table[i]) && (i < 16); i++);
					tmp |= i << 4;
					for (i = 0; (dataIn[index + 2] != table[i]) && (i < 16); i++);
					tmp |= i;

					*QPDataOut++ = (char)tmp;
					index += 3;
				}
				else
				{
					*QPDataOut++ = dataIn[index];
					index++;
				}
			}

			*sizeOut = QPDataOut - *dataOut;
		}
	}
}

void base64Encode(const char* dataIn, size_t sizeIn, char** dataOut, size_t* sizeOut)
{
	if (dataIn && sizeIn)
	{
		*dataOut = (char*)malloc(sizeIn / 3 * 4 + 4);

		if (*dataOut)
		{
			char alphabetBase[64] = {
				'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
				'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
				'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'
			};
			char* base64DataOut = *dataOut;
			size_t index = 0;
			DWORD tmp = 0;

			for (; index < sizeIn / 3 * 3; index += 3)
			{
				tmp |= ((DWORD)dataIn[index] & 0xFF) << 0x18;
				tmp |= ((DWORD)dataIn[index + 1] & 0xFF) << 0x10;
				tmp |= ((DWORD)dataIn[index + 2] & 0xFF) << 0x08;

				*base64DataOut++ = alphabetBase[(tmp >> 0x1A) & 0x3F]; tmp <<= 0x06;
				*base64DataOut++ = alphabetBase[(tmp >> 0x1A) & 0x3F]; tmp <<= 0x06;
				*base64DataOut++ = alphabetBase[(tmp >> 0x1A) & 0x3F]; tmp <<= 0x06;
				*base64DataOut++ = alphabetBase[(tmp >> 0x1A) & 0x3F]; tmp <<= 0x06;
			}

			switch (sizeIn % 3)
			{
			case 1:
				tmp |= ((DWORD)dataIn[index] & 0xFF) << 0x18;

				*base64DataOut++ = alphabetBase[(tmp >> 0x1A) & 0x3F]; tmp <<= 0x06;
				*base64DataOut++ = alphabetBase[(tmp >> 0x1A) & 0x3F]; tmp <<= 0x06;
				*base64DataOut++ = '=';
				*base64DataOut++ = '=';
				break;
			case 2:
				tmp |= ((DWORD)dataIn[index] & 0xFF) << 0x18;
				tmp |= ((DWORD)dataIn[index + 1] & 0xFF) << 0x10;

				*base64DataOut++ = alphabetBase[(tmp >> 0x1A) & 0x3F]; tmp <<= 0x06;
				*base64DataOut++ = alphabetBase[(tmp >> 0x1A) & 0x3F]; tmp <<= 0x06;
				*base64DataOut++ = alphabetBase[(tmp >> 0x1A) & 0x3F]; tmp <<= 0x06;
				*base64DataOut++ = '=';
				break;
			}

			*sizeOut = base64DataOut - *dataOut;
		}
	}
}
void base64Decode(const char* dataIn, size_t sizeIn, char** dataOut, size_t* sizeOut)
{
	if (dataIn && sizeIn)
	{
		*dataOut = (char*)malloc(sizeIn / 4 * 3);

		if (*dataOut)
		{
			char* base64DataOut = *dataOut;
			size_t index = 0;
			DWORD tmp = 0;

			for (; index < sizeIn / 4 * 4; index += 4)
			{
				tmp |= ((DWORD)getIndexOfElementInAlphabetBase64(dataIn[index]) & 0x3F) << 0x1A;
				tmp |= ((DWORD)getIndexOfElementInAlphabetBase64(dataIn[index + 1]) & 0x3F) << 0x14;
				if (dataIn[index + 2] != '=')
					tmp |= ((DWORD)getIndexOfElementInAlphabetBase64(dataIn[index + 2]) & 0x3F) << 0x0E;
				if (dataIn[index + 3] != '=')
					tmp |= ((DWORD)getIndexOfElementInAlphabetBase64(dataIn[index + 3]) & 0x3F) << 0x08;

				*base64DataOut++ = (tmp >> 0x18) & 0xFF; tmp <<= 0x08;
				if (dataIn[index + 2] != '=')
					*base64DataOut++ = (tmp >> 0x18) & 0xFF; tmp <<= 0x08;
				if (dataIn[index + 3] != '=')
					*base64DataOut++ = (tmp >> 0x18) & 0xFF; tmp <<= 0x08;
			}

			*sizeOut = base64DataOut - *dataOut;
		}
	}
}

size_t getIndexOfElementInAlphabetBase64(const char element)
{
	size_t index = -1;

	char alphabetBase[64] = {
		'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
		'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
		'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'
	};

	for (size_t i = 0; i < 64; i++)
	{
		if (alphabetBase[i] == element)
		{
			index = i;
			break;
		}
	}

	return index;
}
char* getFileExtensionW(const wchar_t* wFileName)
{
	char* extension = NULL;
	const wchar_t* wExtension = wcsrchr(wFileName, L'.');

	if (wExtension)
	{
		const unsigned SIZE_EXTENSION = WideCharToMultiByte(CP_ACP, 0, wExtension, -1, extension, 0, NULL, NULL);
		extension = (char*)malloc(SIZE_EXTENSION);

		WideCharToMultiByte(CP_ACP, 0, wExtension, -1, extension, SIZE_EXTENSION, NULL, NULL); // перекодирование символов Unicode в однобайтовые
	}

	return extension;
}
char* getFileNameW(const wchar_t* wFileName)
{
	char* fileName = NULL;
	const wchar_t* fileNameW = wcsrchr(wFileName, L'\\');

	if (fileNameW)
	{
		const unsigned SIZE_FILENAME = WideCharToMultiByte(CP_ACP, 0, fileNameW + 1, -1, fileName, 0, NULL, NULL);
		fileName = (char*)malloc(SIZE_FILENAME);

		WideCharToMultiByte(CP_ACP, 0, fileNameW + 1, -1, fileName, SIZE_FILENAME, NULL, NULL); // перекодирование символов Unicode в однобайтовые
	}
	else
	{
		const unsigned SIZE_FILENAME = WideCharToMultiByte(CP_ACP, 0, wFileName, -1, fileName, 0, NULL, NULL);
		fileName = (char*)malloc(SIZE_FILENAME);

		WideCharToMultiByte(CP_ACP, 0, wFileName, -1, fileName, SIZE_FILENAME, NULL, NULL); // перекодирование символов Unicode в однобайтовые
	}

	return fileName;
}
void swapBuffers(char** bufferA, char** bufferB)
{
	char* tmp = *bufferA;
	*bufferA = *bufferB;
	*bufferB = tmp;
}