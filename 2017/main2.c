#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct TArticol TArticol;

struct TArticol {
	char *continut;
	unsigned int id;
	char abrogat; // 0 - neabrogat; 1 - abrogat
	TArticol *articoleAbrogate[10]; // se pot abroga maxim 10 articole
	unsigned int nrArtAbrog;
};

typedef struct {
	char id[11]; // id[10] = '\n'
	TArticol *articole;
	int nrMaxArt;
	int nrCrtArt;
}TLege;

typedef struct {
	int nrMaxLegi;
	int nrCrtLegi;
	TLege *legi;
} TColectieLegi;

TColectieLegi *alocaTColectie(int nrMaxLegi)
{
	TColectieLegi *c = (TColectieLegi*)malloc(sizeof(TColectieLegi));

	if (c)
	{
		c->nrCrtLegi = 0;
		c->nrMaxLegi = nrMaxLegi;
		c->legi = (TLege*)malloc(nrMaxLegi * sizeof(TLege));
	}

	return c;
}

void elibereazaTColectie(TColectieLegi **c)
{
	int i, j;
	if (*c)
	{
		if ((*c)->legi)
		{
			for (i = 0; i < (*c)->nrMaxLegi; i++)
				if ((*c)->legi[i].articole)
				{
					for (j = 0; j < (*c)->legi[i].nrMaxArt; j++)
						if ((*c)->legi[i].articole[j].continut)
							free((*c)->legi[i].articole[j].continut);
					free((*c)->legi[i].articole);
				}
			free((*c)->legi);
		}
		free(*c);
		*c = NULL;
	}
}

char generateRandomLetter(char startLetter, int max)
{
	return startLetter + rand() % max;
}

char* generateRandomString(char start, int max, int len)
{
	char *str = malloc((len + 1) * sizeof(char));
	int i;
	for (i = 0; i < len; i++)
	{
		str[i] = generateRandomLetter(start, max);
	}
	str[len] = '\0';

	return str;
}

TArticol* generareArticole(int nrMaxArt, int nrCrtArt)
{
	int i, j, ok;
	TArticol *art = (TArticol*)malloc(nrMaxArt * sizeof(TArticol));
	
	if (art)
	{
		for (i = 0; i < nrCrtArt; i++)
		{
			art[i].continut = generateRandomString('a', 26, rand() % 500 + 1);	
		
			do {
				ok = 1;
				art[i].id = rand() % nrMaxArt + 1;
				for (j = 0; j < i; j++)
				{
					if (art[j].id == art[i].id)
					{
						ok = 0;
						break;
					}
				}
			} while (!ok);
			
			art[i].abrogat = rand() % 2;
 		}
	}
	
	return art;
}

TArticol* cautaArticol(TColectieLegi *c, char idLege[11], unsigned int idArt)
{
	int i, j;
	
	for (i = 0; i < c->nrCrtLegi; i++)
	{
		if (!strcmp(idLege, c->legi[i].id))
		{
			for (j = 0; j < c->legi[i].nrCrtArt; j++)
			{
				if (c->legi[i].articole[j].id == idArt)
					return &c->legi[i].articole[j];
			}
		}
	}
	
	return NULL;
}

void abrogaArticoleRandom(TColectieLegi *c)
{
	int i, j, k;
	for (i = 0; i < c->nrCrtLegi; i++)
	{
		for (j = 0; j < c->legi[i].nrCrtArt; j++)
		{
			TArticol *art = &c->legi[i].articole[j];
			if (i == 0)
			{				
				art->nrArtAbrog = rand() % 10 + 1;
				
				for (k = 0; k < art->nrArtAbrog; k++)
				{
					do {
						int rndIndex = rand() % c->nrCrtLegi;
						char rndLegeId[11];
						strcpy(rndLegeId, c->legi[rndIndex].id);
						int rndArtId = rand() % c->legi[rndIndex].nrCrtArt + 1;
						
						art->articoleAbrogate[k] = cautaArticol(c, rndLegeId, rndArtId);
					} while (!art->articoleAbrogate[k]);
				}				
			}
			else art->nrArtAbrog = 0;
		}
	}
}

