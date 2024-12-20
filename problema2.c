#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define WORD_SIZE 20

struct dictionary_entry {
	char* word;
	int priority;
};

int comparator(const void* l, const void* r)
{
	int left = ((struct dictionary_entry*)l)->priority;
	int right = ((struct dictionary_entry*)r)->priority;
	if (left != right)
		return -(left - right);

	char* left_word = ((struct dictionary_entry*)l)->word;
	char* right_word = ((struct dictionary_entry*)r)->word;
	return strcmp(left_word, right_word);	
}

int notSpecial(const char* word)
{
	if (word == NULL)
		return 1;

	if (*word == ',' || word[0] == '.'
		|| word[0] == '?' || word[0] == '!')
		return 0;
	return 1;
}

int star(const char* word)
{
	if (word == NULL)
		return 0;

	if (word[strlen(word) - 1] == '*')
		return 1;
	return 0;
}

int find(const char* sir, struct dictionary_entry* dict, int dim)
{
	if (sir == NULL || dict == NULL)
		return -1;

	int len = strlen(sir);
	for (int i = 0; i < dim; i++)
		if (strncmp(dict[i].word, sir, len) == 0)
			return i;

	return -1;
}

int find_absolute(const char* sir, struct dictionary_entry* dict, int dim)
{
	if (sir == NULL || dict == NULL)
		return -1;

	for (int i = 0; i < dim; i++)
		if (strcmp(dict[i].word, sir) == 0)
			return i;

	return -1;
}

void addDictionary(struct dictionary_entry** dict, int* n, const char* word)
{
	if (dict == NULL || n == NULL || word == NULL)
		return;

	(*n)++;
	*dict = (struct dictionary_entry*)realloc(*dict, (*n) * sizeof(struct dictionary_entry));
	struct dictionary_entry nou;
	nou.word = (char*)calloc(strlen(word) + 1, sizeof(char));
	strcpy(nou.word, word);
	nou.priority = 1;
	
	(*dict)[*n - 1] = nou;

	qsort(*dict, *n, sizeof(struct dictionary_entry), comparator);
}

void printResult(char** rez, int n)
{
	if (rez == NULL)
		return;
	
	for (int i = 0; i < n; i++)
		printf("%s ", rez[i]);
}

int main()
{
	int N;
	scanf("%d", &N);
	char dump;
	scanf("%c", &dump); // iau enter-ul de dupa scanf
	struct dictionary_entry* dictionar = (struct dictionary_entry*)calloc(N, sizeof(struct dictionary_entry));
	int cop = N;
	char word[WORD_SIZE + 1];
	int wi = 0;
	int N_ITEMS = N;

	while (cop)
	{
		scanf("%s", word);
		
		dictionar[wi].word = (char*)calloc(strlen(word) + 1, sizeof(char));
		strcpy(dictionar[wi].word, word);
		wi++;
		cop--;
	}

	qsort(dictionar, N_ITEMS, sizeof(struct dictionary_entry), comparator);

	int M;
	scanf("%d", &M);
	scanf("%c", &dump); // iau enterul de dupa scanf

	char** output = (char**)calloc(M, sizeof(char*));
	wi = 0;
	int cop_M = M;

	while (cop_M)
	{
		scanf("%s", word);

		cop_M--;
		// intai verificam sa nu fie sir de caractere speciale
		if (notSpecial(word))
		{
			int starred = 0;
			if (star(word))
			{
				starred = 1;
				// scoatem steluta
				word[strlen(word) - 1] = '\0';
			
			}
			int pos = find(word, dictionar, N_ITEMS);
			// daca cuvantul nu exista in dictionar il adaugam
			if (pos == -1)
			{
				output[wi] = (char*)calloc(strlen(word) + 1, sizeof(char));
				strcpy(output[wi], word);
				addDictionary(&dictionar, &N_ITEMS, word);
			}
			// daca cuvantul exista il luam din dictionar pentru a-l adauga
			// la afisare si ii marim prioritatea
			else
			{
				if (!starred)
				{
					output[wi] = (char*)calloc(
									strlen(dictionar[pos].word) + 1, sizeof(char));
					
					strcpy(output[wi], dictionar[pos].word);
					
					dictionar[pos].priority++;
				}
				// daca a fost cu *
				else
				{
					pos = find_absolute(word, dictionar, N_ITEMS);
					output[wi] = (char*)calloc(
									strlen(word) + 1, sizeof(char));
					
					strcpy(output[wi], word);
					if (pos != -1)
						dictionar[pos].priority++;	
					// daca nu exista in dictionar il adaugam cu prioritatea 1
					else
						addDictionary(&dictionar, &N_ITEMS, word);
					
				}					
				
				qsort(dictionar, N_ITEMS, 
					sizeof(struct dictionary_entry), comparator);
							
			}
		}
		// daca este cuvant cu caracter special
		else
		{
			output[wi] = (char*)calloc(strlen(word) + 1, sizeof(char));
			strcpy(output[wi], word);
		}

		wi++;
	}

	printResult(output, M);

	for (int i = 0; i < M; i++)
		free(output[i]);
	free(output);

	for (int i = 0; i < N; i++)
		free(dictionar[i].word);
	free(dictionar);

	return 0;
}
