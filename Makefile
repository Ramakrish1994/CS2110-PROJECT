# Makefile
#

SH = bash
CFLAGS = -g
LDFLAGS = -lm

all : maze

maze : gameeng.o bot3.o bot2.o Queue3.o
	gcc $(LDFLAGS) $^ -o $@


	
gameeng.o : gameeng.c bot3.h Queue3.h bot2.h 
	gcc -c $(CFLAGS) $< -o $@

bot3.o: bot3.c Queue3.h bot3.h
	gcc -c $(CFLAGS) $(LDFLAGS) $< -o $@
	
bot2.o: bot2.c Queue3.h bot2.h
	gcc -c $(CFLAGS) $(LDFLAGS) $< -o $@
	
bot1.o: bot1.c bot1.h
	gcc -c $(CFLAGS) $(LDFLAGS) $< -o $@
	
Queue3.o : Queue3.c Queue3.h
	gcc -c $(CFLAGS) $(LDFLAGS) $< -o $@
	

clean : 
	rm -rf *.o
	rm -rf maze

