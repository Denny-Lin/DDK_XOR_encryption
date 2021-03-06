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
	unsigned int file_num = 0;//???????F?X??????
	char sdir[256], filter[256];//???????|argv[1] ?n?Q?}??????????????,????????
	struct _finddata_t c_file[3000];//???????}?? filter[256]??????????
	long hFile;

	struct File *file[3000];//???WSourceFile ?M???????e ??????
	char SourceFile[256] = "";// SourceFile=c_file[j].name?????x?s???????????W?r
	unsigned char Key[20];//???_= temp[20]
	char temp[20] = "";//temp=?[?K???rargv[2]
	unsigned long int KeySize;//?[?K???r?`???? temp[20]?????F?h??????	
	
	LARGE_INTEGER startTime, endTime, fre;//?}?l????,????????,
	long double times;//?p????????????

	if (argc != 3) {
		printf("Usage: exe source_file key\n");
		exit(1);
	}

	strcpy_s(sdir, 256, argv[1]);//?????J?????????J?n?Q?}?????????????????|
	strcpy_s(filter, 256, "*.txt");//???e?????U???n?Q?}????????????,?o???O???????| ex:??.txt???|?Q?}??
	//???J????????
	_chdir(sdir);
	hFile = _findfirst(filter, &c_file[0]);//(????????,????????????)
	if (hFile != -1)
	{	
		do {
			file_num++;
		} while (_findnext(hFile, &c_file[file_num]) == 0);
	}	

	//?p??????,?o???O?}?l???????l??
	QueryPerformanceFrequency(&fre); //???oCPU?W?v
	QueryPerformanceCounter(&startTime); //???o?}?????{?b?g?L?X??CPU Cycle

	for (int i = 0; i<1000000000; i++);//?????p?G???u?|??????,?????w??????,?]?t?b??????????
	//---------------------------------------------------------------------------------------------------
	strcpy_s(temp, argv[2]);//?U???O????key ?????s???? ?u?????O????Key
	for (int j = 0; j < file_num; j++) {
		strcpy_s(SourceFile, c_file[j].name);//???W	
        //printf("%d\n", j);
		file[j] = (struct File *)malloc(sizeof(File));
		GetFileContent(SourceFile, file[j]);//file???l?? ?????????W???J ???? ?s?Jstruct file??		
	}
	KeySize = ConvertKey(temp, Key);//temp=Key ???K?p???j?p
	for (int j = 0; j < file_num; j++) {
		XorOperation(file[j], Key, KeySize);//?[?K ?b???@???N?O???K
	}
	//---------------------------------------------------------------------------------------------------
    //?p??????,?o???O????????
	QueryPerformanceCounter(&endTime); //???o?}?????{???????????g?L?X??CPU Cycle
	times = ((double)endTime.QuadPart - (double)startTime.QuadPart) / fre.QuadPart;
	cout << fixed << setprecision(20) << times << 's' << endl;
	
	system("pause");
	return 0;
}