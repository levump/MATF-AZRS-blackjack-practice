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


void error(const char* msg, int exit_code)
{
	printf("%s\n", msg);
	exit(exit_code);
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

	return 0;
}
