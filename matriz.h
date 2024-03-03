#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <conio2.h>

struct tpRegistro
{
	int linha, coluna;
	char valor[50];
};
typedef struct tpRegistro tpRegistro;

struct tpCaixa
{
    int linha;
    char coluna, valor[50];
    struct tpCaixa *proxLinha, *proxColuna;
};
typedef struct tpCaixa tpCaixa;

struct tpColuna
{
    struct tpColuna *prox;
    char coluna;
    tpCaixa *caixa;
};
typedef struct tpColuna tpColuna;

struct tpLinha
{
    struct tpLinha *prox;
    int linha;
    tpCaixa *caixa;
};
typedef struct tpLinha tpLinha;

struct tpDesc
{
    tpLinha *linha;
    tpColuna *coluna;
};
typedef struct tpDesc tpDesc;

tpCaixa *novaCaixa(int linha, char coluna, char valor[50])
{
    tpCaixa *nc = (tpCaixa *)malloc(sizeof(tpCaixa));
    nc -> linha = linha;
    nc -> coluna = coluna;
    nc -> proxLinha = NULL;
    nc -> proxColuna = NULL;
    strcpy(nc -> valor, valor);
    return nc;
}

tpColuna *novaColuna(char coluna)
{
	tpColuna *nc = (tpColuna *)malloc(sizeof(tpColuna));
	nc -> coluna = coluna;
	nc -> prox = NULL;
	nc -> caixa = NULL;
	return nc;
}

tpLinha *novaLinha(int linha)
{
	tpLinha *nc = (tpLinha *)malloc(sizeof(tpLinha));
	nc -> linha = linha;
	nc -> prox = NULL;
	nc -> caixa = NULL;
	return nc;
}

void inicializar(tpDesc *desc)
{
	(*desc).linha = NULL;
	(*desc).coluna = NULL;
}

void inserir(tpDesc *desc, int linha, char coluna, char valor[50])
{
    tpLinha *l, *lAnt;
    tpColuna *c, *cAnt;
    tpCaixa *cx, *cxAnt, *cx2;
    
    l = (*desc).linha;
    lAnt = NULL;
    while(l != NULL && l -> linha < linha)
    {
    	lAnt = l;
    	l = l -> prox;
    }
    if(l == NULL || l -> linha != linha)
    {
		l = novaLinha(linha);
		if(lAnt == NULL)
		{
			l -> prox = (*desc).linha;
			(*desc).linha = l;
		}
		else
		{
			l -> prox = lAnt -> prox;
			lAnt -> prox = l;
		}
	}
    
    cx = l -> caixa;
    cxAnt = NULL;
    while(cx != NULL && cx -> coluna < coluna)
    {
    	cxAnt = cx;
    	cx = cx -> proxColuna;
    }
    if(cx != NULL && cx -> coluna == coluna)
    {
		strcpy(cx -> valor, valor);
    }
    else
    {
		cx = novaCaixa(linha, coluna, valor);
	    if(cxAnt == NULL)
	    {
	    	cx -> proxColuna = l -> caixa;
	    	l -> caixa = cx;
		}
		else
		{
	    	cx -> proxColuna = cxAnt -> proxColuna;
	    	cxAnt -> proxColuna = cx;
		}
		
		c = (*desc).coluna;
		cAnt = NULL;
		while(c != NULL && c -> coluna < coluna)
		{
			cAnt = c;
			c = c -> prox;
		}
		if(c == NULL || c -> coluna != coluna)
		{
			c = novaColuna(coluna);
			if(cAnt == NULL)
			{
				c -> prox = (*desc).coluna;
				(*desc).coluna = c;
			}
			else
			{
				c -> prox = cAnt -> prox;
				cAnt -> prox = c;
			}
		}
		
		cx2 = cx;
		cx = c -> caixa;
		cxAnt = NULL;
		while(cx != NULL && cx -> linha < linha)
		{
			cxAnt = cx;
			cx = cx -> proxLinha;
		}
		cx2 -> proxLinha = cx;
		if(cxAnt == NULL)
			c -> caixa = cx2;
		else
			cxAnt -> proxLinha = cx2;
	}
}

