

prog: webstore.c hash_table.c linked_list.c iterator.c utils.c database.c
	gcc -Wall -pedantic -g webstore.c hash_table.c linked_list.c iterator.c utils.c database.c


clean:
	rm -f prog
	rm -f *~
