
/************************************************************
*
* Unit testing for Dominion card game implemented via C
* -> this file provides coverage of function: getCost
*
*************************************************************/


#include "dominion.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <string.h>

char* getEnumName(int enumValue);

int main(int argc, char** argv) {

    srand(time(NULL));

    int randomSeed = 12;
    int numPlayers = 4;

    struct gameState testGame;

    //bool to track if particular test failed
    int failureFound = 0;
    int randomCard;
    int handCardReturnValue;
    int costNotFound;

    //loop count ints
    int i, j, y, x;

    //hold cost value returned
    int cost;

    //enum values:      15     12        22       24        25         26            8            20         23       19
    int kCards[10] = {baron, remodel, embargo, salvager, sea_hag, treasure_map, council_room, ambassador, outpost, tribute};

    printf("\n\n-----------------------------------------\nUNIT TEST 4: Testing function: getCost\n");
    printf("-----------------------------------------\nTEST 1: getCost has a cost (value) for every card\n\n");

    for(i = 0; i < 1; i++) {
        //get min enum value
        int minEnum = curse;

        //get max enum value
        int maxEnum = treasure_map;

        //compute total number of valid Dominion cards
        int totalCardCount = maxEnum - minEnum;

        for(x = 0; x <= totalCardCount; x++) {

            //Note: getCost was not in dominion.h, had to be declared there before testing could be completed
            cost = getCost(x);
            printf("Card %d - %s: ", x, getEnumName(x));
            if (cost == -1) {
                printf("COST NOT FOUND\n");
                printf("Grade: FAILURE\n\n");
                costNotFound = 1;
                failureFound = 1;
            } else {
                printf("COST FOUND\n");
                printf("Grade: PASS\n\n");
            }
        }

        if (costNotFound == 1) {
            printf("\nCost not found for at least 1 valid Dominion card, see above for such card(s)!\n\n");
        } else {
            printf("\nCost found for all valid Dominion cards!\n\n");
        }
    }

    printf("-----------------------------------------\nTEST 2: getCost returns -1 for invalid card enum values - minimum boundry\n\n");

    for(i = 0; i < 1; i++) {
        //get min enum value
        int minEnum = curse;

        //get max enum value
        int maxEnum = treasure_map;

        cost = getCost(minEnum - 1);

        printf("Minimum valid enum value: %d\n", minEnum);
        printf("Value given to getCost: %d\n", (minEnum - 1));
        printf("Expect return value: %d\n", -1);
        printf("Actual return value: %d\n", cost);
        if (cost == -1) {
            printf("Grade: PASS\n\n");
        } else {
            printf("Grade: FAILURE\n\n");
        }

    }

    printf("-----------------------------------------\nTEST 3: getCost returns -1 for invalid card enum values - maximum boundry\n\n");

    for(i = 0; i < 1; i++) {
        //get min enum value
        int minEnum = curse;

        //get max enum value
        int maxEnum = treasure_map;

        cost = getCost(maxEnum + 1);

        printf("Maximum valid enum value: %d\n", maxEnum);
        printf("Value given to getCost: %d\n", (maxEnum + 1));
        printf("Expect return value: %d\n", -1);
        printf("Actual return value: %d\n", cost);
        if (cost == -1) {
            printf("Grade: PASS\n\n");
        } else {
            printf("Grade: FAILURE\n\n");
        }

    }

    printf("-----------------------------------------\nTEST 4: getCost returns correct cost for all valid enum values (cards)\n\n");

    for(i = 0; i < 1; i++) {
        //get min enum value
        int minEnum = curse;

        //get max enum value
        int maxEnum = treasure_map;

        char *returnedName;

        returnedName = getEnumName(0);

        printf("Name %s", returnedName);

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
