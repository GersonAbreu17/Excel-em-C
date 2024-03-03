#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char isNumero(char str[50])
{
	int i=0;
	while(i < strlen(str) && (str[i] >= '0' && str[i] <= '9' || str[i] == '.'))
		i++;
	return i == strlen(str);
}

char isFormula(char str[50])
{
	int i=0;
	while(i < strlen(str) && str[i] == ' ')
		i++;
	return str[i] == '=';
}

int min(int v1, int v2)
{
	return (v1 < v2) ? v1 : v2;
}

int max(int v1, int v2)
{
	return (v1 > v2) ? v1 : v2;
}
