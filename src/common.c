#include "common.h"
#include <string.h>
#include <stdio.h>
#include "list.h"
#include "hash.h"
#include "bitmap.h"
#include "func_list.h"
#include "func_hash.h"
#include "func_bitmap.h"

extern struct namedList listPool[MAX_ELEMENT];
extern struct namedHash hashPool[MAX_ELEMENT];
extern struct namedBitmap bitmapPool[MAX_ELEMENT];

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
