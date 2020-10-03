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
#include "bitmap.h"

#define MAX_CMD_LINE 50
#define MAX_ELEMENT 11
/* decide what kind of the command is.
   "OTHERS" means it is data structure dependent command */
typedef enum {CREATE, DELETE, DUMPDATA, QUIT, OTHERS} COMMAND;

struct namedList listPool[MAX_ELEMENT];
struct namedHash hashPool[MAX_ELEMENT];
struct namedBitmap bitmapPool[MAX_ELEMENT];

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
	BITMAP_SCAN_AND_FLIP, BITMAP_SCAN, BITMAP_SET, BITMAP_SET_MULTIPLE, BITMAP_TEST,
	BITMAP_DUMP, BITMAP_SIZE
}FUNCTION;

#define FUNCTION_CNT (BITMAP_SIZE + 1)

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
	"bitmap_scan_and_flip", "bitmap_scan", "bitmap_set", "bitmap_set_multiple", "bitmap_test",
	"bitmap_dump", "bitmap_size"
};

FUNCTION getFunction(char* functionName){
	for(int i=0;i<FUNCTION_CNT;i++){
		if(!strcmp(functionName, functionNameList[i])) return i;
	}
	return -1;
}



int getEmptyList(){
	for(int i=0;i<MAX_ELEMENT;i++){
		if(!listPool[i].item) {
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
		fprintf(stdout, "ERROR : TOO MUCH ARGUMENT. JUST TYPE NAME WITHOUT WHITESPACE\n");
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

int getMatchingBitmap(char* name){
	for(int i=0;i<MAX_ELEMENT;i++){
		if(!strcmp(name, bitmapPool[i].name)) return i;
	}
	return -1;
}

void func_list_front(char* name){
	if(!getNameOnly(name)) return;
	
	int listIndex = getMatchingList(name); 
	if(listIndex < 0) return;
	
	if(list_empty(listPool[listIndex].item)) return;

	struct list_elem* e = list_front(listPool[listIndex].item);
	struct list_item *temp = list_entry(e, struct list_item, elem);
	fprintf(stdout, "%d\n", temp->data);
	
}

void func_list_back(char* name){
	if(!getNameOnly(name)) return;

	int listIndex = getMatchingList(name);
	if(listIndex < 0) return;

	if(list_empty(listPool[listIndex].item)) return;

	struct list_elem* e = list_back(listPool[listIndex].item);
	struct list_item *temp = list_entry(e, struct list_item, elem);

	fprintf(stdout, "%d\n", temp->data);
}

void func_list_push_front(char* commandLine){
	int value;
	fgets(commandLine, MAX_CMD_LINE, stdin);

	char name[50], buffer[50];
	int argc = sscanf(commandLine, "%s %d %s", name, &value, buffer);
	
	if(argc>2){


		fprintf(stdout, "ERROR : TOO MUCH ARGUMENT.(list_push_front) \n");
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
		fprintf(stdout, "ERROR : TOO MUCH ARGUMENT.(list_push_back)\n");
		return;
	}

	int listIndex = getMatchingList(name);
	if(listIndex < 0) return;
	
	struct list_item* new_item = (struct list_item*)malloc(sizeof(struct list_item));
	new_item->data = value;
	
	list_push_back(listPool[listIndex].item, &(new_item->elem));

}

void func_list_pop_front(char* name){
	if(!getNameOnly(name)) return;

	int listIndex = getMatchingList(name);
	if(listIndex < 0) return;

	struct list_elem* e = list_pop_front(listPool[listIndex].item);
	struct list_item *del = list_entry(e, struct list_item, elem);
	
	free(del);
	del = NULL;
}

void func_list_pop_back(char* name){
	if(!getNameOnly(name)) return;

	int listIndex = getMatchingList(name);
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
		fprintf(stdout, "ERROR : TOO MUCH ARGUMENT.\n");
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
		fprintf(stdout, "ERROR : TOO MUCH ARGUMENT.\n");
		return;
	}

	int listIndex = getMatchingList(name);
	if(listIndex < 0) return;
	
	struct list_item* new_item = (struct list_item*)malloc(sizeof(struct list_item));
	new_item->data = value;
	
	list_insert_ordered(listPool[listIndex].item, &(new_item->elem), my_list_less_func, NULL);
}

void func_list_empty(char* name){
	if(!getNameOnly(name)) return;

	int listIndex = getMatchingList(name);
	if(listIndex < 0) return;
	
	if(list_empty(listPool[listIndex].item)) 
		fprintf(stdout, "true\n");
	else 
		fprintf(stdout, "false\n");
}

void func_list_size(char* name){
	if(!getNameOnly(name)) return;

	int listIndex = getMatchingList(name);
	if(listIndex < 0) return;
	
	size_t listSize = list_size(listPool[listIndex].item);
	fprintf(stdout, "%zu\n",listSize);
}

void func_list_max(char* name){
	if(!getNameOnly(name)) return;

	int listIndex = getMatchingList(name);
	if(listIndex < 0) return;

	struct list_elem* e = list_max(listPool[listIndex].item, my_list_less_func, NULL);
	struct list_item* item = list_entry(e, struct list_item, elem);
	fprintf(stdout, "%d\n", item->data);
}

void func_list_min(char* name){
	if(!getNameOnly(name)) return;

	int listIndex = getMatchingList(name);
	if(listIndex < 0) return;

	struct list_elem* e = list_min(listPool[listIndex].item, my_list_less_func, NULL);
	struct list_item* item = list_entry(e, struct list_item, elem);
	fprintf(stdout, "%d\n", item->data);
}

	
void func_list_remove(char* commandLine){
	int index;
	fgets(commandLine, MAX_CMD_LINE, stdin);

	char name[50], buffer[50];
	int argc = sscanf(commandLine, "%s %d %s", name, &index, buffer);
	
	if(argc>2){
		fprintf(stdout, "ERROR : TOO MUCH ARGUMENT.\n");
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

void func_list_reverse(char* name){
	if(!getNameOnly(name)) return;

	int listIndex = getMatchingList(name);
	if(listIndex < 0) return;
	
	list_reverse(listPool[listIndex].item);

}

void func_list_sort(char* name){
	if(!getNameOnly(name)) return;

	int listIndex = getMatchingList(name);
	if(listIndex < 0) return;
	
	list_sort(listPool[listIndex].item, my_list_less_func, 0);

}

void func_list_splice(char* commandLine){
	int pos, first, last;
	fgets(commandLine, MAX_CMD_LINE, stdin);

	char name1[50], name2[50], buffer[50];
	int argc = sscanf(commandLine, "%s %d %s %d %d %s", name1, &pos, name2, &first, &last, buffer);
	
	if(argc>5){
		fprintf(stdout, "ERROR : TOO MUCH ARGUMENT.\n");
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
		fprintf(stdout, "ERROR : TOO MUCH ARGUMENT.\n");
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
		fprintf(stdout, "ERROR : TOO MUCH ARGUMENT.\n");
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


void func_list_shuffle(char* name){
	if(!getNameOnly(name)) return;

	int index = getMatchingList(name);

	list_shuffle(listPool[index].item);

}

void func_hash_insert(char* commandLine){
	int value;
	fgets(commandLine, MAX_CMD_LINE, stdin);

	char name[50], buffer[50];
	int argc = sscanf(commandLine, "%s %d %s", name, &value, buffer);

	if(argc>2){
		fprintf(stdout, "ERROR : TOO MUCH ARGUMENT.\n");
		return;
	}

	int index = getMatchingHash(name);
	if(index < 0) return;

	struct hash_item* new_item = (struct hash_item*)malloc(sizeof(struct hash_item));
	new_item->data = value;
	hash_insert(hashPool[index].item, &(new_item->elem));

}

void func_hash_apply(char* commandLine){
	fgets(commandLine, MAX_CMD_LINE, stdin);

	char name[50], apply[50], buffer[50];
	int argc = sscanf(commandLine, "%s %s %s", name, apply, buffer);

	if(argc != 2){
		fprintf(stdout, "ERROR : ARGUMENT NOT MATCHED\n");
		return;
	}

	int index = getMatchingHash(name);
	if(index < 0) return;

	if(!strcmp(apply, "square")){
		hash_apply(hashPool[index].item, square_func);
	}
	else if(!strcmp(apply, "triple")){
		hash_apply(hashPool[index].item, triple_func);
	}	

}

void func_hash_delete(char* commandLine){
	fgets(commandLine, MAX_CMD_LINE, stdin);

	char name[50], buffer[50];
	int value;
	int argc = sscanf(commandLine, "%s %d %s", name, &value, buffer);

	if(argc != 2){
		fprintf(stdout, "ERROR : ARGUMENT NOT MATCHED\n");
		return;
	}

	int index = getMatchingHash(name);
	if(index < 0) return;
	
	struct hash_item i;
	i.data = value;

	struct hash_elem* target = hash_find(hashPool[index].item, &(i.elem));
	if(target) {
		struct hash_item* del = hash_entry(target, struct hash_item, elem);
		hash_delete(hashPool[index].item, target);
		free(del);
	}

}

void func_hash_empty(char* name){
	if(!getNameOnly(name)) return;

	int index = getMatchingHash(name);
	if(index < 0) return;

	if(hash_empty(hashPool[index].item)) fprintf(stdout, "true\n");
	else fprintf(stdout, "false\n");

}

void func_hash_size(char* name){
	if(!getNameOnly(name)) return;

	int index = getMatchingHash(name);
	if(index < 0) return;

	fprintf(stdout, "%zu\n", hash_size(hashPool[index].item));

}

void func_hash_clear(char* name){
	if(!getNameOnly(name)) return;

	int index = getMatchingHash(name);
	if(index < 0) return;

	hash_clear(hashPool[index].item, dealloc_func);

}

void func_hash_find(char* commandLine){
	fgets(commandLine, MAX_CMD_LINE, stdin);

	char name[50], buffer[50];
	int value;
	int argc = sscanf(commandLine, "%s %d %s", name, &value, buffer);

	if(argc != 2){
		fprintf(stdout, "ERROR : ARGUMENT NOT MATCHED\n");
		return;
	}

	int index = getMatchingHash(name);
	if(index < 0) return;
	
	struct hash_item i;
	i.data = value;

	struct hash_elem* target = hash_find(hashPool[index].item, &(i.elem));
	if(target){
		struct hash_item* item = hash_entry(target, struct hash_item, elem);
		fprintf(stdout, "%d\n", item->data);
	}
}

void func_hash_replace(char* commandLine){
	fgets(commandLine, MAX_CMD_LINE, stdin);

	char name[50], buffer[50];
	int value;
	int argc = sscanf(commandLine, "%s %d %s", name, &value, buffer);

	if(argc != 2){
		fprintf(stdout, "ERROR : ARGUMENT NOT MATCHED\n");
		return;
	}

	int index = getMatchingHash(name);
	if(index < 0) return;
	
	struct hash_item* new_item = (struct hash_item*)malloc(sizeof(struct hash_item));
	new_item->data = value;
	
	struct hash_elem* old = hash_replace(hashPool[index].item, &(new_item->elem));
	struct hash_item* del = hash_entry(old, struct hash_item, elem);
	free(del);
}

void func_bitmap_mark(char* commandLine){
	fgets(commandLine, MAX_CMD_LINE, stdin);

	char name[50], buffer[50];
	size_t bit_index;
	int argc = sscanf(commandLine, "%s %zu %s", name, &bit_index, buffer);

	if(argc != 2){
		fprintf(stdout, "ERROR : ARGUMENT NOT MATCHED\n");
		return;
	}

	int index = getMatchingBitmap(name);
	if(index < 0) return;

	bitmap_mark(bitmapPool[index].item, bit_index);
}


void func_bitmap_all(char* commandLine){
	fgets(commandLine, MAX_CMD_LINE, stdin);

	char name[50], buffer[50];
	size_t start, cnt;
	int argc = sscanf(commandLine, "%s %zu %zu %s", name, &start, &cnt, buffer);

	if(argc != 3){
		fprintf(stdout, "ERROR : ARGUMENT NOT MATCHED\n");
		return;
	}

	int index = getMatchingBitmap(name);
	if(index < 0) return;
	
	fprintf(stdout, bitmap_all(bitmapPool[index].item, start, cnt) ? "true\n" : "false\n");
}



void func_bitmap_any(char* commandLine){
	fgets(commandLine, MAX_CMD_LINE, stdin);

	char name[50], buffer[50];
	size_t start, cnt;
	int argc = sscanf(commandLine, "%s %zu %zu %s", name, &start, &cnt, buffer);

	if(argc != 3){
		fprintf(stdout, "ERROR : ARGUMENT NOT MATCHED\n");
		return;
	}

	int index = getMatchingBitmap(name);
	if(index < 0) return;
	
	fprintf(stdout, bitmap_any(bitmapPool[index].item, start, cnt) ? "true\n" : "false\n");
}

void func_bitmap_contains(char* commandLine){
	fgets(commandLine, MAX_CMD_LINE, stdin);

	char name[50], value[10], buffer[50];
	size_t start, cnt;
	int argc = sscanf(commandLine, "%s %zu %zu %s %s", name, &start, &cnt, value, buffer);

	if(argc != 4){
		fprintf(stdout, "ERROR : ARGUMENT NOT MATCHED\n");
		return;
	}

	int index = getMatchingBitmap(name);
	if(index < 0) return;
	
	int result = -1;
	if(!strcmp(value, "true"))
		result = bitmap_contains(bitmapPool[index].item, start, cnt, 1);
	else if(!strcmp(value, "false"))
		result = bitmap_contains(bitmapPool[index].item, start, cnt, 0);
	
	if(result>-1)
		fprintf(stdout, result ? "true\n" : "false\n");

}

void func_bitmap_count(char* commandLine){
	fgets(commandLine, MAX_CMD_LINE, stdin);

	char name[50], value[10], buffer[50];
	size_t start, cnt;
	int argc = sscanf(commandLine, "%s %zu %zu %s %s", name, &start, &cnt, value, buffer);

	if(argc != 4){
		fprintf(stdout, "ERROR : ARGUMENT NOT MATCHED\n");
		return;
	}

	int index = getMatchingBitmap(name);
	if(index < 0) return;
	
	int result = -1;
	if(!strcmp(value, "true"))
		result = bitmap_count(bitmapPool[index].item, start, cnt, 1);
	else if(!strcmp(value, "false"))
		result = bitmap_count(bitmapPool[index].item, start, cnt, 0);
	
	if(result > -1)
		fprintf(stdout,"%d\n", result); 
}

void func_bitmap_expand(char* commandLine){
	fgets(commandLine, MAX_CMD_LINE, stdin);

	char name[50], buffer[50];
	size_t cnt;
	int argc = sscanf(commandLine, "%s %zu %s", name, &cnt, buffer);

	if(argc != 2){
		fprintf(stdout, "ERROR : ARGUMENT NOT MATCHED\n");
		return;
	}

	int index = getMatchingBitmap(name);
	if(index < 0) return;
		
	bitmapPool[index].item = bitmap_expand(bitmapPool[index].item, cnt);

}

void func_bitmap_set_all(char* commandLine){
	fgets(commandLine, MAX_CMD_LINE, stdin);

	char name[50], value[10], buffer[50];
	int argc = sscanf(commandLine, "%s %s %s", name, value, buffer);

	if(argc != 2){
		fprintf(stdout, "ERROR : ARGUMENT NOT MATCHED\n");
		return;
	}

	int index = getMatchingBitmap(name);
	if(index < 0) return;
	
	if(!strcmp(value, "true")){
		bitmap_set_all(bitmapPool[index].item, 1);
	}
	else if(!strcmp(value, "false")){
		bitmap_set_all(bitmapPool[index].item, 0);
	}
}

void func_bitmap_flip(char* commandLine){
	fgets(commandLine, MAX_CMD_LINE, stdin);

	char name[50], buffer[50];
	size_t bit_idx;
	int argc = sscanf(commandLine, "%s %zu %s", name, &bit_idx, buffer);

	if(argc != 2){
		fprintf(stdout, "ERROR : ARGUMENT NOT MATCHED\n");
		return;
	}

	int index = getMatchingBitmap(name);
	if(index < 0) return;
	
	bitmap_flip(bitmapPool[index].item, bit_idx);

}

void func_bitmap_none(char* commandLine){
	fgets(commandLine, MAX_CMD_LINE, stdin);

	char name[50], buffer[50];
	size_t start, cnt;
	int argc = sscanf(commandLine, "%s %zu %zu %s", name, &start, &cnt, buffer);

	if(argc != 3){
		fprintf(stdout, "ERROR : ARGUMENT NOT MATCHED\n");
		return;
	}

	int index = getMatchingBitmap(name);
	if(index < 0) return;
	
	if(bitmap_none(bitmapPool[index].item, start, cnt)) 
		fprintf(stdout, "true\n");
	else
		fprintf(stdout, "false\n");

}

void func_bitmap_reset(char* commandLine){
	fgets(commandLine, MAX_CMD_LINE, stdin);

	char name[50], buffer[50];
	size_t bit_idx;
	int argc = sscanf(commandLine, "%s %zu %s", name, &bit_idx, buffer);

	if(argc != 2){
		fprintf(stdout, "ERROR : ARGUMENT NOT MATCHED\n");
		return;
	}

	int index = getMatchingBitmap(name);
	if(index < 0) return;
	
	bitmap_reset(bitmapPool[index].item, bit_idx);
}

void func_bitmap_scan_and_flip(char* commandLine){
	fgets(commandLine, MAX_CMD_LINE, stdin);

	char name[50], value[10], buffer[50];
	size_t start, cnt;
	int argc = sscanf(commandLine, "%s %zu %zu %s %s", name, &start, &cnt, value, buffer);

	if(argc != 4){
		fprintf(stdout, "ERROR : ARGUMENT NOT MATCHED\n");
		return;
	}

	int index = getMatchingBitmap(name);
	if(index < 0) return;

	size_t result;
	if(!strcmp(value, "true"))
		result = bitmap_scan_and_flip(bitmapPool[index].item, start, cnt, 1);
	else if(!strcmp(value, "false"))	
		result = bitmap_scan_and_flip(bitmapPool[index].item, start, cnt, 0);

	fprintf(stdout, "%zu\n", result);
}

void func_bitmap_scan(char* commandLine){
	fgets(commandLine, MAX_CMD_LINE, stdin);

	char name[50], value[10], buffer[50];
	size_t start, cnt;
	int argc = sscanf(commandLine, "%s %zu %zu %s %s", name, &start, &cnt, value, buffer);

	if(argc != 4){
		fprintf(stdout, "ERROR : ARGUMENT NOT MATCHED\n");
		return;
	}

	int index = getMatchingBitmap(name);
	if(index < 0) return;
	
	size_t result;
	if(!strcmp(value, "true"))
		result = bitmap_scan(bitmapPool[index].item, start, cnt, 1);
	else if(!strcmp(value, "false"))	
		result = bitmap_scan(bitmapPool[index].item, start, cnt, 0);

	fprintf(stdout, "%zu\n", result);
}

void func_bitmap_set(char* commandLine){
	fgets(commandLine, MAX_CMD_LINE, stdin);

	char name[50], value[10], buffer[50];
	size_t idx; 
	int argc = sscanf(commandLine, "%s %zu %s %s", name, &idx, value, buffer);

	if(argc != 3){
		fprintf(stdout, "ERROR : ARGUMENT NOT MATCHED\n");
		return;
	}

	int index = getMatchingBitmap(name);
	if(index < 0) return;
	
	if(!strcmp(value, "true"))
		bitmap_set(bitmapPool[index].item, idx, 1);
	if(!strcmp(value, "false"))
		bitmap_set(bitmapPool[index].item, idx, 0);
}

void func_bitmap_set_multiple(char* commandLine){
	fgets(commandLine, MAX_CMD_LINE, stdin);

	char name[50], value[10], buffer[50];
	size_t start, cnt; 
	int argc = sscanf(commandLine, "%s %zu %zu %s %s", name, &start, &cnt, value, buffer);

	if(argc != 4){
		fprintf(stdout, "ERROR : ARGUMENT NOT MATCHED\n");
		return;
	}

	int index = getMatchingBitmap(name);
	if(index < 0) return;
	
	if(!strcmp(value, "true"))
		bitmap_set_multiple(bitmapPool[index].item, start, cnt, 1);
	if(!strcmp(value, "false"))
		bitmap_set_multiple(bitmapPool[index].item, start, cnt, 0);

}

void func_bitmap_test(char* commandLine){
	fgets(commandLine, MAX_CMD_LINE, stdin);

	char name[50], buffer[50];
	size_t idx; 
	int argc = sscanf(commandLine, "%s %zu %s", name, &idx, buffer);

	if(argc != 2){
		fprintf(stdout, "ERROR : ARGUMENT NOT MATCHED\n");
		return;
	}

	int index = getMatchingBitmap(name);
	if(index < 0) return;
	
	fprintf(stdout, bitmap_test(bitmapPool[index].item, idx) ? "true\n" : "false\n");
}


void func_bitmap_dump(char* name){
	if(!getNameOnly(name)) return;

	int index = getMatchingBitmap(name);
	if(index < 0) return;

	bitmap_dump(bitmapPool[index].item);
}

void func_bitmap_size(char* name){
	if(!getNameOnly(name)) return;

	int index = getMatchingBitmap(name);
	if(index < 0) return;

	fprintf(stdout, "%zu\n", bitmap_size(bitmapPool[index].item));
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
			func_hash_apply(commandLine);
			break;
		case HASH_DELETE:
			func_hash_delete(commandLine);
			break;
		case HASH_EMPTY:
			func_hash_empty(commandLine);
			break;
		case HASH_SIZE:
			func_hash_size(commandLine);
			break;
		case HASH_CLEAR:
			func_hash_clear(commandLine);
			break;
		case HASH_FIND:
			func_hash_find(commandLine);
			break;
		case HASH_REPLACE:
			func_hash_replace(commandLine);
			break;
		/* BITMAP COMMANDS */
		case BITMAP_MARK:
			func_bitmap_mark(commandLine);
			break;
		case BITMAP_ALL:
			func_bitmap_all(commandLine);
			break;
		case BITMAP_ANY:
			func_bitmap_any(commandLine);
			break;
		case BITMAP_CONTAINS:
			func_bitmap_contains(commandLine);
			break;
		case BITMAP_COUNT:
			func_bitmap_count(commandLine);
			break;
		case BITMAP_EXPAND:
			func_bitmap_expand(commandLine);
			break;
		case BITMAP_SET_ALL:
			func_bitmap_set_all(commandLine);
			break;
		case BITMAP_FLIP:
			func_bitmap_flip(commandLine);
			break;
		case BITMAP_NONE:
			func_bitmap_none(commandLine);
			break;
		case BITMAP_RESET:
			func_bitmap_reset(commandLine);
			break;
		case BITMAP_SCAN_AND_FLIP:
			func_bitmap_scan_and_flip(commandLine);
			break;
		case BITMAP_SCAN:
			func_bitmap_scan(commandLine);
			break;
		case BITMAP_SET:
			func_bitmap_set(commandLine);
			break;
		case BITMAP_SET_MULTIPLE:
			func_bitmap_set_multiple(commandLine);
			break;
		case BITMAP_TEST:
			func_bitmap_test(commandLine);
			break;
		case BITMAP_DUMP:
			func_bitmap_dump(commandLine);
			break;
		case BITMAP_SIZE:
			func_bitmap_size(commandLine);
			break;
		default:
			break;
	}

}


int getEmptyHash(){
	for(int i=0; i<MAX_ELEMENT;i++)
		if(!hashPool[i].item) return i;
	return -1;
}

void createHash(char* name){
	int hashIndex = getEmptyHash();
	if(hashIndex<0) return;
	
	strcpy(hashPool[hashIndex].name, name);
	hashPool[hashIndex].item = (struct hash*)malloc(sizeof(struct hash));
	hash_init(hashPool[hashIndex].item, my_hash_func, my_hash_less_func, NULL);

}

int getEmptyBitmap(){
	for(int i=0;i<MAX_ELEMENT;i++)
		if(!bitmapPool[i].item) return i;
	return -1;
}

void createBitmap(char* name, int bitmap_cnt){
	int bitmapIndex = getEmptyBitmap();
	if(bitmapIndex < 0) return;

	strcpy(bitmapPool[bitmapIndex].name, name);
	bitmapPool[bitmapIndex].item = bitmap_create(bitmap_cnt);

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
		fprintf(stdout, "ERROR : INVALID DATA STRUCTURE. CANNOT CREATE.\n");
		return;
	}
	
	fgets(commandLine, MAX_CMD_LINE, stdin);

	char name[50], buffer[50];
	int bitmap_cnt = -1;
	int argc = sscanf(commandLine, "%s %d %s", name, &bitmap_cnt, buffer);
	
	if(argc>2){
		fprintf(stdout, "ERROR : TOO MUCH ARGUMENT. JUST TYPE NAME WITHOUT WHITESPACE\n");
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
			createBitmap(name, bitmap_cnt);
			break;
		default:
			break;
	}
}


int deleteList(char* name){
	int index = getMatchingList(name);

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
	int index = getMatchingHash(name);

	if(index>=0){
		struct hash* hashPtr = hashPool[index].item;
		struct hash_iterator iter;
		
		for(hash_first(&iter, hashPtr), hash_next(&iter);;){
			if(!hash_cur(&iter)) break;
			struct hash_elem* e = hash_delete(hashPtr, hash_cur(&iter));
			struct hash_item* del = hash_entry(e, struct hash_item, elem);
			hash_next(&iter);
			free(del);
		}
		strcpy(hashPool[index].name, "\0");
		free(hashPool[index].item);
		hashPool[index].item = NULL;
	}

	return index;
}

int deleteBitmap(char* name){
	int index = getMatchingBitmap(name);

	if(index>=0){
		bitmap_destroy(bitmapPool[index].item);
		bitmapPool[index].item = NULL;
		strcpy(bitmapPool[index].name, "\0");
	}

	return index;
}

void delete(char* commandLine){
	fgets(commandLine, MAX_CMD_LINE, stdin);

	char name[50], buffer[50];
	int argc = sscanf(commandLine, "%s %s", name, buffer);

	if(argc > 1){
		fprintf(stdout, "ERROR : TOO MUCH ARGUMENT. JUST TYPE THE NAME WITHOUT WHITSPACE\n");
		return;
	}

	if(deleteList(name)>-1) return;
	
	if(deleteHash(name)>-1) return;
	
	if(deleteBitmap(name)>-1) return;

	fprintf(stdout, "MESSAGE : NO MATCHED NAME FOUND\n");
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
			fprintf(stdout, "%d ", i->data);
			hasData = true;
		}
		if(hasData)
			fprintf(stdout, "\n");
		return;
	}
	// 해시에서 탐색
	index = getMatchingHash(commandLine);
	if(index>-1){
		struct hash* hashPtr = hashPool[index].item;
		struct hash_iterator iter;
		hash_first(&iter, hashPtr);

		while(hash_next(&iter)){
			struct hash_item* item = hash_entry(hash_cur(&iter), struct hash_item, elem);
			fprintf(stdout, "%d ", item->data);
			hasData = true;
		}
		if(hasData) fprintf(stdout, "\n");
		return;
	}

	// 비트맵에서 탐색
	index = getMatchingBitmap(commandLine);
	if(index>-1){
		struct bitmap* bitmap = bitmapPool[index].item;
		size_t size = bitmap_size(bitmap);
		for(size_t idx=0; idx<size; idx++){
			fprintf(stdout, "%d", bitmap_test(bitmap, idx));
		}
		fprintf(stdout, "\n");
		return;
	}

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

		bitmapPool[i].item = NULL;
		strcpy(bitmapPool[i].name, "\0");
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
			strcpy(listPool[i].name, "\0");
			free(listPool[i].item);
			listPool[i].item = NULL;
		}

		if(hashPool[i].item){
			struct hash* hashPtr = hashPool[i].item;
			struct hash_iterator iter;
			
			for(hash_first(&iter, hashPtr), hash_next(&iter);;){
				if(!hash_cur(&iter)) break;
				struct hash_elem* e = hash_delete(hashPtr, hash_cur(&iter));
				struct hash_item* del = hash_entry(e, struct hash_item, elem);
				hash_next(&iter);
				free(del);
			}
			strcpy(hashPool[i].name, "\0");
			free(hashPool[i].item);
			hashPool[i].item = NULL;

		}

		if(bitmapPool[i].item){
			bitmap_destroy(bitmapPool[i].item);
			bitmapPool[i].item = NULL;
			strcpy(bitmapPool[i].name, "\0");
		}
	}
}

int main(int argc, char *argv[]){
	init();

	parser();

	freeData();
	return 0;

}
