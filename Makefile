SOURCE=crawler.c hashtable.c bag.c curl.c pagedir.c url.c
OBJ=crawler.c hashtable.o bag.o curl.o pagedir.o url.o
HEADER=crawler.h bag.h curl.h pagedir.h url.h

crawler: ${OBJ} ${HEADER}
	$(CC) $(OBJ) -o crawler -lcurl

%.o: %.c $(HEADER)
	$(CC) $< -c

clean:
	rm -rf crawler
	rm -rf *.o
