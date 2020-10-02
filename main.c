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
#include "hash.h"

#define MAX_CMD_LINE 50
#define MAX_ELEMENT 11

/* decide what kind of the command is.
   "OTHERS" means it is data structure dependent command */
typedef enum {CREATE, DELETE, DUMPDATA, QUIT, OTHERS} COMMAND;

struct namedList listPool[MAX_ELEMENT];
struct namedHash hashPool[MAX_ELEMENT];

typedef enum {
	// List funcions
	LIST_FRONT = 0, LIST_BACK, LIST_PUSH_FRONT, LIST_PUSH_BACK, LIST_POP_FRONT, 
	LIST_POP_BACK, LIST_INSERT, LIST_INSERT_ORDERED, LIST_EMPTY, LIST_SIZE,
	LIST_MAX, LIST_MIN,	LIST_REMOVE, LIST_REVERSE, LIST_SORT,
	LIST_SPLICE, LIST_SWAP, LIST_SHUFFLE, LIST_UNIQUE,
	// Hash functions
	HASH_INSERT, HASH_APPLY, HASH_DELETE, HASH_EMPTY, HASH_SIZE,
	HASH_CLEAR, HASH_FIND, HASH_REPLACE,
	// Bitmap functions
	BITMAP_MARK, BITMAP_ALL, BITMAP_ANY, BITMAP_CONTAINS, BITMAP_COUNT,
	BITMAP_EXPAND, BITMAP_SET_ALL, BITMAP_FLIP, BITMAP_NONE, BITMAP_RESET,
	BITMAP_SCAN_AND_FLIP, BITMAP_SCAN, BITMAP_SET, BITMAP_SET_MULTIPLE, BITMAP_TEST
}FUNCTION;

