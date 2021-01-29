#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bitmap.h"
#include "common.h"
#include "func_bitmap.h"

extern struct namedBitmap bitmapPool[MAX_ELEMENT];


int getMatchingBitmap(char* name){
	for(int i=0;i<MAX_ELEMENT;i++){
		if(!strcmp(name, bitmapPool[i].name)) return i;
	}
	return -1;
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


int deleteBitmap(char* name){
	int index = getMatchingBitmap(name);

	if(index>=0){
		bitmap_destroy(bitmapPool[index].item);
		bitmapPool[index].item = NULL;
		strcpy(bitmapPool[index].name, "\0");
	}

	return index;
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

