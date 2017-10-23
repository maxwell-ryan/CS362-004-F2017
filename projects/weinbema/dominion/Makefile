CFLAGS = -Wall -fpic -coverage -lm

rngs.o: rngs.h rngs.c
	gcc -c rngs.c -g  $(CFLAGS)

dominion.o: dominion.h dominion.c rngs.o
	gcc -c dominion.c -g  $(CFLAGS)

testcard1: dominion.o rngs.o
	gcc -o testcard1 -g cardtest1.c dominion.o rngs.o $(CFLAGS)

testcard2: dominion.o rngs.o
	gcc -o testcard2 -g cardtest2.c dominion.o rngs.o $(CFLAGS)

testcard3: dominion.o rngs.o
	gcc -o testcard3 -g cardtest3.c dominion.o rngs.o $(CFLAGS)

testcard4: dominion.o rngs.o
	gcc -o testcard4 -g cardtest4.c dominion.o rngs.o $(CFLAGS)

testunit1: dominion.o rngs.o
	gcc -o testunit1 -g unittest1.c dominion.o rngs.o $(CFLAGS)

testunit2: dominion.o rngs.o
	gcc -o testunit2 -g unittest2.c dominion.o rngs.o $(CFLAGS)

testunit3: dominion.o rngs.o
	gcc -o testunit3 -g unittest3.c dominion.o rngs.o $(CFLAGS)

testunit4: dominion.o rngs.o
	gcc -o testunit4 -g unittest4.c dominion.o rngs.o $(CFLAGS)

unittestresults.out: testcard1 testcard2 testcard3 testcard4 testunit1 testunit2 testunit3 testunit4
	./testcard1 >> unittestresults.out
	gcov -b dominion.c >> unittestresults.out
	rm -f dominion.gcda

	./testcard2 >> unittestresults.out
	gcov -b dominion.c >> unittestresults.out
	rm -f dominion.gcda

	./testcard3 >> unittestresults.out
	gcov -b dominion.c >> unittestresults.out
	rm -f dominion.gcda

	./testcard4 >> unittestresults.out
	gcov -b dominion.c >> unittestresults.out
	rm -f dominion.gcda

	./testunit1 >> unittestresults.out
	gcov -b dominion.c >> unittestresults.out
	rm -f dominion.gcda

	./testunit2 >> unittestresults.out
	gcov -b dominion.c >> unittestresults.out
	rm -f dominion.gcda

	./testunit3 >> unittestresults.out
	gcov -b dominion.c >> unittestresults.out
	rm -f dominion.gcda

	./testunit4 >> unittestresults.out
	gcov -b dominion.c >> unittestresults.out
	rm -f dominion.gcda

playdom: dominion.o playdom.c
	gcc -o playdom playdom.c -g dominion.o rngs.o $(CFLAGS)
#To run playdom you need to entere: ./playdom <any integer number> like ./playdom 10*/
testDrawCard: testDrawCard.c dominion.o rngs.o
	gcc  -o testDrawCard -g  testDrawCard.c dominion.o rngs.o $(CFLAGS)

badTestDrawCard: badTestDrawCard.c dominion.o rngs.o
	gcc -o badTestDrawCard -g  badTestDrawCard.c dominion.o rngs.o $(CFLAGS)

testBuyCard: testDrawCard.c dominion.o rngs.o
	gcc -o testDrawCard -g  testDrawCard.c dominion.o rngs.o $(CFLAGS)

testAll: dominion.o testSuite.c
	gcc -o testSuite testSuite.c -g  dominion.o rngs.o $(CFLAGS)

interface.o: interface.h interface.c
	gcc -c interface.c -g  $(CFLAGS)

runtests: testDrawCard 
	./testDrawCard &> unittestresult.out
	gcov dominion.c >> unittestresult.out
	cat dominion.c.gcov >> unittestresult.out


player: player.c interface.o
	gcc -o player player.c -g  dominion.o rngs.o interface.o $(CFLAGS)

all: playdom player testDrawCard testBuyCard badTestDrawCard testcard1 testcard2 testcard3 testcard4 testunit1 testunit2 testunit3 testunit4

clean:
	rm -f *.o testcard1 testcard2 testcard3 testcard4 testunit1 testunit2 testunit3 testunit4 playdom.exe playdom player player.exe  *.gcov *.gcda *.gcno *.so *.out testDrawCard testDrawCard.exe
