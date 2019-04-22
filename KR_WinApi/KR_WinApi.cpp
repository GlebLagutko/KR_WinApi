#include "stdafx.h"
#include "WinApiModule.h"
#include <string>
#define _USE_MATH_DEFINES
#include <ostream>
#include <iostream>
#include <commdlg.h>
#include <math.h>


#define DLGTITLE  L"My dialog"
#define DLGFONT   L"MS Shell Dlg"

using namespace std;

bool paused = false;
double g = 0.6;
int speed = 0;
int t = 0;
bool down = true;
int floor1 = 400;
int h = 340;
int top1 = 50;
int left1 = 272;
int top2 = 190;
int left2 = 432;
int top3 = 190;
int left3 = 472;
int r1 = 255;
int g1 = 150;
int b1 = 0;
double phi1 = - M_PI;


HINSTANCE hInst;

namespace Color
{
	static COLORREF FromHex(int hex)
	{
		auto r = (hex & 0xFF0000) >> 16;
		auto g = (hex & 0x00FF00) >> 8;
		auto b = (hex & 0x0000FF) >> 0;
		return RGB(r, g, b);
	}

	static const COLORREF Firebrick = FromHex(0xB22222);
	static const COLORREF Goldenrod1 = FromHex(0xFFC125);
	static const COLORREF SpringGreen3 = FromHex(0x00CD66);
	static const COLORREF Black = FromHex(0x000000);
	static const COLORREF White = FromHex(0xFFFFFF);
	static const COLORREF SteelBlue2 = FromHex(0x5CACEE);
	static const COLORREF SlateBlue = FromHex(0x6A5ACD);
	static const COLORREF Sienna1 = FromHex(0xFF8247);
	static const COLORREF Turquoise3 = FromHex(0x00C5CD);
}

namespace Control
{
	int const IDC_TestTextBoxB = 1001;
	int const IDC_TestTextBoxA = 1002;
	int const IDC_TestButtonOk = 1003;
	int const IDC_TestButtonCancel = 1004;
	int const IDC_TestARadioButton = 1005;
	int const IDC_TestBRadioButton = 1006;
	int const IDC_TestComboBox = 1007;
	int const IDC_InvisibleButton = 1008;
	int const IDC_Test2GroupBox = 1009;
	int const IDC_TestCRadioButton = 1010;
	int const IDC_TestDRadioButton = 1011;
	int const IDC_MessageButton = 1012;
	int const IDC_TestLabel = 1013;
}


#pragma pack(push, 4)                 
static struct
{
	DWORD  style;
	DWORD  dwExtendedStyle;
	WORD   ccontrols; // ?????????? ?????????. ???????? ????? ????????? ????? ???, ? ???? ????????? ????? ???? ?????, ?? ??????
	short  x;
	short  y;
	short  cx;
	short  cy;
	WORD   menu;
	WORD   windowClass;
	WCHAR  wszTitle[ARRAYSIZE(DLGTITLE)];
	short  pointsize; // ??????????????? ?????????. ??. FromUnit ??? ?????????????? ?????????.
	WCHAR  wszFont[ARRAYSIZE(DLGFONT)];
}
EmptyDialogTemplate =
{
   DS_SHELLFONT | DS_SETFONT | DS_MODALFRAME | DS_FIXEDSYS | WS_POPUP | WS_CAPTION | WS_SYSMENU, // ?????????!
   0x0,
   0,
	0,  0, 150, 80,
   0,                       // menu: none
   0,                       // window class: none
   DLGTITLE,                // Window caption
   8,                       // font pointsize
   DLGFONT,
};
#pragma pack(pop)

class MathToHdcConverter
{
	HWND _hWnd;

public:
	explicit MathToHdcConverter(HWND hWnd) : _hWnd(hWnd) {  }

	int GetX(const double x) const
	{
		return GetCenterY() + x ;
	}

	int GetY(const double y) const
	{
		return GetCenterX() - y ;
	}

	

	double PolarToX(double r, double phi)
	{
		return r * cos(phi);; // TODO.
	}

