#include<iostream>
#include<iomanip>
#include<windows.h>
using namespace std;
int main()
{
	LARGE_INTEGER startTime, endTime, fre;
	double times;
	QueryPerformanceFrequency(&fre); //���oCPU�W�v
	QueryPerformanceCounter(&startTime); //���o�}����{�b�g�L�X��CPU Cycle
	//doing something
	for (int i = 0; i<1000000000; i++);
	//doing something
	QueryPerformanceCounter(&endTime); //���o�}����{�����槹���g�L�X��CPU Cycle
	times = ((double)endTime.QuadPart - (double)startTime.QuadPart) / fre.QuadPart;
	cout << fixed << setprecision(20) << times << 's' << endl;
	system("pause");
	return 0;
}