
/************************************************************
*
* Unit testing for Dominion card game implemented via C
* -> this file provides coverage of function: shuffle
*
*************************************************************/


#include "dominion.h"
#include "dominion_helpers.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <string.h>


int main(int argc, char** argv) {

    srand(time(NULL));

    int randomSeed = 12;
    int numPlayers = 1;

    //enum values:      15     12        22       24        25         26            8            20         23       19
    int kCards[10] = {baron, remodel, embargo, salvager, sea_hag, treasure_map, council_room, ambassador, outpost, tribute};

    //game state struct
    struct gameState testGame;
    //loop counters
    int i, j, x;
    //variable containers
    int returnInt, randomCard;

    int maxDeckCount = 4;
    int minDeckCount = -1;

    int preShuffleDeck[MAX_DECK];

    //bool to identify if test result 0 = shuffle -1 = no shuffle
    int result;
    //bool to identify expected test result 0 = shuffle -1 = no shuffle
    int expect;
    //bool to track if particular test failed
    int failureFound = 0;

    printf("\n\n-----------------------------------------\nUNITTEST 1: Testing function: shuffle\n");
    printf("-----------------------------------------\nTEST 1: Shuffle only when deck count greater than 1\n\n");
    for (i = 0; i < numPlayers; i++) {
        for (j = minDeckCount; j <= maxDeckCount; j++) {

            //clear gameState memory to null bytes
            memset(&testGame, '\0', sizeof(struct gameState));

            //init gameState
            returnInt = initializeGame(numPlayers, kCards, randomSeed, &testGame);

            testGame.deckCount[i] = 0;

            //fill deck with random cards, if deck count greater than 0
            //note: random refers to a random card for game's available kingdom cards, not a truly 'random' card
            if (j > 0) {

                for (x = 0; x < j; x++) {

                    randomCard = (rand() % (sizeof(kCards) / sizeof(int)));
                    //printf("Random card: %d\n", kCards[randomCard]);
                    //fill deck with random card from available kingdom cards
                    testGame.deck[i][x] = kCards[randomCard];
                    testGame.deckCount[i]++;
                }
            }


            printf("Player: %d\nDeck count: %d\n", i+1, j);

            result = shuffle(i, &testGame);

            //set expectation to only shuffle if deckCount greater than 1
            //note: shuffling a single card is pointless, deck state will remain unchanged after func call
            if (j > 1) {
                printf("Expect: Shuffle\n");
                expect = 0;
            } else {
                printf("Expect: No shuffle\n");
                expect = -1;
            }

            if (result == 0) {
                printf("Result: Shuffle\n");
            } else if (result == -1) {
                printf("Result: No shuffle\n");
            }

            if (expect == result) {
                printf("Grade: PASS\n\n");
            } else {
                printf("Grade: FAILURE\n\n");
                failureFound = 1;
            }
        }
    }

    printf("-----------------------------------------\nTEST 2: Deck count unchanged after shuffle\n\n");
    for (i = 0; i < numPlayers; i++) {
        for (j = 0; j <= 5; j++) {

            //clear gameState memory to null bytes
            memset(&testGame, '\0', sizeof(struct gameState));

            //init gameState
            returnInt = initializeGame(numPlayers, kCards, randomSeed, &testGame);

            testGame.deckCount[i] = 0;
            //fill deck with random cards, if deck count greater than 0
            if (j > 0) {
                for (x = 0; x < j; x++) {
                    randomCard = (rand() % (sizeof(kCards) / sizeof(int)));
                    testGame.deck[i][x] = kCards[randomCard];
                    testGame.deckCount[i]++;
                }
            }

                printf("Player: %d\nDeck count: %d\n", i+1, j);

                result = shuffle(i, &testGame);

                if (result == -1) {
                    printf("N/A - shuffle did not execute\n\n");
                } else if (result == 0){
                    printf("Expect: %d\n", j);
                    printf("Result: %d\n", testGame.deckCount[i]);

                    if (testGame.deckCount[i] == j) {
                        printf("Grade: PASS\n\n");
                    } else {
                        printf("Grade: FAILURE\n\n");
                    }

                }
        }
    }

    printf("-----------------------------------------\nTEST 3: Deck contents unchanged after shuffle (same cards remain in deck)\n\n");
    for (i = 0; i < numPlayers; i++) {
        for (j = 1; j <= 20; j++) {

            //clear gameState memory to null bytes
            memset(&testGame, '\0', sizeof(struct gameState));

            //init gameState
            returnInt = initializeGame(numPlayers, kCards, randomSeed, &testGame);

            int PreShuffleDeckSum = 0;
            int PostShuffleDeckSum = 0;
            testGame.deckCount[i] = 0;

            //fill deck with random cards, if deck count greater than 0
            if (j > 0) {
                for (x = 0; x < j; x++) {
                    randomCard = (rand() % (sizeof(kCards) / sizeof(int)));
                    testGame.deck[i][x] = kCards[randomCard];
                    PreShuffleDeckSum += kCards[randomCard];
                    testGame.deckCount[i]++;
                }
            }

                printf("Player: %d\nDeck count: %d\n", i+1, j);

                result = shuffle(i, &testGame);

                int orderChangeFound = 0;
                //loop through entire deck, comparing each card in sequence to verify order changed
                for (x = 0; x < j; x++) {
                    if(preShuffleDeck[x] != testGame.deck[i][x]){
                        orderChangeFound = 1;
                    }
                    PostShuffleDeckSum += testGame.deck[i][x];
                    //printf("Pre card: %d Post card: %d\n", preShuffleDeckOrder[x], testGame.deck[i][x]);
                }

                if (PreShuffleDeckSum == PostShuffleDeckSum) {
                    printf("Result: Deck makeup unchanged\n");
                    printf("Grade: PASS\n\n");
                } else {
                    printf("Result: Deck makeup changed by shuffle\n");
                    printf("Grade: FAILURE\n\n");
                }
        }
    }

    printf("-----------------------------------------\nTEST 4: Deck order changed after shuffle\n\n");
    for (i = 0; i < numPlayers; i++) {
        for (j = 4; j <= 20; j++) {

            //clear gameState memory to null bytes
            memset(&testGame, '\0', sizeof(struct gameState));

            //init gameState
            returnInt = initializeGame(numPlayers, kCards, randomSeed, &testGame);

            testGame.deckCount[i] = 0;
            //fill deck with random cards, if deck count greater than 0
            if (j > 0) {
                for (x = 0; x < j; x++) {
                    randomCard = (rand() % (sizeof(kCards) / sizeof(int)));
                    testGame.deck[i][x] = kCards[randomCard];
                    preShuffleDeck[x] = kCards[randomCard];
                    testGame.deckCount[i]++;
                }
            }

                printf("Player: %d\nDeck count: %d\n", i+1, j);

                result = shuffle(i, &testGame);

                int orderChangeFound = 0;
                //loop through entire deck, comparing each card in sequence to verify order changed
                for (x = 0; x < j; x++) {
                    if(preShuffleDeck[x] != testGame.deck[i][x]){
                        orderChangeFound = 1;
                    }
                    //printf("Pre card: %d Post card: %d\n", preShuffleDeckOrder[x], testGame.deck[i][x]);
                }

                //verify shuffle altered order by checking bytes in deck changed from preShuffle bytes
                if (memcmp(&preShuffleDeck, &testGame.deck[i], sizeof(int) * j) != 0) {
                    printf("Result: Shuffle altered deck order\n");
                    printf("Grade: PASS\n\n");
                } else {
                    printf("Result: Shuffle did not alter deck order\n");
                    printf("Grade: FAILURE\n\n");
                }
        }
    }


    if (failureFound == 0) {
        printf("\n--> OVERALL RESULT: ALL TESTS PASSED\n-----------------------------------------\n\n");
    } else {
        printf("\n--> OVERALL RESULT: FAILURES FOUND. SEE DETAILS ABOVE\n-----------------------------------------\n\n");
    }

    return 0;
}
