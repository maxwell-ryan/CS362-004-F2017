
/************************************************************
*
* Unit testing for Dominion card game implemented via C
* -> this file provides coverage of function: handCard
*
*************************************************************/


#include "dominion.h"
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

    printf("\n\n-----------------------------------------\nUNIT TEST 2: Testing function: handCard");
    printf("-----------------------------------------\nTEST 1: handCard uses the correct currentPlayer (the player who's turn it currently is)\n\n");
    for (i = 0; i < numPlayers; i++) {
        for (j = 1; j <= 1; j++) {

            //clear gameState memory to null bytes
            memset(&testGame, 0, sizeof(struct gameState));

            //set turn to current loop counter
            testGame.whoseTurn = i;
            //only fill the whoseTurn hand with a valid card
            testGame.hand[testGame.whoseTurn][0] = kCards[0];

            //init gameState
            //returnInt = initializeGame(numPlayers, kCards, randomSeed, &testGame);
            int returnedHandCard = handCard(0, &testGame);

            //verify value returned is kCard value assigned - any other value must indicate an error
            printf("Current player turn: %d\n", i);
            printf("Card value exected: %d\n", kCards[0]);
            printf("Card value returned: %d\n", returnedHandCard);
            //verify expectation observed, resulting in test passing
            if (kCards[0] == returnedHandCard) {
                printf("Grade: PASS\n\n");
            } else {
                printf("Grade: FAILURE\n\n");
                failureFound = 1;
            }
        }
    }

    printf("-----------------------------------------\nTEST 2: handCard returns the correct card (correct card enum value)\n\n");
    for (i = 0; i < 1; i++) {

        int invalidCardFound = 0;

        for (j = 0; j <= MAX_HAND; j++) {

            int correctHandContents[j];


            //clear gameState memory to null bytes
            memset(&testGame, '\0', sizeof(struct gameState));

            //manually set whoseTurn to sole player in testGame
            testGame.whoseTurn = 0;

            //fill deck with valid, but random, kingdom card; save deck order outside gameState
            for (x = 0; x <= j; x++) {
                randomCard = (rand() % (sizeof(kCards) / sizeof(int)));
                testGame.hand[i][x] = kCards[randomCard];
                correctHandContents[x] = kCards[randomCard];
                testGame.handCount[i]++;
            }

            //for each card in deck, verify handCard returns correct card
            for (y = 0; y <= j; y++){
                // UNCOMMENT below block to prove if handCard did return bad value, tester catches and reports details
                //if (y == 465) {
                //    correctHandContents[y] = 88;
                //}
                if (correctHandContents[y] != handCard(y, &testGame)) {
                    printf("Invalid card returned at handPos: %d when deckCount was: %d\n", y, j);
                    invalidCardFound = 1;
                }
            }
        }

        if (invalidCardFound) {
            failureFound = 1;
            printf("Grade: FAILURE\n\n");
        } else {
            printf("Result: Every card, in every hand count, from handCount of: %d to %d returned correctly by handPos\n", 0, MAX_HAND);
            printf("Grade: PASS\n\n");
        }
    }

    printf("-----------------------------------------\nTEST 3: handCard denies operating on handPos greater than handCount\n\n");
    for (i = 0; i < 10; i++) {

            int correctHandContents[j];

            //clear gameState memory to null bytes
            memset(&testGame, '\0', sizeof(struct gameState));

            //manually set whoseTurn to sole player in testGame
            testGame.whoseTurn = 2;

            //fill deck with valid, but random, kingdom card; save deck order outside gameState
            for (x = 0; x <= i; x++) {
                randomCard = (rand() % (sizeof(kCards) / sizeof(int)));
                testGame.hand[2][x] = kCards[randomCard];
                correctHandContents[x] = kCards[randomCard];
                testGame.handCount[2]++;
            }

            //feed handCard 1 greater than handCount established
            handCardReturnValue = handCard((testGame.handCount[2] + 1), &testGame);

            printf("Hand Count: %d\n", testGame.handCount[2]);
            printf("Hand Pos. Requested: %d\n", (testGame.handCount[2] + 1));
            printf("Expected return value: -1\n");
            printf("Result return value: %d\n", handCardReturnValue);

            if (handCardReturnValue != -1) {
                printf("Grade: FAILURE\n\n");
                failureFound = 1;
            } else {
                printf("Grade: PASS\n\n");
            }
    }

    printf("-----------------------------------------\nTEST 4: handCard denies operating on invalid, negative handPos\n\n");
    for (i = 0; i < 1; i++) {

            int correctHandContents[j];

            //clear gameState memory to null bytes
            memset(&testGame, '\0', sizeof(struct gameState));

            //negative handPos Requested
            int negativeHandPos = -1;

            //manually set whoseTurn to sole player in testGame
            testGame.whoseTurn = 2;
            testGame.handCount[2] = 0;

            //feed handCard 1 greater than handCount established
            handCardReturnValue = handCard(negativeHandPos, &testGame);

            printf("Hand Count: %d\n", testGame.handCount[2]);
            printf("Hand Pos. Requested: %d\n", negativeHandPos);
            printf("Expected return value: -1\n");
            printf("Result return value: %d\n", handCardReturnValue);

            if (handCardReturnValue != -1) {
                printf("Grade: FAILURE\n\n");
                failureFound = 1;
            } else {
                printf("Grade: PASS\n\n");
            }
    }

    if (failureFound == 0) {
        printf("\n--> OVERALL RESULT: ALL TESTS PASSED\n-----------------------------------------\n\n");
    } else {
        printf("\n--> OVERALL RESULT: FAILURES FOUND. SEE DETAILS ABOVE\n-----------------------------------------\n\n");
    }

    return 0;
}
