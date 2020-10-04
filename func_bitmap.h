#ifndef __FUNC_BITMAP__
#define __FUNC_BITMAP__

int getMatchingBitmap(char* name);
int getEmptyBitmap();

void createBitmap(char* name, int bitmap_cnt);
int deleteBitmap(char* name);

void func_bitmap_mark(char* commandLine);
void func_bitmap_all(char* commandLine);
void func_bitmap_any(char* commandLine);
void func_bitmap_contains(char* commandLine);
void func_bitmap_count(char* commandLine);
void func_bitmap_expand(char* commandLine);
void func_bitmap_set_all(char* commandLine);
void func_bitmap_flip(char* commandLine);
void func_bitmap_none(char* commandLine);
void func_bitmap_reset(char* commandLine);
void func_bitmap_scan_and_flip(char* commandLine);
void func_bitmap_scan(char* commandLine);
void func_bitmap_set(char* commandLine);
void func_bitmap_set_multiple(char* commandLine);
void func_bitmap_test(char* commandLine);
void func_bitmap_dump(char* name);
void func_bitmap_size(char* name);

#endif
