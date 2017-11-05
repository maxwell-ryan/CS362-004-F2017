#include "dominion.h"
#include "dominion_helpers.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <string.h>
#include "rngs.h"
#include <math.h>

#define CARDUNDERTEST "great_hall"

int checkSmithy(int, int, struct gameState*, struct gameState*);

int main () {

  int players, p1;
  int n, i, j;

  //enum values:      15     12        22       24        25         26            8            20         23       19
  int kCards[10] = {baron, remodel, embargo, salvager, sea_hag, treasure_map, council_room, ambassador, outpost, tribute}; //kingdom cards for game
  //valid deck cards = kCards + 3 treasure cards
  int validCards[13] = {baron, remodel, embargo, salvager, sea_hag, treasure_map,
    council_room, ambassador, outpost, tribute, copper, silver, gold};

  //smithy does not utilize choice options, set all to off / 0
  int choice1 = 0;
  int choice2 = 0;
  int choice3 = 0;
  //smithy does not utilize bonus, set to off / 0
  int bonus = 0;


  int cardUnderTestHandPos = 0; //tracks position, in hand, of card under test
  int randomCard; //to assign random card values from validCards array

  int randomSeed = 5; //seed for random game initialization
  int seedValue = 12; //seed for calls to rand()
  int failureFound = 0; //bool to track identification of bug/error

  struct gameState testGame;
  struct gameState controlGame;

  printf("RANDOM TESTING: %s\n", CARDUNDERTEST);

  srand(seedValue);

  //randomize the gameState struct with random bytes
  for (n = 0; n < 20000; n++) {
    for (i = 0; i < sizeof(struct gameState); i++) {
      ((char*)&controlGame)[i] = rand() % (255 + 1 - 0) + 0;
    }

    //manually override bytes that Adventurer expects to meet certain pre-criteria
    players = rand() % ((MAX_PLAYERS + 1 - 0) + 0);
    //cannot play game without at least 2 players
    if (players < 2) {
      players = 2;
    } else if (players > 4) { //players should never be above 4 - block provides extra safety
      players = 4;
    }

    controlGame.numPlayers = players;
    //printf("Number plyr: %d\n", players);

    initializeGame(players, kCards, randomSeed, &controlGame);

    //randomly select player playing card under test
    p1 = rand() % (players);

    //printf("P1 %d\n", p1);
    for (j = 0; j < players; j++){
      //assign random deckCount, then fill it with cards
      controlGame.deckCount[j] = rand() % (MAX_DECK + 1 - 0) + 0;
      for (i = 0; i < controlGame.deckCount[j]; i++) {
        randomCard = rand() % ((sizeof(validCards) / sizeof(int)) + 1 - 0) + 0;
        controlGame.deck[j][i] = validCards[randomCard];
      }
      //assign random discardCount, then fill it with cards
      controlGame.discardCount[j] = rand() % (MAX_DECK + 1 - 0) + 0;
      for (i = 0; i < controlGame.discardCount[j]; i++) {
        randomCard = rand() % ((sizeof(validCards) / sizeof(int)) + 1 - 0) + 0;
        controlGame.discard[j][i] = validCards[randomCard];
      }
      //assign random handCount, then fill it with cards
      controlGame.handCount[j] = rand() % (MAX_HAND + 1 - 0) + 0;
      for (i = 0; i < controlGame.handCount[j]; i++) {
        randomCard = rand() % ((sizeof(validCards) / sizeof(int)) + 1 - 0) + 0;
        controlGame.hand[j][i] = validCards[randomCard];
      }
      //if (j == p1) {
        //printf("p1 handcount: %d\n", controlGame.handCount[p1]);
      //}
      //printf("Deck: %d, discard: %d, hand: %d\n", controlGame.deckCount[j], controlGame.discardCount[j], controlGame.handCount[j]);
    }

    //printf("handPos hand count : %d\n", controlGame.handCount[p1]);
    //pick random spot in hand to insert card under test
    cardUnderTestHandPos = rand() % (controlGame.handCount[p1] + 1 - 0) + 0;
    controlGame.numActions = rand() % 11;
    //printf("Cardundertesthandpos: %d\n", cardUnderTestHandPos);
    //printf("Attempting to set P1 handpos of : %d\n", cardUnderTestHandPos);
    controlGame.hand[p1][cardUnderTestHandPos] = great_hall;

    //manipulate game so it is the player with card under test turn
    controlGame.whoseTurn = p1;

    //copy controlGame state to testGame state
    memcpy(&testGame, &controlGame, sizeof(struct gameState));

    //printf("choice1: %d, choice2: %d, choice3: %d, testGame: %p, handPos: %d, bonus: %d", choice1, choice2, choice3, &controlGame, cardUnderTestHandPos, bonus);
    cardEffect(great_hall, choice1, choice2, choice3, &testGame, cardUnderTestHandPos, &bonus);

    failureFound = checkGreatHall(p1, cardUnderTestHandPos, &testGame, &controlGame);

  }

  if (failureFound == 1) {
    printf("****** | OVERALL RESULT: >>>TESTING FAILURES FOUND WITH SEED: %d<<< | ******\n\n", seedValue);
  } else {
    printf("****** | OVERALL RESULT: >>>ALL TESTS PASSED USING SEED: %d<<< | ******\n\n", seedValue);
  }

  return 0;
}

