#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <conio2.h>

#include "funcoes.h"
#include "matriz.h"
#include "formula.h"

void infoEsq(int tLinha)
{
	textbackground(3);
	for(int i=0;i<25;i++)
	{
		gotoxy(1,i+5);
		printf("%03d",i+tLinha);
	}
	textbackground(0);
}

void infoCima(int tColuna)
{
	textbackground(3);
	gotoxy(4,4);
	for(int i=0;i<9;i++)
	{
		printf("      %c      ",'A'+tColuna-1+i);
	}
	textbackground(0);
}

void irParaCelula(int tLinha, int tColuna, int cLinha, int cColuna)
{
	gotoxy(4+(cColuna-tColuna)*13,5+cLinha-tLinha);
}

void limpar()
{
	gotoxy(3,2);
	printf("                                                          ");
	for(int i=0;i<25;i++)
	{
		gotoxy(4,5+i);
		printf("                                                                                                                     ");
	}
	
}

void infoCelula(tpCaixa *c, tpDesc matriz)
{
	int i = 0;
	char aux[50];
	strcpy(aux, c -> valor);
	if(aux[0] == '=')
	{
		snprintf(aux, 50, "%.2f", resolverExpressao(aux, c -> linha, c -> coluna, matriz));
		i = strlen(aux)-1;
		while(i >= 0 && aux[i] == '0')
			i--;
		if(aux[i] == '.')
			aux[i] = '\0';
		else
			aux[i+1] = '\0';
	}
	else
	{
		if(strlen(aux) > 11)
		{
			aux[10] = '.';
			aux[11] = '\0';
		}
	}
	printf(" %s", aux);
}

void mostrarCelulas(int tLinha, int tColuna, tpDesc matriz)
{
	tpLinha *l = matriz.linha;
	tpCaixa *c;
	while(l != NULL && l -> linha < tLinha)
		l = l -> prox;
	while(l != NULL && l -> linha < tLinha+25)
	{
		c = l -> caixa;
		while(c != NULL && c -> coluna-64 < tColuna)
			c = c -> proxColuna;
		while(c != NULL && c -> coluna-64 < tColuna+9)
		{
			irParaCelula(tLinha, tColuna, c -> linha, c -> coluna-64);
			infoCelula(c, matriz);
			c = c -> proxColuna;
		}
		
		l = l -> prox;
	}
}

void desenhaCursor(int tLinha, int tColuna, int cLinha, int cColuna, tpDesc matriz)
{
	tpCaixa *c;
	textbackground(3);
	irParaCelula(tLinha, tColuna, cLinha, cColuna);
	printf("             ");
	irParaCelula(tLinha, tColuna, cLinha, cColuna);
	c = buscar(matriz, cLinha, cColuna+64);
	if(c != NULL)
		infoCelula(c, matriz);
	
	textbackground(0);
	gotoxy(3,2);
	printf("                                                          ");
	gotoxy(3,2);
	if(c != NULL)
		printf(" %s", c -> valor);
}

void atualizarTela(int tLinha, int tColuna, int cLinha, int cColuna, tpDesc matriz)
{
	infoEsq(tLinha);
	infoCima(tColuna);
	limpar();
	mostrarCelulas(tLinha, tColuna, matriz);
	desenhaCursor(tLinha, tColuna, cLinha, cColuna, matriz);
}

void visual()
{
	textbackground(3);
	gotoxy(4,4);
	printf("                                                                                                                     ");
	for(int i=0;i<26;i++)
	{
		gotoxy(1,i+4);
		printf("   ");
	}
	textbackground(0);
}

void concertarString(char str[50])
{
	int i = 0, j = 0;
	if(isFormula(str))
	{
		while(i < strlen(str))
		{
			if(str[i] != ' ')
				str[j++] = toupper(str[i]);
			i++;
		}
		str[j] = '\0';
	}
	else
	{
		while(i < strlen(str) && str[i] == ' ')
			i++;
		while(i < strlen(str))
		{
			str[j++] = str[i++];
		}
		i = strlen(str)-1;
		while(i >= 0 && str[i] == ' ')
			i--;
		str[i+1] = '\0';
	}
}

void iniciarExcel()
{
	tpDesc matriz;
	int tLinha=1, tColuna=1, cLinha=1, cColuna=1;
	char valor[50];
	inicializar(&matriz);
	visual();
	
	char op;
	do
	{
		atualizarTela(tLinha, tColuna, cLinha, cColuna, matriz);
		
		gotoxy(120,30);
		op = toupper(getch());
		if(op == -32)
		{
			op = toupper(getch());
			switch(op)
			{
				//CIMA
				case 72:
					if(cLinha > 1)
					{
						cLinha--;
						if(cLinha < tLinha)
							tLinha--;
					}
					break;
				//BAIXO
				case 80:
					if(cLinha < 100)
					{
						cLinha++;
						if(cLinha > tLinha+24)
							tLinha++;
					}
					break;
				//ESQUERDA
				case 75:
					if(cColuna+64 > 'A')
					{
						cColuna--;
						if(cColuna < tColuna)
							tColuna--;
					}
					break;
				//DIREITA
				case 77:
					if(cColuna+64 < 'Z')
					{
						cColuna++;
						if(cColuna > tColuna+8)
							tColuna++;
					}
					break;
			}
		}
		else if(op == 0)
		{
			op = toupper(getch());
			switch(op)
			{
				//F1 Limpar
				case 59:
					limparMatriz(&matriz);
					tLinha = 1;
					cLinha = 1;
					tColuna = 1;
					cColuna = 1;
					break;
				//F2 Editar
				case 60:
					gotoxy(3,2);
					printf("                                                          ");
					gotoxy(4,2);
					fflush(stdin);
					gets(valor);
					concertarString(valor);
					if(strlen(valor) > 0)
					{
						if(valor[0] == '=' && isCircular(cLinha, cColuna+64, valor, matriz))
							strcpy(valor, "ERRO!");
						inserir(&matriz, cLinha, cColuna+64, valor);
					}
					break;
				//F3 Salvar
				case 61:
					gotoxy(3,2);
					printf("                                                          ");
					gotoxy(4,2);
					fflush(stdin);
					textcolor(2);
					printf("Salvar: ");
					textcolor(15);
					gets(valor);
					if(strlen(valor) > 0)
						salvar(matriz, valor);
					break;
				//F4 Carregar
				case 62:
					gotoxy(3,2);
					printf("                                                          ");
					gotoxy(4,2);
					fflush(stdin);
					textcolor(2);
					printf("Carregar: ");
					textcolor(15);
					gets(valor);
					if(strlen(valor) > 0)
					{
						if(carregar(&matriz, valor))
						{
							tLinha = 1;
							cLinha = 1;
							tColuna = 1;
							cColuna = 1;
						}
					}
					break;
			}
		}
		else if(op == 8)
		{
			excluir(&matriz, cLinha, cColuna+64);
		}
	} while(op != 27);
}

int main ()
{
	iniciarExcel();
	return 0;
}



// 117
