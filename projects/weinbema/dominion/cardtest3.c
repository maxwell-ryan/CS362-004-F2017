/************************************************************
*
* Unit testing for Dominion card game implemented via C
* -> this file provides coverage of card: getCost
*
*************************************************************/


#include "dominion.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <string.h>

#define CARDUNDERTEST "cutpurse"

char* getEnumName(int enumValue);

int main(int argc, char** argv) {

    srand(time(NULL));

    int randomSeed = 12;
    int numPlayers = 4;

    struct gameState testGame, controlGame, manipulatedControlGame;

    //hold card names if needed for debug/printing
    char *cardName;

    //newly initalized  games set current turn to 0
    int currentPlayer = 0;

    //bool to track if particular test failed
    int failureFound = 0;

    //expect exact gain of 2 coin from cutpurse
    int expectedCoinDelta = 2;

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

    //smithy does not utilize choice options, set all to off / 0
    int choice1 = 0;
    int choice2 = 0;
    int choice3 = 0;

    //smithy does not utilize bonus, set to off / 0
    int bonus = 0;

    //tracks position, in hand, of card under test
    int handPos;

    initializeGame(numPlayers, kCards, randomSeed, &controlGame);

    printf("\n\n-----------------------------------------\nCARD TEST 1: Testing function: %s\n", CARDUNDERTEST);
    printf("-----------------------------------------\nTEST 1: cutpurse results in exactly +2 coin to current player\n\n");

    //copy controlGame state to testGame state
    memcpy(&testGame, &controlGame, sizeof(struct gameState));

    cardEffect(cutpurse, choice1, choice2, choice3, &testGame, handPos, &bonus);

    printf("Pre-cutpurse coin: %d\n", controlGame.coins);
    printf("Post-cutpurse coin: %d\n", testGame.coins);
    printf("Coin delta: %d\n", (testGame.coins - controlGame.coins));
    printf("Expected delta: %d\n", expectedCoinDelta);

    if ((controlGame.coins + expectedCoinDelta) == testGame.coins) {
        printf("Grade: PASS\n\n");
    } else {
        printf("Grade: FAILURE\n\n");
        failureFound = 1;
    }

    printf("-----------------------------------------\nTEST 2: cutpurse results in each opponent discarding exactly one copper from hand, if hand contains at least 1 copper\n\n");

    //copy controlGame state to manipulatedControlGame state
    memcpy(&manipulatedControlGame, &controlGame, sizeof(struct gameState));

    //ensure current players first card is cutpurse, to ensure copper count integrity
    manipulatedControlGame.hand[currentPlayer][0] = cutpurse;

    //fill each players hand with varying copper amounts
    for (x = 1; x < numPlayers; x++) {
        manipulatedControlGame.handCount[x] = 5;
        for (j = 0; j < manipulatedControlGame.handCount[x]; j++) {
            manipulatedControlGame.hand[x][j] = kCards[4];

            //grab random value to override assigned card with copper - ensures variability in test/player copper count
            int randomValue = (rand() % 2);
            if (randomValue == 1) {
                manipulatedControlGame.hand[x][j] = copper;
                //printf("COPPER ASSIGNED\n");
            }

        }
    }

    memcpy(&testGame, &manipulatedControlGame, sizeof(struct gameState));

    cardEffect(cutpurse, choice1, choice2, choice3, &testGame, handPos, &bonus);

    //loop over every player - excluding currentPlayer (player who played cutpurse)
    for (y = 0; y < numPlayers; y++) {
        int preCopperCount = 0;
        int postCopperCount = 0;

        //if (y != currentPlayer) {
            //get copper count from player's current handCard (before cutpurse played)
            for (x = 0; x < manipulatedControlGame.handCount[y]; x++) {
                if (manipulatedControlGame.hand[y][x] == copper) {
                    preCopperCount++;
                }
            }

            //get copper count from player's current handCard (after cutpurse played)
            for (x = 0; x < testGame.handCount[y]; x++) {
                if (testGame.hand[y][x] == copper) {
                    postCopperCount++;
                }
            }

        printf("Player %d had %d copper(s) and now has %d copper(s) - ", y, preCopperCount, postCopperCount);
        int copperChange = preCopperCount - postCopperCount;
        if (y != currentPlayer) {

            if (preCopperCount >= 1) {

                if (copperChange == 1) {
                    printf("PLAYER %d DISCARDED %d COPPER\n", y, copperChange);
                    printf("Grade: PASS\n\n");
                } else if (copperChange == 0) {
                    printf("PLAYER %d DISCARDED %d COPPER\n", y, copperChange);
                    printf("Grade: FAILURE\n\n");
                    failureFound = 1;
                } else if (copperChange > 1) {
                    printf("PLAYER %d DISCARDED %d COPPERS\n", y, copperChange);
                    printf("Grade: FAILURE\n\n");
                    failureFound = 1;
                }

            } else {
                printf("NO COPPER FOR PLAYER TO DISCARD\n");
                printf("Grade: PASS\n\n");
            }

        } else if (y == currentPlayer) {

            if (copperChange >= 1) {
                printf("CUTPURSE PLAYING PLAYER INCORRECTLY DISCARDED %d COPPER\n", copperChange);
                printf("Grade: FAILURE\n\n");
                failureFound = 1;

            } else if (copperChange == 0) {
                printf("CUTPURSE PLAYING PLAYER DISCARDED %d COPPER\n", copperChange);
                printf("Grade: PASS\n\n");
            }
        }

    }

    printf("-----------------------------------------\nTEST 3: cutpurse results in each opponent hand count decreasing by 1, if copper held\n\n");

    //copy controlGame state to manipulatedControlGame state
    memcpy(&manipulatedControlGame, &controlGame, sizeof(struct gameState));

    //ensure current players first card is cutpurse, to ensure copper count integrity
    manipulatedControlGame.hand[currentPlayer][0] = cutpurse;

    //fill each players hand with varying copper amounts
    for (x = 1; x < numPlayers; x++) {
        manipulatedControlGame.handCount[x] = 5;
        for (j = 0; j < manipulatedControlGame.handCount[x]; j++) {
            manipulatedControlGame.hand[x][j] = kCards[4];

            //grab random value to override assigned card with copper - ensures variability in test/player copper count
            int randomValue = (rand() % 2);
            //assign copper, aside from last opponent who shall remain copper-less for edge testing
            if (randomValue == 1 && x != (numPlayers - 1)) {
                manipulatedControlGame.hand[x][j] = copper;
                //printf("COPPER ASSIGNED\n");
            }

        }
    }

    memcpy(&testGame, &manipulatedControlGame, sizeof(struct gameState));

    cardEffect(cutpurse, choice1, choice2, choice3, &testGame, handPos, &bonus);

    //loop over every player - excluding currentPlayer (player who played cutpurse)
    for (y = 0; y < numPlayers; y++) {
        int preCopperCount = 0;
        int postCopperCount = 0;

        if (y != currentPlayer) {
            //get copper count from player's current handCard (before cutpurse played)
            for (x = 0; x < manipulatedControlGame.handCount[y]; x++) {
                if (manipulatedControlGame.hand[y][x] == copper) {
                    preCopperCount++;
                }
            }

            //get copper count from player's current handCard (after cutpurse played)
            for (x = 0; x < testGame.handCount[y]; x++) {
                if (testGame.hand[y][x] == copper) {
                    postCopperCount++;
                }
            }
        }


        int handCountChange = manipulatedControlGame.handCount[y] - testGame.handCount[y];

        if (y != currentPlayer) {

            if (preCopperCount >= 1) {

                if (handCountChange == 1) {
                    printf("Opponent %d pre-hand count: %d\n", y, manipulatedControlGame.handCount[y]);
                    printf("Opponent %d post-hand count: %d\n", y, testGame.handCount[y]);
                    printf("Grade: PASS\n\n");
                } else if (handCountChange == 0 || handCountChange > 1) {
                    printf("Opponent %d pre-hand count: %d\n", y, manipulatedControlGame.handCount[y]);
                    printf("Opponent %d post-hand count: %d\n", y, testGame.handCount[y]);
                    printf("Grade: FAILURE\n\n");
                    failureFound = 1;
                }

            } else {
                printf("Opponent %d had no copper to discard\n", y);
                printf("Opponent %d pre-hand count: %d\n", y, manipulatedControlGame.handCount[y]);
                printf("Opponent %d post-hand count: %d\n", y, testGame.handCount[y]);
                printf("Grade: PASS\n\n");
            }

        }
    }

    printf("-----------------------------------------\nTEST 4: cutpurse leaves makeup of opponent hand unchanged, aside from copper discard\n\n");

    //copy controlGame state to manipulatedControlGame state
    memcpy(&manipulatedControlGame, &controlGame, sizeof(struct gameState));

    //ensure current players first card is cutpurse, to ensure copper count integrity
    manipulatedControlGame.hand[currentPlayer][0] = cutpurse;

    //fill each players hand with varying copper amounts
    for (x = 1; x < numPlayers; x++) {
        manipulatedControlGame.handCount[x] = 5;
        for (j = 0; j < manipulatedControlGame.handCount[x]; j++) {

            int randomValue = (rand() % 17);
            manipulatedControlGame.hand[x][j] = validCards[randomValue];

            //grab random value to override assigned card with copper - ensures variability in test/player copper count
            randomValue = (rand() % 2);
            //assign copper, aside from last opponent who shall remain copper-less for edge testing
            if (randomValue == 1 && x != (numPlayers - 1)) {
                manipulatedControlGame.hand[x][j] = copper;
                //printf("COPPER ASSIGNED\n");
            }

        }
    }

    memcpy(&testGame, &manipulatedControlGame, sizeof(struct gameState));

    cardEffect(cutpurse, choice1, choice2, choice3, &testGame, handPos, &bonus);

    for(x = 0; x < numPlayers; x++) {
        if (x != currentPlayer) {
            //uncomment to prove unexpected change to hand makeup as result of playing adventurer would be detected by test
            //testGame.hand[currentPlayer][y] = duchy;
            int differentMakeupDetected = 0;

            int preHandMakeup = 0;
            for (y = 0; y < manipulatedControlGame.handCount[x]; y++) {
                preHandMakeup += manipulatedControlGame.hand[x][y];
            }

            int postHandMakeup = 0;
            for (j = 0; j < testGame.handCount[x]; j++) {
                postHandMakeup += testGame.hand[x][j];
            }

            if ((preHandMakeup - 4) != postHandMakeup) {
                differentMakeupDetected = 1;
            }

            printf("Pre-cutpurse hand makeup : ");
            for (y = 0; y < manipulatedControlGame.handCount[x]; y++) {
                printf("%d ", manipulatedControlGame.hand[x][y]);
            }
            printf("\n");
            printf("Post-cutpurse hand makeup: ");
            for (y = 0; y < testGame.handCount[x]; y++) {
                printf("%d ", testGame.hand[x][y]);
            }
            printf("\n");
            if (differentMakeupDetected == 1) {
                printf("Unexpected change to hand makeup detected\n");
            } else {
                printf("No unexpected change to hand makeup detected\n");
            }

            if (!differentMakeupDetected) {
                printf("Grade: PASS\n\n");
            } else {
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