int checkGreatHall(int currentPlayer, int handPos, struct gameState* testGame, struct gameState* controlGame) {

  int expectedActionCountDelta = 1;
  int expectedDiscardCount = 1;
  int expectedPlayedCountDelta = 1;
  int expectedHandDifference = 1 - 1; //draw 1, discard 1, net +0
  int failureFound;
  int x;

  printf("-----------------------------------------");
  //verify player has 1 more card1 in hand
  if ((controlGame->handCount[currentPlayer] + ((expectedHandDifference))) != testGame->handCount[currentPlayer]) {
    printf("TEST 1: great_hall results in exactly 1 card being drawn from player's deck\n");
    printf("Test 1 - Hand Count +1\n");
    printf("Grade: FAILURE\n");
    printf("Post hand count: %d, expected %d\n", testGame->handCount[currentPlayer], controlGame->handCount[currentPlayer] + (expectedHandDifference));
    printf("Difference: %d\n\n", (testGame->handCount[currentPlayer] - (controlGame->handCount[currentPlayer] + (expectedHandDifference))));
    failureFound = 1;
  }

  if (expectedDiscardCount != (testGame->discardCount[currentPlayer] - controlGame->discardCount[currentPlayer])) {
    printf("\nTEST 2: great_hall results in exactly 1 card being discarded\n");
    printf("Test 2 - Discard Count +1\n");
    printf("Grade: FAILURE\n");
    printf("Pre-great_hall discard count: %d\n", controlGame->discardCount[currentPlayer]);
    printf("Post-great_hall discard count: %d\n", testGame->discardCount[currentPlayer]);
    printf("Discard count delta: %d\n", (testGame->discardCount[currentPlayer] - controlGame->discardCount[currentPlayer]));
    printf("Expected change: %d\n\n", expectedDiscardCount);
    failureFound = 1;
  }

  if (expectedPlayedCountDelta != (testGame->playedCardCount - controlGame->playedCardCount)) {
    printf("\nTEST 3: verify great_hall affect on played cards pile\n");
    printf("Test 3 - Played Card Pile +1\n");
    printf("Grade: FAILURE\n");
    printf("Pre-great_hall played card count: %d\n", controlGame->playedCardCount);
    printf("Post-great_hall played card count: %d\n", testGame->playedCardCount);
    printf("Played count delta: %d\n", (testGame->playedCardCount - controlGame->playedCardCount));
    printf("Expected change: %d\n\n", expectedPlayedCountDelta);
    failureFound = 1;
  }

  if ((testGame->numActions - controlGame->numActions) != expectedActionCountDelta) {
    printf("\nTEST 4: great_hall results in consumption of exactly 1 action\n");
    printf("Test 4 - Number of Actions -1\n");
    printf("Grade: FAILURE\n");
    printf("Pre-great_hall action count: %d\n", controlGame->numActions);
    printf("Post-great_hall action count: %d\n", testGame->numActions);
    printf("Action count delta: %d\n", (testGame->numActions - controlGame->numActions));
    printf("Expected change: %d\n\n", expectedActionCountDelta);
    failureFound = 1;
  }

  if (controlGame->hand[currentPlayer][handPos] == testGame->hand[currentPlayer][handPos]) {
    printf("\nTEST 5: great_hall card played removed from hand after being played\n");
    printf("Test 5 - great_hall Card Removed from Hand\n");
    printf("Grade: FAILURE\n");
    printf("Card enum value before playing great_hall: %d\n", controlGame->hand[currentPlayer][handPos]);
    printf("Card enum value after playing great_hall: %d\n\n", testGame->hand[currentPlayer][handPos]);
    failureFound = 1;
  }
  return failureFound;
}