tpCaixa *buscar(tpDesc desc, int linha, char coluna)
{
	tpLinha *l = desc.linha;
	tpCaixa *c = NULL;
	while(l != NULL && l -> linha != linha)
		l = l -> prox;
	if(l != NULL)
	{
		c = l -> caixa;
		while(c != NULL && c -> coluna < coluna)
			c = c -> proxColuna;
		if(c != NULL && c -> coluna != coluna)
			c = NULL;
	}
	return c;
}

void excluir(tpDesc *desc, int linha, char coluna)
{
	tpLinha *l, *lAnt;
	tpColuna *c, *cAnt;
	tpCaixa *cx, *cxAnt;
	
	if(l != NULL)
	{
		//PROCURANDO LINHA
		l = (*desc).linha;
		lAnt = NULL;
		while(l != NULL && l -> linha != linha)
		{
			lAnt = l;
			l = l -> prox;
		}
		if(l != NULL)
		{
			//DESLIGANDO DA LINHA
			cxAnt = NULL;
			cx = l -> caixa;
			while(cx != NULL && cx -> coluna != coluna)
			{
				cxAnt = cx;
				cx = cx -> proxColuna;
			}
			if(cx != NULL)
			{
				if(cxAnt == NULL)
				{
					l -> caixa = cx -> proxColuna;
				}
				else
				{
					cxAnt -> proxColuna = cx -> proxColuna;
				}
				//PROCURANDO COLUNA
				cAnt = NULL;
				c = (*desc).coluna;
				while(c != NULL && c -> coluna != coluna)
				{
					cAnt = c;
					c = c -> prox;
				}
				if(c != NULL)
				{
					//DESLIGANDO DA COLUNA
					cx = c -> caixa;
					cxAnt = NULL;
					while(cx != NULL && cx -> linha != linha)
					{
						cxAnt = cx;
						cx = cx -> proxLinha;
					}
					if(cxAnt == NULL)
					{
						c -> caixa = cx -> proxLinha;
					}
					else
					{
						cxAnt -> proxLinha = cx -> proxLinha;
					}
					free(cx);
					//APAGANDO LINHA
					if(l -> caixa == NULL)
					{
						if(lAnt == NULL)
						{
							(*desc).linha = l -> prox;
						}
						else
						{
							lAnt -> prox = l -> prox;
						}
						free(l);
					}
					//APAGANDO COLUNA
					if(c -> caixa == NULL)
					{
						if(cAnt == NULL)
						{
							(*desc).coluna = c -> prox;
						}
						else
						{
							cAnt -> prox = c -> prox;
						}
						free(c);
					}
				}
			}
		}
	}
}

char salvar(tpDesc desc, char nome[50])
{
	tpRegistro reg;
	tpLinha *l;
	tpCaixa *cx;
	FILE *ptr = fopen(nome, "wb");
	if(ptr != NULL)
	{
		l = desc.linha;
		while(l != NULL)
		{
			cx = l -> caixa;
			while(cx != NULL)
			{
				reg.linha = cx -> linha;
				reg.coluna = cx -> coluna;
				strcpy(reg.valor, cx -> valor);
				fwrite(&reg,sizeof(reg),1,ptr);
				
				cx = cx -> proxColuna;
			}
			
			l = l -> prox;
		}
		fclose(ptr);
	}
	else
		return 0;
	return 1;
}

void limparMatriz(tpDesc *desc)
{
	tpLinha *l, *lAnt;
	tpColuna *c, *cAnt;
	tpCaixa *cx, *cxAnt;
	l = (*desc).linha;
	while(l != NULL)
	{
		cx = l -> caixa;
		while(cx != NULL)
		{
			cxAnt = cx;
			cx = cx -> proxColuna;
			free(cxAnt);
		}
		
		lAnt = l;
		l = l -> prox;
		free(lAnt);
	}
	c = (*desc).coluna;
	while(c != NULL)
	{
		cAnt = c;
		c = c -> prox;
		free(cAnt);
	}
	(*desc).linha = NULL;
	(*desc).coluna = NULL;
}

char carregar(tpDesc *desc, char nome[50])
{
	tpRegistro reg;
	FILE *ptr = fopen(nome, "rb");
	if(ptr != NULL)
	{
		limparMatriz(desc);
		fread(&reg,sizeof(reg),1,ptr);
		while(!feof(ptr))
		{
			inserir(desc, reg.linha, reg.coluna, reg.valor);
			
			fread(&reg,sizeof(reg),1,ptr);
		}
		fclose(ptr);
	}
	else
		return 0;
	return 1;
}

























