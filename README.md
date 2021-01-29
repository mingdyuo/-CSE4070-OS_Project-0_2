## [Sogang Univ.] CSE4070 OS_Project #0_2

### Introduction
The purpose of this project is to practice and get experienced with data structures used in PintOs such as "List", "Hash", and "Bitmap". Not as usual way, struct which has pointers, for previous and next item, doesn't have data. Instead, the struct with pointers is embedded in another struct which has data. So we have to get through one more process to access data in one element. To get used to these way, we made a program that get input commands and handle each data structures according to that commands. 

<br>

### Usage

```
$ cd src
$ make
$ ./testlib
```

<br>

### Assumptions
- All inputs are from standard input (stdin).
- All the types used in the program are integer.
- The number of list, hash, bitmap is less than or equal to 10.

<br>

### Functions to implement
* Share commands

  ```
  create [list/hash/bitmap] [name]
  dumpdata [name]
  delete [name]
  quit
  ```

* List commands

  ```
  list_front [name]
  list_back [name]
  list_push_front [name] [value]
  list_push_back [name] [value]
  list_pop_front [name]
  list_pop_back [name]
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
  ```

* Bitmap commands

  ```
  bitmap_mark [name] [bit_index]
  bitmap_all [name] [start] [cnt]
  bitmap_any [name] [start] [cnt]
  bitmap_contains [name] [start] [cnt] [value]
  bitmap_count [name] [start] [end] [value]
  bitmap_expand [name] [cnt]
  bitmap_set_all [name] [value]
  bitmap_flip [name] [value]
  bitmap_none [name] [?] [?]
  bitmap_reset [name] [value]
  bitmap_scan_and_flip [name] [start] [end] [value]
  bitmap_scan [name] [start] [end] [value]
  bitmap_set [name] [position] [value]
  bitmap_set_multiple [name] [start] [cnt] [value]
  bitmap_test [name] [value]
  ```

* Hash commands

  ```
  hash_insert [name] [value]
  hash_apply [name] [sqaure/triple]
  hash_delete [name] [value]
  hash_empty [name]
  hash_size [name]
  hash_clear [name]
  hash_find [name] [value]
  hash_replace [name] [value]
  ```

<br>

### More details

- Included in `document_20171487.docx`
