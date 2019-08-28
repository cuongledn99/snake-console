#include"huycuong.h"

double _tickCount; // Biến đếm thời gian, sử dụng cho Mainloop

HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE); // Lấy handle cửa sổ console hiện hành
//ẩn/hiện con trỏ false là ẩn true là hiện
void ShowCur(bool CursorVisibility)
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursor = { 1, CursorVisibility };
	SetConsoleCursorInfo(handle, &cursor);
}
//thay đổi kích thước chữ
void setFontSize(int FontSize)
{
	CONSOLE_FONT_INFOEX info = { 0 };
	info.cbSize = sizeof(info);
	info.dwFontSize.Y = FontSize; // leave X as zero
	info.FontWeight = FW_NORMAL;
	wcscpy_s(info.FaceName, L"Lucida Console");
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), NULL, &info);
}
// Thiết lập chế độ hiển thị, có fullscreen hay không
BOOL NT_SetConsoleDisplayMode(HANDLE hOutputHandle, DWORD dwNewMode)
{
	typedef BOOL(WINAPI *SCDMProc_t) (HANDLE, DWORD, LPDWORD);
	SCDMProc_t SetConsoleDisplayMode;
	HMODULE hKernel32;
	BOOL bFreeLib = FALSE, ret;
	const char KERNEL32_NAME[] = "kernel32.dll";

	hKernel32 = GetModuleHandleA(KERNEL32_NAME);
	if (hKernel32 == NULL)
	{
		hKernel32 = LoadLibraryA(KERNEL32_NAME);
		if (hKernel32 == NULL)
			return FALSE;

		bFreeLib = true;
	}

	SetConsoleDisplayMode =
		(SCDMProc_t)GetProcAddress(hKernel32, "SetConsoleDisplayMode");
	if (SetConsoleDisplayMode == NULL)
	{
		SetLastError(ERROR_CALL_NOT_IMPLEMENTED);
		ret = FALSE;
	}
	else
	{
		DWORD tmp;
		ret = SetConsoleDisplayMode(hOutputHandle, dwNewMode, &tmp);
	}

	if (bFreeLib)
		FreeLibrary(hKernel32);

	return ret;
}
// Xóa toàn bộ nội dung cửa sổ console - chôm từ MSDN
void cls(HANDLE hConsole)
{
	COORD coordScreen = { 0, 0 };    // home for the cursor
	DWORD cCharsWritten;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	DWORD dwConSize;

	// Get the number of character cells in the current buffer.

	if (!GetConsoleScreenBufferInfo(hConsole, &csbi))
		return;
	dwConSize = csbi.dwSize.X * csbi.dwSize.Y;

	// Fill the entire screen with blanks.

	if (!FillConsoleOutputCharacter(hConsole, (TCHAR) ' ',
		dwConSize, coordScreen, &cCharsWritten))
		return;

	// Get the current text attribute.

	if (!GetConsoleScreenBufferInfo(hConsole, &csbi))
		return;

	// Set the buffer's attributes accordingly.

	if (!FillConsoleOutputAttribute(hConsole, csbi.wAttributes,
		dwConSize, coordScreen, &cCharsWritten))
		return;

	// Put the cursor at its home coordinates.

	SetConsoleCursorPosition(hConsole, coordScreen);
}
// Viết lại hàm clrscr, không cần cũng được nhưng để nhìn vào code cho đẹp mắt
void xoamanhinh()
{
	cls(hCon);
}

