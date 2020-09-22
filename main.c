#include <stdio.h>
#include <string.h>
#include "bitmap.h"
#include "debug.h"
#include "hex_dump.h"
#include "round.h"
#include "limits.h"
#include "list.h"

int main(int argc, char *argv[]){
	char word[50];
	char *w = word;
	char c;
	while(true){
		while((c = getchar()) != '\n' && c != EOF){
			*w++ = c;
		}
		*w = '\0';
	}

	printf("%s\n", word);

	return 0;

}
