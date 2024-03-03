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

void gerarFila(tpFila **fila, char str[50])
{
	int i=0;
	char aux[50];
	
	while(i < strlen(str) && str[i] == ' ' || str[i] == '=')
		i++;
	while(i < strlen(str))
	{
		while(i < strlen(str) && str[i] == ' ')
			i++;
		if(str[i] >= '0' && str[i] <= '9')
		{
			int j=0;
			while(i < strlen(str) && str[i] >= '0' && str[i] <= '9' || str[i] == '.')
				aux[j++] = str[i++];
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
	double r;
	tpListaGen *p = *lista;
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
				p -> valor.decimal = p -> valor.decimal / p -> prox -> prox -> valor.decimal;
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

double resolverExpressaum(char str[50])
{
	tpPilha *pi = NULL;
	tpListaGen *lista = NULL;
	tpFila *fila = NULL;
	gerarFila(&fila, str);
	gerarListaGen(&lista, &fila, &pi);
	return resolverListaGen(&lista, &pi);
}










