### [Sogang Univ.] CSE4070 OS_Project

#### Project #0_2
The purpose of this project is to practice and get experienced with data structures used in PintOs such as "List", "Hash", and "Bitmap". Not as usual way, struct which has pointers, for previous and next item, doesn't have data. Instead, the struct with pointers is embedded in another struct which has data. So we have to get through one more process to access data in one element. To get used to these way, we made a program that get input commands and handle each data structures according to that commands. 

##### Assumptions
- All inputs are from standard input (stdin).
- All the types used in the program are integer.
- The number of list, hash, bitmap is less than or equal to 10.
##### Requirements in this project
- Make the program which process input and parse to execute each commands.
- Functions to implement
1. list_swap
2. list_shuffle
3. hash_int_2
4. bitmap_expand
>##### **Commands**
>##### Shared commands
>1. create [list/hash/bitmap] [name]
>2. dumpdata [name]
>3. delete [name]
>4. quit


>##### List commands
>1. list_front [name]
>2. list_back [name]
>3. list_push_front [name] [value]
>4. list_push_back [name] [value]
>5. list_pop_front [name]
>6. list_pop_back [name]
list_insert [name] [position] [value]
list_insert_ordered [name] [value]
list_empty [name]
list_size  [name]
list_max  [name]
list_min  [name]
list_remove [name] [index]
list_reverse [name]
list_sort [name]
list_splice [name1] [position(before)] [name2] [first] [last(exclusice)]
list_swap [name] [pos1] [pos2]
list_shuffle [name]
list_unique [name1] [name2]


...작성중 
