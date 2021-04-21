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
