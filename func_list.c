#include "func_list.h"
#include "common.h"
#include "list.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

extern struct namedList listPool[MAX_ELEMENT];

int getMatchingList(char* name){
	for(int i=0;i<MAX_ELEMENT;i++){
		if(!strcmp(name, listPool[i].name)) return i;
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


void createList(char* name){
	int listIndex = getEmptyList();
	if(listIndex<0) return;

	strcpy(listPool[listIndex].name, name);
	listPool[listIndex].item = (struct list*)malloc(sizeof(struct list));
	list_init(listPool[listIndex].item);

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
