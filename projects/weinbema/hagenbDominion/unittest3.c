
/************************************************************
*
* Unit testing for Dominion card game implemented via C
* -> this file provides coverage of function: handCard
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
    int numPlayers = 4;

    struct gameState testGame;

    //bool to track if particular test failed
    int failureFound = 0;

    int randomCard;
    int handCardReturnValue;

    //loop count ints
    int i, j, y, x;

    //enum values:      15     12        22       24        25         26            8            20         23       19
    int kCards[10] = {baron, remodel, embargo, salvager, sea_hag, treasure_map, council_room, ambassador, outpost, tribute};

    //get number of kingdom cards
    int numKCards = (sizeof(kCards) / sizeof(int));

    //valid non-kingdom card types (curse + estate + duchy + province + copper + silver + gold)
    int valid_non_KCard_count = 7;

    //array of all valid card types in play for current game
    int validCards[(numKCards + valid_non_KCard_count)];

    //copy all kingdom cards to validCards array
    memcpy((void*)(validCards), (void*)(kCards), (sizeof(int) * numKCards));

    //add remaining valid cards, common to all Dominion games, regardless of kingdom cards chosen
    validCards[10] = curse;
    validCards[11] = estate;
    validCards[12] = duchy;
    validCards[13] = province;
    validCards[14] = copper;
    validCards[15] = silver;
    validCards[16] = gold;

    printf("\n\n-----------------------------------------\nUNIT TEST 3: Testing function: fullDeckCount\n");
    printf("-----------------------------------------\nTEST 1: fullDeckCount returns 0 when deck is empty, for all valid card arguments passed\n\n");
    for (i = 0; i < numPlayers; i++) {

        //clear gameState memory to null bytes
        memset(&testGame, 23, sizeof(struct gameState));

        testGame.handCount[i] = 0;
        testGame.deckCount[i] = 0;
        testGame.discardCount[i] = 0;

        for(y = 0; y < (sizeof(validCards) / sizeof(int)); y++) {
            int fullDeckCountReturnValue = fullDeckCount(i, validCards[y], &testGame);

            printf("Player: %d\n", i);
            printf("Card passed to fullDeckCount: %d\n", validCards[y]);
            printf("Expect: 0\n");

            if (fullDeckCountReturnValue != 0) {
                printf("Result: %d\n", fullDeckCountReturnValue);
                printf("Grade: FAILURE\n\n");
                failureFound = 1;
            } else if (fullDeckCountReturnValue == 0) {
                printf("Result: %d\n", fullDeckCountReturnValue);
                printf("Grade: PASS\n\n");
            }
        }
    }

    // printf("-----------------------------------------\nTEST 2: fullDeckCount denies operating on player argument > numPlayers\n\n");
    // for (i = 0; i < numPlayers; i++) {
    //
    //     //clear gameState memory to null bytes
    //     memset(&testGame, 23, sizeof(struct gameState));
    //
    //     initializeGame(i, kCards, randomSeed, &testGame);
    //
    //     int fullDeckCountReturnValue = fullDeckCount((i + 1), validCards[y], &testGame);
    //
    //         printf("Num. Players: %i\n", i);
    //         printf("Player passed to fullDeckCount: %d\n", (i + 1));
    //         printf("Expect: -1\n");
    //
    //         if (fullDeckCountReturnValue != -1) {
    //             printf("Result: %d\n", fullDeckCountReturnValue);
    //             printf("Grade: FAILURE\n\n");
    //             failureFound = 1;
    //         } else if (fullDeckCountReturnValue == -1) {
    //             printf("Result: %d\n", fullDeckCountReturnValue);
    //             printf("Grade: PASS\n\n");
    //         }
    // }

    printf("-----------------------------------------\nTEST 2: fullDeckCount returns correct deck count (hand and discard set to 0)\n\n");
    for (i = 0; i < 1; i++) {
        for(j = 0; j < 17; j++) {

            //pile captures the count of deck + hand + discard (each is randomly set to valid value from 0 to MAX_HAND/MAX_DECK)
            int pileCount = 0;

            int randomValue = (rand() % 501);
            int discardPileCount = 0;
            int handPileCount = 0;
            int deckPileCount = 0;

            //clear gameState memory to null bytes
            memset(&testGame, 23, sizeof(struct gameState));

            deckPileCount = testGame.deckCount[i] = randomValue;

            for (x = 0; x < testGame.deckCount[i]; x++) {
                //fill hand pile with random, valid cards
                int randomCard = (rand() % 17);
                testGame.deck[i][x] = validCards[randomCard];
            }

            handPileCount = testGame.handCount[i] = 0;
            discardPileCount = testGame.discardCount[i] = 0;

            pileCount = deckPileCount + handPileCount + discardPileCount;

            int fullDeckCountReturnValue = fullDeckCount(i, validCards[j], &testGame);

            printf("Card passed (enum value): %d\n", validCards[j]);
            printf("Deck pile set to: %d\n", deckPileCount);
            printf("Hand pile set to: %d\n", handPileCount);
            printf("Discard pile set to: %d\n", discardPileCount);
            printf("Expect total pile count: %d\n", pileCount);

            if (fullDeckCountReturnValue == deckPileCount) {
                printf("Result: %d\n", fullDeckCountReturnValue);
                printf("Grade: PASS\n\n");
            } else {
                printf("Result: %d\n", fullDeckCountReturnValue);
                printf("Grade: FAILURE\n\n");
                failureFound = 1;
            }
        }
    }

    printf("-----------------------------------------\nTEST 3: fullDeckCount returns correct hand count (deck and discard set to 0)\n\n");
    for (i = 0; i < 1; i++) {
        for(j = 0; j < 17; j++) {
            //pile captures the count of deck + hand + discard (each is randomly set to valid value from 0 to MAX_HAND/MAX_DECK)
            int pileCount = 0;

            int randomValue = (rand() % 501);
            int discardPileCount = 0;
            int handPileCount = 0;
            int deckPileCount = 0;

            //clear gameState memory to null bytes
            memset(&testGame, 23, sizeof(struct gameState));

            handPileCount = testGame.handCount[i] = randomValue;

            for (x = 0; x < testGame.handCount[i]; x++) {
                //fill hand pile with random, valid cards
                int randomCard = (rand() % 17);
                testGame.hand[i][x] = validCards[randomCard];
            }

            deckPileCount = testGame.deckCount[i] = 0;
            discardPileCount = testGame.discardCount[i] = 0;

            pileCount = deckPileCount + handPileCount + discardPileCount;

            int fullDeckCountReturnValue = fullDeckCount(i, validCards[j], &testGame);

            printf("Card passed (enum value): %d\n", validCards[j]);
            printf("Deck pile set to: %d\n", deckPileCount);
            printf("Hand pile set to: %d\n", handPileCount);
            printf("Discard pile set to: %d\n", discardPileCount);
            printf("Expect total pile count: %d\n", pileCount);

            if (fullDeckCountReturnValue == deckPileCount) {
                printf("Result: %d\n", fullDeckCountReturnValue);
                printf("Grade: PASS\n\n");
            } else {
                printf("Result: %d\n", fullDeckCountReturnValue);
                printf("Grade: FAILURE\n\n");
                failureFound = 1;
            }
        }
    }

    printf("-----------------------------------------\nTEST 4: fullDeckCount returns correct discard count (hand and deck set to 0)\n\n");
    for (i = 0; i < 1; i++) {
        for(j = 0; j < 17; j++) {
            //pile captures the count of deck + hand + discard (each is randomly set to valid value from 0 to MAX_HAND/MAX_DECK)
            int pileCount = 0;

            int randomValue = (rand() % 501);
            int discardPileCount = 0;
            int handPileCount = 0;
            int deckPileCount = 0;

            //clear gameState memory to null bytes
            memset(&testGame, 23, sizeof(struct gameState));

            discardPileCount = testGame.discardCount[i] = randomValue;

            for (x = 0; x < testGame.discardCount[i]; x++) {
                //fill hand pile with random, valid cards
                int randomCard = (rand() % 17);
                testGame.discard[i][x] = validCards[randomCard];
            }

            handPileCount = testGame.handCount[i] = 0;
            deckPileCount = testGame.deckCount[i] = 0;

            pileCount = deckPileCount + handPileCount + discardPileCount;

            int fullDeckCountReturnValue = fullDeckCount(i, validCards[j], &testGame);

            printf("Card passed (enum value): %d\n", validCards[j]);
            printf("Deck pile set to: %d\n", deckPileCount);
            printf("Hand pile set to: %d\n", handPileCount);
            printf("Discard pile set to: %d\n", discardPileCount);
            printf("Expect total pile count: %d\n", pileCount);

            if (fullDeckCountReturnValue == deckPileCount) {
                printf("Result: %d\n", fullDeckCountReturnValue);
                printf("Grade: PASS\n\n");
            } else {
                printf("Result: %d\n", fullDeckCountReturnValue);
                printf("Grade: FAILURE\n\n");
                failureFound = 1;
            }
        }
    }

    printf("-----------------------------------------\nTEST 5: fullDeckCount returns correct count (random pile counts (deck + discard + hand)), for random, valid card argument passed\n\n");
    for (i = 0; i < 1; i++) {
        for (j = 0; j < 10; j++) {

            //pile captures the count of deck + hand + discard (each is randomly set to valid value from 0 to MAX_HAND/MAX_DECK)
            int pileCount = 0;
            int discardPileCount = 0;
            int handPileCount = 0;
            int deckPileCount = 0;

            //clear gameState memory to null bytes
            memset(&testGame, 23, sizeof(struct gameState));

            //get three random values from 0 to MAX_HAND/MAX_DECK
            for(y = 0; y < 3; y++) {
                int randomValue = (rand() % 200);
                //switch on y, one case for each pile: hand, deck, discard
                //each case receives random count/cards, other piles set to 0
                switch(y) {
                    case 0:
                        testGame.handCount[i] = randomValue;

                        for (x = 0; x < testGame.handCount[i]; x++) {
                            //fill hand pile with random, valid cards
                            int randomCard = (rand() % 17);
                            testGame.hand[i][x] = validCards[randomCard];
                        }
                        handPileCount = testGame.handCount[i];
                        pileCount += testGame.handCount[i];
                        break;

                    case 1:
                        testGame.deckCount[i] = randomValue;


                        for (x = 0; x < testGame.deckCount[i]; x++) {
                            int randomCard = (rand() % 17);
                            //fill hand pile with random, valid cards
                            testGame.deck[i][x] = validCards[randomCard];
                        }
                        deckPileCount = testGame.deckCount[i];
                        pileCount += testGame.deckCount[i];
                        break;

                    case 2:
                        testGame.discardCount[i] = randomValue;

                        for (x = 0; x < testGame.discardCount[i]; x++) {
                            int randomCard = (rand() % 17);
                            //fill hand pile with random, valid cards
                            testGame.discard[i][x] = validCards[randomCard];
                        }
                        discardPileCount = testGame.discardCount[i];
                        pileCount += testGame.discardCount[i];
                        break;
                }
            }

                int fullDeckCountReturnValue = fullDeckCount(i, validCards[(rand() % 17)], &testGame);

                printf("Player: %d\n", i);
                printf("Deck pile set to: %d\n", deckPileCount);
                printf("Hand pile set to: %d\n", handPileCount);
                printf("Discard pile set to: %d\n", discardPileCount);
                printf("Expect total pile count: %d\n", pileCount);

                if (fullDeckCountReturnValue == pileCount) {
                    printf("Result: %d\n", fullDeckCountReturnValue);
                    printf("Grade: PASS\n\n");
                } else {
                    printf("Result: %d\n", fullDeckCountReturnValue);
                    printf("Grade: FAILURE\n\n");
                    failureFound = 1;
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
