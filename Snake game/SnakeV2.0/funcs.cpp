#include <stdio.h>
#include <Windows.h>

void loading() {

	char loading[4][36] = { { 'x',' ',' ',' ',' ',' ','x','x',' ',' ',' ',' ','x','x',' ',' ',' ','x','x','x',' ',' ',' ','x',' ',' ','x',' ',' ','x',' ',' ',' ','x','x',' ' },
	{ 'x',' ',' ',' ',' ','x',' ',' ','x',' ',' ','x',' ',' ','x',' ',' ','x',' ',' ','x',' ',' ',' ',' ',' ','x','x',' ','x',' ',' ','x',' ',' ',' ' },
	{ 'x',' ',' ',' ',' ','x',' ',' ','x',' ',' ','x','x','x','x',' ',' ','x',' ',' ','x',' ',' ','x',' ',' ','x',' ','x','x',' ',' ','x',' ','x','x' },
	{ 'x','x','x',' ',' ',' ','x','x',' ',' ',' ','x',' ',' ','x',' ',' ','x','x','x',' ',' ',' ','x',' ',' ','x',' ',' ','x',' ',' ',' ','x','x',' ' } };

	for (short i = 0, index = 8; i < 4; i++)
	{
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 22, index++ });
		for (short j = 0; j < 36; j++)
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
			if (loading[i][j] == 'x')
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 255);
			}
			printf("%c", loading[i][j]);
		}
	}

	printf("\n\n\n\n\t      ");

	for (short i = 14; i < 65; i++)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 170);
		printf(" ");
		Sleep(30);
	}

	for (short i = 20; i < 45; i++)
	{
		printf("\n");
		Sleep(10);
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
}

void win() {

	char win[5][45] = { { 'x',' ',' ',' ','x',' ',' ',' ','x','x','x',' ',' ',' ','x',' ',' ',' ','x',' ',' ',' ',' ','o',' ',' ',' ',' ',' ',' ',' ','o',' ',' ','o',' ',' ','o',' ',' ',' ','o',' ',' ','o' },
	{ ' ','x',' ','x',' ',' ',' ','x',' ',' ',' ','x',' ',' ','x',' ',' ',' ','x',' ',' ',' ',' ','o',' ',' ',' ','o',' ',' ',' ','o',' ',' ',' ',' ',' ','o','o',' ',' ','o',' ',' ','o' },
	{ ' ',' ','x',' ',' ',' ',' ','x',' ',' ',' ','x',' ',' ','x',' ',' ',' ','x',' ',' ',' ',' ','o',' ',' ',' ','o',' ',' ',' ','o',' ',' ','o',' ',' ','o',' ','o',' ','o',' ',' ','o' },
	{ ' ',' ','x',' ',' ',' ',' ','x',' ',' ',' ','x',' ',' ','x',' ',' ',' ','x',' ',' ',' ',' ','o',' ',' ',' ','o',' ',' ',' ','o',' ',' ','o',' ',' ','o',' ',' ','o','o',' ',' ',' ' },
	{ ' ',' ','x',' ',' ',' ',' ',' ','x','x','x',' ',' ',' ',' ','x','x','x',' ',' ',' ',' ',' ',' ','o','o','o',' ','o','o','o',' ',' ',' ','o',' ',' ','o',' ',' ',' ','o',' ',' ','o' } };

	for (short i = 0, index = 9; i < 5; i++)
	{
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 18, index++ });
		for (short j = 0; j < 45; j++)
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
			if (win[i][j] == 'x')
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 255);
			}
			else if (win[i][j] == 'o')
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 170);
			}
			printf("%c", win[i][j]);
		}
	}
}

void lose() {

	char lose[5][49] = { { 'x',' ',' ',' ','x',' ',' ',' ','x','x','x',' ',' ',' ','x',' ',' ',' ','x',' ',' ',' ',' ','o',' ',' ',' ',' ',' ',' ','o','o','o',' ',' ',' ',' ','o','o','o',' ',' ','o','o','o','o',' ',' ','o' },
	{ ' ','x',' ','x',' ',' ',' ','x',' ',' ',' ','x',' ',' ','x',' ',' ',' ','x',' ',' ',' ',' ','o',' ',' ',' ',' ',' ','o',' ',' ',' ','o',' ',' ','o',' ',' ',' ',' ',' ','o',' ',' ',' ',' ',' ','o' },
	{ ' ',' ','x',' ',' ',' ',' ','x',' ',' ',' ','x',' ',' ','x',' ',' ',' ','x',' ',' ',' ',' ','o',' ',' ',' ',' ',' ','o',' ',' ',' ','o',' ',' ','o','o','o','o',' ',' ','o','o','o','o',' ',' ','o' },
	{ ' ',' ','x',' ',' ',' ',' ','x',' ',' ',' ','x',' ',' ','x',' ',' ',' ','x',' ',' ',' ',' ','o',' ',' ',' ',' ',' ','o',' ',' ',' ','o',' ',' ',' ',' ',' ','o',' ',' ','o',' ',' ',' ',' ',' ',' ' },
	{ ' ',' ','x',' ',' ',' ',' ',' ','x','x','x',' ',' ',' ',' ','x','x','x',' ',' ',' ',' ',' ','o','o','o','o',' ',' ',' ','o','o','o',' ',' ',' ','o','o','o',' ',' ',' ','o','o','o','o',' ',' ','o' } };

	for (short i = 0, index = 9; i < 5; i++)
	{
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 16, index++ });
		for (short j = 0; j < 49; j++)
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
			if (lose[i][j] == 'x')
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 255);
			}
			else if (lose[i][j] == 'o')
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 204);
			}
			printf("%c", lose[i][j]);
		}
	}
}

