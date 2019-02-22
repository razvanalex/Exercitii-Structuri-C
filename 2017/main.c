/* FILE DESCRIPTION 
 * main.c
 *
 * Rezolvarea subiectului 1 de la examenul de programare, seria CB, din 2017.
 * 
 * Program testat cu valgrind (peste 100-200 teste facute):
 *   Rezultate: [...] no leaks are possible [...]  x errors from 1 contexts
 *   - Erorile apar din cauza unui posibil salt conditionat provocat de noul element 
 *     agaugat in colectie (valori neinitializate). In plus, "eroarea" apare la printare
 *     Nu este ceva chiar asa de important in cazul de fata.
 *
 *   Mai multe detalii aici:
 *   http://stackoverflow.com/questions/2612447/pinpointing-conditional-jump-or-move-depends-on-uninitialized-values-valgrin
 *
 * In data.txt este generata o lege care urmeaza a fi adaugata.
 * 
 * Pentru nelamuriri, greseli in cod sau alte lucruri legate 
 * de cod, va rog sa-mi spuneti.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Pentru a putea folosi TArticol in structura cu acelasi nume, trebuie sa 
// fie mai intai declarata. Altfel apar erori de compilare.
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

// Functie ce aloca memorie pentru o colectie.
// Functia rezolva cerinta b)
TColectieLegi *alocaTColectie(int nrMaxLegi)
{
	int i;
	TColectieLegi *c = (TColectieLegi*)malloc(sizeof(TColectieLegi));

	if (c)
	{
		c->nrCrtLegi = 0;
		c->nrMaxLegi = nrMaxLegi;
		c->legi = (TLege*)malloc(nrMaxLegi * sizeof(TLege));
		for (i = 0; i < nrMaxLegi; i++)
		{
			c->legi[i].articole = NULL;
			c->legi[i].nrCrtArt = 0;
			c->legi[i].nrMaxArt = 0;
		}
	}

	return c;
}

// Prin apelul acestei functii se elibereaza memoria,
// pentru a nu exista memory leaks
void elibereazaTLege(TLege **l)
{
	int i;
	if (*l)
	{
		if ((*l)->articole)
		{
			for (i = 0; i < (*l)->nrMaxArt; i++)
				if ((*l)->articole[i].continut)
					free((*l)->articole[i].continut);
			free((*l)->articole);
		}
		free(*l);
		*l = NULL;
	}
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

// Nu are treaba cu cerinta. Urmatoarea functie este doar
// pentru a genera valori random.
char generateRandomLetter(char startLetter, int max)
{
	return startLetter + rand() % max;
}

// Nu are treaba cu cerinta. Urmatoarea functie este doar
// pentru a genera valori random.
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

// Nu are treaba cu cerinta. Urmatoarea functie este doar
// pentru a genera valori random.
TArticol* generareArticole(int nrMaxArt, int nrCrtArt)
{
	int i, j, ok;
	TArticol *art = (TArticol*)malloc(nrMaxArt * sizeof(TArticol));

	//Initialize pointers
	for (i = 0; i < nrMaxArt; i++)
	{
		art[i].continut = NULL;
		for (j = 0; j < 10; j++)
			art[i].articoleAbrogate[j] = NULL;
	}


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
			art[i].nrArtAbrog = 0;
			for (j = 0; j < 10; j++)
				art[i].articoleAbrogate[j] = NULL;
		}
	}

	return art;
}

// Cauta un articol dupa idLege si idArticol si returneaza un pointer catre
// valoarea gasita. Rezolva cerinta d)
TArticol* cautaArticol(TColectieLegi *c, char idLege[11], unsigned int idArt)
{
	int i, j;

	for (i = 0; i < c->nrCrtLegi; i++)
	{
		if (!strcmp(idLege, c->legi[i].id)) 	// strcmp intoarce 0 atunci cand stringurile
		{					// sunt egale, deci valoarea trebuie negata
			for (j = 0; j < c->legi[i].nrCrtArt; j++)
			{
				if (c->legi[i].articole[j].id == idArt)
					return &c->legi[i].articole[j];
			}
		}
	}

	return NULL;
}

// Nu conteaza pentru examen.
void creazaArtAbrog(TColectieLegi *c, TArticol *art)
{
	int i;
	art->nrArtAbrog = rand() % 10 + 1;

	for (i = 0; i < art->nrArtAbrog; i++)
	{
		do {
			int rndIndex = rand() % c->nrCrtLegi;
			char rndLegeId[11];
			strcpy(rndLegeId, c->legi[rndIndex].id);
			int rndArtId = rand() % c->legi[rndIndex].nrCrtArt + 1;
			art->articoleAbrogate[i] = cautaArticol(c, rndLegeId, rndArtId);
		} while (!art->articoleAbrogate[i]);
	}
}

// Nu conteaza pentru examen.
void abrogaArticoleRandom(TColectieLegi *c)
{
	int i, j;
	for (i = 0; i < c->nrCrtLegi; i++)
	{
		for (j = 0; j < c->legi[i].nrCrtArt; j++)
		{
			TArticol *art = &c->legi[i].articole[j];
			if (i == 0)
			{
				art->nrArtAbrog = rand() % 10 + 1;
				creazaArtAbrog(c, art);
			}
			else art->nrArtAbrog = 0;
		}
	}
}

// Nu conteaza pentru examen
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

// Nu conteaza pentru examen.
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

// Urmatoarele 3 functii fac parte din rezolvarea cerintei c)
// Functia de comparare pentru legi
int cmpLege(const void *a, const void *b)
{
	TLege *l1 = (TLege*)a;
	TLege *l2 = (TLege*)b;
	return strcmp(l1->id, l2->id);
}

// Functia de comparare pentru articole
int cmpArt(const void *a, const void *b)
{
	TArticol *a1 = (TArticol*)a;
	TArticol *a2 = (TArticol*)b;
	return a1->id - a2->id;
}

// Functia ce contine apelurile pentru qsort()
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

// Functia rezolva cerinta f)
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

// Nu conteaza pentru examen.
TLege* cautaLegeDupaArt(TColectieLegi *c, TArticol *art)
{
	int i, j;

	for (i = 0; i < c->nrCrtLegi; i++)
		for (j = 0; j < c->legi[i].nrCrtArt; j++)
			if (art == &c->legi[i].articole[j])
				return &c->legi[i];

	return NULL;
}

// Nu conteaza pt examen. Functia creaza un fisier 
// pentru a putea testa programul.
void creazaFisier(TColectieLegi *c, FILE *file)
{
	int i, j;
	TLege *lege = (TLege*)malloc(sizeof(TLege));

	char *tmp_id = generateRandomString('a', 26, 10);
	if (tmp_id)
	{
		strcpy(lege->id, tmp_id);
		free(tmp_id);
	}

	lege->nrMaxArt = rand() % 50 + 1; // Max 50 Art. per Lege
	lege->nrCrtArt = rand() % lege->nrMaxArt + 1;
	lege->articole = generareArticole(lege->nrMaxArt, lege->nrCrtArt);

	fprintf(file, "%d\n", lege->nrCrtArt);
	fprintf(file, "%s\n", lege->id);

	for (i = 0; i < lege->nrCrtArt; i++)
	{
		TArticol *art = &lege->articole[i];

		fprintf(file, "%d\n", art->id);
		fprintf(file, "%s\n", art->continut);

		creazaArtAbrog(c, art);
		fprintf(file, "%d\n", art->nrArtAbrog);


		for (j = 0; j < art->nrArtAbrog; j++)
		{
			TLege *l = cautaLegeDupaArt(c, art->articoleAbrogate[j]);

			fprintf(file, "%d\n", art->articoleAbrogate[j]->id);
			fprintf(file, "%s\n", l->id);
		}
	}

	// Elibareaza memoria
	elibereazaTLege(&lege);
}


// Rezolva cerinta e)
TLege* adaugaLege(TColectieLegi *c, FILE *file)
{	
	int i, j;

	// Redimensioneaza vectorul de legi, daca acesta este plin
	if (c->nrCrtLegi + 1 >= c->nrMaxLegi)
	{
		c->nrMaxLegi++;
		TLege *tmp = (TLege*)realloc(c->legi, c->nrMaxLegi * sizeof(TLege));
		if (tmp)
		{
			c->legi = tmp;
			tmp = NULL;
		}
	}
	
	c->nrCrtLegi++;
	TLege *lege = &c->legi[c->nrCrtLegi - 1];

	fscanf(file, "%d/n", &lege->nrCrtArt);
	fscanf(file, "%s/n", lege->id);
	
	lege->articole = (TArticol*)malloc(lege->nrCrtArt * sizeof(TArticol));
	if (!lege->articole)
		return NULL;
	lege->nrMaxArt = lege->nrCrtArt;

	for (i = 0; i < lege->nrCrtArt; i++)
	{
		TArticol *art = &lege->articole[i];

		fscanf(file, "%d\n", &art->id);
		j = 0;
		int stringSize = 10;
		art->continut = calloc(stringSize, sizeof(char));
		do {
			if (j == stringSize)
			{
				stringSize *= 2;
				char *tmp = (char*)realloc(art->continut, stringSize);
				if (tmp)
				{
					art->continut = tmp;
					tmp = NULL;
				}
			}
			fscanf(file, "%c", &art->continut[j]);
			j++;
		} while (art->continut[j - 1] != '\n' && art->continut[j - 1] != EOF);
		art->continut[j] = '\0';

		fscanf(file, "%d", &art->nrArtAbrog);

		for (j = 0; j < art->nrArtAbrog; j++)
		{
			unsigned int idArt;
			char idLege[11];

			fscanf(file, "%d", &idArt);
			fscanf(file, "%s", idLege);

			art->articoleAbrogate[j] = cautaArticol(c, idLege, idArt);
		}
	}
	
	return lege;
}

int main()
{
	int i;
	TColectieLegi *c = alocaTColectie(10);
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
	TArticol *artGasit = cautaArticol(c, 		// colectia de articole
					rndLegeId, 	// o lege random
					rndArtId);	// un articol random
	if (artGasit)
		printf("Continutul: %s\n", artGasit->continut);
	else printf("Nu exista articolul.\n");

	for (i = 0; i < c->nrCrtLegi; i++)
		abrogaLegiDinArticole(&c->legi[i]);

	printf("\n\n---------------- ABROGATE ----------------\n\n");
	printareColectie(c);
	
	// Genereaza o lege random intr-un fisier
	FILE *f = fopen("data.txt", "wt");
	creazaFisier(c, f);
	fclose(f);

	f = fopen("data.txt", "rt");
	adaugaLege(c, f);
	fclose(f);

	printf("\n\n---------------- NOUA COLECTIE DE LEGI ----------------\n\n");
	printareColectie(c);

	elibereazaTColectie(&c);
	
	return 0;
}
