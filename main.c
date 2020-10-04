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
#include "common.h"
#include "func_list.h"
#include "func_hash.h"
#include "func_bitmap.h"

struct namedList listPool[MAX_ELEMENT];
struct namedHash hashPool[MAX_ELEMENT];
struct namedBitmap bitmapPool[MAX_ELEMENT];


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
