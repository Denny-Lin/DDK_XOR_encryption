#include<iostream>
#include<iomanip>
#include<windows.h>
using namespace std;
int main()
{
	LARGE_INTEGER startTime, endTime, fre;
	double times;
	QueryPerformanceFrequency(&fre); //取得CPU頻率
	QueryPerformanceCounter(&startTime); //取得開機到現在經過幾個CPU Cycle
	//doing something
	for (int i = 0; i<1000000000; i++);
	//doing something
	QueryPerformanceCounter(&endTime); //取得開機到程式執行完成經過幾個CPU Cycle
	times = ((double)endTime.QuadPart - (double)startTime.QuadPart) / fre.QuadPart;
	cout << fixed << setprecision(20) << times << 's' << endl;
	system("pause");
	return 0;
}