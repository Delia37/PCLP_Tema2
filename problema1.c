#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define N_KEY_WORDS 15 // numarul de cuvinte cheie din dictionar
#define N_DOUBLE 4
#define MAX_LINE_LEN 103 // pentru testul 8 rand 12

// functie care genereaza dictionarul
void generate(char*** dictionar)
{
	if (!dictionar)
		return;

	*dictionar = (char**)calloc(N_KEY_WORDS, sizeof(char*));
	char mat[N_KEY_WORDS][10] = { 
									{"first of"},
									{"for each"},
									{"is a"},
									{"list of"},									
									{"for"},									
									{"from"},
									{"in"},	
									{"is"},	
									{"unit"},
									{"or"},
									{"while"},
									{"int"},
									{"float"},
									{"double"},
									{"string"},
								};
	// alocam memorie atat cat trebuie
	// lungimea cuvantului cheie + 1 pentru terminator
	for (int i = 0; i < N_KEY_WORDS; i++)
	{
		(*dictionar)[i] = (char*)calloc(strlen(mat[i]) + 1, sizeof(char));
		strcpy((*dictionar)[i], mat[i]);
	}
}

void eliberare_dictionar(char** dictionar)
{
	for (int i = 0; i < N_KEY_WORDS; i++)
		free(dictionar[i]);
	free(dictionar);
}

int find_double(const char* sir, char** dictionar)
{
	//printf("find_double pentru %s\n", sir);
	if (sir == NULL || dictionar == NULL)
		return 0;

	for (int i = 0; i < N_DOUBLE; i++)
		if (strcmp(dictionar[i], sir) == 0)
			return 1;
	return 0;
}

int find(const char* sir, char** dictionar)
{
	//printf("find pentru %s\n", sir);
	if (sir == NULL || dictionar == NULL)
		return 0;

	for (int i = N_DOUBLE - 1; i < N_KEY_WORDS; i++)
		if (strcmp(dictionar[i], sir) == 0)
			return 1;
	return 0;
}


void proces(char* line, char** dictionar)
{
	if (line == NULL || dictionar == NULL)
		return;
	
	printf("%s\n", line);
	//printf("lungime:%d\n", strlen(line));
	char* rez = (char*)calloc(strlen(line) + 1, sizeof(char));	
	
	int end = strlen(line);
	int wi = 0;
	char *first, *second;
	first = strtok(line, " ");
	second = strtok(NULL, " ");
	int i1 = first - line;
	int i2 = -1;
	if (second)
		i2 = second - line;
	//printf("i1 = %d, i2 = %d\n", i1, i2);
	while (wi < end)
	{
		if (second != NULL)
		{
			char temp[40];
			strcpy(temp, first);
			strcat(temp, " ");
			strcat(temp, second);
			if (find_double(temp, dictionar))
			{
				// spatii inainte de cuvant
				while (wi < i1)
					rez[wi++] = ' ';			

				for (int i = 0; i < strlen(first); i++)
					rez[wi++] = '_';

				//printf("wi = %d\n", wi);
				// underline si pe spatiile dintre cuvinte
				while (wi < i2)
					rez[wi++] = '_';
				//printf("wi = %d\n", wi);

				for (int i = 0; i < strlen(second); i++)
					rez[wi++] = '_';
				//printf("wi = %d\n", wi); 
			}
			else if (find(first, dictionar))
			{
				while (wi < i1)
					rez[wi++] = ' ';
			
				for (int i = 0; i < strlen(first); i++)
					rez[wi++] = '_';
			}		
		}
		else if (find(first, dictionar))
		{
			while (wi < i1)
				rez[wi++] = ' ';
			
			for (int i = 0; i < strlen(first); i++)
				rez[wi++] = '_';
		}
		else
		{
			while (wi < i1)
				rez[wi++] = ' ';	

			for (int i = 0; i < strlen(first); i++)
				rez[wi++] = ' ';
		}
		
		first = second;
		if (first)
			i1 = i2;
		else
			break; // nu mai sunt cuvint

		second = strtok(NULL, " ");
		if (second)
			i2 = second - line;
	}

	// daca a iesit cu break si mai erau spatii la spate
	while (wi < end)
		rez[wi++] = ' ';

	printf("%s\n", rez);
	
	free(rez);
}

int main()
{
	char** dictionar;
	generate(&dictionar);
	int n;
	scanf("%d", &n);
	char dump;
	scanf("%c", &dump); // sa iau enter-ul
	char** vector = (char**)calloc(n, sizeof(char*));
	int wi = 0; // indice de scriere pentru linii
	int cop = n; // pentru eliberarea memoriei
	while (n)
	{
		char line[MAX_LINE_LEN];
		fgets(line, MAX_LINE_LEN, stdin);
		line[strlen(line) - 1] = '\0';
		vector[wi] = (char*)calloc(strlen(line) + 1, sizeof(char));
		strcpy(vector[wi], line);
		
		wi++;
			//printf("%s\n", line);
		n--;
		
	}
	for (int i = 0; i < wi; i++)
		proces(vector[i], dictionar);

	eliberare_dictionar(dictionar);
	for (int i = 0; i < cop; i++)
		free(vector[i]);
	free(vector);

	return 0;
}
