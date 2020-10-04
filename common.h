#ifndef __COMMON_H__
#define __COMMON_H__

#define MAXFNAME	10
#define MAXCMDLEN	50

#define MAX_CMD_LINE 50
#define MAX_ELEMENT 11

#include <stdbool.h>
/* decide what kind of the command is.
   "OTHERS" means it is data structure dependent command */
typedef enum {CREATE, DELETE, DUMPDATA, QUIT, OTHERS} COMMAND;

typedef enum {NONE, LIST, HASH, BITMAP} DATATYPE;

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

FUNCTION getFunction(char* functionName);
COMMAND getCommand(char *command);
bool getNameOnly(char* );

void create(char *commandLine);
void delete(char* commandLine);
void dumpdata(char* commandLine);

#endif
