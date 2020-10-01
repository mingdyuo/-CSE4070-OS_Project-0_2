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
#define MAX_ELEMENT 11

/* decide what kind of the command is.
   "OTHERS" means it is data structure dependent command */
typedef enum {CREATE, DELETE, DUMPDATA, QUIT, OTHERS} COMMAND;

struct namedList listPool[MAX_ELEMENT];

typedef enum {
	LIST_FRONT = 0, LIST_BACK, LIST_PUSH_FRONT, LIST_PUSH_BACK, LIST_POP_FRONT, LIST_POP_BACK,
	LIST_INSERT, LIST_INSERT_ORDERED, LIST_EMPTY, LIST_SIZE, LIST_MAX, LIST_MIN,
	LIST_REMOVE, LIST_REVERSE, LIST_SORT, LIST_SPLICE, LIST_SWAP, LIST_UNIQUE

}FUNCTION;

char* functionNameList[] = {
	"list_front", "list_back", "list_push_front", "list_push_back", "list_pop_front", "lsit_pop_back",
	"list_insert", "list_insert_ordered", "list_empty", "list_size", "list_max", "list_min",
	"list_remove", "list_reverse", "list_sort", "list_splice", "list_swap", "list_unique"
};
FUNCTION getFunction(char* functionName){
	for(int i=0;i<18;i++){
		if(!strcmp(functionName, functionNameList[i])) return i;

		
	}
	return -1;
}



int getEmptyList(){
	for(int i=0;i<MAX_ELEMENT;i++){
		if(!listPool[i].item) {
			listPool[i].item = (struct list*)malloc(sizeof(struct list));
			return i;
		}
	}
	return -1;
}

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
	int listIndex = getEmptyList();
	if(listIndex<0) return;

	strcpy(listPool[listIndex].name, name);
	list_init(listPool[listIndex].item);

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

void execute(char* commandLine){
	char* w = commandLine;
	char c;

	while((c = getchar()) != '\n' && c != EOF && c != ' '){
		*w++ = c;
		*w = '\0';
	}
	getchar();
	int function = getFunction(commandLine);
	if(function<0) return;

	switch(function){
		case LIST_FRONT:

			break;
		case LIST_BACK:

			break;
		case LIST_PUSH_FRONT:

			break;
		case LIST_PUSH_BACK:

			break;
		case LIST_POP_FRONT:

			break;
		case LIST_POP_BACK:

			break;
		case LIST_INSERT:

			break;
		case LIST_INSERT_ORDERED:
			
			break;
		case LIST_EMPTY:

			break;
		case LIST_SIZE:

			break;
		case LIST_MAX:

			break;
		case LIST_MIN:

			break;
		case LIST_REMOVE:

			break;
		case LIST_REVERSE:

			break;
		case LIST_SORT:

			break;
		case LIST_SPLICE:

			break;
		case LIST_SWAP:

			break;
		case LIST_UNIQUE:

			break;

		default:
			break;
	}

	


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

int deleteList(char* name){
	int index = -1;

	for(int i=0;i<MAX_ELEMENT;i++){
		if(!strcmp(name,listPool[i].name)) {
			index = i;
			break;
		}
	}

	if(index>=0){
		free(listPool[index].item);
	}

	return index;
}

void delete(char* commandLine){
	fgets(commandLine, MAX_CMD_LINE, stdin);

	char name[50], buffer[50];
	int argc = sscanf(commandLine, "%s %s", name, buffer);

	if(argc > 1){
		printf("ERROR : TOO MUCH ARGUMENT. JUST TYPE THE NAME WITHOUT WHITSPACE\n");
		return;
	}

	if(deleteList(name)>-1) return;
	


	printf("MESSAGE : NO MATCHED NAME FOUND\n");
	return;
	
}


void parser(){
	char commandLine[MAX_CMD_LINE];
	while(true){
		printf("command>> ");
		COMMAND command = getCommand(commandLine);
		getchar();

		switch(command){
			case CREATE:
				create(commandLine);
				break;
			case DELETE: 
				delete(commandLine);
				break;
			case DUMPDATA:

				break;
			case QUIT:
				return;
			case OTHERS:
				execute(commandLine);
				break;
			default:
				break;


		}

	}
}


void init(){

}

void freeData(){
	for(int i=0;i<MAX_ELEMENT;i++){
		if(!listPool[i].item) free(listPool[i].item);
	}
}

int main(int argc, char *argv[]){
	init();

	parser();

	freeData();
	return 0;

}
