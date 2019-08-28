#include "huycuong.h"
int main()
{
	
	ofstream ofs;
	ofs.open("High_Score.txt",ios::app);
	srand(time(NULL));//khởi tạo bộ sinh số ngẫu nhiên
	snake ran;
	hoaqua hq;

	setFullScreen();

	Welcome();

	khoitao(ran, hq);

	ShowCur(false);

	int ma = 0;
	int diem = 0;
	int thoigiansleep = 200;
	while (1)
	{
		//show objs on screen
		if (diem < 10)
		{
			hienthi(ran, hq, diem);
			ShowHighScore();
			ma = xuly(ran, hq, diem, thoigiansleep);
		}
		else if (diem >= 10)
		{
			Hienthi2(ran, hq, diem);
			ShowHighScore();
			ma = xuly2(ran, hq, diem, thoigiansleep);
		}
		//control
		int something=dieukhien_dichguyen(ran);
		if (something == 1)
			break;
		//process win lose
		
		if (ma == -1)
		{
			gotoxy(5, 10);
			cout << "YOU LOSE!!!";
			ofs << diem<<"\t";
			ofs.close();
			_getch();
			break;
		}
		Sleep(thoigiansleep);
	}
	return 0;
}