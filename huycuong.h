#ifndef _huycuong_h_
#define _huycuong_h_
#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <time.h>
#include<fstream>
#include<string>
#define ColorCode_Back			0
#define ColorCode_DarkBlue		1
#define ColorCode_DarkGreen		2
#define ColorCode_DarkCyan		3
#define ColorCode_DarkRed		4
#define ColorCode_DarkPink		5
#define ColorCode_DarkYellow	6
#define ColorCode_DarkWhite		7
#define ColorCode_Grey			8
#define ColorCode_Blue			9
#define ColorCode_Green			10
#define ColorCode_Cyan			11
#define ColorCode_Red			12
#define ColorCode_Pink			13
#define ColorCode_Yellow		14
#define ColorCode_White			15

#define default_ColorCode		7


#define key_Up		1072
#define key_Down	1080
#define key_Left	1075
#define key_Right	1077
#define key_none	-1
using namespace std;
#define chieudaitung 20
#define chieudaihoanh 20

//tao mot enum de biet dieu khien con ran
enum trangthai { phai, trai, len, xuong };

struct toado
{
	int x, y;
};

struct snake
{
	toado dot[31];
	int n;//chieu dai
	trangthai tt;
};
struct hoaqua
{
	toado td;
};
//nhảy tới toạ độ xy
void gotoxy(int x, int y);
//tạo màu chữ
void TextColor(int x);
//khởi tạo rắn
void khoitao(snake &ran, hoaqua &hq);
//xử lý màn 1
int xuly(snake &ran, hoaqua &hq, int &diem, int &thoigiansleep);
//xử lý màn 2
int xuly2(snake &ran, hoaqua &hq, int &diem, int &thoigiansleep);
//điều khiển rắn
int dieukhien_dichguyen(snake &ran);
//hiển thị rắn, hoa quả , điểm số ra màn hình
void hienthi(snake ran, hoaqua hq, int diem);
//xoá màn hình
void clrscr();
//hàm này để lấy key, ko cần quan tâm
int inputKey();
//hiển thị rắn, hoa quả, điểm màn 2
void Hienthi2(snake ran, hoaqua hq, int diem);
//chạy màn hình chào mừng
void Welcome();
//ẩn hiện con trỏ
void ShowCur(bool CursorVisibility);
//thiết lập có full màn hình hay ko
void setFullScreen();
//hàm xử lý, ko cần quan tâm
BOOL NT_SetConsoleDisplayMode(HANDLE hOutputHandle, DWORD dwNewMode);
//xoá màn hình
void cls(HANDLE hConsole);
void xoamanhinh();
//chỉnh cỡ chữ
void setFontSize(int FontSize);
//từ đây trở xuống sẽ cmt bằng tiếng anh cho chuyên nghiệp <3
void GetData(char *path, int *&a, int &n);//get data from file put to array
int  MaxOfArr(int *a, int n);
int CountN(char *path);//count how many item in file
void ShowHighScore();

#endif