// Đặt chế độ FullScreen
void setFullScreen()
{
	NT_SetConsoleDisplayMode(GetStdHandle(STD_OUTPUT_HANDLE), 1);
}
//show welcome screen
void Welcome()
{
	char c;
	ifstream ifs;
	ifs.open("logo.txt");//mở file
	if (!ifs)
		cout << "error";
	while (!ifs.eof())//in ra từ từ
	{
		ifs.get(c);
		if (!ifs.eof())
			cout << c;
		Sleep(1);
		if (_kbhit())//nếu phát hiện có nhấn phím
		{
			ifs.close();//đóng file
			break;
		}
	}
		
	system("pause");
	while (true)
	{
		cout << "\a";
		clrscr();//xoá màn hình
		ifs.open("logo.txt");//mở file
		if (!ifs)
			cout << "error";
		TextColor(rand() % 15);
		while (!ifs.eof())//in ra lập tức
		{
			ifs.get(c);
			if (!ifs.eof())
				cout << c;
		}
		ifs.close();//đóng file
		
		if (_kbhit())
			break;
		
	}
	system("pause");
	//sang trang intro mới
	clrscr();  
	gotoxy(30, 0);
	setFontSize(30);
	cout << "INTRUCTION: "<<endl;
	gotoxy(30, 1);
	cout << "-> OR d/D: move left" << endl;
	gotoxy(30, 2);
	cout << "<- OR a/A: move right" << endl;
	gotoxy(30, 3);
	cout << "^ OR w/W: move up" << endl;
	gotoxy(30, 4);
	cout << "Press DOWN button OR s/S to move down"<<endl;
	system("pause");
}
void gotoxy(int x, int y)
{
	static HANDLE h = NULL;
	if (!h)
		h = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD c = { x, y };
	SetConsoleCursorPosition(h, c);
}
void TextColor(int x)
{
	HANDLE mau;
	mau = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(mau, x);
}
void khoitao(snake &ran, hoaqua &hq)
{
	//khởi tạo rắn
	ran.n = 1;
	ran.dot[0].x = 5;
	ran.dot[0].y = 5;
	ran.tt = xuong;
	//khởi tạo hoa quả
	hq.td.x = 13;
	hq.td.y = 13;

}
void GetData(char *path, int *&a, int &n)
{
	n = CountN("High_Score.txt");
	ifstream ifs;
	ifs.open(path);
	if (!ifs)
		cout << "error!!!";
	a = new int[n];
	for (int i = 0; i < n; i++)
		ifs >> a[i];
	ifs.close();
}
int  MaxOfArr(int *a, int n)
{
	int max = a[0];
	for (int i = 0; i < n; i++)
		if (a[i] > max)
			max = a[i];
	return max;

}
int CountN(char *path)
{
	int counter = 0, temp;
	ifstream ifs;
	ifs.open(path);
	if (!ifs)
		cout << "error!!!";
	while (!ifs.eof())
	{
		ifs >> temp;
		counter++;
	}
	ifs.close();
	return counter-1;

}
void ShowHighScore()
{
	int *a, n;
	GetData("High_Score.txt", a, n);
	gotoxy(25, 4);
	cout << "HIGH SCORE: " << MaxOfArr(a,n);
}

