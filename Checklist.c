#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
//#include <curses.h>

typedef struct manga
{
	char *titulo;
	int volume, tam_titulo;
	struct manga *prox;
}Manga;


void InicializaChecklist(Manga *meses[]);
void AdicionaTitulo(int mes, Manga *meses[]);
void RemoveTitulo(int mes, Manga *meses[]);
void ConsultaTitulo(Manga *mes);
void FinalizaChecklist(Manga *meses[]);

int main()
{
	Manga *meses[12];
	int mes, opcao=1;
	//initscr();
	InicializaChecklist(meses);
	while(opcao!=0)
	{
		printf("Digite 1 para adicionar um titulo, 2 para remover um titulo, 3 para exibir checklist ou 0 para sair do programa: ");
		scanf("%d", &opcao);
		if(opcao==1)
		{
			printf("Em qual mes sera adicionado [1-12]: ");
			scanf("%d", &mes);
			AdicionaTitulo(mes-1, meses);
		}
		else if(opcao==2)
		{
			printf("Em qual mes sera removido [1-12]: ");
			scanf("%d", &mes);
			RemoveTitulo(mes-1, meses);
		}
		else if(opcao==3)
		{
			printf("Qual mes sera consultado [1-12]: ");
			scanf("%d", &mes);
			ConsultaTitulo(meses[mes-1]);
		}
	}
	FinalizaChecklist(meses);
	//endwin();
	return(0);
}


void InicializaChecklist(Manga *meses[])
{
	char arq[8];
	int i;
	Manga *aux, *aux2;
	FILE *ptr;
	for(i=0; i<12; i++)
	{
		if(i==0)
			strcpy(arq,"Jan.dat");
		else if(i==1)
			strcpy(arq,"Fev.dat");
		else if(i==2)
			strcpy(arq,"Mar.dat");
		else if(i==3)
			strcpy(arq,"Abr.dat");
		else if(i==4)
			strcpy(arq,"Mai.dat");
		else if(i==5)
			strcpy(arq,"Jun.dat");
		else if(i==6)
			strcpy(arq,"Jul.dat");
		else if(i==7)
			strcpy(arq,"Ago.dat");
		else if(i==8)
			strcpy(arq,"Set.dat");
		else if(i==9)
			strcpy(arq,"Out.dat");
		else if(i==10)
			strcpy(arq,"Nov.dat");
		else if(i==11)
			strcpy(arq,"Dez.dat");
		ptr = fopen(arq, "rb");
		if(ptr==NULL)
			meses[i]=NULL;
		else
		{
			meses[i] = (Manga *)malloc(sizeof(Manga));
			aux = meses[i];
			while(fread(&(aux->tam_titulo), sizeof(int), 1, ptr)==1)
			{
				aux->titulo = (char *)malloc(aux->tam_titulo+1);
				fread(aux->titulo, sizeof(char), aux->tam_titulo, ptr);
				aux->titulo[aux->tam_titulo]='\0';
				fread(&(aux->volume), sizeof(int), 1, ptr);
				aux->prox = (Manga *)malloc(sizeof(Manga));
				aux2=aux;
				aux = aux->prox;
			}
			free(aux);
			aux2->prox=NULL;
			fclose(ptr);
		}
	}
}

void AdicionaTitulo(int mes, Manga *meses[])
{
	Manga *aux=meses[mes];
	char c;
	int i;
	do
	{
		i=0;
		if(aux==NULL)
		{
			meses[mes] = (Manga *)malloc(sizeof(Manga));
			aux=meses[mes];
		}
		else
		{
			while(aux->prox!=NULL)
				aux=aux->prox;
			aux->prox = (Manga *)malloc(sizeof(Manga));
			aux=aux->prox;
		}
		aux->prox=NULL;
		printf("Digite o titulo: ");
		c=getchar();
		if(c=='\n')
			c=getchar();
		aux->titulo = (char *)malloc(1);
		do
		{
			aux->titulo[i] = c;
			i++;
			aux->titulo = (char *)realloc(aux->titulo, i+1);
			aux->titulo[i]='\0';
			c = getchar();
		}
		while(c!='\n');
		aux->tam_titulo=i;
		printf("Digite o volume: ");
		scanf("%d", &i);
		aux->volume=i;
		//clear();
		puts("Titulo adicionado com sucesso");
		printf("Deseja adicionar outro titulo nesse mes? <s/n>: ");
		c=getchar();
		if(c=='\n')
			c=getchar();
	}
	while(c=='s');
}

