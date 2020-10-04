#ifndef __LIST_FUNC__
#define __LIST_FUNC__


int getEmptyList();
int getMatchingList(char* name);

void createList(char* name);
int deleteList(char* name);

void func_list_front(char* name);
void func_list_back(char* name);
void func_list_push_front(char* commandLine);
void func_list_push_back(char* commandLine);
void func_list_pop_front(char* name);
void func_list_pop_back(char* name);
void func_list_insert(char* commandLine);
void func_list_insert_ordered(char* commandLine);
void func_list_empty(char* name);
void func_list_size(char* name);
void func_list_max(char* name);
void func_list_min(char* name);
void func_list_remove(char* commandLine);
void func_list_reverse(char* name);
void func_list_sort(char* name);
void func_list_splice(char* commandLine);
void func_list_swap(char* commandLine);
void func_list_unique(char* commandLine);
void func_list_shuffle(char* name);


#endif
