JerryBoree: Defs.h Jerry.h LinkedList.h KeyValuePair.h HashTable.h MultiValueHashTable.h Jerry.o LinkedList.o KeyValuePair.o HashTable.o MultiValueHashTable.o JerryBoreeMain.o
	gcc Jerry.o LinkedList.o KeyValuePair.o HashTable.o MultiValueHashTable.o JerryBoreeMain.o -o JerryBoree

Jerry.o: Jerry.h Defs.h
	gcc -c Jerry.c

LinkedList.o: LinkedList.h
	gcc -c LinkedList.c
	
KeyValuePair.o: Defs.h KeyValuePair.h
	gcc -c KeyValuePair.c

HashTable.o: LinkedList.h KeyValuePair.h HashTable.h
	gcc -c HashTable.c
	
MultiValueHashTable.o: KeyValuePair.h MultiValueHashTable.h
	gcc -c MultiValueHashTable.c
	
JerryBoreeMain.o: KeyValuePair.h MultiValueHashTable.h Jerry.h
	gcc -c JerryBoreeMain.c

clean :
	rm -f *.o
