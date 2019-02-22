#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
	unsigned long timestamp;
	char idDest[256];
	int len;
	char *mesaj;
} TMesaj;

typedef struct {
	TMesaj *mesaje;
	int nrMaxMesaje;
	int nrCrtMesaje;
	int dimSursa;
	char *sursa;
} TLog;

typedef struct {
	TMesaj *fmsg;
	int numMsg;
} TMsgSent;

TLog* alocaLog(int nrMaxMesaje)
{
	TLog *logs = (TLog*)malloc(sizeof(TLog));
	if (logs)
	{
		logs->mesaje = (TMesaj*)malloc(nrMaxMesaje * sizeof(TMesaj));
		logs->nrMaxMesaje = nrMaxMesaje;
		logs->nrCrtMesaje = 0;
		logs->dimSursa = 0;
		logs->sursa = NULL;
	}

	return logs;
}

void freeLogs(TLog *logs)
{
	int i;

	if (logs)
	{
		for (i = 0; i < logs->nrCrtMesaje; i++)
		{
			free(logs->mesaje[i].mesaj);
		}
		free(logs->mesaje);
		free(logs->sursa);
		free(logs);
		logs = NULL;
	}
}

TLog* citireLogs(FILE *file)
{
	int nrMaxMesaje, i;
	
	fread(&nrMaxMesaje, sizeof(int), 1, file);
	TLog *logs = alocaLog(nrMaxMesaje);
	if (logs)
	{
		fread(&logs->dimSursa, sizeof(int), 1, file);
		logs->sursa = malloc((logs->dimSursa + 1)* sizeof(char));
		fread(logs->sursa, sizeof(int), logs->dimSursa, file);
		fread(&logs->nrCrtMesaje, sizeof(int), 1, file);

		for (i = 0; i < logs->nrCrtMesaje; i++)
		{
			fread(&logs->mesaje[i].timestamp, sizeof(unsigned long), 1, file);
			fread(logs->mesaje[i].idDest, sizeof(char), 256, file);
			fread(&logs->mesaje[i].len, sizeof(int), 1, file);
			logs->mesaje[i].mesaj = malloc(logs->mesaje[i].len * sizeof(char));
			fread(logs->mesaje[i].mesaj, sizeof(char), logs->mesaje[i].len, file);
		}
	}

	return logs;
}

char generateRandomLetter(int max)
{
	return 'a' + rand() % max;
}

char* generateRandomString(int len)
{
	char *str = malloc((len + 1) * sizeof(char));
	int i;
	if (str)
	{
		for (i = 0; i < len; i++)
			str[i] = generateRandomLetter(26);
		str[len] = '\0';
	}
	return str;
}

TLog* generateRandomLog()
{
	int nrMaxMesaje, i;
	srand(time(NULL));

	nrMaxMesaje = rand() % 10 + 1; // maximum 10 massages

	TLog *logs = alocaLog(nrMaxMesaje);
	if (logs)
	{
		logs->dimSursa = rand() % 10 + 1;
		logs->sursa = generateRandomString(logs->dimSursa);
		logs->nrCrtMesaje = rand() % 10 + 1;
		
		if (logs->nrCrtMesaje > logs->nrMaxMesaje)
			logs->nrCrtMesaje = logs->nrMaxMesaje;

		for (i = 0; i < logs->nrCrtMesaje; i++)
		{
			logs->mesaje[i].timestamp = rand() % 54200 + 1;
			char *dest = generateRandomString(rand() % 254 + 1);
			if (dest)
			{
				strcpy(logs->mesaje[i].idDest, dest);
				free(dest);
			}
			logs->mesaje[i].len = rand() % 200 + 1;
			logs->mesaje[i].mesaj = generateRandomString(logs->mesaje[i].len);
		}
	}
	return logs;
}

void printLogs(TLog *logs)
{
	int i;
	
	printf("Max Mesaje = %d\n", logs->nrMaxMesaje);
	printf("Nr Mesaje = %d\n", logs->nrCrtMesaje);
	printf("Dim Sursa = %d\n", logs->dimSursa);
	printf("Sursa = %s\n", logs->sursa);
	printf("Mesaje\n");
	
	for(i = 0; i < logs->nrCrtMesaje; i++)
	{
		printf("%d\n", i);
		printf("timestamp = %ld\n", logs->mesaje[i].timestamp);
		printf("idDest = %s\n", logs->mesaje[i].idDest);
		printf("len = %d\n", logs->mesaje[i].len);
		printf("mesaj = %s\n", logs->mesaje[i].mesaj);
	}
}

int cmpDest(const void *a, const void *b)
{
	TMesaj msg1 = *(TMesaj*)a;
	TMesaj msg2 = *(TMesaj*)b;

	if(strcmp(msg1.idDest, msg2.idDest))
		return strcmp(msg1.idDest, msg2.idDest);

	return msg1.timestamp - msg2.timestamp;
}

void sortMessages(TLog *logs)
{
	qsort(logs->mesaje, logs->nrCrtMesaje, sizeof(TMesaj), cmpDest);
}

TMsgSent getMessages(TLog *logs, char *idDest, int st, int ft)
{
	int i;
	TMsgSent msg;
	msg.fmsg = NULL;
	msg.numMsg = 0;	

	for (i = 0; i < logs->nrCrtMesaje; i++)
	{
		if (strcmp(logs->mesaje[i].idDest, idDest))
		{
			if (logs->mesaje[i].timestamp >= st && 
			    logs->mesaje[i].timestamp <= ft)
			{
				if(msg.numMsg == 0)
				{
					msg.fmsg = &logs->mesaje[i];
				}
				msg.numMsg++;
			}
		}
	}

	return msg;
}

int main()
{
	TLog *log = generateRandomLog();
	printLogs(log);
	printf("Sorting...");
	sortMessages(log);	
	printLogs(log);

	TMsgSent asd;
	char* authorToFind = log->mesaje[log->nrCrtMesaje - 1].idDest;
	asd = getMessages(log, authorToFind, 0, time(NULL));
	printf("Messages found %d", asd.numMsg);
	freeLogs(log);

	return 0;
}
