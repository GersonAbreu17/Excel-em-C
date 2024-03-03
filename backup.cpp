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

tpLinha *inserirLinha(tpLinha **inicio, int linha)
{
    tpLinha *p, *nc;
    if(*inicio == NULL)
    {
        *inicio = novaLinha(linha);
        p = *inicio;
    }
    else if((*inicio) -> linha == linha)
    {
        p = *inicio;
    }
    else if((*inicio) -> linha > linha)
    {
        nc = novaLinha(linha);
        nc -> prox = *inicio;
        *inicio = nc;
        p = nc;
    }
    else
    {
        p = *inicio;
        while(p -> prox != NULL && p -> prox -> linha < linha)
            p = p -> prox;
        
        if(p -> prox == NULL || p -> prox -> linha != linha)
        {
            nc = novaLinha(linha);
            nc -> prox = p -> prox;
            p -> prox = nc;
        }
        p = p -> prox;
    }
    return p;
}

tpColuna *inserirColuna(tpColuna **inicio, char coluna)
{
    tpColuna *p, *nc;
    if(*inicio == NULL)
    {
        *inicio = novaColuna(coluna);
        p = *inicio;
    }
    else if((*inicio) -> coluna == coluna)
    {
        p = *inicio;
    }
    else if((*inicio) -> coluna > coluna)
    {
        nc = novaColuna(coluna);
        nc -> prox = *inicio;
        *inicio = nc;
        p = nc;
    }
    else
    {
        p = *inicio;
        while(p -> prox != NULL && p -> prox -> coluna < coluna)
            p = p -> prox;
        
        if(p -> prox == NULL || p -> prox -> coluna != coluna)
        {
            nc = novaColuna(coluna);
            nc -> prox = p -> prox;
            p -> prox = nc;
        }
        p = p -> prox;
    }
    return p;
}

tpCaixa *inserirCaixa(tpCaixa **inicio, int linha, char coluna, char valor[50])
{
    tpCaixa *p, *nc;
    if(*inicio == NULL)
    {
        nc = novaCaixa(linha, coluna, valor);
        *inicio = nc;
    }
    else if((*inicio) -> coluna == coluna)
    {
        strcpy((*inicio) -> valor, valor);
        nc = *inicio;
    }
    else if((*inicio) -> coluna > coluna)
    {
        nc = novaCaixa(linha, coluna, valor);
        nc -> proxColuna = *inicio;
        *inicio = nc;
    }
    else
    {
        p = *inicio;
        while(p -> proxColuna != NULL && p -> proxColuna -> coluna < coluna)
            p = p -> proxColuna;
        
        if(p -> proxColuna == NULL || p -> proxColuna -> coluna != coluna)
        {
            nc = novaCaixa(linha, coluna, valor);
            nc -> proxColuna = p -> proxColuna;
            p -> proxColuna = nc;
        }
        else
        {
            nc = p -> proxColuna;
        }
    }
    return nc;
}

void inserirCaixaColuna(tpCaixa **inicio, tpCaixa *cx, int linha)
{
    tpCaixa *p;
    if(*inicio == NULL)
    {
        *inicio = cx;
    }
    else if((*inicio) -> linha > linha)
    {
        cx -> proxLinha = *inicio;
        *inicio = cx;
    }
    else
    {
        p = *inicio;
        while(p -> proxLinha != NULL && p -> proxLinha -> linha < linha)
            p = p -> proxLinha;
            
        cx -> proxLinha = p -> proxLinha;
        p -> proxLinha = cx;
    }
}

void inserir(tpDesc *desc, int linha, char coluna, char valor[50])
{
    tpLinha *l;
    tpColuna *c;
    tpCaixa *cx;
    
    l = inserirLinha(&(*desc).linha, linha);
    c = inserirColuna(&(*desc).coluna, coluna);
    cx = inserirCaixa(&l -> caixa, linha, coluna, valor);
    inserirCaixaColuna(&c -> caixa, cx, linha);
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
		while(c != NULL && c -> coluna != coluna)
			c = c -> proxColuna;
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

























