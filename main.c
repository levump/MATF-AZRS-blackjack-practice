#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

#define CARD_NUM (13)
#define MAX_PATH_LEN (100)
#define LINE_SIZE (256)
#define MAX_LINES (32)
#define MAX_HAND_SIZE (12)
#define DECK_SIZE (52)
#define BUFFSIZE  (64)
#define BAR "__________________________________________________\n"

enum card_type{
	C_ACE   = 0,
	C_KING  = 1,
	C_QUEEN = 2,
	C_JACK  = 3,
	C_10    = 4,
	C_9     = 5,
	C_8     = 6,
	C_7     = 7,
	C_6     = 8,
	C_5     = 9,
	C_4     = 10,
	C_3     = 11,
	C_2     = 12

};

typedef struct Card{
	enum card_type type;
	char image_path[MAX_PATH_LEN];

} card;

typedef struct Deck{
	card* cards[DECK_SIZE];
	unsigned next_card;
}deck;

typedef struct Hand{
	unsigned size;
	card* cards[MAX_HAND_SIZE];
}hand;

void error(const char* msg, int exit_code)
{
	printf("%s\n", msg);
	exit(exit_code);
}


void check_card(card card)
{
	FILE *input = fopen(card.image_path, "r");
	if(input == NULL)
		error("Reading card image failed!", 1);

	char *line = malloc(LINE_SIZE);
	while(fgets(line, LINE_SIZE, input) != NULL){
		int line_len = strlen(line);
		if(line_len >= 70)
			error("[ERR] Invalid file(s).", -3);
	}

	free(line);
	fclose(input);
}

void init_cards(card *cards, char *map_path)
{
	struct stat file_info;
	if(stat(map_path, &file_info) == -1)
		error("Reading file info failed!", 1);
	 
	if(!S_ISDIR(file_info.st_mode))
		error("Map path does not point on a directory", 1);

	DIR *dir = opendir(map_path);
	struct dirent *entry = NULL;

	int found_card[CARD_NUM];
	
	for(int i = 0; i < CARD_NUM; i++)
		found_card[i] = 0;

	while((entry = readdir(dir)) != NULL)
	{
		int card_index = -1;
		char *name = entry->d_name;
		if(strcmp(name, "2.txt") == 0)
			card_index = C_2;
		else if(strcmp(name, "3.txt") == 0)
			card_index = C_3;
		else if(strcmp(name, "4.txt") == 0)
			card_index = C_4;
		else if(strcmp(name, "5.txt") == 0)
			card_index = C_5;
		else if(strcmp(name, "6.txt") == 0)
			card_index = C_6;
		else if(strcmp(name, "7.txt") == 0)
			card_index = C_7;
		else if(strcmp(name, "8.txt") == 0)
			card_index = C_8;
		else if(strcmp(name, "9.txt") == 0)
			card_index = C_9;
		else if(strcmp(name, "10.txt") == 0)
			card_index = C_10;
		else if(strcmp(name, "jack.txt") == 0)
			card_index = C_JACK;
		else if(strcmp(name, "queen.txt") == 0)
			card_index = C_QUEEN;
		else if(strcmp(name, "king.txt") == 0)
			card_index = C_KING;
		else if(strcmp(name, "ace.txt") == 0)
			card_index = C_ACE;

		if(card_index == -1)
			continue;
		found_card[card_index] = 1;

		sprintf(cards[card_index].image_path, "./%s/%s", map_path, name);
		cards[card_index].type = card_index;

	}

	closedir(dir);

	for(int i = 0; i < CARD_NUM; i++)
		if(found_card[i] == 0)
			error("Missing a card from map_path!", 1);

	for(int i = 0; i < CARD_NUM; i++)
		check_card(cards[i]);

}


deck new_deck(card *cards, unsigned* seed)
{
	deck d;
	d.next_card = 0;
	for(int i= 0; i < DECK_SIZE; i++)
		d.cards[i] = &cards[i/4];

	return d;
}

void hit(hand *h, deck *d)
{
	h->cards[h->size] = d->cards[d->next_card++];
	h->size++;
}


int hand_value(hand *h)
{

	int has_ace = 0;
	int value = 0;
	for(int i = 0; i < h->size; i++){
		enum card_type type = h->cards[i]->type;
		switch(type){
			case C_2:
				value += 2;
				break;
			case C_3:
				value += 3;
				break;
			case C_4:
				value += 4;
				break;
			case C_5:
				value += 5;
				break;
			case C_6:
				value += 6;
				break;
			case C_7:
				value += 7;
				break;
			case C_8:
				value += 8;
				break;
			case C_9:
				value += 9;
				break;
			case C_10:
			case C_JACK:
			case C_QUEEN:
			case C_KING:
				value += 10;
				break;
			case C_ACE:
				value += 1;
				has_ace = 1;
				break;
		}

	}
	if (has_ace && value <= 11)
		value += 10;
	
	return value;
}

void show_hand(hand *h, char *caption)
{
	printf("%s\n\n", caption);
	printf(BAR);
	
	char **lines = malloc(MAX_LINES * sizeof(char *));
	int size = 0;

	for(int i = 0; i < MAX_LINES; i++){
		lines[i] = malloc(LINE_SIZE);
		for(int j = 0; j < LINE_SIZE; j++)
			lines[i][j] = '\0';

	}
	
	for(int i = 0; i < h->size; i++)
	{
		FILE *input = fopen(h->cards[i]->image_path, "r");
		if(input == NULL)
			error("Reading card image failed!", 1);
		char *line = malloc(LINE_SIZE);
		int line_index = 0;

		while(fgets(line, LINE_SIZE, input) != NULL){
			if(strlen(lines[line_index]) > 0)
				sprintf(lines[line_index], "%s %s", lines[line_index], line);
			else
				strcpy(lines[line_index], line);

			int n = strlen(lines[line_index]);

			if(lines[line_index][n-1] == '\n')
				lines[line_index][n-1] = '\0';
			
			line_index++;
			size = (line_index > size) ? line_index : size;
		}

		free(line);
		fclose(input);
	}

	for(int i = 0; i < size; i++)
		printf("%s\n", lines[i]);
	
	for(int i = 0; i < MAX_LINES; i++)
		free(lines[i]);
	free(lines);

	printf("score:%d\n\n", hand_value(h));
	printf(BAR);

}



void start_game(hand *dealer, hand* player, deck *d)
{
	player->size = 0;
	dealer->size = 0;

	hit(player, d);
	hit(player, d);

	hit(dealer, d);
	hit(dealer, d);

	dealer->size--;

	show_hand(dealer, "DEALERS CARDS: ");
	show_hand(player, "PLAYER CARDS: ");

}
int main(int argc, char **argv){
	if(argc < 2){
		printf("usage: %s <input_folder>", argv[0]);
		exit(-1);
	}

	char *map_path = argv[1];
	unsigned *seed = NULL;

	if(argc >= 3){
		unsigned seed_value = strtol(argv[2], NULL, 10);
		seed = &seed_value;
	}
	
	card* cards = malloc(CARD_NUM*sizeof(card));
	init_cards(cards, map_path);
	
	deck d = new_deck(cards, seed);
	printf("successful loaded deck file :)");	

	return 0;
}