void menu() {

	char menu[13][35] = { { 'x',' ','x',' ',' ','x','x',' ',' ','x',' ',' ','x',' ',' ',' ',' ','/','x','x',' ',' ','/','x','x',' ',' ','x',92 ,'/','x',' ',' ','x','x' },
	{ 'x',92 ,'x',' ',' ','x','-',' ',' ','x',' ',' ','x',' ',' ',' ',' ','x',' ','_',' ',' ','x','_','x',' ',' ','x',' ',' ','x',' ',' ','x','-' },
	{ 'x',' ','x',' ',' ','x','x',' ',' ','x','/',92, 'x',' ',' ',' ',' ','x','x','/',' ',' ','x',' ','x',' ',' ','x',' ',' ','x',' ',' ','x','x' },
	{ ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ' },
	{ ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ' },
	{ 'x','x',' ',' ','x','=','=',' ',' ','x',' ',' ',' ','/','/', 92,' ',' ',' ','x','x',92 ,' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ' },
	{ 'x',' ',' ',' ','x',' ','x',' ',' ','x',' ',' ','<','<',' ',' ','>',' ',' ','<',92 ,'x',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ' },
	{ 'x','x',' ',' ','=','=','x',' ',' ','x','x',' ',' ',92 ,92 ,'/',' ',' ',' ','x',92 ,92 ,' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ' },
	{ ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',92 ,'>',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ' },
	{ ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ' },
	{ 'x','x',' ',' ','x',' ','/',' ',' ','=',' ',' ','x','+','x',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ' },
	{ 'x','-',' ',' ',' ','x',' ',' ',' ','x',' ',' ',' ','|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ' },
	{ 'x','x',' ',' ','/',' ','x',' ',' ','x',' ',' ',' ','|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ' } };

	for (short i = 0, index = 6; i < 13; i++)
	{
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 25, index++ });
		for (short j = 0; j < 35; j++)
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
			if (menu[i][j] == 'x')
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 255);
			}
			else if (menu[i][j] == '-' || menu[i][j] == 92 || menu[i][j] == '=' || menu[i][j] == '/' || menu[i][j] == '_' || menu[i][j] == '|' || menu[i][j] == '+' || menu[i][j] == '<' || menu[i][j] == '>')
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
			}
			printf("%c", menu[i][j]);
		}
	}
}

void color_menu() {

	for (short i = 0, index = 6; i < 13; i++)
	{
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),{ 25, index++ });
		for (short j = 0; j < 5; j++)
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
			if (i < 3)
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 170);
				printf(" ");
			}
			else if (i > 4 && i < 8)
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 238);
				printf(" ");
			}
			else if (i > 9 && i < 13)
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 119);
				printf(" ");
			}
		}
	}

	for (short i = 0, index = 6; i < 13; i++)
	{
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),{ 42, index++ });
		for (short j = 0; j < 5; j++)
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
			if (i < 3)
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 187);
				printf(" ");
			}
			else if (i > 4 && i < 8)
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 221);
				printf(" ");
			}
			else if (i > 9 && i < 13)
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 153);
				printf(" ");
			}
		}
	}
}

void help_panel() {
	for (short i = 0; i < 24; i++)
	{
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 60, i });
		switch (i)
		{
		case 0: {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 255);
			printf("\t\t       ");
		}
				break;
		case 6: {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 255);
			printf("\t\t       ");
		}
				break;
		case 8: {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);
			printf("\t   Control:");
		}
				break;
		case 10: {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);
			printf("\t      W");
		}
				 break;
		case 12: {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);
			printf("\t   A  S  D ");
		}
				 break;
		case 14: {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 255);
			printf("\t\t       ");
		}
				 break;
		case 16: {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
			printf("  Exit to the menu:");
		}
				 break;
		case 18: {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
			printf("\t   - Esc");
		}
				 break;
		case 23: {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 255);
			printf("\t\t       ");
		}
					break;
		}
	}
}