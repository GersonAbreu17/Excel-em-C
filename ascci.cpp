#include <stdio.h>
#include <ctype.h>
#include <conio2.h>

int main()
{
	char op;
	do
	{
		op = toupper(getch());
		printf("%c - %d\n",op, op);
	} while(op != 27);
}