void hienthi(snake ran, hoaqua hq, int diem)
{
	clrscr();
	
	//in màn
	gotoxy(25, 2);
	TextColor(9);
	cout << "Màn 1";
	//in điểm
	gotoxy(25, 5);
	TextColor(15);
	cout << "Score: " << diem;
	//in high score
	//ShowHighScore();
	//in hoa quả
	gotoxy(hq.td.x, hq.td.y);
	TextColor(10);
	putchar('$');
	//in rào phải
	for (int i = 0; i < chieudaitung; i++)
	{
		gotoxy(chieudaihoanh, i);
		TextColor(4);
		putchar('|');
	}
	//in biên dưới
	for (int i = 0; i < chieudaihoanh; i++)
	{
		gotoxy(i, chieudaitung);
		TextColor(4);
		putchar('-');
	}
	//in góc dưới phải
	gotoxy(chieudaihoanh, chieudaitung);
	TextColor(4);
	putchar(188);
	//in cái đầu
	TextColor(rand() % 15);
	gotoxy(ran.dot[0].x, ran.dot[0].y);
	putchar('0');
	//in thân rắn
	for (int i = 1; i < ran.n; i++)
	{
		gotoxy(ran.dot[i].x, ran.dot[i].y);
		TextColor(13);
		putchar('*');
	}

}
void Hienthi2(snake ran, hoaqua hq, int diem)
{
	clrscr();
	//in màn
	gotoxy(25, 2);
	TextColor(9);
	cout << "Màn 2";
	//in điểm
	gotoxy(25, 5);
	TextColor(15);
	cout << "Score: " << diem;
	//in hoa quả
	gotoxy(hq.td.x, hq.td.y);
	TextColor(10);
	putchar('$');
	//in rào phải
	for (int i = 0; i < chieudaitung; i++)
	{
		gotoxy(chieudaihoanh, i);
		TextColor(4);
		putchar('|');
	}
	//in biên dưới
	for (int i = 0; i < chieudaihoanh; i++)
	{
		gotoxy(i, chieudaitung);
		TextColor(4);
		putchar('-');
	}
	//in góc dưới phải
	gotoxy(chieudaihoanh, chieudaitung);
	TextColor(4);
	putchar(188);
	//in mê cung
	for (int i = 5; i < 10; i++)
	{
		gotoxy(i, 10);
		putchar('-');
	}
	for (int i = 5; i < 10; i++)
	{
		gotoxy(10, i);
		putchar('|');
	}
	for (int i = 5; i < 10; i++)
	{
		gotoxy(13, i);
		putchar('|');
	}
	for (int i = 5; i < 16; i++)
	{
		gotoxy(i, 15);
		putchar('-');
		
	}
	for (int i = 14; i < 16; i++)
	{
		gotoxy(i, 10);
		putchar('-');
	}
	gotoxy(10, 10);
	putchar(170);
	gotoxy(13, 10);
	putchar(170);
	//in cái đầu
	TextColor(rand() % 15);
	gotoxy(ran.dot[0].x, ran.dot[0].y);
	putchar('0');
	//in thân rắn
	for (int i = 1; i < ran.n; i++)
	{
		gotoxy(ran.dot[i].x, ran.dot[i].y);
		TextColor(13);
		putchar('*');
	}
}
int dieukhien_dichguyen(snake &ran)
{
	//xử lý thân chạy theo đuôi
	for (int i = ran.n - 1; i > 0; i--)
		ran.dot[i] = ran.dot[i - 1];
	//phát hiện có phím nhấn vào
	//set phím
	if (_kbhit())
	{
		int key = _getch();
		int special_key = inputKey();
		if ((key == 'a' || key == 'A' || special_key == 75) && ran.dot[0].x < chieudaihoanh && ran.dot[0].x>0 && ran.dot[0].y<chieudaitung && ran.dot[0].y>0 && ran.tt != phai)
			ran.tt = trai;
		else if ((key == 'd' || key == 'D' || special_key == 77) && ran.dot[0].x < chieudaihoanh && ran.dot[0].x>0 && ran.dot[0].y<chieudaitung && ran.dot[0].y>0 && ran.tt != trai)
			ran.tt = phai;
		else if ((key == 'w' || key == 'W' || special_key == 72) && ran.dot[0].x < chieudaihoanh && ran.dot[0].x>0 && ran.dot[0].y<chieudaitung && ran.dot[0].y>0 && ran.tt != xuong)
			ran.tt = len;
		else if ((key == 's' || key == 'S' || special_key == 80) && ran.dot[0].x < chieudaihoanh && ran.dot[0].x>0 && ran.dot[0].y<chieudaitung && ran.dot[0].y>0 && ran.tt != len)
			ran.tt = xuong;
		else if (key == 'p' || key == 'P')
		{
			gotoxy(30, 0);
			cout << "Do you want to exits ?"<<endl;
			gotoxy(33, 1);
			cout << "Yes(Y)";
			gotoxy(40, 1);
			cout << "No(N)";
			int choise=_getch();
			if (choise == 'y' || choise == 'Y')
				return 1;
		}
	}
	//xử lý di chuyển
	if (ran.tt == trai)
		ran.dot[0].x--;
	else if (ran.tt == phai)
		ran.dot[0].x++;
	else if (ran.tt == len)
		ran.dot[0].y--;
	else if (ran.tt == xuong)
		ran.dot[0].y++;
}
int xuly(snake &ran, hoaqua &hq, int &diem, int &thoigiansleep)
{
	//nếu toạ độ đầu rắn trùng hoa quả
	if (ran.dot[0].x == hq.td.x && ran.dot[0].y == hq.td.y)
	{
		ran.n++;//dài ra
		diem++;
		if (ran.tt == trai)
			ran.dot[0].x--;
		else if (ran.tt == phai)
			ran.dot[0].x++;
		else if (ran.tt == len)
			ran.dot[0].y--;
		else if (ran.tt == xuong)
			ran.dot[0].y++;
		//tạo hoa quả random
		hq.td.x = rand() % chieudaihoanh;
		hq.td.y = rand() % chieudaitung;
		
		//giảm thời gian sleep
		if (thoigiansleep>30)
			thoigiansleep -= 10;

	}
	
	//đâm vào thân chết
	for (int i = 1; i < ran.n; i++)
		if (ran.dot[i].x == ran.dot[0].x && ran.dot[i].y == ran.dot[0].y)
			return -1;
	//xử lý xuyên tường
	if (ran.dot[0].x > chieudaihoanh - 1)
	{
		ran.dot[0].x = 0;
	}
	else if (ran.dot[0].x < 0)
	{
		ran.dot[0].x = chieudaihoanh;
	}
	else if (ran.dot[0].y < 0)
	{
		ran.dot[0].y = chieudaitung;
	}
	else if (ran.dot[0].y > chieudaitung - 1)
	{
		ran.dot[0].y = 0;
	}
	
}
int xuly2(snake &ran, hoaqua &hq, int &diem, int &thoigiansleep)
{
	//reset thời gian sleep
	thoigiansleep = 300;
	//nếu toạ độ đầu rắn trùng hoa quả
	if (ran.dot[0].x == hq.td.x && ran.dot[0].y == hq.td.y)
	{
		ran.n++;//dài ra
		diem++;
		if (ran.tt == trai)
			ran.dot[0].x--;
		else if (ran.tt == phai)
			ran.dot[0].x++;
		else if (ran.tt == len)
			ran.dot[0].y--;
		else if (ran.tt == xuong)
			ran.dot[0].y++;
		//tạo hoa quả random
		hq.td.x = rand() % chieudaihoanh;
		hq.td.y = rand() % chieudaitung;
		for (int i = 5; i < 11; i++)
			while (hq.td.x == i && hq.td.y == 10)
				hq.td.x = rand() % chieudaihoanh;
		for (int i = 14; i<16; i++)
			while (hq.td.x == i && hq.td.y == 10)
				hq.td.x = rand() % chieudaihoanh;
		for (int i = 5; i<16; i++)
			while (hq.td.x == i && hq.td.y == 15)
				hq.td.x = rand() % chieudaihoanh;
		for (int i = 5; i < 11; i++)
			while (hq.td.y == i && hq.td.x == 10)
				hq.td.y = rand() % chieudaitung;
		for (int i = 5; i < 11; i++)
			while (hq.td.y == i && hq.td.x == 14)
				hq.td.y = rand() % chieudaitung;

		//giảm thời gian sleep
		if (thoigiansleep>30)
			thoigiansleep -= 10;

	}

	//đâm vào thân chết
	for (int i = 1; i < ran.n; i++)
		if (ran.dot[i].x == ran.dot[0].x && ran.dot[i].y == ran.dot[0].y)
			return -1;
	//xử lý xuyên tường
	if (ran.dot[0].x > chieudaihoanh - 1)
	{
		ran.dot[0].x = 0;
	}
	else if (ran.dot[0].x < 0)
	{
		ran.dot[0].x = chieudaihoanh;
	}
	else if (ran.dot[0].y < 0)
	{
		ran.dot[0].y = chieudaitung;
	}
	else if (ran.dot[0].y > chieudaitung - 1)
	{
		ran.dot[0].y = 0;
	}
	//xử lý đụng mê cung
	for (int i = 5; i < 11; i++)
		if (ran.dot[0].x == i && ran.dot[0].y == 10)
			return -1;
	for (int i = 14; i < 16; i++)
		if (ran.dot[0].x == i && ran.dot[0].y == 10)
			return -1;
	for (int i = 5; i < 16; i++)
		if (ran.dot[0].x == i && ran.dot[0].y == 15)
			return -1;
	for (int i = 5; i < 11; i++)
		if (ran.dot[0].y == i && ran.dot[0].x == 10)
			return -1;
	for (int i = 5; i < 11; i++)
		if (ran.dot[0].y == i && ran.dot[0].x == 14)
			return -1;
}



void clrscr()
{
	CONSOLE_SCREEN_BUFFER_INFO	csbiInfo;
	HANDLE	hConsoleOut;
	COORD	Home = { 0,0 };
	DWORD	dummy;

	hConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(hConsoleOut, &csbiInfo);

	FillConsoleOutputCharacter(hConsoleOut, ' ', csbiInfo.dwSize.X * csbiInfo.dwSize.Y, Home, &dummy);
	csbiInfo.dwCursorPosition.X = 0;
	csbiInfo.dwCursorPosition.Y = 0;
	SetConsoleCursorPosition(hConsoleOut, csbiInfo.dwCursorPosition);
}
int inputKey()
{
	if (_kbhit())
	{
		int key = _getch();

		if (key == 224)	// special key
		{
			key = _getch();
			return key + 1000;
		}

		return key;
	}
	else
	{
		return key_none;
	}

	return key_none;
}
