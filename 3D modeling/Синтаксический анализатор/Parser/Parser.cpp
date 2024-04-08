#include <iostream>
#include <fstream>
#include <conio.h>
#include <string>
#include <cstring>

using namespace std;

//using std::cout;
//using std::endl;
//using std::cin;


int main() {

	ifstream fcin;
	ofstream fout_c, fout_l, fout_s;

	cout << "+----------------------------------+\n";
	cout << "| Specify the file location (.obj) |\n";
	cout << "+----------------------------------+\n";
	string file_name;
	string file_way = "D:\\Проекты Cinema 4D\\obj\\";
	
	do
	{
		getline(cin, file_name);
	} while (file_name[0] == '.');

	if (file_name.length() < 4)
	{
		file_name += ".obj";
	}
	else if (file_name[file_name.length() - 4] != '.')
	{
		file_name += ".obj";
	}	

	file_way += file_name;

	//fcin.open("D:\\Проекты Cinema 4D\\obj\\wheel.obj");
	fcin.open(file_way);
	fout_c.open("C:\\Users\\Gesser\\Desktop\\Coordinate sorting\\coord.txt");
	fout_l.open("C:\\Users\\Gesser\\Desktop\\Coordinate sorting\\line.txt");
	fout_s.open("C:\\Users\\Gesser\\Desktop\\Coordinate sorting\\size arr.txt");	

	if (!fcin.is_open())
	{
		exit(EXIT_FAILURE);
	}
	if (!fout_c.is_open() && !fout_l.is_open() && !fout_s.is_open())
	{
		exit(EXIT_FAILURE);
	}

	cout << "\n+-----------------------------------+\t+----------------------+\n";
	cout << "| Enter object name (for animation) |\t| Enter - \"N\" for exit |\n";
	cout << "+-----------------------------------+\t+----------------------+\n";

	string name_object;

	int SIZE_N = 0;

	string *panimation = nullptr;

	do
	{
		getline(cin, name_object);

		if (name_object != "n" && name_object != "N")
		{
			string *panimation_copy = nullptr;

			if (SIZE_N != 0)
			{
				panimation_copy = new string[SIZE_N];

				for (int i = 0; i < SIZE_N; i++)
				{
					*(panimation_copy + i) = *(panimation + i);
				}

				delete[] panimation;
			}

			SIZE_N++;
			panimation = new string[SIZE_N];

			*(panimation + (SIZE_N - 1)) += "g ";
			*(panimation + (SIZE_N - 1)) += name_object;

			if (SIZE_N != 1)
			{
				for (int i = 0; i < SIZE_N-1; i++)
				{
					*(panimation + i) = *(panimation_copy + i);
				}

				delete[] panimation_copy;
			}

		}

	} while (name_object != "n" && name_object != "N");

	
	string str;
	char ch = '\0';
	bool animate = false;

	getline(fcin, str);

	do
	{
		getline(fcin, str);

		if ('v' == str[0])
		{
			for (int i = 2; i < str.size(); i++)
			{
				if (' ' == str[i])
				{
					fout_c << endl;
					continue;
				}
				fout_c << str[i];
			}
			fout_c << endl;
			ch = str[0];
		}
		else if ('#' == str[0])
		{
			for (int i = 2; str[i] != ' ' && i < str.size(); i++)
			{
				fout_s << str[i];
			}

			fout_s << endl;
		}
		else if ('g' == str[0] && SIZE_N)
		{
			for (int i = 0; i < SIZE_N; i++)
			{
				if (*(panimation + i) == str)
				{
					//fout_l << 'b' << endl;
					str[0] = 'b';
					fout_l << str << endl;
					animate = true;
					break;
				}				
			}
		}
		else if ('f' == str[0])
		{
			for (int i = 2; i < str.size(); i++)
			{
				if (' ' == str[i])
				{
					fout_l << endl;
					continue;
				}
				fout_l << str[i];
			}
			fout_l << endl << 's' << endl;
			ch = str[0];
		}
		else if ('f' == ch && "" == str && animate)
		{
			fout_l << 'e' << endl;
			animate = false;
		}
		
	} while (!fcin.eof());

	if (SIZE_N != 0)
	{
		delete[] panimation;
	}
	
	fcin.close();
	fout_c.close();
	fout_l.close();
	fout_s.close();

	return 0;
}