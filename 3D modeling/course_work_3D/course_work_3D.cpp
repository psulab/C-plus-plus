// course_work_3D.cpp : Определяет точку входа для приложения.
//

#include "stdafx.h"
#include "course_work_3D.h"

using namespace std;

#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

ifstream fcin_c, 
		 fcin_l,
		 fcin_s;

const double radian = 3.14159265;

const int R = 360;

double focus = 10000000.0;

double x, y;

bool oc_door = false;

struct vector
{
	double x, y, z;
};

struct quaternion
{
	double x, y, z, w;
};

struct bus
{
	double x, y, z, w;
};

bus **obj;

//struct project
//{
//	double x = 1000.0, y = 1000.0, z = 1000.0;
//}pro;

struct tpo_element
{
	struct scaling //s - маштабирование
	{
		double x = 6.0, y = 6.0, z = 6.0;
	}s;
	struct bias //b - смещение
	{
		double x = 900.0, y = 490.0, z = 0.0;
	}b;
	struct rotation //r - вращение
	{
		double x = 0.0, y = 20.0, z = 0.0; //10.0
	}r;
	struct projection //p - проецирование
	{		
		//double x = 1.0 / pro.x, y = 1.0 / pro.y, z = 1.0 / pro.z;
		double x = 0.0, y = 0.0, z = 1.0 / focus;
	}p;

}matrix;

tpo_element::bias bdoor = { 0.0, 0.0, 0.0 };
double turn = 0.0;

void norma(vector * q); //нормализация вектора
quaternion create_quaternion(vector v, double angle); //создание кватерниона
quaternion quaternion_product(quaternion a, quaternion b); //произведение кватерниона
double norma_quater(quaternion b); //нормализация кватерниона
void quaternion_invert(quaternion * b); //обратный кватернион

bus rotation(bus * a, quaternion b); //вращение вокруг кватерниона
bus rotation_2d(bus * arr, char a, char b, double angle); //2D вращение
bus rotation_2d(bus * arr, char a, char b, double angle, double dot_1, double dot_2); //2D вращение вокруг произвольной точки
bus offset(bus a, tpo_element::bias b); //смещение
bus scope(bus a, tpo_element::scaling b); //маштабирование
void pt(bus arr, double *x, double *y); //перспективные преобразования

tpo_element::bias invert_offset_coordinates_x(tpo_element::bias a); //инвертируек координаты семещения x
tpo_element::bias invert_offset_coordinates_y(tpo_element::bias a); //инвертируек координаты семещения y
tpo_element::bias invert_offset_coordinates_z(tpo_element::bias a); //инвертируек координаты семещения z

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Разместите код здесь.

	fcin_c.open("..\\Coordinate sorting\\coord.txt");
	fcin_s.open("..\\Coordinate sorting\\size arr.txt");

	if (!fcin_c.is_open() && !fcin_s.is_open())
	{
		exit(EXIT_FAILURE);
	}
	
	int SIZE_N = 0;

	while (!fcin_s.eof())
	{
		int value;
		if (fcin_s >> value)
		{
			SIZE_N += value;
		}		
	}

	fcin_s.close();

	bus *arr = new bus[SIZE_N];
	
	int index = 0;

	while (!fcin_c.eof() && index < SIZE_N)
	{
		fcin_c >> (arr + index)->x;
		fcin_c >> (arr + index)->y;
		fcin_c >> (arr + index)->z;
		(arr + index)->w = 1.0f;
		index++;
	}

	fcin_c.close();
	
	obj = &arr;
		
    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_COURSEWORK3D, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_COURSEWORK3D));

    MSG msg;

    // Цикл основного сообщения:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}


