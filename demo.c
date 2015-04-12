#include <stdio.h>
#include <string.h>

int main()
{
	char str[100];
	printf("Nhap: ");fgets(str,100,stdin);str[strlen(str)-1]='\0';
	if (str[0] == '\0')
		printf("1\n");
}