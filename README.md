## [Sogang Univ.] CSE4070 OS_Project #0_2

### Introduction

The purpose of this project is to practice and get experienced to the data structures in PintOs such as "List", "Hash", and "Bitmap". Not as usual way, the data structures which have pointers for previous and next item don't contain data. Instead, they are embedded in another structure which has actual data. So we have to get through one more process to access the data. To get used to this kind of the method, we've made a program that has commands to handle the new data structures. 

<br>

### Usage

```
$ cd src
$ make
$ ./testlib
```

<br>

### Assumptions

- All inputs are standard input (stdin).
- Only integer type is used in this program.
- The number of list, hash, bitmap is less than or equal to 10.

<br>

### Functions to implement

* Shared commands

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
  list_splice [name1] [position(before)] [name2] [first] [last(exclusive)]
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
