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


int main(){


	return 0;
}
