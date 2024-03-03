#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// LISTA GENERALIZADA

union tpValor;
struct tpListaGen;

union tpValor
{
	double decimal;
	char operador;
	struct tpListaGen *lista;
};
typedef union tpValor tpValor;

struct tpListaGen
{
	union tpValor valor;
	char tipo;
	struct tpListaGen *prox;
};
typedef struct tpListaGen tpListaGen;

struct tpPilha
{
	struct tpPilha *prox;
	tpListaGen *lista;
};
typedef struct tpPilha tpPilha;

struct tpFila
{
	struct tpFila *prox;
	char str[50];
};
typedef struct tpFila tpFila;

// PILHA

tpPilha *novaPilha(tpListaGen *lista)
{
	tpPilha *nc = (tpPilha *) malloc(sizeof(tpPilha));
	nc -> lista = lista;
	return nc;
}

void inserirPilha(tpPilha **pi, tpListaGen *lista)
{
	tpPilha *nc = novaPilha(lista);
	nc -> prox = *pi;
	*pi = nc;
}

char vaziaPilha(tpPilha *pi)
{
	return pi == NULL;
}

tpListaGen *retirarPilha(tpPilha **pi)
{
	tpPilha *aux;
	tpListaGen *ret = NULL;
	if(!vaziaPilha(*pi))
	{
		aux = *pi;
		ret = aux -> lista;
		*pi = (*pi) -> prox;
		free(aux);
	}
	return ret;
}

tpListaGen *topPilha(tpPilha *pi)
{
	return pi -> lista;
}

// FILA

tpFila *novaFila(char str[50])
{
	tpFila *nc = (tpFila *) malloc(sizeof(tpFila));
	strcpy(nc -> str, str);
	nc -> prox = NULL;
	return nc;
}

void inserirFila(tpFila **fila, char str[50])
{
	tpFila *p;
	p = *fila;
	if(p == NULL)
	{
		*fila = novaFila(str);
	}
	else
	{
		while(p -> prox != NULL)
			p = p -> prox;
		p -> prox = novaFila(str);
	}
}

char vaziaFila(tpFila *fila)
{
	return fila == NULL;
}

void topFila(tpFila *fila, char str[50])
{
	strcpy(str, fila -> str);
}

void retirarFila(tpFila **fila)
{
	if(!vaziaFila(*fila))
	{
		tpFila *aux = *fila;
		*fila = (*fila) -> prox;
		free(aux);
	}
}

// LISTA


tpListaGen *novaLista(tpValor valor, char tipo)
{
	tpListaGen *nc = (tpListaGen *) malloc(sizeof(tpListaGen));
	nc -> tipo = tipo;
	nc -> valor = valor;
	nc -> prox = NULL;
	return nc;
}

tpListaGen *inserirLista(tpListaGen **lista, tpValor valor, char tipo)
{
	tpListaGen *p;
	if(*lista == NULL)
	{
		*lista = novaLista(valor, tipo);
		p = *lista;
	}
	else
	{
		p = *lista;
		while(p -> prox != NULL)
			p = p -> prox;
		p -> prox = novaLista(valor, tipo);
		p = p -> prox;
	}
	return p;
}

// FUNCOES

double resolverExpressao(char str[50], int cLinha, char cColuna, tpDesc desc);
void gerarFila(tpFila **fila, char str[50], int cLinha, char cColuna, tpDesc desc);
void gerarListaGen(tpListaGen **lista, tpFila **fila, tpPilha **pi2);
void remover2Proximos(tpListaGen *lista);
double resolverParenteses(tpListaGen **lista);
double resolverListaGen(tpListaGen **lista, tpPilha **pi);
double resolverExpressao(char str[50], int cLinha, char cColuna, tpDesc desc);
double sum(tpCaixa *c1, tpCaixa *c2, tpDesc desc, int *qtd);
char isCircular(int linha, char coluna, char str[50], tpDesc desc);
char isRetanguloCircular(int linha, char coluna, int ml, int Ml, char mc, char Mc, tpDesc desc);

tpCaixa *buscarPelaString(tpDesc desc, char str[50], int *i)
{
	int linha, j=0;
	char coluna, aux[50];
	
	coluna = str[(*i)++];
	while(*i < strlen(str) && str[*i] >= '0' && str[*i] <= '9')
		aux[j++] = str[(*i)++];
	aux[j] = '\0';
	linha = atoi(aux);
	return buscar(desc, linha, coluna);
}

