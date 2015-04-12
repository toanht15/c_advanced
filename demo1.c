#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>


/* reads from keypress, doesn't echo */
int getch(void)
{
    struct termios oldattr, newattr;
    int ch;
    tcgetattr( STDIN_FILENO, &oldattr );
    newattr = oldattr;
    newattr.c_lflag &= ~( ICANON | ECHO );
    tcsetattr( STDIN_FILENO, TCSANOW, &newattr );
    ch = getchar();
    tcsetattr( STDIN_FILENO, TCSANOW, &oldattr );
    return ch;
}
void nhap_xau(char str[])
{
	int i=0;
	do{
		str[i] = getch();
		printf("%c", str[i]);		
		i++;
	}while(str[i--] != '\t' || str[i--] != 13);
	str[i-1] = '\0';
}


void main()
{
	int mang[] = {1,2,3,4,5};
	char 	str[100];

	nhap_xau(str);

	printf("this string =  %s\n",str );
		
}