void generareColectie(TColectieLegi *c)
{
	int nrMaxCrtLegi, i;
	
	nrMaxCrtLegi = rand() % c->nrMaxLegi + 1;
	for (i = 0; i < nrMaxCrtLegi; i++)
	{
		c->nrCrtLegi++;

		TLege *lege = &c->legi[i]; // Make an alias
		char *tmp_id = generateRandomString('a', 26, 10);
		if (tmp_id)
		{
			strcpy(lege->id, tmp_id);
			free(tmp_id);
		}
		lege->nrMaxArt = rand() % 50 + 1; // Max 50 Art. per Lege
		lege->nrCrtArt = rand() % lege->nrMaxArt + 1;
		lege->articole = generareArticole(lege->nrMaxArt, lege->nrCrtArt);
	}
	
	abrogaArticoleRandom(c);
}

void printareColectie(TColectieLegi *c)
{
	int i, j, k;
	
	printf("Nr max legi: %d\n", c->nrMaxLegi);
	printf("Nr crt legi: %d\n", c->nrCrtLegi);
	
	for (i = 0; i < c->nrCrtLegi; i++)
	{
		TLege *lege = &c->legi[i];
		printf("\n----- Legea %d -----\n", i + 1);
		printf("ID: %s\n", lege->id);
		printf("Nr max art: %d\n", lege->nrMaxArt);
		printf("Nr crt art: %d\n", lege->nrCrtArt);
		for (j = 0; j < lege->nrCrtArt; j++)
		{
			TArticol *art = &lege->articole[j];
			printf("    Art %d - %s\n", art->id, art->abrogat ? "abrogat" 
									  : "neabrogat");
			if (art->nrArtAbrog)
			{
				printf("Articolul abroga articolele (legea e necunoscuta :) - se stie insa adresa): ");
				for (k = 0; k < art->nrArtAbrog; k++)
					printf("%d ", art->articoleAbrogate[k]->id);
				printf("\n");
			}
			printf("  %s\n", art->continut);
		}
	}
	printf("\n");
}

int cmpLege(const void *a, const void *b)
{
	TLege *l1 = (TLege*)a;
	TLege *l2 = (TLege*)b;
	return strcmp(l1->id, l2->id);
}

int cmpArt(const void *a, const void *b)
{
	TArticol *a1 = (TArticol*)a;
	TArticol *a2 = (TArticol*)b;
	return a1->id - a2->id;
}

void sortareColectie(TColectieLegi *c)
{
	int i;
	qsort(c->legi, c->nrCrtLegi, sizeof(TLege), cmpLege);
	
	for (i = 0; i < c->nrCrtLegi; i++)
	{
		TLege *lege = &c->legi[i];
		qsort(lege->articole, lege->nrCrtArt, sizeof(TArticol), cmpArt);
	}
}

void abrogaLegiDinArticole(TLege *l)
{
	int i, j;
	for (i = 0; i < l->nrCrtArt; i++)
	{
		for (j = 0; j < l->articole[i].nrArtAbrog; j++)
		{
			if (l->articole[i].articoleAbrogate[j]->abrogat == 0)
				l->articole[i].articoleAbrogate[j]->abrogat = 1;
		}
	}

}

void creazaFisier(FILE *file)
{
	int i, nrCrtArt;
	TLege *lege =


	fprintf(file, "%d\n", c->nrCrtArt);
	fprintf(file, "%d\n", c->legi[0].id);


	for (i = 0; i < c->nrCrtArt; i++)
	{
		
	}
}

int main()
{
	int i;	
	TColectieLegi *c = alocaTColectie(1);
	srand(time(NULL));
	
	generareColectie(c);
	sortareColectie(c);
	printareColectie(c);
	
	int rndIndex = rand() % c->nrCrtLegi;
	char rndLegeId[11];
	strcpy(rndLegeId, c->legi[rndIndex].id);
	int rndArtId = rand() % c->legi[rndIndex].nrCrtArt + 1;
	
	printf("Se cauta in legea nr %d %s, articolul %d...\n", rndIndex + 1, 
								rndLegeId, 
								rndArtId);
	TArticol *artGasit = cautaArticol(c, 		// Colectia de articole
					  rndLegeId, 	// O lege random
					  rndArtId);	// Un articol random
	if (artGasit)
		printf("Continutul: %s\n", artGasit->continut);
	else printf("Nu exista articolul.\n");
	
	for (i = 0; i < c->nrCrtLegi; i++)
		abrogaLegiDinArticole(&c->legi[i]);
		
	printf("\n\n---------------- ABROGATE ----------------\n\n");
	printareColectie(c);
	
	elibereazaTColectie(&c);
	return 0;
}
