#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include "funcs.h"
#include <Windows.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>

int main() {

	loading();
	system("cls");	
	menu();

	short cursor_y = 6,
		Best_score = 0,
		color_snake_head = 160,
		color_snake_body = 170;

	while (true)
	{			
		//menu cursor
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 17, cursor_y });//6
		printf("  <\\\n");
		printf("\t\t ");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 255);
		printf("xxx");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
		printf(" >\n");
		printf("\t\t   </");

		char vod = _getch();
		
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 17, cursor_y });
		printf("     \n");
		printf("\t\t      \n");
		printf("\t\t     ");

		if (vod == 72 && cursor_y != 6) //72
		{
			cursor_y -= 5;
		}
		else if (vod == 80 && cursor_y != 16) //80
		{
			cursor_y += 5;
		}

		//New Game
		if (vod == '\r' && cursor_y == 6)
		{
			system("cls");
			srand((unsigned)time(NULL));

			const short N = 24, M = 60;
			double carta[N][M] = {};

			short Score = 0;

			//координаты головы + фрукт
			short x = 8 + rand() % 9,
				y = 19 + rand() % 21,
				x_frut = 0,
				y_frut = 0;

			double counret_inc = 1,
					counret_dec = 1;

			bool fruct = true;	

			//хвот
			short hvost = 1 + rand() % 4,
				counter = 2;

			while (counter != 0)
			{
				switch (hvost)
				{
				case 1: {
					carta[x][y - counter] = counret_inc;
					vod = 'd';
				}
						break;
				case 2: {
					carta[x - counter][y] = counret_inc;
					vod = 's';
				}
						break;
				case 3: {
					carta[x][y + counter] = counret_inc;
					vod = 'a';
				}
						break;
				case 4: {
					carta[x + counter][y] = counret_inc;
					vod = 'w';
				}
						break;
				}
				counret_inc += 0.01;
				counter--;
			}

			//боковая панель подсказок
			help_panel();

			while (true)
			{
				carta[x][y] = counret_inc;

				//генерация фрукта
				while (fruct)
				{
					x_frut = 1 + rand() % 21;
					y_frut = 1 + rand() % 58;
					if (carta[x_frut][y_frut] == 0)
					{
						fruct = false;
					}
				}

				for (short i = 0; i < N; i++)
				{
					SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 0, i });
					for (short j = 0; j < M; j++)
					{
						if (i == 0 || i == (N - 1))
						{
							SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 255);
							printf(" ");
						}
						else if (j == 0 || j == (M - 1))
						{
							SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 255);
							printf(" ");
						}
						else if (carta[i][j] != 0 && carta[i][j] == counret_inc)
						{
							SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color_snake_head); //160
							printf(":");
						}
						else if (carta[i][j] != 0)
						{
							SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color_snake_body); //170
							printf(" ");
						}
						else if (i == x_frut && j == y_frut)
						{
							SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 204);
							printf(" ");
						}
						else
						{
							printf(" ");
						}
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
					}
					if (i == 2)
					{
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
						printf(" Score: %d", Score);
					}
					else if (i == 4 && Best_score != 0)
					{
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
						printf(" Best score: %d", Best_score);
					}
				}

				if (_kbhit() != 0)
				{
					char keyboard = vod;
					vod = _getch();
					if (vod == '\0' || (vod != 'W' && vod != 'w' && vod != -106 && vod != -26 && vod != 'S' && vod != 's' && vod != -101 && vod != -21 && vod != 'A' && vod != 'a' && vod != -108 && vod != -28 && vod != 'D' && vod != 'd' && vod != -126 && vod != -94 && vod != 27) || (carta[x][y] == (counret_inc - 0.01)))
					{
						vod = keyboard;
					}
				}

				if (vod == 'W' || vod == 'w' || vod == -106 || vod == -26)
				{
					x--;
				}
				else if (vod == 'S' || vod == 's' || vod == -101 || vod == -21)
				{
					x++;
				}
				else if (vod == 'A' || vod == 'a' || vod == -108 || vod == -28)
				{
					y--;
				}
				else if (vod == 'D' || vod == 'd' || vod == -126 || vod == -94)
				{
					y++;
				}
				else if (vod == 27)
				{
					goto EXIT_IN_MENU;
				}				

				//выход за пределы карты
				if (x == 0)
				{
					x = (N - 2);
				}
				else if (x == (N - 1))
				{
					x = 1;
				}
				else if (y == 0)
				{
					y = (M - 2);
				}
				else if (y == (M - 1))
				{
					y = 1;
				}


				// солкновение головы с телом
				if (carta[x][y] != 0)
				{
					system("cls");
					lose();
					break;
				}
				//удаление хвоста + условия выйгрыша
				else if (x == x_frut && y == y_frut)
				{
					Score += 10;
					if (Score == (((N - 2)*(M - 2) * 10) - 30))
					{
						system("cls");
						win();
						break;
					}
					fruct = true;
				}
				else
				{					
					for (short i = 0; i < N; i++)
					{
						for (short j = 0; j < M; j++)
						{
							if (carta[i][j] == counret_dec)
							{
								carta[i][j] = 0;
								counret_dec += 0.01;
								i = (N - 1);
								goto EXIT_FOR;
							}
						}						
					}
				}
