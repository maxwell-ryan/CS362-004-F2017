#include "dominion.h"
#include "dominion_helpers.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <string.h>

#define CARDUNDERTEST "adventurer"

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

    //adventurer should always result in change of 2 to treasure count
    int expectedTreasureCountDelta = 2;
    int preAdventurerTreasureCount = 0;
    int postAdventurerTreasureCount = 0;

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

    printf("\n\n-----------------------------------------\nCARD TEST 2: Testing card: %s\n", CARDUNDERTEST);
    printf("-----------------------------------------\nTEST 1: %s results in exactly 2 treasure cards being added to player's hand\n\n", CARDUNDERTEST);

    //set handPos card to adventurer
    controlGame.hand[currentPlayer][handPos] = adventurer;

    //copy controlGame state to testGame state
    memcpy(&testGame, &controlGame, sizeof(struct gameState));


    preAdventurerTreasureCount = 0;

    //cycle over each card in players hand to get count of treasure cards before adventurer is played`
    for(x = 0; x < testGame.handCount[currentPlayer]; x++){
        if (testGame.hand[currentPlayer][x] == copper || testGame.hand[currentPlayer][x] == silver || testGame.hand[currentPlayer][x] == gold){
            preAdventurerTreasureCount += 1;
        }
    }

    cardEffect(adventurer, choice1, choice2, choice3, &testGame, handPos, &bonus);

    postAdventurerTreasureCount = 0;

    //cycle over each card in players hand to get count of treasure cards before adventurer is played`
    for(x = 0; x < testGame.handCount[currentPlayer]; x++){
        if (testGame.hand[currentPlayer][x] == copper || testGame.hand[currentPlayer][x] == silver || testGame.hand[currentPlayer][x] == gold){
            postAdventurerTreasureCount += 1;
        }
    }

    printf("Treasure cards in hand pre-adventurer: %d\n", preAdventurerTreasureCount);
    printf("Treasure cards in hand post-adventurer: %d\n", postAdventurerTreasureCount);
    printf("Treasure count delta: %d\n", (postAdventurerTreasureCount - preAdventurerTreasureCount));
    printf("Expected treasure count delta: %d\n\n", expectedTreasureCountDelta);

    if ((postAdventurerTreasureCount - preAdventurerTreasureCount) == expectedTreasureCountDelta){
        printf("Grade: PASS\n\n");
    } else {
        printf("Grade: FAILURE\n\n");
        failureFound = 1;
    }

    printf("-----------------------------------------\nTEST 2: hand remains unchanged when playing %s aside from addition of 2 treasure cards\n\n", CARDUNDERTEST);

    //copy controlGame state to testGame state
    memcpy(&testGame, &controlGame, sizeof(struct gameState));


    preAdventurerTreasureCount = 0;

    //cycle over each card in players hand to get count of treasure cards before adventurer is played`
    for(x = 0; x < testGame.handCount[currentPlayer]; x++){
        if (testGame.hand[currentPlayer][x] == copper || testGame.hand[currentPlayer][x] == silver || testGame.hand[currentPlayer][x] == gold){
            preAdventurerTreasureCount += 1;
        }
    }

    cardEffect(adventurer, choice1, choice2, choice3, &testGame, handPos, &bonus);

    postAdventurerTreasureCount = 0;

    //cycle over each card in players hand to get count of treasure cards before adventurer is played`
    for(x = 0; x < testGame.handCount[currentPlayer]; x++){
        if (testGame.hand[currentPlayer][x] == copper || testGame.hand[currentPlayer][x] == silver || testGame.hand[currentPlayer][x] == gold){
            postAdventurerTreasureCount += 1;
        }
    }

    //uncomment to prove unexpected change to hand makeup as result of playing adventurer would be detected by test
    //testGame.hand[currentPlayer][y] = duchy;
    int differentCardFound = 0;
    for (y = handPos; y < controlGame.handCount[currentPlayer]; y++) {
        if (testGame.hand[currentPlayer][y] != controlGame.hand[currentPlayer][y]){
            differentCardFound = 1;
        }
    }

    printf("Hand count pre-adventurer: %d\n", controlGame.handCount[currentPlayer]);
    printf("Hand count post-adventurer: %d\n", testGame.handCount[currentPlayer]);
    printf("Hand count delta: %d\n", (testGame.handCount[currentPlayer] - controlGame.handCount[currentPlayer]));
    printf("Treasure count delta: %d\n", (postAdventurerTreasureCount - preAdventurerTreasureCount));
    printf("Change in hand count not accounted for by treasure card change: %d\n", ((testGame.handCount[currentPlayer] - controlGame.handCount[currentPlayer]) - (postAdventurerTreasureCount - preAdventurerTreasureCount)));
    printf("Expected hand count change not accounted for by treasure card change: %d\n\n", 0);

    printf("Pre-adventurer hand makeup : ");
    for (y = 0; y < controlGame.handCount[currentPlayer]; y++) {
        printf("%d ", controlGame.hand[currentPlayer][y]);
    }
    printf("\n");
    printf("Post-adventurer hand makeup: ");
    for (y = 0; y < testGame.handCount[currentPlayer]; y++) {
        printf("%d ", testGame.hand[currentPlayer][y]);
    }
    printf("\n");
    if (differentCardFound == 1) {
        printf("Unexpected change to hand makeup detected\n\n");
    } else {
        printf("No unexpected change to hand makeup detected\n\n");
    }

    if (((testGame.handCount[currentPlayer] - controlGame.handCount[currentPlayer]) - (postAdventurerTreasureCount - preAdventurerTreasureCount)) == 0){
        printf("Grade: PASS\n\n");
    } else {
        printf("Grade: FAILURE\n\n");
        failureFound = 1;
    }

    //test treasure is appropriately added to hand when deck only contains a single type of treaure (copper, silver, gold) - test loops for each type
    for (y = 1; y <=3; y++) {

        char *treasureType;
        int testNumber;

        switch (y){
            case 1:
                treasureType = "copper";
                testNumber = 3;
                break;
            case 2:
                treasureType = "silver";
                testNumber = 4;
                break;
            case 3:
                treasureType = "gold";
                testNumber = 5;
                break;
        }
        printf("-----------------------------------------\nTEST %d: Player playing %s while deck contains only %s results in exactly two treasure cards added to hand\n\n", testNumber, CARDUNDERTEST, treasureType);

        //copy controlGame state to testGame state
        memcpy(&manipulatedControlGame, &controlGame, sizeof(struct gameState));

        //create control scenario desired - current player's deck contains only copper treasure cards, no silver or gold
        manipulatedControlGame.deckCount[currentPlayer] = 20;

        //fill deck with kingdom cards and treasure cards (treasure type depends on what type is currently being tested)
        for(x = 0; x < manipulatedControlGame.deckCount[currentPlayer]; x++) {
            if (x % 2 == 0) {
                manipulatedControlGame.deck[currentPlayer][x] = kCards[2];
            } else {
                switch (y) {
                    case 1:
                        manipulatedControlGame.deck[currentPlayer][x] = copper;
                        break;
                    case 2:
                        manipulatedControlGame.deck[currentPlayer][x] = silver;
                        break;
                    case 3:
                        manipulatedControlGame.deck[currentPlayer][x] = gold;
                        break;
                }
            }
        }
        memcpy(&testGame, &manipulatedControlGame, sizeof(struct gameState));


        preAdventurerTreasureCount = 0;

        //cycle over each card in players hand to get count of treasure cards before adventurer is played`
        for(x = 0; x < testGame.handCount[currentPlayer]; x++){
            if (testGame.hand[currentPlayer][x] == copper || testGame.hand[currentPlayer][x] == silver || testGame.hand[currentPlayer][x] == gold){
                preAdventurerTreasureCount += 1;
            }
        }

        cardEffect(adventurer, choice1, choice2, choice3, &testGame, handPos, &bonus);

        postAdventurerTreasureCount = 0;

        //cycle over each card in players hand to get count of treasure cards before adventurer is played`
        for(x = 0; x < testGame.handCount[currentPlayer]; x++){
            if (testGame.hand[currentPlayer][x] == copper || testGame.hand[currentPlayer][x] == silver || testGame.hand[currentPlayer][x] == gold){
                postAdventurerTreasureCount += 1;
            }
        }

        printf("Treasure cards in hand pre-adventurer: %d\n", preAdventurerTreasureCount);
        printf("Treasure cards in hand post-adventurer: %d\n", postAdventurerTreasureCount);
        printf("Treasure count delta: %d\n", (postAdventurerTreasureCount - preAdventurerTreasureCount));
        printf("Expected treasure count delta: %d\n\n", expectedTreasureCountDelta);

        if ((postAdventurerTreasureCount - preAdventurerTreasureCount) == expectedTreasureCountDelta){
            printf("Grade: PASS\n\n");
        } else {
            printf("Grade: FAILURE\n\n");
            failureFound = 1;
        }
    }

    printf("-----------------------------------------\nTEST 6: Player playing %s while deck contains no treasure cards results in 0 treasure added to hand and NO infinite loop\n\n", CARDUNDERTEST);

    //copy controlGame state to testGame state
    memcpy(&manipulatedControlGame, &controlGame, sizeof(struct gameState));

    //create control scenario desired - current player's deck contains only copper treasure cards, no silver or gold
    manipulatedControlGame.deckCount[currentPlayer] = 20;
    for(x = 0; x < manipulatedControlGame.deckCount[currentPlayer]; x++) {
            manipulatedControlGame.deck[currentPlayer][x] = kCards[2];
    }

    memcpy(&testGame, &manipulatedControlGame, sizeof(struct gameState));


    preAdventurerTreasureCount = 0;

    //cycle over each card in players hand to get count of treasure cards before adventurer is played`
    for(x = 0; x < testGame.handCount[currentPlayer]; x++){
        if (testGame.hand[currentPlayer][x] == copper || testGame.hand[currentPlayer][x] == silver || testGame.hand[currentPlayer][x] == gold){
            preAdventurerTreasureCount += 1;
        }
    }

    cardEffect(adventurer, choice1, choice2, choice3, &testGame, handPos, &bonus);

    postAdventurerTreasureCount = 0;

    //cycle over each card in players hand to get count of treasure cards before adventurer is played`
    for(x = 0; x < testGame.handCount[currentPlayer]; x++){
        if (testGame.hand[currentPlayer][x] == copper || testGame.hand[currentPlayer][x] == silver || testGame.hand[currentPlayer][x] == gold){
            postAdventurerTreasureCount += 1;
        }
    }

    int noTreasureRemainingExpectedDelta = 0;

    printf("Treasure cards in hand pre-adventurer: %d\n", preAdventurerTreasureCount);
    printf("Treasure cards in hand post-adventurer: %d\n", postAdventurerTreasureCount);
    printf("Treasure count delta: %d\n", (postAdventurerTreasureCount - preAdventurerTreasureCount));
    printf("Expected treasure count delta: %d\n\n", noTreasureRemainingExpectedDelta);

    if ((postAdventurerTreasureCount - preAdventurerTreasureCount) == noTreasureRemainingExpectedDelta){
        printf("Grade: PASS\n\n");
    } else {
        printf("Grade: FAILURE\n\n");
        failureFound = 1;
    }

    printf("-----------------------------------------\nTEST 7: Player playing %s while deck contains no treasure cards, but discard does contain at least 2, results in 2 treasure added to hand and NO infinite loop\n\n", CARDUNDERTEST);

    //copy controlGame state to testGame state
    memcpy(&manipulatedControlGame, &controlGame, sizeof(struct gameState));

    //create control scenario desired - current player's deck contains only copper treasure cards, no silver or gold
    manipulatedControlGame.deckCount[currentPlayer] = 20;
    for(x = 0; x < manipulatedControlGame.deckCount[currentPlayer]; x++) {
            manipulatedControlGame.deck[currentPlayer][x] = kCards[2];
    }
    manipulatedControlGame.discardCount[currentPlayer] = 10;
    for(x = 0; x < manipulatedControlGame.discardCount[currentPlayer]; x++) {
            manipulatedControlGame.discard[currentPlayer][x] = copper;
    }

    memcpy(&testGame, &manipulatedControlGame, sizeof(struct gameState));


    preAdventurerTreasureCount = 0;

    //cycle over each card in players hand to get count of treasure cards before adventurer is played`
    for(x = 0; x < testGame.handCount[currentPlayer]; x++){
        if (testGame.hand[currentPlayer][x] == copper || testGame.hand[currentPlayer][x] == silver || testGame.hand[currentPlayer][x] == gold){
            preAdventurerTreasureCount += 1;
        }
    }

    cardEffect(adventurer, choice1, choice2, choice3, &testGame, handPos, &bonus);

    postAdventurerTreasureCount = 0;

    //cycle over each card in players hand to get count of treasure cards before adventurer is played`
    for(x = 0; x < testGame.handCount[currentPlayer]; x++){
        if (testGame.hand[currentPlayer][x] == copper || testGame.hand[currentPlayer][x] == silver || testGame.hand[currentPlayer][x] == gold){
            postAdventurerTreasureCount += 1;
        }
    }

    printf("Treasure cards in hand pre-adventurer: %d\n", preAdventurerTreasureCount);
    printf("Treasure cards in hand post-adventurer: %d\n", postAdventurerTreasureCount);
    printf("Treasure count delta: %d\n", (postAdventurerTreasureCount - preAdventurerTreasureCount));
    printf("Expected treasure count delta: %d\n\n", expectedTreasureCountDelta);

    /* UNCOMMENT block to debug test
    * printf("Pre play deck count: %d\n", manipulatedControlGame.deckCount[currentPlayer]);
    * printf("Post play deck count: %d\n", testGame.deckCount[currentPlayer]);
    * printf("Pre play discard count: %d\n", manipulatedControlGame.discardCount[currentPlayer]);
    * printf("Post play discard count: %d\n", testGame.discardCount[currentPlayer]);
    */

    if ((postAdventurerTreasureCount - preAdventurerTreasureCount) == expectedTreasureCountDelta){
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
