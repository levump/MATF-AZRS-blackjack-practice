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
	int i;
	for(int i= 0; i < DECK_SIZE; i++)
		d.cards[i] = &cards[i/4];

	return d;
}

void hit(hand *h, deck *d)
{
	h->cards[h->size] = d->cards[d->next_card++];
	h->size++;
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
	

	return 0;
}
