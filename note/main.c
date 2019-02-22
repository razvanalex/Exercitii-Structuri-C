#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef enum { false, true } bool;

typedef struct {
	int lenNume;
	char *Nume;
	int nrMaxNote;
	int nrCrtNote;
	int *Note;
	bool AreTeza;
	int Teza;
} TMaterie;

typedef struct {
	int lenNume;
	char *Nume;
	int lenPrenume;
	char *Prenume;
	short Clasa;
	char Litera;
	int nrMaterii;
	TMaterie *materii;
} TElev;

TMaterie *alocaMaterie(int nrNote)
{
	TMaterie *materie = (TMaterie*)malloc(sizeof(TMaterie));

	if (materie)
	{
		materie->lenNume = 0;
		materie->Nume = NULL;
		materie->nrMaxNote = nrNote;
		materie->nrCrtNote = 0;
		materie->Note = malloc(nrNote * sizeof(int));
		materie->AreTeza = false;
		materie->Teza = 0;
	}

	return materie;
}

TElev *alocaElev(int nrMaterii)
{
	TElev *elev = (TElev*)malloc(sizeof(TElev));

	if (elev)
	{
		elev->lenNume = 0;
		elev->Nume = NULL;
		elev->lenPrenume = 0;
		elev->Prenume = NULL;
		elev->Clasa = 0;
		elev->Litera = '-';
		elev->nrMaterii = nrMaterii;
		elev->materii = (TMaterie*)malloc(nrMaterii * sizeof(TMaterie));
	}

	return elev;
}

void creazaElev(TElev *elev, char *Nume, char *Prenume, short Clasa, char Litera,
		int nrMaterii, TMaterie *materii)
{
	if (elev)
	{
		elev->lenNume = strlen(Nume);
		elev->Nume = Nume;
		elev->lenPrenume = strlen(Prenume);
		elev->Prenume = Prenume;
		elev->Clasa = Clasa;
		elev->Litera = Litera;
		elev->nrMaterii = nrMaterii;
		elev->materii = materii;
	}
}

void adaugaMaterie(TElev *elev, TMaterie *materie)
{
	int i;
	if (elev)
	{
		elev->nrMaterii++;
		TMaterie *tmp = realloc(elev->materii, elev->nrMaterii * sizeof(TMaterie));
		if (tmp)
		{
			elev->materii = tmp;
		}
		TMaterie *mat = &elev->materii[elev->nrMaterii - 1];
		mat->lenNume = materie->lenNume;
		mat->Nume = malloc((mat->lenNume + 1) * sizeof(char));
		strcpy(mat->Nume, materie->Nume);
		mat->nrMaxNote = materie->nrMaxNote;
		mat->nrCrtNote = materie->nrCrtNote;
		mat->Note = malloc(mat->nrMaxNote * sizeof(int));
		for (i = 0; i < mat->nrCrtNote; i++)
			mat->Note[i] = materie->Note[i];
		mat->AreTeza = materie->AreTeza;
		mat->Teza = materie->Teza;
	}
}

void citesteString(char **str)
{
	char c;
	int i;

	if (*str)
		free(*str);

	i = 0;
	*str = malloc(2 + sizeof(char));
	while (scanf("%c", &c) && c != '\n')
	{
		(*str)[i++] = c;
		char *tmp = realloc(*str, (i + 1) * sizeof(char));
		if (tmp)
			*str = tmp;
	}
	(*str)[i] = '\0';
}

void stdInElev(TElev **elev)
{
	TMaterie *materie;
	int i, nrNote, nrMaterii;
	char Teza;

	printf("Nr materii: ");
	scanf("%d", &nrMaterii);
	*elev = alocaElev(nrMaterii);
	(*elev)->nrMaterii = nrMaterii;

	printf("Nume elev: ");
	citesteString(&(*elev)->Nume);
	(*elev)->lenNume = strlen((*elev)->Nume);
	printf("Prenume elev: ");
	citesteString(&(*elev)->Prenume);
	(*elev)->lenPrenume = strlen((*elev)->Prenume);

	printf("Clasa: ");
	scanf("%hu", &(*elev)->Clasa);
	printf("Litera: ");
	scanf("%c", &(*elev)->Litera);

	for (i = 0; i < (*elev)->nrMaterii; i++)
	{	 		
		printf("Materie %d: \n", i);
		printf("Nr note: ");
		scanf("%d", &nrNote);

		materie = alocaMaterie(nrNote);

		printf("Nume materie: ");
		citesteString(&materie->Nume);
		materie->lenNume = strlen(materie->Nume);

		printf("Introducere materii: ");
		for (materie->nrCrtNote = 0; materie->nrCrtNote < nrNote; materie->nrCrtNote++)
			scanf("%d", &materie->Note[materie->nrCrtNote]);
		
		do {
			printf("Are teza?(Y/N)\n");
			scanf("%c", &Teza);
			if (Teza == 'Y' || Teza == 'y')
			{
				materie->AreTeza = true;
				printf("Nota la teza: ");
				scanf("%d", &materie->Teza);
			}
			else if (Teza == 'N' || Teza == 'n')
				materie->AreTeza = false;
			else printf("Is not a valid input!\n");
		} while(Teza != 'Y' && Teza != 'N' && Teza == 'y' && Teza == 'n');

		adaugaMaterie(*elev, materie);	
	}

}


int main(void)
{
	TElev *elev;
	stdInElev(&elev);

	return 0;
}
