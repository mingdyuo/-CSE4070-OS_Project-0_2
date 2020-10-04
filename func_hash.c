#include "common.h"
#include "hash.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern struct namedHash hashPool[MAX_ELEMENT];


int getMatchingHash(char* name){
	for(int i=0;i<MAX_ELEMENT;i++){
		if(!strcmp(name, hashPool[i].name)) return i;
	}
	return -1;
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