	double PolarToY(double r, double phi)
	{
		return r * sin(phi); // TODO.
	}

private:
	int GetCenterX() const
	{
		return GetClientRect().right / 2;
	}

	int GetCenterY() const
	{
		return 50;
	}

	RECT GetClientRect() const
	{
		RECT clientRect;
		::GetClientRect(_hWnd, &clientRect);
		return clientRect;
	}

};

double GetNewHeight(HWND hWnd)
{
	auto textLength = GetWindowTextLengthW(hWnd);
	auto buffer = new wchar_t[textLength + 1];
	GetWindowText(hWnd, buffer, textLength + 1);
	double result = wcstod(buffer, NULL);
	return result;
}



INT_PTR CALLBACK DlgProc(HWND hwndDlg, UINT msg, WPARAM wParam,
	LPARAM lParam)
{
	switch (msg)
	{
	case WM_INITDIALOG:
	{
		CreateWindowEx(NULL, TEXT("STATIC"), TEXT("Edit Acce : "),
			WS_CHILD | WS_VISIBLE, 10, 10, 90,
			20, hwndDlg, nullptr, nullptr, nullptr);
		CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("Edit"), TEXT(""),
			WS_CHILD | WS_VISIBLE, 120, 10, 60,
			20, hwndDlg, (HMENU)Control::IDC_TestTextBoxA, nullptr, nullptr);


		CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("Button"), TEXT("Ok"),
			WS_CHILD | WS_VISIBLE, 100, 40, 30,
			20, hwndDlg, (HMENU)Control::IDC_TestButtonOk, NULL, NULL);
		CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("Button"), TEXT("Cancel"),
			WS_CHILD | WS_VISIBLE, 150, 40, 50,
			20, hwndDlg, (HMENU)Control::IDC_TestButtonCancel, NULL, NULL);


		return (INT_PTR)TRUE;
	}
	case WM_COMMAND:
	{
		switch (wParam)
		{
		case Control::IDC_TestButtonOk:
		{
			auto TextHeight = GetDlgItem(hwndDlg, Control::IDC_TestTextBoxA);
			if (GetWindowTextLengthW(TextHeight) != 0)
			{
				if (GetNewHeight(TextHeight) >= 0)
				{
					g = GetNewHeight(TextHeight);
					if (paused)
						paused = !paused;
				}
				else
					MessageBoxA(hwndDlg, "Error!!! Wrong Acce", NULL, MB_OK);
			}


			EndDialog(hwndDlg, IDCANCEL);
		}
		break;

		case Control::IDC_TestButtonCancel:
		{
			EndDialog(hwndDlg, IDCANCEL);
		}
		break;
		}
	}
	break;


	case WM_CLOSE:
	{
		EndDialog(hwndDlg, 0);
		return FALSE;
	}
	}
	return FALSE;

}

class Application
{
	static const int MinDialogWidth = 300;
	static const int MinDialogHeight = 250;

	static const int Timer1ID = 1;

	static const int Timer1Interval = 20;

	static const int Timer1MaxValue = 2;

	static int _timer1Ticks;

public:
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
		case WM_CREATE:
		{
			OnCreate(hWnd);
			return true;
		}

		case WM_GETMINMAXINFO:
		{
			OnGetMinMaxInfo((LPMINMAXINFO)lParam);
			return TRUE;
		}

		case WM_TIMER:
		{
			OnTimer1(hWnd, (int)wParam);
			return true;
		}

		case WM_KEYDOWN:
		{
			switch (wParam)
			{
			case VK_F3:
			{
				DialogBoxIndirectParamW(hInst, (LPCDLGTEMPLATEW)&EmptyDialogTemplate, hWnd, DlgProc, NULL);
			}
			break;
			case 'P' :
			case 'p' :
			{
				paused = !paused;
			}
			break;
			}

		}
		break;

		case WM_LBUTTONDBLCLK :
		{
			int x = LOWORD(lParam);
			int y = HIWORD(lParam);

			if(x > left1 && x < left1 + 40 && y > top1 && y < top1 + 40)
			{
				r1 = 255 - r1; g1 = 255 - g1; b1 = 255 - b1;
			}
			break;
		}

		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			auto hdc = BeginPaint(hWnd, &ps);
			OnPaint(hWnd, hdc);
			EndPaint(hWnd, &ps);
			break;
		}


		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}

		return 0;
	}

