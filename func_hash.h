#ifndef __FUNC_HASH__
#define __FUNC_HASH__





int getMatchingHash(char* name);
int getEmptyHash();

void createHash(char* name);
int deleteHash(char* name);



void func_hash_insert(char* commandLine);
void func_hash_apply(char* commandLine);
void func_hash_delete(char* commandLine);
void func_hash_empty(char* name);
void func_hash_size(char* name);
void func_hash_clear(char* name);
void func_hash_find(char* commandLine);
void func_hash_replace(char* commandLine);









#endif
