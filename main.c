#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "bitmap.h"
#include "debug.h"
#include "hex_dump.h"
#include "round.h"
#include "limits.h"
#include "list.h"

#define MAX_CMD_LINE 50

/* decide what kind of the command is.
   "OTHERS" means it is data structure dependent command */
typedef enum {CREATE, DELETE, DUMPDATA, QUIT, OTHERS} COMMAND;

struct namedList* headList;

COMMAND getCommand(char *command){
	char* w = command;
	char c;

	while((c = getchar()) != '\n' && c != EOF && c!=' '){
		*w++ = c;
		*w = '\0';
		if(!strcmp(command,"create")) return CREATE;
		else if(!strcmp(command, "dumpdata")) return DUMPDATA;
		else if(!strcmp(command, "delete")) return DELETE;
		else if(!strcmp(command, "quit")) return QUIT;
	}
	return OTHERS;
}

typedef enum {NONE, LIST, HASH, BITMAP} DATATYPE;

void createList(char* name){
	if(headList->next == NULL){
		struct namedList* newList = (struct namedList*)malloc(sizeof(struct namedList));
		strcpy(newList->name, name);
		newList->next = NULL;
		newList->item = (struct list*)malloc(sizeof(struct list));
		list_init(newList->item);

		headList->next = newList;
		return;
	}

	struct namedList* curr = headList->next;
	
	while(!(curr->next)){
		if(!strcmp(curr->name, name)){
			printf("ERROR : ALREADY EXISTING NAME.\n");
			return;
		}
		curr = curr->next;
	}

	struct namedList* newList = (struct namedList*)malloc(sizeof(struct namedList));
	strcpy(newList->name, name);
	newList->next = NULL;
	newList->item = (struct list*)malloc(sizeof(struct list));
	list_init(newList->item);

	curr->next = newList;
}


char *trimWhiteSpace(char *str){
	char* end;
	while(isspace((unsigned char)*str)) str++;

	if(*str==0)
		return str;
	
	end = str + strlen(str) - 1;
	while(end > str && isspace((unsigned char)*end)) end--;

	end[1] = '\0';

	return str;
}


void create(char *commandLine){
	char* w = commandLine;
	char c;	
	DATATYPE dataType = NONE;

	while((c = getchar()) != '\n' && c != EOF && c!=' '){
		*w++ = c;
		*w = '\0';
		if(!strcmp(commandLine, "list")) dataType = LIST;
		else if(!strcmp(commandLine, "hashtable")) dataType = HASH;
		else if(!strcmp(commandLine, "bitmap")) dataType = BITMAP;
	}
	
	if(dataType == NONE){
		printf("ERROR : INVALID DATA STRUCTURE. CANNOT CREATE.\n");
		return;
	}
	
	fgets(commandLine, MAX_CMD_LINE, stdin);

	char name[50], buffer[50];
	int argc = sscanf(commandLine, "%s %s", name, buffer);
	
	if(argc>1){
		printf("ERROR : TOO MUCH ARGUMENT. JUST TYPE NAME WITHOUT WHITESPACE\n");
		return;
	}


	switch(dataType){
		case LIST:
			createList(name);
			

			break;
		case HASH:

			break;
		case BITMAP:

			break;
		default:
			break;
	}
}


void parser(){
	char commandLine[MAX_CMD_LINE];
	while(true){
		COMMAND command = getCommand(commandLine);
		getchar();
		switch(command){
			case CREATE:
				create(commandLine);
				break;
			case DELETE: 

				break;
			case DUMPDATA:

				break;
			case QUIT:
				return;
			case OTHERS:

				break;
			default:
				break;


		}

	}
}


void init(){
	headList = (struct namedList*)malloc(sizeof(struct namedList));
	headList->item = NULL;
	strcpy(headList->name,"__HEADLIST__");
	headList->next = NULL;

}



int main(int argc, char *argv[]){
	init();
	parser();
	return 0;

}
