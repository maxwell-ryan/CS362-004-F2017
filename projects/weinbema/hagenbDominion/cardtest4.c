/************************************************************
*
* Unit testing for Dominion card game implemented via C
* -> this file provides coverage of card: getCost
*
*************************************************************/


#include "dominion.h"
#include "dominion_helpers.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <string.h>

#define CARDUNDERTEST "great_hall"

char* getEnumName(int enumValue);

int main(int argc, char** argv) {

    srand(time(NULL));

    int randomSeed = 12;
    int numPlayers = 4;

    struct gameState testGame, controlGame;

    //hold card names if needed for debug/printing
    char *cardName;

    //newly initalized  games set current turn to 0
    int currentPlayer = 0;

    //bool to track if particular test failed
    int failureFound = 0;

    //loop count ints
    int i, j, y, x;

    //enum values:      15     12        22       24        25         26            8            20         23       19
    int kCards[10] = {baron, remodel, embargo, salvager, sea_hag, treasure_map, council_room, ambassador, outpost, tribute};

    //smithy does not utilize choice options, set all to off / 0
    int choice1 = 0;
    int choice2 = 0;
    int choice3 = 0;

    //smithy does not utilize bonus, set to off / 0
    int bonus = 0;

    //tracks position, in hand, of card under test
    int handPos = 0;

    initializeGame(numPlayers, kCards, randomSeed, &controlGame);

    printf("\n\n-----------------------------------------\nCARD TEST 4: Testing card: %s\n", CARDUNDERTEST);
    printf("-----------------------------------------\nTEST 1: great_hall results in exactly 1 card being drawn from player's deck\n\n");

    //set handPos card to great_hall
    controlGame.hand[currentPlayer][handPos] = great_hall;
    //copy controlGame state to testGame state
    memcpy(&testGame, &controlGame, sizeof(struct gameState));

    cardEffect(great_hall, choice1, choice2, choice3, &testGame, handPos, &bonus);

    //printf("TestGame Current turn: %d\n", whoseTurn(&testGame));
    //printf("ControlGame Current turn: %d\n", whoseTurn(&testGame));

    int expectedDrawCount = 1;
    int expectedDiscardCount = 1;

    printf("Pre-great_hall hand count: %d\n", controlGame.handCount[currentPlayer]);
    printf("Post-great_hall hand count: %d\n", testGame.handCount[currentPlayer]);
    printf("Hand count delta: %d\n", (testGame.handCount[currentPlayer] - controlGame.handCount[currentPlayer]));
    printf("Expected change (+1 card - 1 discard): %d\n\n", (expectedDrawCount - expectedDiscardCount));

    printf("Pre-great_hall deck count: %d\n", controlGame.deckCount[currentPlayer]);
    printf("Post-great_hall deck count: %d\n", testGame.deckCount[currentPlayer]);
    printf("Deck count delta: %d\n", (controlGame.deckCount[currentPlayer] - testGame.deckCount[currentPlayer]));
    printf("Expected change: %d\n", expectedDrawCount);
    if ((expectedDrawCount - expectedDiscardCount) == (testGame.handCount[currentPlayer]- controlGame.handCount[currentPlayer])){
        printf("Grade: PASS\n\n");
    } else {
        printf("Grade: FAILURE\n\n");
        failureFound = 1;
    }

    printf("-----------------------------------------\nTEST 2: great_hall results in exactly 1 card being placed in discard\n\n");

    //copy controlGame state to testGame state
    memcpy(&testGame, &controlGame, sizeof(struct gameState));

    cardEffect(great_hall, choice1, choice2, choice3, &testGame, handPos, &bonus);

    //printf("TestGame Current turn: %d\n", whoseTurn(&testGame));
    //printf("ControlGame Current turn: %d\n", whoseTurn(&testGame));

    expectedDrawCount = 1;
    expectedDiscardCount = 1;

    printf("Pre-great_hall discard count: %d\n", controlGame.discardCount[currentPlayer]);
    printf("Post-great_hall discard count: %d\n", testGame.discardCount[currentPlayer]);
    printf("Discard count delta: %d\n", (testGame.discardCount[currentPlayer] - controlGame.discardCount[currentPlayer]));
    printf("Expected change: %d\n", expectedDiscardCount);
    if (expectedDiscardCount == (testGame.discardCount[currentPlayer] - controlGame.discardCount[currentPlayer])){
        printf("Grade: PASS\n\n");
    } else {
        printf("Grade: FAILURE\n\n");
        failureFound = 1;
    }

    printf("-----------------------------------------\nTEST 3: verify great_hall affect on played cards pile\n\n");

    //copy controlGame state to testGame state
    memcpy(&testGame, &controlGame, sizeof(struct gameState));

    cardEffect(great_hall, choice1, choice2, choice3, &testGame, handPos, &bonus);

    //printf("TestGame Current turn: %d\n", whoseTurn(&testGame));
    //printf("ControlGame Current turn: %d\n", whoseTurn(&testGame));

    int expectedPlayedCountDelta = 1;

    printf("Pre-great_hall played card count: %d\n", controlGame.playedCardCount);
    printf("Post-great_hall played card count: %d\n", testGame.playedCardCount);
    printf("Played count delta: %d\n", (testGame.playedCardCount - controlGame.playedCardCount));
    printf("Expected change: %d\n", expectedPlayedCountDelta);
    if (expectedPlayedCountDelta == (testGame.playedCardCount - controlGame.playedCardCount)){
        printf("Grade: PASS\n\n");
    } else {
        printf("Grade: FAILURE\n\n");
        failureFound = 1;
    }

    printf("-----------------------------------------\nTEST 4: great_hall results in exactly +1 action added to player's turn\n\n");

    //copy controlGame state to testGame state
    memcpy(&testGame, &controlGame, sizeof(struct gameState));

    cardEffect(great_hall, choice1, choice2, choice3, &testGame, handPos, &bonus);

    //printf("TestGame Current turn: %d\n", whoseTurn(&testGame));
    //printf("ControlGame Current turn: %d\n", whoseTurn(&testGame));

    int expectedActionCountDelta = 1;

    printf("Pre-great_hall action count: %d\n", controlGame.numActions);
    printf("Post-great_hall action count: %d\n", testGame.numActions);
    printf("Action count delta: %d\n", (testGame.numActions - controlGame.numActions));
    printf("Expected change: %d\n", expectedActionCountDelta);
    if (expectedActionCountDelta == (testGame.numActions - controlGame.numActions)){
        printf("Grade: PASS\n\n");
    } else {
        printf("Grade: FAILURE\n\n");
        failureFound = 1;
    }

    printf("-----------------------------------------\nTEST 5: great_hall card played removed from hand after being played\n\n");

    //copy controlGame state to testGame state
    memcpy(&testGame, &controlGame, sizeof(struct gameState));

    controlGame.hand[currentPlayer][handPos] = great_hall;
    testGame.hand[currentPlayer][handPos] = great_hall;

    cardEffect(great_hall, choice1, choice2, choice3, &testGame, handPos, &bonus);

    //printf("TestGame Current turn: %d\n", whoseTurn(&testGame));
    //printf("ControlGame Current turn: %d\n", whoseTurn(&testGame));

    printf("Card enum value before playing great_hall: %d\n", controlGame.hand[currentPlayer][handPos]);
    cardName = getEnumName(controlGame.hand[currentPlayer][handPos]);
    printf("Enum value correlates to card name: %s\n", cardName);
    printf("Card enum value after playing great_hall: %d\n", testGame.hand[currentPlayer][handPos]);
    cardName = getEnumName(testGame.hand[currentPlayer][handPos]);
    printf("Enum value correlates to card name: %s\n", cardName);

    if (controlGame.hand[currentPlayer][handPos] != testGame.hand[currentPlayer][handPos]){
        printf("Grade: PASS\n\n");
    } else {
        printf("Grade: FAILURE\n\n");
        failureFound = 1;
    }

    if (failureFound == 0) {
        printf("\n--> OVERALL RESULT: ALL TESTS PASSED\n-----------------------------------------\n\n");
    } else {
        printf("\n--> OVERALL RESULT: FAILURES FOUND. SEE DETAILS ABOVE\n-----------------------------------------\n\n");
    }

    return 0;
}

