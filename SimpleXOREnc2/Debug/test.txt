#include <stdlib.h>
#include <stdio.h>
#include <string.h>


void XorOperation(char *SourceFile, int FileSize, unsigned char *Key, int KeySize)
{
	FILE *in_fp, *out_fp;
	errno_t err;
	unsigned char *s_buffer, *d_buffer;
	int index = 0;
	int k;
	err = fopen_s(&in_fp, SourceFile, "rb");
	if (err != 0)
	{
		printf("source file open error\n");
		exit(1);
	}

	err = fopen_s(&out_fp, "result.txt", "wb");
	if (err != 0)
	{
		printf("destination file open error\n");
		exit(1);
	}

	
	s_buffer = (unsigned char *)malloc(sizeof(unsigned char)*FileSize);
	d_buffer = (unsigned char *)malloc(sizeof(unsigned char)*FileSize);

	fread_s(s_buffer, FileSize, sizeof(unsigned char), FileSize, in_fp);
	
	while (index < FileSize)
	{
		k = Key[index%KeySize];
		d_buffer[index] = s_buffer[index] ^ k;
		index++;
	}
	fwrite(d_buffer, sizeof(unsigned char), FileSize, out_fp);

	fclose(in_fp);
	fclose(out_fp);
	free(s_buffer);
	free(d_buffer);
}

void ConvertKey(char *source, unsigned char *destination)
{
	int i;
	int length;
	
	length = strlen(source);
	printf("key length= %d\n", length);
	for (i = 0; i < length - 1; i++)
		destination[i] = source[i];
}

int main(int argc, char *argv[])
{
	unsigned char Key[20];
	FILE *fp;
	errno_t err;
	char SourceFile[20], DestinationFile[20],temp[20];
	int size;

	if (argc!=4)
	{
		printf("Usage: exe source_file destination_file key\n");
		exit(1);
	}
	strcpy_s(SourceFile, argv[1]);
	strcpy_s(DestinationFile, argv[2]);
	strcpy_s(temp,argv[3]);
	ConvertKey(temp,Key);
	err = fopen_s(&fp, SourceFile, "r");
	if (err != 0)
	{
		printf("source file open error\n");
		exit(1);
	}
	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	fclose(fp);

	XorOperation(SourceFile,size,Key,10);
	system("pause");
}