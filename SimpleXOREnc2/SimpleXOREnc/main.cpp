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

void XorOperation(struct File *file, unsigned char *Key, int KeySize)
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
void OutputFile(struct File *file) {
	FILE *out_fp;
	errno_t err;
	char NewFileName[30];

	err = fopen_s(&out_fp, NewFileName, "wb");
	if (err != 0)
	{
		printf("destination file open error\n");
		exit(1);
	}

	fwrite(file->NewContent, sizeof(unsigned char), file->Size, out_fp);
	//fwrite(file->OriginalContent, sizeof(unsigned char), file->Size, out_fp);

	free(file->OriginalContent);
	free(file->NewContent);
	fclose(out_fp);
}

int ConvertKey(char *source, unsigned char *destination)
{
	int i;
	int length;
	
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
	unsigned int file_num = 0;//紀錄讀了幾筆資料
	char sdir[256], filter[256];//絕對路徑argv[1] 要被開啟檔案們的目錄,檔案格式
	struct _finddata_t c_file[3000];//用來放開啟 filter[256]的檔案資料
	long hFile;

	struct File *file[3000];//檔名SourceFile 和檔案內容 及其它
	char SourceFile[256] = "";// SourceFile=c_file[j].name用來儲存單筆檔案的名字
	unsigned char Key[20];//金鑰= temp[20]
	char temp[20] = "";//temp=加密文字argv[2]
	unsigned long int KeySize;//加密文字總長度 temp[20]裡用了多少長度	
	
	LARGE_INTEGER startTime, endTime, fre;//開始時間,結束時間,
	long double times;//計算所花的時間

	if (argc != 3) {
		printf("Usage: exe source_file key\n");
		exit(1);
	}

	strcpy_s(sdir, 256, argv[1]);//用輸入的方式讀入要被開啟的檔案目錄的路徑
	strcpy_s(filter, 256, "*.txt");//目前目錄下的要被開啟的檔案格式,這裡是相對路徑 ex:有.txt的會被開啟
	//讀入所有檔案
	_chdir(sdir);
	hFile = _findfirst(filter, &c_file[0]);//(檔案格式,放檔案的資料)
	if (hFile != -1)
	{	
		do {
			file_num++;
		} while (_findnext(hFile, &c_file[file_num]) == 0);
	}	

	//計算時間,這裡是開始時間初始化
	QueryPerformanceFrequency(&fre); //取得CPU頻率
	QueryPerformanceCounter(&startTime); //取得開機到現在經過幾個CPU Cycle

	for (int i = 0; i<1000000000; i++);//時間如果太短會測不到,給個緩衝時間,包含在執行時間裡
	//---------------------------------------------------------------------------------------------------
	strcpy_s(temp, argv[2]);//下指令時的key 暫時存放用 真正的是變數Key
	for (int j = 0; j < file_num; j++) {
		strcpy_s(SourceFile, c_file[j].name);//檔名	
        //printf("%d\n", j);
		file[j] = (struct File *)malloc(sizeof(File));
		GetFileContent(SourceFile, file[j]);//file初始化 把單筆檔名傳入 讀檔 存入struct file裡		
	}
	KeySize = ConvertKey(temp, Key);//temp=Key 順便計算大小
	for (int j = 0; j < file_num; j++) {
		XorOperation(file[j], Key, KeySize);//加密 在做一次就是解密
	}
	//---------------------------------------------------------------------------------------------------
    //計算時間,這裡是完成時間
	QueryPerformanceCounter(&endTime); //取得開機到程式執行完成經過幾個CPU Cycle
	times = ((double)endTime.QuadPart - (double)startTime.QuadPart) / fre.QuadPart;
	cout << fixed << setprecision(20) << times << 's' << endl;
	
	system("pause");
	return 0;
}