void pegarRetangulo(int *ml, int *Ml, char *mc, char *Mc, char str[50], int *i)
{
	int j = 0;
	int maxl, minl;
	char maxc, minc;
	char aux[50];
	
	*mc = str[(*i)++];
	while(*i < strlen(str) && str[*i] >= '0' && str[*i] <= '9')
		aux[j++] = str[(*i)++];
	aux[j] = '\0';
	*ml = atoi(aux);
	
	(*i)++;
	
	j=0;
	*Mc = str[(*i)++];
	while(*i < strlen(str) && str[*i] >= '0' && str[*i] <= '9')
		aux[j++] = str[(*i)++];
	aux[j] = '\0';
	*Ml = atoi(aux);
	
	(*i)++;
	
	maxl = max(*Ml, *ml);
	minl = min(*Ml, *ml);
	maxc = max(*Mc, *mc);
	minc = min(*Mc, *mc);
	
	*ml = minl;
	*Ml = maxl;
	*mc = minc;
	*Mc = maxc;
}

double sum(int ml, int Ml, char mc, char Mc, tpDesc desc, int *qtd)
{
	double soma = 0;
	tpCaixa *c;
	tpLinha *lin;
	
	*qtd = 0;
	
	lin = desc.linha;
	
	while(lin != NULL && lin -> linha < ml)
		lin = lin -> prox;
	
	while(lin != NULL && lin -> linha <= Ml)
	{
		c = lin -> caixa;
		while(c != NULL && c -> coluna < mc)
			c = c -> proxColuna;
			
		while(c != NULL && c -> coluna <= Mc)
		{
			if(c -> valor[0] == '=')
			{
				soma += resolverExpressao(c -> valor, c -> linha, c -> coluna, desc);
				*qtd += 1; 
			}
			else if(isNumero(c -> valor))
			{
				soma += strtod(c -> valor, NULL);
				*qtd += 1; 
			}
			
			c = c -> proxColuna;
		}
		
		lin = lin -> prox;
	}
	return soma;
}

char isRetanguloCircular(int linha, char coluna, int ml, int Ml, char mc, char Mc, tpDesc desc)
{
	char flag = 1;
	tpCaixa *c;
	tpLinha *lin;
	
	lin = desc.linha;
	
	while(lin != NULL && lin -> linha < ml)
		lin = lin -> prox;
	
	while(lin != NULL && lin -> linha <= Ml && flag)
	{
		c = lin -> caixa;
		while(c != NULL && c -> coluna < mc)
			c = c -> proxColuna;
			
		while(c != NULL && c -> coluna <= Mc && flag)
		{
			if(isCircular(linha, coluna, c -> valor, desc))
				flag = 0;
			
			c = c -> proxColuna;
		}
		
		lin = lin -> prox;
	}
	return !flag;
}

char isCircular(int linha, char coluna, char str[50], tpDesc desc)
{
	int ml, Ml, j, i=1;
	tpCaixa *c;
	char mc, Mc, aux[50], flag = 1;
	while(i < strlen(str) && flag)
	{
		if(str[i] >= 'A' && str[i] <= 'Z')
		{
			if(str[i+1] >= '0' && str[i+1] <= '9')
			{
				mc = str[i++];
				j=0;
				while(i < strlen(str) && str[i] >= '0' && str[i] <= '9')
					aux[j++] = str[i++];
				aux[j] = '\0';
				ml = atoi(aux);
				if(ml != linha || mc != coluna)
				{
					c = buscar(desc, ml, mc);
					if(c != NULL)
					{
						if(isCircular(linha, coluna, c -> valor, desc))
							flag = 0;
					}
				}
				else
					flag = 0;
			}
			else
			{
				i+=4;
				pegarRetangulo(&ml, &Ml, &mc, &Mc, str, &i);
				if(ml <= linha && mc <= coluna && Ml >= linha && Mc >= coluna)
					flag = 0;
				else if(isRetanguloCircular(linha, coluna, ml, Ml, mc, Mc, desc))
					flag = 0;
			}
		}
		i++;
	}
	return !flag;
}