char* functionNameList[] = {
	// List functions
	"list_front", "list_back", "list_push_front", "list_push_back", "list_pop_front", 
	"list_pop_back", "list_insert", "list_insert_ordered", "list_empty", "list_size", 
	"list_max", "list_min",	"list_remove", "list_reverse", "list_sort", 
	"list_splice", "list_swap", "list_shuffle",	"list_unique",
	// Hash functions
	"hash_insert", "hash_apply", "hash_delete", "hash_empty", "hash_size",
	"hash_clear", "hash_find", "hash_replace",
	// Bitmap functions
	"bitmap_mark", "bitmap_all", "bitmap_any", "bitmap_contains", "bitmap_count",
	"bitmap_expand", "bitmap_set_all", "bitmap_flip", "bitmap_none", "bitmap_reset",
	"bitmap_scan_and_flip", "bitmap_scan", "bitmap_set", "bitmap_set_multiple", "bitmap_test"
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
	listPool[listIndex].item = (struct list*)malloc(sizeof(struct list));
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

bool getNameOnly(char* commandLine){

	fgets(commandLine, MAX_CMD_LINE, stdin);

	char name[50], buffer[50];
	int argc = sscanf(commandLine, "%s %s", name, buffer);
	
	if(argc>1){
		printf("ERROR : TOO MUCH ARGUMENT. JUST TYPE NAME WITHOUT WHITESPACE\n");
		return false;
	}

	strcpy(commandLine, name);
	return true;
}


int getMatchingList(char* name){
	for(int i=0;i<MAX_ELEMENT;i++){
		if(!strcmp(name, listPool[i].name)) return i;
	}
	return -1;
}

int getMatchingHash(char* name){
	for(int i=0;i<MAX_ELEMENT;i++){
		if(!strcmp(name, hashPool[i].name)) return i;
	}
	return -1;
}

void func_list_front(char* commandLine){
	if(!getNameOnly(commandLine)) return;
	
	int listIndex = getMatchingList(commandLine); 
	if(listIndex < 0) return;
	
	struct list_elem* e = list_front(listPool[listIndex].item);
	struct list_item *temp = list_entry(e, struct list_item, elem);
// 비엇을 때 되는지 확인해야함  근데 예시로 안들어올듯
	printf("%d\n", temp->data);
	
}

void func_list_back(char* commandLine){
	if(!getNameOnly(commandLine)) return;

	int listIndex = getMatchingList(commandLine);
	if(listIndex < 0) return;

	struct list_elem* e = list_back(listPool[listIndex].item);
	struct list_item *temp = list_entry(e, struct list_item, elem);

	printf("%d\n", temp->data);
}

void func_list_push_front(char* commandLine){
	int value;
	fgets(commandLine, MAX_CMD_LINE, stdin);

	char name[50], buffer[50];
	int argc = sscanf(commandLine, "%s %d %s", name, &value, buffer);
	
	if(argc>2){


		printf("ERROR : TOO MUCH ARGUMENT.(list_push_front) \n");
		return;
	}

	int listIndex = getMatchingList(name);
	if(listIndex < 0) return;
	
	struct list_item* new_item = (struct list_item*)malloc(sizeof(struct list_item));
	new_item->data = value;
	
	list_push_front(listPool[listIndex].item, &(new_item->elem));

}

void func_list_push_back(char* commandLine){
	int value;
	fgets(commandLine, MAX_CMD_LINE, stdin);

	char name[50], buffer[50];
	int argc = sscanf(commandLine, "%s %d %s", name, &value, buffer);
	
	if(argc>2){
		printf("ERROR : TOO MUCH ARGUMENT.(list_push_back)\n");
		return;
	}

	int listIndex = getMatchingList(name);
	if(listIndex < 0) return;
	
	struct list_item* new_item = (struct list_item*)malloc(sizeof(struct list_item));
	new_item->data = value;
	
	list_push_back(listPool[listIndex].item, &(new_item->elem));

}

void func_list_pop_front(char* commandLine){
	if(!getNameOnly(commandLine)) return;

	int listIndex = getMatchingList(commandLine);
	if(listIndex < 0) return;

	struct list_elem* e = list_pop_front(listPool[listIndex].item);
	struct list_item *del = list_entry(e, struct list_item, elem);
	
	free(del);
	del = NULL;
}

void func_list_pop_back(char* commandLine){
	if(!getNameOnly(commandLine)) return;

	int listIndex = getMatchingList(commandLine);
	if(listIndex < 0) return;

	struct list_elem* e = list_pop_back(listPool[listIndex].item);
	struct list_item *del = list_entry(e, struct list_item, elem);
	
	free(del);
	del = NULL;
}

void func_list_insert(char* commandLine){
	int value, position;
	fgets(commandLine, MAX_CMD_LINE, stdin);

	char name[50], buffer[50];
	int argc = sscanf(commandLine, "%s %d %d %s", name, &position, &value, buffer);
	
	if(argc>3){
		printf("ERROR : TOO MUCH ARGUMENT.\n");
		return;
	}

	int listIndex = getMatchingList(name);
	if(listIndex < 0) return;
	


	struct list_item* new_item = (struct list_item*)malloc(sizeof(struct list_item));
	new_item->data = value;

	struct list_elem* curr = list_begin(listPool[listIndex].item);
	for(int i = 0; i < position; ++i){
		curr = curr->next;	
	}
	
	list_insert(curr, &(new_item->elem));

}


void func_list_insert_ordered(char* commandLine){
	int value;
	fgets(commandLine, MAX_CMD_LINE, stdin);

	char name[50], buffer[50];
	int argc = sscanf(commandLine, "%s %d %s", name, &value, buffer);
	
	if(argc>2){
		printf("ERROR : TOO MUCH ARGUMENT.\n");
		return;
	}

	int listIndex = getMatchingList(name);
	if(listIndex < 0) return;
	
	struct list_item* new_item = (struct list_item*)malloc(sizeof(struct list_item));
	new_item->data = value;
	
	list_insert_ordered(listPool[listIndex].item, &(new_item->elem), my_list_less_func, 0);
}

void func_list_empty(char* commandLine){
	if(!getNameOnly(commandLine)) return;

	int listIndex = getMatchingList(commandLine);
	if(listIndex < 0) return;
	
	if(list_empty(listPool[listIndex].item)) 
		printf("true\n");
	else 
		printf("false\n");
}

void func_list_size(char* commandLine){
	if(!getNameOnly(commandLine)) return;

	int listIndex = getMatchingList(commandLine);
	if(listIndex < 0) return;
	
	size_t listSize = list_size(listPool[listIndex].item);
	printf("%ld\n",listSize);
}

void func_list_max(char* commandLine){
	if(!getNameOnly(commandLine)) return;

	int listIndex = getMatchingList(commandLine);
	if(listIndex < 0) return;

	struct list_elem* e = list_max(listPool[listIndex].item, my_list_less_func, NULL);
	struct list_item* item = list_entry(e, struct list_item, elem);
	printf("%d\n", item->data);
}

void func_list_min(char* commandLine){
	if(!getNameOnly(commandLine)) return;

	int listIndex = getMatchingList(commandLine);
	if(listIndex < 0) return;

	struct list_elem* e = list_min(listPool[listIndex].item, my_list_less_func, NULL);
	struct list_item* item = list_entry(e, struct list_item, elem);
	printf("%d\n", item->data);
}

	
void func_list_remove(char* commandLine){
	int index;
	fgets(commandLine, MAX_CMD_LINE, stdin);

	char name[50], buffer[50];
	int argc = sscanf(commandLine, "%s %d %s", name, &index, buffer);
	
	if(argc>2){
		printf("ERROR : TOO MUCH ARGUMENT.\n");
		return;
	}

	int listIndex = getMatchingList(name);
	if(listIndex < 0) return;
	
	struct list_elem* e = list_begin(listPool[listIndex].item);
	for(int i=0; i<index; i++)
		e = e->next;
	list_remove(e);
	struct list_item* del = list_entry(e, struct list_item, elem);
	free(del);
	del = NULL;
}

void func_list_reverse(char* commandLine){
	if(!getNameOnly(commandLine)) return;

	int listIndex = getMatchingList(commandLine);
	if(listIndex < 0) return;
	
	list_reverse(listPool[listIndex].item);

}

void func_list_sort(char* commandLine){
	if(!getNameOnly(commandLine)) return;

	int listIndex = getMatchingList(commandLine);
	if(listIndex < 0) return;
	
	list_sort(listPool[listIndex].item, my_list_less_func, 0);

}

void func_list_splice(char* commandLine){
	int pos, first, last;
	fgets(commandLine, MAX_CMD_LINE, stdin);

	char name1[50], name2[50], buffer[50];
	int argc = sscanf(commandLine, "%s %d %s %d %d %s", name1, &pos, name2, &first, &last, buffer);
	
	if(argc>5){
		printf("ERROR : TOO MUCH ARGUMENT.\n");
		return;
	}
	
	int listIndex1 = getMatchingList(name1), listIndex2 = getMatchingList(name2);
	if(listIndex1 < 0 || listIndex2 < 0) return;

	struct list_elem* pos_elem = list_begin(listPool[listIndex1].item);
	for(int i = 0; i < pos; ++i)
		pos_elem = pos_elem->next;	
	
	struct list_elem* first_elem, *last_elem;
	first_elem = last_elem = list_begin(listPool[listIndex2].item);
	for(int i=0;i<first;i++) 
		first_elem = first_elem->next;
	for(int i=0;i<last;i++)
		last_elem = last_elem->next;
	
	list_splice(pos_elem, first_elem, last_elem);

}

void func_list_swap(char* commandLine){
	int pos1, pos2;
	fgets(commandLine, MAX_CMD_LINE, stdin);

	char name[50], buffer[50];
	int argc = sscanf(commandLine, "%s %d %d %s", name, &pos1, &pos2, buffer);
	
	if(argc>3){
		printf("ERROR : TOO MUCH ARGUMENT.\n");
		return;
	}

	int listIndex = getMatchingList(name);
	if(listIndex < 0) return;

	struct list_elem* elem1, *elem2;
	elem1 = elem2 = list_begin(listPool[listIndex].item);
	for(int i=0;i<pos1;i++)
		elem1 = elem1->next;
	for(int i=0;i<pos2;i++)
		elem2 = elem2->next;

	list_swap(elem1, elem2);

}

void func_list_unique(char* commandLine){
	char name1[50], name2[50], buffer[50];	
	fgets(commandLine, MAX_CMD_LINE, stdin);
	int argc = sscanf(commandLine, "%s %s %s", name1, name2, buffer);
	
	if(argc>2){
		printf("ERROR : TOO MUCH ARGUMENT.\n");
		return;
	}
	
	int listIndex1 = getMatchingList(name1), listIndex2 = getMatchingList(name2);
	if(listIndex1 < 0)  return;

	if(argc == 1)
		list_unique(listPool[listIndex1].item, NULL, my_list_less_func, NULL);
	else if(argc == 2 && listIndex2 > -1)
		list_unique(listPool[listIndex1].item, listPool[listIndex2].item, my_list_less_func, NULL);

	return;
}


void func_list_shuffle(char* commandLine){
	if(!getNameOnly(commandLine)) return;

	int index = getMatchingList(commandLine);

	list_shuffle(listPool[index].item);

}

void func_hash_insert(char* commandLine){
	int value;
	fgets(commandLine, MAX_CMD_LINE, stdin);

	char name[50], buffer[50];
	int argc = sscanf(commandLine, "%s %d %s", name, &value, buffer);
	
	if(argc>2){
		printf("ERROR : TOO MUCH ARGUMENT.\n");
		return;
	}
	
	int index = getMatchingHash(name);
	if(index > -1) return;

	struct hash_item* new_item = (struct hash_item*)malloc(sizeof(struct hash_item));
	new_item->data = value;
	hash_insert(hashPool[index].item, &(new_item->elem));

}

void execute(char* commandLine){
	int function = getFunction(commandLine);
	if(function<0) return;

	switch(function){
		/* LIST COMMANDS */
		case LIST_FRONT:
			func_list_front(commandLine);
			break;
		case LIST_BACK:
			func_list_back(commandLine);
			break;
		case LIST_PUSH_FRONT:
			func_list_push_front(commandLine);
			break;
		case LIST_PUSH_BACK:
			func_list_push_back(commandLine);
			break;
		case LIST_POP_FRONT:
			func_list_pop_front(commandLine);
			break;
		case LIST_POP_BACK:
			func_list_pop_back(commandLine);
			break;
		case LIST_INSERT:
			func_list_insert(commandLine);
			break;
		case LIST_INSERT_ORDERED:
			func_list_insert_ordered(commandLine);
			break;
		case LIST_EMPTY:
			func_list_empty(commandLine);
			break;
		case LIST_SIZE:
			func_list_size(commandLine);
			break;
		case LIST_MAX:
			func_list_max(commandLine);
			break;
		case LIST_MIN:
			func_list_min(commandLine);
			break;
		case LIST_REMOVE:
			func_list_remove(commandLine);
			break;
		case LIST_REVERSE:
			func_list_reverse(commandLine);
			break;
		case LIST_SORT:
			func_list_sort(commandLine);
			break;
		case LIST_SPLICE:
			func_list_splice(commandLine);
			break;
		case LIST_SWAP:
			func_list_swap(commandLine);
			break;
		case LIST_SHUFFLE:
			func_list_shuffle(commandLine);
			break;
		case LIST_UNIQUE:
			func_list_unique(commandLine);
			break;
		/* HASH COMMANDS */
		case HASH_INSERT:
			func_hash_insert(commandLine);
			break;
		case HASH_APPLY:
			
			break;
		case HASH_DELETE:

			break;
		case HASH_EMPTY:

			break;
		case HASH_SIZE:

			break;
		case HASH_CLEAR:

			break;
		case HASH_FIND:
			
			break;
		case HASH_REPLACE:

			break;
		/* BITMAP COMMANDS */
		case BITMAP_MARK:

			break;
		case BITMAP_ALL:

			break;
		case BITMAP_ANY:

			break;
		case BITMAP_CONTAINS:

			break;
		case BITMAP_COUNT:

			break;
		case BITMAP_EXPAND:

			break;
		case BITMAP_SET_ALL:

			break;
		case BITMAP_FLIP:

			break;
		case BITMAP_NONE:

			break;
		case BITMAP_RESET:

			break;
		case BITMAP_SCAN_AND_FLIP:

			break;
		case BITMAP_SCAN:

			break;
		case BITMAP_SET:

			break;
		case BITMAP_SET_MULTIPLE:

			break;
		case BITMAP_TEST:

			break;
		default:
			break;
	}

	


}


int getEmptyHash(){
	for(int i=0; i<MAX_ELEMENT;i++)
		if(!hashPool[i]) return i;
	return -1;
}

void createHash(char* name){
	int hashIndex = getEmptyList();
	if(hashIndex<0) return;
	
	strcpy(hashPool[hashIndex].name, name);
	hashPool[hashIndex].item = (struct hash*)malloc(sizeof(struct hash));
	hash_init(hashPool[hashIndex].item, my_hash_func, my_hash_less_func, NULL);

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
			createHash(name);
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
		strcpy(listPool[index].name, "\0");

		struct list_elem* e = list_begin(listPool[index].item);
		while(e != list_end(listPool[index].item)){
			struct list_item* del = list_entry(e, struct list_item, elem);
			e = list_remove(e);
			free(del);
			del = NULL;
		}
		
		free(listPool[index].item);
		listPool[index].item = NULL;
	}

	return index;
}