EXIT_FOR:
				counret_inc += 0.01;
			}

			/*if (Best_score < Score)
			{
				Best_score = Score;
			}*/
			__asm {
				mov ax, Best_score //ax = Best_score
				cmp ax, Score // ax > Score
				ja label //true

				mov ax, Score
				mov Best_score, ax

				label:				
			}
			printf("\n\n\n\n\n\n\n\n\n\n\n");
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
			system("pause");

			for (short i = 24; i < 60; i += 2)
			{
				printf("\n\n");
				Sleep(10);
			}
EXIT_IN_MENU:
			system("cls");
			menu();
		}
		//Color
		else if (vod == '\r' && cursor_y == 11)
		{
			system("cls");
			color_menu();
			short cursor_x = 17;
			cursor_y = 6;

			while (true)
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
				SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { cursor_x, cursor_y });//6
				printf("  <\\");
				SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { cursor_x, ++cursor_y });//6
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 255);
				printf("xxx");
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
				printf(" >");
				SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { cursor_x, ++cursor_y });//6
				printf("  </");

				vod = _getch();

				SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { cursor_x, cursor_y-- });//6
				printf("    ");
				SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { cursor_x, cursor_y-- });//6
				printf("   ");
				printf("   ");
				SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { cursor_x, cursor_y });//6
				printf("    ");

				if (vod == 72 && cursor_y != 6) //up
				{
					cursor_y -= 5;
				}
				else if (vod == 80 && cursor_y != 16) //down
				{
					cursor_y += 5;
				}
				else if (vod == 75 && cursor_x != 17) //left
				{
					cursor_x -= 17;
				}
				else if (vod == 77 && cursor_x != 34) //right
				{
					cursor_x += 17;
				}

				//выбор цвета
				if (vod == 13 && cursor_x == 17)
				{
					switch (cursor_y)
					{
					case 6: {
						color_snake_head = 160;
						color_snake_body = 170;
					}
						break;
					case 11: {
						color_snake_head = 224;
						color_snake_body = 238;
					}
							break;
					case 16: {
						color_snake_head = 112;
						color_snake_body = 119;
					}
							break;
					}
					break;
				}
				else if (vod == 13 && cursor_x == 34)
				{
					switch (cursor_y)
					{
					case 6: {
						color_snake_head = 176;
						color_snake_body = 187;
					}
							break;
					case 11: {
						color_snake_head = 208;
						color_snake_body = 221;
					}
							 break;
					case 16: {
						color_snake_head = 144;
						color_snake_body = 153;
					}
							 break;
					}
					break;
				}
			}

			system("cls");
			cursor_y = 6;
			menu();
		}
		//Exit
		else if (vod == '\r' && cursor_y == 16)
		{
			break;
		}
	}

	return 0;
}