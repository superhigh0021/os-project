#!/bin/make

CC = gcc
STATIC = ar rcs
CFLAGS = -g

all :my_type my_copy my_dir my_mkdir my_move my_rm my_ren my_sort


my_type: my_type.c libtype_src.a
	$(CC) $(CFLAGS) -o ./out/my_type my_type.c -L ./lib -ltype_src

libtype_src.a: type_src.o
	$(STATIC) ./lib/$@ type_src.o 

my_copy: my_copy.c libcopy_src.a
	$(CC) $(CFLAGS) -o ./out/my_copy my_copy.c -L ./lib -lcopy_src

libcopy_src.a: copy_move_src.o file_isexist.o get_user_confirmation.o rm_src.o
	$(STATIC) ./lib/$@ copy_move_src.o
	$(STATIC) ./lib/$@ file_isexist.o
	$(STATIC) ./lib/$@ get_user_confirmation.o
	$(STATIC) ./lib/$@ rm_src.o

my_dir: my_dir.c libdir_src.a
	$(CC) $(CFLAGS) -o ./out/my_dir my_dir.c -L ./lib -ldir_src

libdir_src.a: dir_src.o
	$(STATIC) ./lib/$@ dir_src.o 

my_mkdir: my_mkdir.c libmkdir_src.a
	$(CC) $(CFLAGS) -o ./out/my_mkdir my_mkdir.c -L ./lib -lmkdir_src

libmkdir_src.a: mkdir_src.o file_isexist.o
	$(STATIC) ./lib/$@ mkdir_src.o
	$(STATIC) ./lib/$@ file_isexist.o

my_move: my_move.c libmove_src.a
	$(CC) $(CFLAGS) -o ./out/my_move my_move.c -L ./lib -lmove_src

libmove_src.a: copy_move_src.o file_isexist.o get_user_confirmation.o rm_src.o
	$(STATIC) ./lib/$@ copy_move_src.o
	$(STATIC) ./lib/$@ file_isexist.o
	$(STATIC) ./lib/$@ get_user_confirmation.o
	$(STATIC) ./lib/$@ rm_src.o

my_rm: my_rm.c librm_src.a
	$(CC) $(CFLAGS) -o ./out/my_rm my_rm.c -L ./lib -lrm_src

librm_src.a: copy_move_src.o file_isexist.o get_user_confirmation.o rm_src.o
	$(STATIC) ./lib/$@ copy_move_src.o
	$(STATIC) ./lib/$@ file_isexist.o
	$(STATIC) ./lib/$@ get_user_confirmation.o
	$(STATIC) ./lib/$@ rm_src.o

my_ren: my_ren.c libren_src.a
	$(CC) $(CFLAGS) -o ./out/my_ren my_ren.c -L ./lib -lren_src

libren_src.a: ren_src.o file_isexist.o
	$(STATIC) ./lib/$@ ren_src.o
	$(STATIC) ./lib/$@ file_isexist.o

my_sort: my_sort.c libsort_src.a
	$(CC) $(CFLAGS) -o ./out/my_sort my_sort.c -L ./lib -lsort_src

libsort_src.a: sort_src.o is_digit.o err_msg.o
	$(STATIC) ./lib/$@ sort_src.o
	$(STATIC) ./lib/$@ is_digit.o
	$(STATIC) ./lib/$@ err_msg.o

%.o:%.c
	$(CC) $(CFLAGS) $< -c

clean_o: 
	rm -rf *.o