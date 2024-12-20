#include <stdlib.h>
#include <time.h>
#include <ncurses.h>
#include <string.h>

#define N_KEY_WORDS 27
#define WORD_SIZE 5
#define BACKSPACE 8
#define BACKSPACE2 127

void init_pairs()
{
	start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLUE);
	init_pair(2, COLOR_GREEN, COLOR_WHITE);
	init_pair(3, COLOR_YELLOW, COLOR_WHITE);
	init_pair(4, COLOR_BLACK, COLOR_WHITE);
	init_pair(5, COLOR_WHITE, COLOR_BLACK);
}

// functie care genereaza dictionarul-preluata din exercitiul 1
void generate(char*** dictionar)
{
	if (!dictionar)
		return;

	*dictionar = (char**)calloc(N_KEY_WORDS, sizeof(char*));
	char mat[N_KEY_WORDS][WORD_SIZE + 1] = { 
									{"arici"}, {"atent"}, {"baiat"}, 
									{"ceata"}, {"debut"}, {"fixat"},	
									{"hamac"}, {"harta"}, {"jalon"},
									{"jucam"}, {"lacat"}, {"magie"},
									{"nufar"}, {"oaste"}, {"peste"},
									{"perus"}, {"rigle"}, {"roman"}, 
									{"sanie"}, {"scris"}, {"sonda"},
									{"texte"}, {"tipar"}, {"titan"},
									{"zebra"}, {"vapor"}, {"vatra"},
								};
	// alocam memorie atat cat trebuie
	// lungimea cuvantului cheie + 1 pentru terminator
	for (int i = 0; i < N_KEY_WORDS; i++)
	{
		(*dictionar)[i] = (char*)calloc(WORD_SIZE + 1, sizeof(char));
		strcpy((*dictionar)[i], mat[i]);
	}
}

void eliberare_dictionar(char** dictionar)
{
	for (int i = 0; i < N_KEY_WORDS; i++)
		free(dictionar[i]);
	free(dictionar);
}

void get_new_word(char** word, char** dictionar)
{
	if (word == NULL || dictionar == NULL)
		return;

	if (*word != NULL)
		free(*word);

	*word = (char*)calloc(WORD_SIZE + 1, sizeof(char));
	int n = rand() % N_KEY_WORDS;
	//printf("n = %d\n", n);
	strcpy(*word, dictionar[n]);
}

void ask(WINDOW* win, int* lives)
{
	//printf("Intra in ask cu %d\n", *lives); 
	if (win == NULL || lives == NULL)
		return;

	werase(win);
	wmove(win, 0, 0);
	if (*lives == 6)
		waddstr(win, "Please give the first try:");
	else if (*lives == 1)
		waddstr(win, "Last try:");
	else
		waddstr(win, "Try again:");
	wrefresh(win);
	
}

void getword(char* sir)
{
	int wi = 0;
	int ch = getch();
    while ( ch != '\n' )
    {
		// functionalitate backspace
		if (ch == BACKSPACE || ch == BACKSPACE2)
		{		
			if (wi == 0)
			{
				ch = getch();
				continue;
			}
			else
				wi--;
		}
		else
        	sir[wi++] = ch;
        
		ch = getch();
    }
	sir[wi] = '\0';
	//printf("strlen = %d\n", strlen(sir));
}

int validate(WINDOW* board, char* word, char* input, int* cursor)
{
	if (board == NULL || word == NULL || input == NULL || cursor == NULL)
		return 0;

	wmove(board, *cursor, 0);
	(*cursor)++;

	if (strcmp(word, input) == 0)
	{
		wattron(board, COLOR_PAIR(2));
		for (int i = 0; i < WORD_SIZE; i++)
			waddch(board, word[i]);
		wattroff(board, COLOR_PAIR(2));
		return 1;
	}

	for (int i = 0; i < WORD_SIZE; i++)
	{
		char* f = strchr(word, input[i]);
		if (f == NULL)
		{
			wattron(board, COLOR_PAIR(4));
			waddch(board, input[i]);			
			wattroff(board, COLOR_PAIR(4));
		}
		else
		{
			if (input[i] == word[i])
			{
				wattron(board, COLOR_PAIR(2));
				waddch(board, input[i]);			
				wattroff(board, COLOR_PAIR(2));
			}
			else
			{
				wattron(board, COLOR_PAIR(3));
				waddch(board, input[i]);			
				wattroff(board, COLOR_PAIR(3));
			}
		}
	}

	return 0;
}

int main()
{	
	char** dictionar;
	generate(&dictionar);

	srand(time(NULL));
	char *word = NULL;

	get_new_word(&word, dictionar);	

	initscr();	/* porneste utilizarea librariei ncurses */
	init_pairs(); 

	int tries = 6;
	int board_cursor = 0; // pe care rand scriu

	WINDOW *intro, *board, *inter_active;
	intro = subwin(stdscr, 2, 80, 0, 0);
	board = subwin(stdscr, 7, 80, 2, 0);
	inter_active = subwin(stdscr, 7, 80, 9, 0);	
	
	wbkgd(intro, COLOR_PAIR(1));
	waddstr(intro, "Welcome to my wordle game!!");
	
	// Astea 2 linii se decomenteaza daca vrei sa trisezi sa vezi cuvantul generat prima data	
	/*wmove(intro, 1, 0);	
	waddstr(intro, word);*/	

	wbkgd(board, COLOR_PAIR(4));
	//wrefresh(board);
	//wrefresh(intro);

	wbkgd(inter_active, COLOR_PAIR(1));
	wattron(inter_active, COLOR_PAIR(5));
	refresh();

	
	int win = 0;
	char input_word[20];
	// game loop
	while (1)
	{
		while (tries)
		{
			ask(inter_active, &tries);
			//refresh();
			move(10, 0);
			getword(input_word);
			if (strlen(input_word) != WORD_SIZE)
			{
				wmove(inter_active, 3, 0);
				waddstr(inter_active, "Word must have 5 letters. Press any key to continue.");
				wrefresh(inter_active);
				getch();
				continue;
			}
			else
			{
				int rez = validate(board, word, input_word, &board_cursor);
				wrefresh(board);
				if (rez)
				{
					werase(inter_active);
					waddstr(inter_active, "Congratulations");
					wrefresh(inter_active);
					win = 1;
					break;
				}
				
				tries--;
			}
		}
		
		if (!win)
		{
			wmove(inter_active, 3, 0);
			waddstr(inter_active, "You have failed.");
		}

		wmove(inter_active, 4, 0);
		waddstr(inter_active, "To exit the program please press 'q'");
		wmove(inter_active, 5, 0);
		waddstr(inter_active, "To start a new game press any other key.");
		wrefresh(inter_active);		

		int ret = getch();
		if (ret == 'q')
			break;


		board_cursor = 0;
		tries = 6;
		win = 0;
		werase(board);
		wrefresh(board);
		get_new_word(&word, dictionar);
	}
	
	
	refresh();
	
	//getch();
	
	delwin(intro);
	delwin(board);
	delwin(inter_active);

	endwin();			/* iese din	extensia ncurses  */

	eliberare_dictionar(dictionar);

	return 0;
}