//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  ЦЕЛЬ: Регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_COURSEWORK3D));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_COURSEWORK3D);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   ЦЕЛЬ: Сохраняет маркер экземпляра и создает главное окно
//
//   КОММЕНТАРИИ:
//
//        В этой функции маркер экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ЦЕЛЬ: Обрабатывает сообщения в главном окне.
//
//  WM_COMMAND  - обработать меню приложения
//  WM_PAINT    - Отрисовка главного окна
//  WM_DESTROY  - отправить сообщение о выходе и вернуться
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{	
	PAINTSTRUCT ps;
	HDC hdc;

    switch (message)
    {
	case WM_KEYDOWN:
	{
		switch (wParam)
		{
		case 0x46://f
		{
			if (!oc_door)
			{
				if (bdoor.x < 7.0)
				{
					bdoor.x++;
				}
				else if (bdoor.y > -1.0)
				{
					bdoor.y--;
				}
				else if (bdoor.z < 30.0)
				{
					bdoor.z++;
				}
				else
				{
					oc_door = true;
				}							
			}
			else
			{			
				if (bdoor.z > 0.0)
				{
					bdoor.z--;
				}
				else if (bdoor.y < 0.0)
				{
					bdoor.y++;
				}
				else if (bdoor.x > 0.0)
				{
					bdoor.x--;
				}
				else
				{
					oc_door = false;
				}
			}

					
			InvalidateRect(hWnd, NULL, true);
			UpdateWindow(hWnd);
		}
		break;
			// смещение
		case 0x57:
		{
			matrix.b.y -= 10;
			InvalidateRect(hWnd, NULL, true);
			UpdateWindow(hWnd);
		}
		break;

		case 0x53:
		{
			matrix.b.y += 10;
			InvalidateRect(hWnd, NULL, true);
			UpdateWindow(hWnd);
		}
		break;

		case 0x41:
		{
			matrix.b.x -= 10;
			InvalidateRect(hWnd, NULL, true);
			UpdateWindow(hWnd);
		}
		break;

		case 0x44:
		{
			matrix.b.x += 10;
			InvalidateRect(hWnd, NULL, true);
			UpdateWindow(hWnd);
		}
		break;

		//проецирование

		/*case 49:
		{
			pro.x += 10000.0;
			matrix.p.x = 1.0 / pro.x;
			InvalidateRect(hWnd, NULL, true);
			UpdateWindow(hWnd);
		}
		break;

		case 50:
		{
			pro.y += 10000.0;
			matrix.p.y = 1.0 / pro.x;
			InvalidateRect(hWnd, NULL, true);
			UpdateWindow(hWnd);
		}
		break;

		case 51:
		{
			pro.z += 10000.0;
			matrix.p.z = 1.0 / pro.x;
			InvalidateRect(hWnd, NULL, true);
			UpdateWindow(hWnd);
		}
		break;*/

		//вращение
		case VK_LEFT:
		{
			

			InvalidateRect(hWnd, NULL, true);
			UpdateWindow(hWnd);
		}
		break;
		case VK_RIGHT:
		{
			
			InvalidateRect(hWnd, NULL, true);
			UpdateWindow(hWnd);
		}
		break;

		case VK_UP:
		{			
			//turn++;
			turn += 10;
			
			InvalidateRect(hWnd, NULL, true);
			UpdateWindow(hWnd);
		}
		break;

		case VK_DOWN:
		{
			//turn--;
			turn -= 10;
			
			InvalidateRect(hWnd, NULL, true);
			UpdateWindow(hWnd);
		}
		break;

		case VK_NUMPAD8: //up
		{
			matrix.r.x++;
			
			InvalidateRect(hWnd, NULL, true);
			UpdateWindow(hWnd);
		}
		break;

		case VK_NUMPAD2: //down
		{
			matrix.r.x--;
			
			InvalidateRect(hWnd, NULL, true);
			UpdateWindow(hWnd);
		}
		break;

		case VK_NUMPAD4: //left
		{
			//matrix.r.y++;
			matrix.r.y += 2;

			InvalidateRect(hWnd, NULL, true);
			UpdateWindow(hWnd);
		}
		break;

		case VK_NUMPAD6: //right
		{
			//matrix.r.y--;
			matrix.r.y -= 2;

			InvalidateRect(hWnd, NULL, true);
			UpdateWindow(hWnd);
		}
		break;
		
		case VK_SPACE:
		{
			matrix.s.x += 0.1;
			matrix.s.y += 0.1;
			matrix.s.z += 0.1;
			InvalidateRect(hWnd, NULL, true);
			UpdateWindow(hWnd);
		}
		break;
		}
	}
		break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Разобрать выбор в меню:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            hdc = BeginPaint(hWnd, &ps);
									
			vector vx = { 1.0, 0.0, 0.0 };
			quaternion qx = create_quaternion(vx, matrix.r.x);

			vector vy = { 0.0, 1.0, 0.0 };
			quaternion qy = create_quaternion(vy, matrix.r.y);

			quaternion q = quaternion_product(qx, qy);			
												
			fcin_l.open("..\\Coordinate sorting\\line.txt");

			if (!fcin_l.is_open())
			{
				exit(EXIT_FAILURE);
			}

			int connect_lines;
			string name_animate;
			
			while (!fcin_l.eof())
			{
				if (fcin_l >> connect_lines)
				{
					if ("b ld" == name_animate)
					{						
						pt(rotation(&offset(*(*obj + (connect_lines - 1)), invert_offset_coordinates_x(bdoor)), q), &x, &y);
					}
					else if ("b rd" == name_animate)
					{												
						pt(rotation(&offset(*(*obj + (connect_lines - 1)), bdoor), q), &x, &y);
					}
					else if ("b fw" == name_animate)
					{					
						pt(rotation(&rotation_2d((*obj + (connect_lines - 1)), 'y', 'z', turn, 20.093, -30.0), q), &x, &y);
					}
					else if ("b bw" == name_animate)
					{
						pt(rotation(&rotation_2d((*obj + (connect_lines - 1)), 'y', 'z', turn, 20.093, 30.0), q), &x, &y);
					}					
					else
					{
						pt(rotation((*obj + (connect_lines - 1)), q), &x, &y);
					}

					MoveToEx(hdc, x, y, NULL);
				}				

				while (true)
				{
					if (fcin_l >> connect_lines)
					{	
						if ("b ld" == name_animate)
						{
							pt(rotation(&offset(*(*obj + (connect_lines - 1)), invert_offset_coordinates_x(bdoor)), q), &x, &y);
						}
						else if ("b rd" == name_animate)
						{
							pt(rotation(&offset(*(*obj + (connect_lines - 1)), bdoor), q), &x, &y);
						}
						else if ("b fw" == name_animate)
						{
							pt(rotation(&rotation_2d((*obj + (connect_lines - 1)), 'y', 'z', turn, 20.093, -30.0), q), &x, &y);
						}
						else if ("b bw" == name_animate)
						{
							pt(rotation(&rotation_2d((*obj + (connect_lines - 1)), 'y', 'z', turn, 20.093, 30.0), q), &x, &y);
						}
						else
						{
							pt(rotation((*obj + (connect_lines - 1)), q), &x, &y);
						}

						LineTo(hdc, x, y);
					}
					else
					{						
						break;
					}					
				}

				fcin_l.clear();
				string str;
				getline(fcin_l, str);

				if ('b' == str[0])
				{
					name_animate = str;
				}
				else if ("e" == str)
				{
					name_animate = "";
				}			
			}

			fcin_l.close();											   						
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Обработчик сообщений для окна "О программе".
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

void pt(bus a, double *x, double *y) {

	//s - маштабирование
	//b - смещение
	//r - вращение
	//p - проецирование
		
	a.x = a.x*matrix.s.x + a.w*matrix.b.x;
	a.y = a.y*matrix.s.y + a.w*matrix.b.y;
	a.z = a.z*matrix.s.z + a.w*matrix.b.z;
	a.w = a.x*matrix.p.x + a.y*matrix.p.y + a.z*matrix.p.z + a.w;
	
	*x = a.x / a.w;
	*y = a.y / a.w;
}

//нормализация вектора
void norma(vector * v) {

	double norma = sqrt(v->x * v->x + v->y * v->y + v->z * v->z);
	v->x /= norma;
	v->y /= norma;
	v->z /= norma;
}

//создание кватерниона
quaternion create_quaternion(vector v, double angle) {

	norma(&v);

	quaternion q = 
	{
		sin((angle / 2.0) * radian / 180.0)*v.x,
		sin((angle / 2.0) * radian / 180.0)*v.y,
		sin((angle / 2.0) * radian / 180.0)*v.z,
		cos((angle / 2.0) * radian / 180.0)
	};
	return q;
}

//произведение кватернионов
quaternion quaternion_product(quaternion a, quaternion b) {

	quaternion res;
	res.w = a.w*b.w - a.x*b.x - a.y*b.y - a.z*b.z;
    res.x = a.w*b.x + b.w*a.x + a.y*b.z - b.y*a.z;
    res.y = a.w*b.y + b.w*a.y + a.z*b.x - b.z*a.x;
    res.z = a.w*b.z + b.w*a.z + a.x*b.y - b.x*a.y;
	return res;
}

double norma_quater(quaternion b) {

	return sqrt(b.w*b.w + b.x*b.x + b.y*b.y + b.z*b.z);
}

void quaternion_invert(quaternion * b) {

	double norma = pow(norma_quater(*b), 2.0);
	b->x = -b->x / norma;
	b->y = -b->y / norma;
	b->z = -b->z / norma;
}

bus rotation(bus * arr, quaternion b) {

	quaternion a =
	{
		arr->x,
		arr->y,
		arr->z,
		0.0
	};

	a = quaternion_product(b, a);

	quaternion_invert(&b);

	a = quaternion_product(a, b);

	bus res = { a.x,a.y,a.z,1.0 };
	
	return res;
}

bus rotation_2d(bus * arr, char a, char b, double angle)
{
	bus r;

	if ('x' == a && 'y' == b)
	{
		r.x = arr->x * cos(angle * radian / 180.0) - arr->y * sin(angle * radian / 180.0);
		r.y = arr->y * cos(angle * radian / 180.0) + arr->x * sin(angle * radian / 180.0);
		r.z = arr->z;
		r.w = 1.0;
	}
	else if ('y' == a && 'z' == b)
	{
		r.y = arr->y * cos(angle * radian / 180.0) - arr->z * sin(angle * radian / 180.0);
		r.z = arr->z * cos(angle * radian / 180.0) + arr->y * sin(angle * radian / 180.0);
		r.x = arr->x;
		r.w = 1.0;
	}
	else if ('z' == a && 'x' == b)
	{
		r.z = arr->z * cos(angle * radian / 180.0) - arr->x * sin(angle * radian / 180.0);
		r.x = arr->x * cos(angle * radian / 180.0) + arr->z * sin(angle * radian / 180.0);
		r.y = arr->y;
		r.w = 1.0;
	}

	return r;
}

bus rotation_2d(bus * arr, char a, char b, double angle, double dot_1, double dot_2)
{
	bus r;

	if ('x' == a && 'y' == b)
	{
		r.x = dot_1 + (arr->x - dot_1) * cos(angle * radian / 180.0) - (arr->y - dot_2) * sin(angle * radian / 180.0);
		r.y = dot_2 + (arr->y - dot_2) * cos(angle * radian / 180.0) + (arr->x - dot_1) * sin(angle * radian / 180.0);
		r.z = arr->z;
		r.w = 1.0;
	}
	else if ('y' == a && 'z' == b)
	{
		r.y = dot_1 + (arr->y - dot_1) * cos(angle * radian / 180.0) - (arr->z - dot_2) * sin(angle * radian / 180.0);
		r.z = dot_2 + (arr->z - dot_2) * cos(angle * radian / 180.0) + (arr->y - dot_1) * sin(angle * radian / 180.0);
		r.x = arr->x;
		r.w = 1.0;
	}
	else if ('z' == a && 'x' == b)
	{
		r.z = dot_1 + (arr->z - dot_1) * cos(angle * radian / 180.0) - (arr->x - dot_2) * sin(angle * radian / 180.0);
		r.x = dot_2 + (arr->x - dot_2) * cos(angle * radian / 180.0) + (arr->z - dot_1) * sin(angle * radian / 180.0);
		r.y = arr->y;
		r.w = 1.0;
	}

	return r;
}

bus offset(bus a, tpo_element::bias b) {
	
	a.x += b.x;
	a.y += b.y;
	a.z += b.z;	
	
	return a;
}

bus scope(bus a, tpo_element::scaling b) {
	
	a.x *= b.x;
	a.y *= b.y;
	a.z *= b.z;

	return a;
}

tpo_element::bias invert_offset_coordinates_x(tpo_element::bias a) {
	
	a.x *= -1;

	return a;
}

tpo_element::bias invert_offset_coordinates_y(tpo_element::bias a) {
	
	a.y *= -1;

	return a;
}

tpo_element::bias invert_offset_coordinates_z(tpo_element::bias a) {

	a.z *= -1;

	return a;
}