private:

	

	static void OnCreate(HWND hWnd)
	{
		SetTimer(hWnd, Timer1ID, Timer1Interval, nullptr);
	}

	static void OnGetMinMaxInfo(LPMINMAXINFO minMaxInfo)
	{
		minMaxInfo->ptMinTrackSize.x = MinDialogWidth;
		minMaxInfo->ptMinTrackSize.y = MinDialogHeight;
	}

	static void CalculatePosition(int top,int left,double phi,HWND hWnd)
	{
		if (_timer1Ticks == 0 && !paused)
		{
			auto converter = MathToHdcConverter(hWnd);
			if (down)
			{
				t++;
				phi += t * g;
				auto a = converter.PolarToX(140, phi);
				top1 = converter.GetX(a);
				auto b = converter.PolarToY(140, phi);
				left1 = converter.GetY(b);
			}
			else
			{

				t++;
				phi -= t * g;
				auto a = converter.PolarToX(140, phi);
				top1 = converter.GetX(a);
				auto b = converter.PolarToY(140, phi);
				left1 = converter.GetY(b);
			}
		
		}
	}

	static void OnTimer1(HWND hWnd, int timerID)
	{
		if (timerID != Timer1ID) return;
		if (!paused)
			++_timer1Ticks;

		if (_timer1Ticks > Timer1MaxValue)
		{
			_timer1Ticks = 0;
		}

		InvalidateRect(hWnd, nullptr, true);
	}

	static void CheckDown(double phi)
	{
		if (down)
		{
			if (phi + (t + 1) * g > 0)
			{

				t = 0;
				phi = 0;
				down = !down;
			}
     	}
		else
		{
			if (phi - (t + 1) * g < - M_PI)
			{

				t = 0;
				phi = -M_PI;

				down = !down;
			}
		}
	}


	static void OnPaint(HWND hWnd, HDC hdc)
	{

		auto clientRect = GetClientRect(hWnd);
		auto width = (clientRect.right - clientRect.left) / 2;
		floor1 = clientRect.top;
	//	CheckDown(phi1);
		CalculatePosition(top1,left1,phi1,hWnd);

		SelectObject(hdc, CreateSolidBrush(Color::SteelBlue2));
		Rectangle(hdc, width - 50, floor1, width + 50, floor1 + 50);
		SelectObject(hdc, CreateSolidBrush(RGB(r1, g1, b1)));
		Ellipse(hdc, left1 , top1, left1 + 40, top1 + 40);
	
		// 1
		auto pen3 = CreatePen(PS_NULL, 1, RGB(0, 0, 0));
		SelectObject(hdc, pen3);
		LineTo(hdc, width, 50);
		auto pen2 = CreatePen(PS_SOLID, 1, Color::Sienna1);
		SelectObject(hdc, pen2);
		LineTo(hdc, left1 + 20, top1);
	

	}


	static RECT GetClientRect(HWND hWnd)
	{
		RECT rect;
		::GetClientRect(hWnd, &rect);
		return rect;
	}

	static HFONT CreateFont(std::wstring name, int size, int weight)
	{
		auto lf = LOGFONT{};
		wcscpy_s(lf.lfFaceName, name.data());
		lf.lfHeight = size;
		lf.lfWeight = weight;
		lf.lfCharSet = DEFAULT_CHARSET;
		return CreateFontIndirect(&lf);
	}

	static void SelectObjectAndDeletePrevious(HDC hDeviceContext, HGDIOBJ gdiObject)
	{
		auto previousGdiObject = SelectObject(hDeviceContext, gdiObject);
		DeleteObject(previousGdiObject);
	}

};

int Application::_timer1Ticks = 0;

int APIENTRY wWinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	auto winApiModule = WinApiModule(
		L"9_Timer_and_Drawing",
		L"KR.WinApi",
		hInstance,
		Application::WndProc);

	return winApiModule.Run(nCmdShow);
}