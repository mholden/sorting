CC=gcc
CFLAGS=-g
DEVEL=/home/mholden/devel
DS=$(DEVEL)/data_structs
SO=$(DEVEL)/sorting
INCLUDES=-I$(SO)/include -I$(DS)/include

all: test_sorts

test_sorts: test_sorts.c ../utils.c ../selection_sort.c ../tree_sort.c ../insertion_sort.c \
		../quick_sort.c ../merge_sort.c ../heap_sort.c
	$(CC) $(CFLAGS) $(INCLUDES) test_sorts.c ../utils.c ../selection_sort.c ../tree_sort.c \
		$(DS)/avl_trees/avl_tree.c $(DS)/hash_tables/hash_table.c $(DS)/linked_lists/linked_list.c  \
		../insertion_sort.c ../quick_sort.c ../merge_sort.c ../heap_sort.c -o test_sorts

clean:
	rm *o test_sorts

.PHONY: all clean