char* getEnumName(int enumValue) {

    char *name;

    switch (enumValue) {
        case 0:
            name = "curse";
            break;
        case 1:
            name = "estate";
            break;
        case 2:
            name = "duchy";
            break;
        case 3:
            name = "province";
            break;
        case 4:
            name = "copper";
            break;
        case 5:
            name = "silver";
            break;
        case 6:
            name = "gold";
            break;
        case 7:
            name = "adventurer";
            break;
        case 8:
            name = "council_room";
            break;
        case 9:
            name = "feast";
            break;
        case 10:
            name = "gardens";
            break;
        case 11:
            name = "mine";
            break;
        case 12:
            name = "remodel";
            break;
        case 13:
            name = "smithy";
            break;
        case 14:
            name = "village";
            break;
        case 15:
            name = "baron";
            break;
        case 16:
            name = "great_hall";
            break;
        case 17:
            name = "minion";
            break;
        case 18:
            name = "steward";
            break;
        case 19:
            name = "tribute";
            break;
        case 20:
            name = "ambassador";
            break;
        case 21:
            name = "cutpurse";
            break;
        case 22:
            name = "embargo";
            break;
        case 23:
            name = "outpost";
            break;
        case 24:
            name = "salvager";
            break;
        case 25:
            name = "sea_hag";
            break;
        case 26:
            name = "treasure_map";
            break;
    }

    return name;
}