int deleteHash(char* name){
	int index = -1;

	for(int i=0;i<MAX_ELEMENT;i++){
		if(!strcmp(name,hashPool[i].name)) {
			index = i;
			break;
		}
	}

	if(index>=0){
		strcpy(hashPool[index].name, "\0");
// TODO : 여기 해야 함!!!
		/*
		struct list_elem* e = list_begin(listPool[index].item);
		while(e != list_end(listPool[index].item)){
			struct list_item* del = list_entry(e, struct list_item, elem);
			e = list_remove(e);
			free(del);
			del = NULL;
		}
		*/
		
		free(hashPool[index].item);
		hashPool[index].item = NULL;
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
		free(hashPool[i].item);
	}

	if(deleteList(name)>-1) return;
	
	if(deleteHash(name)>-1) return;


	printf("MESSAGE : NO MATCHED NAME FOUND\n");
	return;
	
}


void dumpdata(char* commandLine){
	if(!getNameOnly(commandLine)) return;

	// 리스트에서 탐색
	int index = getMatchingList(commandLine);
	bool hasData = false;
	if(index>-1){
		struct list_elem* e = list_head(listPool[index].item);
		struct list_item* i;
		while((e = list_next (e)) != list_end(listPool[index].item)){
			i = list_entry(e, struct list_item, elem);
			printf("%d ", i->data);
			hasData = true;
		}
		if(hasData)
			printf("\n");
		return;
	}

	// 해시에서 탐색

	// 비트맵에서 탐색
	

}

