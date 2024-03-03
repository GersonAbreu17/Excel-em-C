#include <stdio.h>
#include <conio2.h>
#include <ctype.h>
#include <stdlib.h>
#include <windows.h>
#include <string.h>

struct tpPilha
{
	int valor;
	tpPilha *prox;
};

tpPilha *novaPi(int valor)
{
	tpPilha *nc = (tpPilha *)malloc(sizeof(tpPilha));
	nc -> prox = NULL;
	nc -> valor = valor;
	return nc;
}

void push(tpPilha **pi, int valor)
{
	tpPilha *nc = novaPi(valor);
	nc -> prox = *pi;
	*pi = nc;
}

int top(tpPilha *pi)
{
	return pi -> valor;
}

int pop(tpPilha **pi)
{
	int i = (*pi) -> valor;
	tpPilha *aux = *pi;
	*pi = (*pi) -> prox;
	free(aux);
	return i;
}

char vazia(tpPilha *pi)
{
	return pi == NULL;
}

void exibir(tpPilha *pi)
{
	while(pi != NULL)
	{
		printf("  %d",pi -> valor);
		pi = pi -> prox;
	}
	printf("\n\n");
}

int main()
{
	tpPilha *pi = NULL;
	char op;
	int valor;
	do
	{
		clrscr();
		printf("*** Teste de Pilha ***\n\n");
		exibir(pi);
		printf("  [1] Push\n");
		printf("  [2] Pop\n");
		printf("  [3] Top\n");
		printf("  [4] Vazia\n");
		op = toupper(getch());
		switch(op)
		{
			case '1':
				clrscr();
				printf("*** Push ***\n\n  Valor: ");
				scanf("%d",&valor);
				push(&pi, valor);
				break;
			case '2':
				clrscr();
				printf("*** Pop ***\n\n  Pop: %d\n",pop(&pi));
				getch();
				break;
			case '3':
				clrscr();
				printf("*** Top ***\n\n  Top: %d\n",top(pi));
				getch();
				break;
			case '4':
				clrscr();
				printf("*** Vazia ***\n\n  Vazia: ");
				if(vazia(pi))
					printf("true");
				else
					printf("false");
				getch();
		}
	} while(op != 27);
	
}
