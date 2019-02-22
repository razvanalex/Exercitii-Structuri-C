#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
	char nrMasina[9];
	int anFabr;
	int nrAmenzi;
} TMasina;

typedef struct {
	TMasina *masina;
	float valoare;
	char achitat;
	int lenPV;
	char *PV;
} TAmenda;

typedef struct {
	int nrMaxAmenzi;
	int nrCrtAmenzi;
	int nrMaxMasini;
	int nrCrtMasini;
	TMasina *masini;
	TAmenda *amenzi;
} TColectie;

TColectie* alocaColectie(int nrMaxMasini, int nrMaxAmenzi)
{
	TColectie *colectie = (TColectie*)malloc(sizeof(TColectie));
	if (colectie)
	{
		colectie->nrMaxMasini = nrMaxMasini;
		colectie->nrCrtMasini = 0;
		colectie->nrMaxAmenzi = nrMaxAmenzi;
		colectie->nrCrtAmenzi = 0;
		colectie->masini = (TMasina*)malloc(nrMaxMasini * sizeof(TMasina));
		colectie->amenzi = (TAmenda*)malloc(nrMaxAmenzi * sizeof(TAmenda));
	}

	return colectie;
}

void elibereazaColectie(TColectie *c)
{
	int i;

	if (c)
	{
		free(c->masini);
		for (i = 0; i < c->nrCrtAmenzi; i++)
		{
			free(c->amenzi[i].PV);
		}
		free(c->amenzi);
		free(c);
		c = NULL;
	}
}

TAmenda* adaugaAmenda(TColectie *c, TMasina *m, char *PV, float valoare)
{
	TAmenda *amenda = NULL;
	int i;
	
	for (i = 0; i < c->nrCrtMasini; i++)
	{
		if (strcmp(c->masini[i].nrMasina, m->nrMasina))
		{
			c->masini[i].nrAmenzi++;
			c->nrCrtAmenzi++;
			if (c->nrCrtAmenzi >= c->nrMaxAmenzi)
			{
				c->nrMaxAmenzi *= 2;
				TAmenda *tmp = (TAmenda*)realloc(c->amenzi, c->nrMaxAmenzi * sizeof(TAmenda));
				if (tmp)
				{
					free(c->amenzi);
					c->amenzi = tmp;
					tmp = NULL;
				}
			}
			c->amenzi[c->nrCrtAmenzi].masina = &c->masini[i];
			c->amenzi[c->nrCrtAmenzi].valoare = valoare;
			c->amenzi[c->nrCrtAmenzi].achitat = 0;
			c->amenzi[c->nrCrtAmenzi].lenPV = strlen(PV);
			c->amenzi[c->nrCrtAmenzi].PV = PV;
		}
	}

	return amenda;
}

char* generateRandomNumber(int max)
{
	char *num = malloc((max + 1) * sizeof(int));
	int i;
	if (num)
	{
		for (i = 0; i < max; i++)
			num[i] = '0' + rand() % 10;
	}
	num[max] = '\0';
	return num;
}

char generateRandomLetter(int max)
{
	return 'A' + rand() % max;
}

char* generateRandomString(int len)
{
	char *str = malloc((len + 1) * sizeof(len));
	int i;
	for (i = 0; i < len; i++)
	{
		str[i] = generateRandomLetter(26);
	}
	str[len] = '\0';

	return str;
}

char* generateRandomCarNumber()
{
	char *num = malloc(9 * sizeof(char));
	int i = rand() % 42;
	char *l = generateRandomString(3);
	char *j[42] = { "AB", "AG", "AR", "B", "BC", "BH", "BN", "BR", "BT", "BV", "BZ", "CJ", "CL", "CS", "CT", "CV", 
		     "DB", "DJ", "GJ", "GL", "GR", "HD", "HR", "IF", "IL", "IS", "MH", "MM", "MS", "NT", "OT", "PH",
		     "SB", "SJ", "SM", "SV", "TL", "TM", "TR", "VL", "VN", "VS"};
	char *n;
	if (!strcmp(j[i], "B"))
		n = generateRandomNumber(3);
	else n = generateRandomNumber(2);

	strcpy(num, j[i]);
	strcat(num, n);
	strcat(num, l);

	free(l);
	free(n);
	return num;
}

TColectie* generateRandomColectie(int nrMaxMasini, int nrMaxAmenzi)
{
	TColectie *c = alocaColectie(nrMaxMasini, nrMaxAmenzi);
	

}

int main()
{
	srand(time(NULL));
	for (int i = 0; i < 100; i++)
	{
		char *numarAuto = generateRandomCarNumber();
		printf("%s\n", numarAuto);
		free(numarAuto);	
	}

	return 0;
}