void parser(){
	char commandLine[MAX_CMD_LINE];
	while(true){
		COMMAND command = getCommand(commandLine);
		//getchar();

		switch(command){
			case CREATE:
				getchar();
				create(commandLine);
				break;
			case DELETE: 
				getchar();
				delete(commandLine);
				break;
			case DUMPDATA:
				dumpdata(commandLine);
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
	for(int i=0; i<MAX_ELEMENT; i++){
		listPool[i].item = NULL;
		strcpy(listPool[i].name, "\0");

		hashPool[i].item = NULL;
		strcpy(hashPool[i].name, "\0");
	}
	
}

void freeData(){
	for(int i=0;i<MAX_ELEMENT;i++){
		if(listPool[i].item){
			struct list_elem* e = list_begin(listPool[i].item);
			while(e != list_end(listPool[i].item)){
				struct list_item* del = list_entry(e, struct list_item, elem);
				e = list_remove(e);
				free(del);
				del = NULL;
			}
			free(listPool[i].item);
			listPool[i].item = NULL;
		}

		if(hashPool[i]){
			// TODO : free hashPool;
			free(hashPool[i].item);
			hashPool[i].item = NULL;
		}
	}
}

int main(int argc, char *argv[]){
	init();

	parser();

	freeData();
	return 0;

}