double maior(int ml, int Ml, char mc, char Mc, tpDesc desc)
{
	double maior = 0, aux;
	tpCaixa *c;
	tpLinha *lin;
	
	lin = desc.linha;
	
	while(lin != NULL && lin -> linha < ml)
		lin = lin -> prox;
	
	while(lin != NULL && lin -> linha <= Ml)
	{
		c = lin -> caixa;
		while(c != NULL && c -> coluna < mc)
			c = c -> proxColuna;
			
		while(c != NULL && c -> coluna <= Mc)
		{
			if(c -> valor[0] == '=')
				aux = resolverExpressao(c -> valor, c -> linha, c -> coluna, desc);
			else if(isNumero(c -> valor))
				aux = strtod(c -> valor, NULL);
			else
				aux = 0;
				
			if(maior < aux)
				maior = aux;
			
			c = c -> proxColuna;
		}
		
		lin = lin -> prox;
	}
	return maior;
}

double menor(int ml, int Ml, char mc, char Mc, tpDesc desc)
{
	double menor, aux;
	char flag = 1;
	tpCaixa *c;
	tpLinha *lin;
	
	lin = desc.linha;
	
	while(lin != NULL && lin -> linha < ml)
		lin = lin -> prox;
	
	while(lin != NULL && lin -> linha <= Ml)
	{
		c = lin -> caixa;
		while(c != NULL && c -> coluna < mc)
			c = c -> proxColuna;
			
		while(c != NULL && c -> coluna <= Mc)
		{
			if(flag)
			{
				menor = strtod(c -> valor, NULL);
				flag = 0;
			}
			if(c -> valor[0] == '=')
				aux = resolverExpressao(c -> valor, c -> linha, c -> coluna, desc);
			else if(isNumero(c -> valor))
				aux = strtod(c -> valor, NULL);
			else
				aux = 0;
				
			if(menor > aux)
				menor = aux;
			
			c = c -> proxColuna;
		}
		
		lin = lin -> prox;
	}
	return menor;
}

void gerarFila(tpFila **fila, char str[50], int cLinha, char cColuna, tpDesc desc)
{
	char mc, Mc;
	double soma;
	int i=1,j=0, ml, Ml, qtd;
	tpCaixa *c, *c2;
	char aux[50];
	while(i < strlen(str))
	{
		if(str[i] >= 'A' && str[i] <= 'Z')
		{
			if(str[i+1] >= '0' && str[i+1] <= '9')
			{
				c = buscarPelaString(desc, str, &i);
				if(c != NULL)
				{
					if(isNumero(c -> valor))
						strcpy(aux, c -> valor);
					else if(c -> valor[0] == '=' && (cLinha != c -> linha || cColuna != c -> coluna))
						snprintf(aux, 50, "%f", resolverExpressao(c -> valor, c -> linha, c -> coluna, desc));
					else
					{
						aux[0] = '0';
						aux[1] = '\0';
					}
				}
				else
				{
					aux[0] = '0';
					aux[1] = '\0';
				}
			}
			else
			{
				if(str[i] == 'S' && str[i+1] == 'U' && str[i+2] == 'M')
				{
					i+=4;
					pegarRetangulo(&ml, &Ml, &mc, &Mc, str, &i);
					
					soma = sum(ml, Ml, mc, Mc, desc, &qtd);
					snprintf(aux,50,"%f",soma);
				}
				else if(str[i] == 'A' && str[i+1] == 'V' && str[i+2] == 'G')
				{
					i+=4;
					pegarRetangulo(&ml, &Ml, &mc, &Mc, str, &i);
					
					soma = sum(ml, Ml, mc, Mc, desc, &qtd);
					if(qtd > 0)
						snprintf(aux,50,"%f", soma / qtd);
					else
					{
						aux[0] = '0';
						aux[1] = '\0';
					}
				}
				else if(str[i] == 'M' && str[i+1] == 'A' && str[i+2] == 'X')
				{
					i+=4;
					pegarRetangulo(&ml, &Ml, &mc, &Mc, str, &i);
					
					snprintf(aux,50,"%f", maior(ml, Ml, mc, Mc, desc));
				}
				else if(str[i] == 'M' && str[i+1] == 'I' && str[i+2] == 'N')
				{
					i+=4;
					pegarRetangulo(&ml, &Ml, &mc, &Mc, str, &i);
					
					snprintf(aux,50,"%f", menor(ml, Ml, mc, Mc, desc));
				}
			}
			inserirFila(fila, aux);
		}
		else if(str[i] >= '0' && str[i] <= '9')
		{
			j=0;
			while(i < strlen(str) && (str[i] >= '0' && str[i] <= '9' || str[i] == '.'))
			{
				aux[j] = str[i];
				j++;
				i++;
			}
			aux[j] = '\0';
			inserirFila(fila, aux);
		}
		else if(str[i] == '+' || str[i] == '-' || str[i] == '*' || str [i] == '/' || str[i] == '(' || str[i] == ')')
		{
			aux[0] = str[i++];
			aux[1] = '\0';
			inserirFila(fila, aux);
		}
	}
}