void RemoveTitulo(int mes, Manga *meses[])
{
	char *titulo_prov, c='s';
	Manga *aux, *aux2;
	int i=0;
	if(meses[mes]!=NULL)
	{
		printf("Digite o titulo a ser removido: ");
		c = getchar();
		if(c=='\n')
			c = getchar();
		titulo_prov = (char *)malloc(1);
		do
		{
			titulo_prov[i] = c;
			i++;
			titulo_prov = (char *)realloc(titulo_prov, i+1);
			titulo_prov[i]='\0';
			c = getchar();
		}
		while(c!='\n');
		if(strcmp(meses[mes]->titulo, titulo_prov)==0)
		{
			aux=meses[mes];
			meses[mes]=meses[mes]->prox;
			free(aux->titulo);
			free(aux);
			free(titulo_prov);
			puts("Titulo removido com sucesso");
			return;
		}
		for(aux=meses[mes]; aux->prox!=NULL; aux=aux->prox)
		{
			if(strcmp(titulo_prov, aux->prox->titulo)==0)
			{
				aux2 = aux->prox;
				aux->prox = aux->prox->prox;
				free(aux2->titulo);
				free(aux2);
				free(titulo_prov);
				puts("Titulo removido com sucesso");
				return;
			}
		}
		free(titulo_prov);
		puts("O titulo nao foi encontrado");
	}
	else
		puts("Esta checklist esta vazia");
}

void ConsultaTitulo(Manga *mes)
{
	Manga *aux;
	char c;
	FILE *ptr;
	if(mes!=NULL)
	{
		puts("----------------------------------------");
		for(aux=mes; aux!=NULL; aux=aux->prox)
			printf("%s %d\n", aux->titulo, aux->volume);
		puts("----------------------------------------");
		printf("Deseja criar um .txt desta checklist?<s/n>: ");
		getchar();
		scanf("%c", &c);
		if(c=='s')
		{
			ptr = fopen("Checklist.txt", "w");
			if(ptr==NULL)
			{
				puts("Ocorreu um erro");
				return;
			}
			for(aux=mes; aux!=NULL; aux=aux->prox)
				fprintf(ptr, "%s %d\n", aux->titulo, aux->volume);
			fclose(ptr);
			puts("Checklist criada com sucesso");
		}
	}
	else
		puts("Esta checklist esta vazia");
}

void FinalizaChecklist(Manga *meses[])
{
	char arq[8];
	int i;
	Manga *aux;
	FILE *ptr;
	for(i=0; i<12; i++)
	{
		if(i==0)
			strcpy(arq,"Jan.dat");
		else if(i==1)
			strcpy(arq,"Fev.dat");
		else if(i==2)
			strcpy(arq,"Mar.dat");
		else if(i==3)
			strcpy(arq,"Abr.dat");
		else if(i==4)
			strcpy(arq,"Mai.dat");
		else if(i==5)
			strcpy(arq,"Jun.dat");
		else if(i==6)
			strcpy(arq,"Jul.dat");
		else if(i==7)
			strcpy(arq,"Ago.dat");
		else if(i==8)
			strcpy(arq,"Set.dat");
		else if(i==9)
			strcpy(arq,"Out.dat");
		else if(i==10)
			strcpy(arq,"Nov.dat");
		else if(i==11)
			strcpy(arq,"Dez.dat");
		if(meses[i]!=NULL)
		{
			aux=meses[i];
			ptr=fopen(arq, "wb");
			while(aux!=NULL)
			{
				fwrite(&(aux->tam_titulo), sizeof(int), 1, ptr);
				fwrite(aux->titulo, sizeof(char), aux->tam_titulo, ptr);
				fwrite(&(aux->volume), sizeof(int), 1, ptr);
				aux=aux->prox;
			}
			fclose(ptr);
		}
		else
			remove(arq);
		for(aux=meses[i]; aux!=NULL; aux=meses[i])
		{
			meses[i]=meses[i]->prox;
			free(aux->titulo);
			free(aux);
		}
	}
}