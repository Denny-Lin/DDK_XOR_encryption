#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <io.h> //c_file
#include <direct.h> 
#include<iomanip>
#include<windows.h>
#include<iostream>
using namespace std;
struct File{
	char FileName[20];
	int Size;
	unsigned char *OriginalContent;
	unsigned char *NewContent;
};

void XorOperation(struct File *file, unsigned char *Key,unsigned int KeySize)
{
	FILE *out_fp;
	errno_t err;
	int index = 0;
	unsigned char k;
	char NewFileName[30];
	char seps[] = ".";
	char *token1 = NULL;
	char *token2 = NULL;
	char *next_token1 = NULL;
	char temp[20];

	strcpy_s(temp, file->FileName);
	token1 = strtok_s(file->FileName, seps, &next_token1);
	token2 = strtok_s(NULL, seps, &next_token1);

	sprintf_s(NewFileName, "%s_enc.%s", token1,token2);
	//printf("%s\n",NewFileName);
	err = fopen_s(&out_fp, NewFileName, "wb");
	if (err != 0)
	{
		printf("destination file open error\n");
		exit(1);
	}

	//printf("%s", file->OriginalContent);
	while (index < file->Size)
	{
		k = Key[index%KeySize];
		file->NewContent[index] = file->OriginalContent[index] ^ k;
		index++;
	}
	fwrite(file->NewContent, sizeof(unsigned char), file->Size, out_fp);
	//fwrite(file->OriginalContent, sizeof(unsigned char), file->Size, out_fp);

	free(file->OriginalContent);
	free(file->NewContent);
	fclose(out_fp);
}

unsigned int ConvertKey(char *source, unsigned char *destination)
{
	int i;
	unsigned int length;
	
	length = strlen(source);
	//printf("key length= %d\n", length);
	for (i = 0; i < length - 1; i++)
		destination[i] = source[i];
	return length;
}

void GetFileContent(char *filename, struct File *file)
{
	FILE *fp;
	errno_t err;
	err = fopen_s(&fp, filename, "rb");
	if (err != 0)
	{
		printf("source file open error\n");
		exit(1);
	}
	strcpy_s(file->FileName, filename);
	fseek(fp, 0, SEEK_END);
	file->Size = ftell(fp);
	fclose(fp);
	file->OriginalContent = (unsigned char *)malloc(sizeof(unsigned char)*file->Size);
	file->NewContent = (unsigned char *)malloc(sizeof(unsigned char)*file->Size);
	err = fopen_s(&fp, filename, "rb");
	if (err != 0)
	{
		printf("source file open error\n");
		exit(1);
	}
	int length=fread_s(file->OriginalContent, file->Size, sizeof(unsigned char), file->Size, fp);
	//file->Size = length;
	//printf("length=%d\n",length);
	fclose(fp);
}

int main(int argc, char *argv[])
{
	char sdir[256], filter[256];//path[256];
	struct _finddata_t c_file;
	long hFile;

	unsigned char Key[20];
	struct File *file;
	char SourceFile[20]="",temp[20]="";
	unsigned long int KeySize;

	if (argc != 3) {
		printf("Usage: exe source_file key\n");
		exit(1);
	}

	LARGE_INTEGER startTime, endTime, fre;
	long double times;
	QueryPerformanceFrequency(&fre); //取得CPU頻率
	QueryPerformanceCounter(&startTime); //取得開機到現在經過幾個CPU Cycle
	//doing something
	for (int i = 0; i<1000000000; i++);
//------------------------------------------------------------------------------	
	strcpy_s(sdir, 256, argv[1]);//printf("(%s)\n",sdir);
	
	strcpy_s(filter, 256, "*.txt");//strcpy_s(filter, 256, "*");
	
	_chdir(sdir);
	hFile = _findfirst(filter, &c_file);
	if (hFile != -1)
	{
		do {
			//sprintf_s(path, 256, "%s\\%s", argv[1], c_file.name);//sdir目錄+c_file.name目錄下的檔案名字
			//printf("%s\n", path);
	        strcpy_s(SourceFile, c_file.name);
	        strcpy_s(temp,argv[2]);	

	        file = (struct File *)malloc(sizeof(File));
	        GetFileContent(SourceFile,file);
	        KeySize=ConvertKey(temp,Key);
 
	        XorOperation(file,Key,KeySize);
		} while (_findnext(hFile, &c_file) == 0);
	}
//------------------------------------------------------------------------------
	QueryPerformanceCounter(&endTime); //取得開機到程式執行完成經過幾個CPU Cycle
	times = ((double)endTime.QuadPart - (double)startTime.QuadPart) / fre.QuadPart;
	cout << fixed << setprecision(20) << times << 's' << endl;
	system("pause");
	return 0;
}