void gerarListaGen(tpListaGen **lista, tpFila **fila, tpPilha **pi2)
{
	tpPilha *pi = NULL;
	tpListaGen *ultimo = NULL;
	tpValor valor;
	char str[50];
	
	while(!vaziaFila(*fila))
	{
		topFila(*fila, str);
		if(str[0] >= '0' && str[0] <= '9')
		{
			valor.decimal = strtod(str, NULL);
			if(vaziaPilha(pi))
				inserirLista(lista, valor, 'd');
			else
				inserirLista(&(topPilha(pi) -> valor.lista), valor, 'd');
		}
		else if(str[0] == '*' || str[0] == '+' || str[0] == '-' || str[0] == '/')
		{
			valor.operador = str[0];
			if(vaziaPilha(pi))
				inserirLista(lista, valor, 'o');
			else
				inserirLista(&(topPilha(pi) -> valor.lista), valor, 'o');
		}
		else if(str[0] == '(')
		{
			valor.lista = NULL;
			if(vaziaPilha(pi))
				ultimo = inserirLista(lista, valor, 'l');
			else
				ultimo = inserirLista(&(topPilha(pi) -> valor.lista), valor, 'l');
			inserirPilha(&pi, ultimo);
			inserirPilha(pi2, ultimo);
		}
		else if(str[0] == ')')
		{
			retirarPilha(&pi);
		}
		retirarFila(fila);
	}
}

void exibirLista(tpListaGen *lista, int esp)
{
	int i=0;
	if(lista != NULL)
	{
		if(lista -> tipo == 'l')
		{
			exibirLista(lista -> valor.lista, esp+1);
		}
		else
		{
			for(i=0;i<esp;i++)
				printf("  ");
			printf("%c\n",lista -> tipo);
		}
		exibirLista(lista -> prox, esp);
	}
}

void remover2Proximos(tpListaGen *lista)
{
	tpListaGen *p = lista -> prox;
	lista -> prox = lista -> prox -> prox -> prox;
	free(p -> prox);
	free(p);
}

double resolverParenteses(tpListaGen **lista)
{
	double r = 0;
	tpListaGen *p = *lista;
	if(p != NULL)
	{
		while(p -> prox != NULL)
		{
			if(p -> prox -> tipo == 'o')
			{
				if(p -> prox -> valor.operador == '*')
				{
					p -> valor.decimal = p -> valor.decimal * p -> prox -> prox -> valor.decimal;
					remover2Proximos(p);
				}
				else if(p -> prox -> valor.operador == '/')
				{
					if(p -> prox -> prox -> valor.decimal != 0)
						p -> valor.decimal = p -> valor.decimal / p -> prox -> prox -> valor.decimal;
					else
						p -> valor.decimal = 0;
					remover2Proximos(p);
				}
				else
					p = p -> prox;
			}
			else
				p = p -> prox;
		}
	
		p = *lista;
		while(p -> prox != NULL)
		{
			if(p -> prox -> tipo == 'o')
			{
				if(p -> prox -> valor.operador == '+')
				{
					p -> valor.decimal = p -> valor.decimal + p -> prox -> prox -> valor.decimal;
					remover2Proximos(p);
				}
				else if(p -> prox -> valor.operador == '-')
				{
					p -> valor.decimal = p -> valor.decimal - p -> prox -> prox -> valor.decimal;
					remover2Proximos(p);
				}
			}
		}
	
		r = (*lista) -> valor.decimal;
		free(*lista);
		*lista = NULL;
	}
	
	return r;
}

double resolverListaGen(tpListaGen **lista, tpPilha **pi)
{
	while(!vaziaPilha(*pi))
	{
		topPilha(*pi) -> tipo = 'd';
		topPilha(*pi) -> valor.decimal = resolverParenteses(&(topPilha(*pi) -> valor.lista));
		retirarPilha(pi);
	}
	return resolverParenteses(lista);
}

double resolverExpressao(char str[50], int cLinha, char cColuna, tpDesc desc)
{
	tpPilha *pi = NULL;
	tpListaGen *lista = NULL;
	tpFila *fila = NULL;
	gerarFila(&fila, str, cLinha, cColuna, desc);
	gerarListaGen(&lista, &fila, &pi);
	return resolverListaGen(&lista, &pi);
}










