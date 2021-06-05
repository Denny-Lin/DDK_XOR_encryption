#include <stdio.h>
#include <string.h>
#include <io.h> //c_file
#include <direct.h> 
#include <stdlib.h>

void main()
{
	char sdir[256], filter[256], path[256];
	struct _finddata_t c_file;
	long hFile;

	strcpy_s(sdir, 256,"C:\\Users\\HungYuLin\\Desktop\\星期四 專題1\\GetAllFileNameinDirectory\\Debug");
	strcpy_s(filter, 256,"*.txt");
	//strcpy_s(filter, 256, "*");

	_chdir(sdir);
	hFile = _findfirst(filter, &c_file);
	if (hFile != -1)
	{
		do{
			sprintf_s(path, 256,"%s\\%s", sdir, c_file.name);//sdir目錄+c_file.name目錄下的檔案名字
			printf("%s\n", path);
		} while (_findnext(hFile, &c_file) == 0);
	}
	system("pause");
}