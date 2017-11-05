#include "dominion.h"
#include "dominion_helpers.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <string.h>
#include "rngs.h"
#include <math.h>

#define CARDUNDERTEST "adventurer"

int checkAdventurer(int, struct gameState*, struct gameState*, int);

int main () {

  int players, p1;
  int n, i, j;
  //enum values:      15     12        22       24        25         26            8            20         23       19
  int kCards[10] = {baron, remodel, embargo, salvager, sea_hag, treasure_map, council_room, ambassador, outpost, tribute};

  int validCards[13] = {baron, remodel, embargo, salvager, sea_hag, treasure_map, council_room, ambassador, outpost, tribute, copper, silver, gold};

  //adventurer does not utilize choice options, set all to off / 0
  int choice1 = 0;
  int choice2 = 0;
  int choice3 = 0;

  //adventurer does not utilize bonus, set to off / 0
  int bonus = 0;

  //tracks position, in hand, of card under test
  int cardUnderTestHandPos, randomCard;

  int randomSeed = 5; //seed for random game initialization
  int seedValue = 12; //seed for calls to rand()
  int failureFound = 0; //bool to track identification of bug/error
  int overallResult = 0;
  int testCases = 20000;
  struct gameState testGame, controlGame;

  printf("RANDOM TESTING: %s\n", CARDUNDERTEST);

  srand(seedValue);

  //randomize the gameState struct with random bytes
  for (n = 0; n < testCases; n++) {
    for (i = 0; i < sizeof(struct gameState); i++) {
      ((char*)&controlGame)[i] = rand() % (255 + 1 - 0) + 0;
    }

    //manually override bytes that Adventurer expects to meet certain pre-criteria
    players = rand() % ((MAX_PLAYERS + 1 - 0) + 0);
    //cannot play game without at least 2 playersv
    if (players < 2) {
      players = 2;
    } else if (players > 4) { //players should never be above 4 - block provides extra safety
      players = 4;
    }
    //controlGame.numPlayers = players;
    initializeGame(players, kCards, randomSeed, &controlGame);

    //randomly select player playing card under test
    p1 = rand() % (players);

    for (j = 0; j < players; j++){
      //assign random deckCount, then fill it with cards
      controlGame.deckCount[j] = rand() % (MAX_DECK + 0 - 0) + 0;
      //printf("Deck Count: %d\n", controlGame.deckCount[j]);
      for (i = 0; i < controlGame.deckCount[j]; i++) {
        randomCard = rand() % 14;
        controlGame.deck[j][i] = validCards[randomCard];
      }
      //assign random discardCount, then fill it with cards
      controlGame.discardCount[j] = rand() % (MAX_DECK + 0 - 0) + 0;
      //printf("Discard Count: %d\n", controlGame.discardCount[j]);
      for (i = 0; i < controlGame.discardCount[j]; i++) {
        randomCard = rand() % 14;
        controlGame.discard[j][i] = validCards[randomCard];
      }
      //assign random handCount, then fill it with cards
      controlGame.handCount[j] = rand() % (MAX_HAND + 0 - 0) + 0;
      //printf("Hand Count: %d\n", controlGame.handCount[j]);
      for (i = 0; i < controlGame.handCount[j]; i++) {
        randomCard = rand() % 14;
        controlGame.hand[j][i] = validCards[randomCard];
      }
    }

    //pick random spot in hand to insert card under test
    cardUnderTestHandPos = rand() % (controlGame.handCount[p1] + 1 - 0) + 0;
    controlGame.hand[p1][cardUnderTestHandPos] = adventurer;

    //manipulate game so it is the player with card under test turn
    controlGame.whoseTurn = p1;

    //copy controlGame state to testGame state
    memcpy(&testGame, &controlGame, sizeof(struct gameState));

    cardEffect(adventurer, choice1, choice2, choice3, &testGame, cardUnderTestHandPos, &bonus);


    failureFound = checkAdventurer(p1, &testGame, &controlGame, 0);

    if (failureFound && overallResult != 1) {
      overallResult = 1;
    }
  }

  if (overallResult == 1) {
    printf("****** | OVERALL RESULT: >>>TESTING FAILURES FOUND WITH SEED: %d<<< | ******\n\n", seedValue);
  } else {
    printf("****** | OVERALL RESULT: >>>ALL TESTS PASSED USING SEED: %d<<< | ******\n\n", seedValue);
  }

  return 0;
}




int checkAdventurer(int currentPlayer, struct gameState* testGame, struct gameState* controlGame, int extraTests) {

  struct counts {
    int copper;
    int silver;
    int gold;
    int total;
  };

  int failureFound = 0;
  static int actionTestFailureCount = 0;
  int expectedActionCountDelta = 1;

  struct counts preHand;
  struct counts preDeck;
  struct counts preDiscard;
  struct counts postHand;
  struct counts postDeck;
  struct counts postDiscard;

  preHand.copper = 0;
  preHand.silver = 0;
  preHand.gold = 0;
  preHand.total = 0;
  preDeck.copper = 0;
  preDeck.silver = 0;
  preDeck.gold = 0;
  preDeck.total = 0;
  preDiscard.copper = 0;
  preDiscard.silver = 0;
  preDiscard.gold = 0;
  preDiscard.total = 0;

  postHand.copper = 0;
  postHand.silver = 0;
  postHand.gold = 0;
  postHand.total = 0;
  postDeck.copper = 0;
  postDeck.silver = 0;
  postDeck.gold = 0;
  postDeck.total = 0;
  postDiscard.copper = 0;
  postDiscard.silver = 0;
  postDiscard.gold = 0;
  postDiscard.total = 0;

  int preAdventurerTreasureCount, postAdventurerTreasureCount;
  int noTreasureRemainingExpectedDelta = 0;
  int expectedTreasureCountDelta = 2;
  int x;

  //cycle over each card in players hand to get count of treasure cards in hand before adventurer is played
  preAdventurerTreasureCount = 0;
  for(x = 0; x < controlGame->handCount[currentPlayer]; x++){
      if (controlGame->hand[currentPlayer][x] == copper) {
        preHand.copper += 1;
      } else if (controlGame->hand[currentPlayer][x] == silver) {
        preHand.silver += 1;
      } else if (controlGame->hand[currentPlayer][x] == gold) {
        preHand.gold += 1;
      }
  }
  //cycle over each card in players hand to get count of treasure cards in hand after adventurer is played
  postAdventurerTreasureCount = 0;
  for(x = 0; x < testGame->handCount[currentPlayer]; x++){
      if (testGame->hand[currentPlayer][x] == copper) {
        postHand.copper += 1;
      } else if (testGame->hand[currentPlayer][x] == silver) {
        postHand.silver += 1;
      } else if (testGame->hand[currentPlayer][x] == gold) {
        postHand.gold += 1;
      }
  }
  //cycle over each card in players deck to get count of treasure cards in deck after adventurer is played
  for(x = 0; x < controlGame->deckCount[currentPlayer]; x++){
      if (controlGame->deck[currentPlayer][x] == copper) {
        preDeck.copper += 1;
      } else if (controlGame->deck[currentPlayer][x] == silver) {
        preDeck.silver += 1;
      } else if (controlGame->deck[currentPlayer][x] == gold) {
        preDeck.gold += 1;
      }
  }
  //cycle over each card in players discard to get count of treasure cards in discard after adventurer is played
  for(x = 0; x < controlGame->discardCount[currentPlayer]; x++){
      if (controlGame->discard[currentPlayer][x] == copper) {
        preDiscard.copper += 1;
      } else if (controlGame->discard[currentPlayer][x] == silver) {
        preDiscard.silver += 1;
      } else if (controlGame->discard[currentPlayer][x] == gold) {
        preDiscard.gold += 1;
      }
  }
  //cycle over each card in players deck to get count of treasure cards in deck after adventurer is played
  for(x = 0; x < testGame->deckCount[currentPlayer]; x++){
      if (testGame->deck[currentPlayer][x] == copper) {
        postDeck.copper += 1;
      } else if (testGame->deck[currentPlayer][x] == silver) {
        postDeck.silver += 1;
      } else if (testGame->deck[currentPlayer][x] == gold) {
        postDeck.gold += 1;
      }
  }
  //cycle over each card in players discard to get count of treasure cards in discard after adventurer is played
  for(x = 0; x < testGame->discardCount[currentPlayer]; x++){
      if (testGame->discard[currentPlayer][x] == copper) {
        postDiscard.copper += 1;
      } else if (testGame->discard[currentPlayer][x] == silver) {
        postDiscard.silver += 1;
      } else if (testGame->discard[currentPlayer][x] == gold) {
        postDiscard.gold += 1;
      }
  }
  //add up totals for each respective subcategory
  preHand.total = preHand.copper + preHand.silver + preHand.gold;
  preDeck.total = preDeck.copper + preDeck.silver + preDeck.gold;
  preDiscard.total = preDiscard.copper + preDiscard.silver + preDiscard.gold;
  postHand.total = postHand.copper + postHand.silver + postHand.gold;
  postDeck.total = postDeck.copper + postDeck.silver + postDeck.gold;
  postDiscard.total = postDiscard.copper + postDiscard.silver + postDiscard.gold;

  //assign preHand total values to var used in test result evaluations
  preAdventurerTreasureCount = preHand.total;
  postAdventurerTreasureCount = postHand.total;


  if ((postAdventurerTreasureCount - preAdventurerTreasureCount) != expectedTreasureCountDelta){
    printf("-----------------------------------------\nTEST 1: %s results in exactly 2 treasure cards being added to player's hand\n\n", CARDUNDERTEST);
    printf("Test 1 Grade: FAILURE\n");
    printf("Hand treasure pre: %d (%d copper, %d silver, %d gold)\n", preAdventurerTreasureCount, preHand.copper, preHand.silver, preHand.gold);
    printf("Hand treasure post: %d (%d copper, %d silver, %d gold)\n", postAdventurerTreasureCount, postHand.copper, postHand.silver, postHand.gold);
    printf("Hand treasure delta: %d, expected: %d\n\n", (postAdventurerTreasureCount - preAdventurerTreasureCount), expectedTreasureCountDelta);
    printf("Deck treasure pre: %d (%d copper, %d silver, %d gold)\n", preDeck.total, preDeck.copper, preDeck.silver, preDeck.gold);
    printf("Discard treasure pre: %d (%d copper, %d silver, %d gold)\n\n", preDiscard.total, preDiscard.copper, preDiscard.silver, preDiscard.gold);
    printf("Deck treasure post: %d (%d copper, %d silver, %d gold)\n", postDeck.total, postDeck.copper, postDeck.silver, postDeck.gold);
    printf("Discard treasure post: %d (%d copper, %d silver, %d gold)\n\n", postDiscard.total, postDiscard.copper, postDiscard.silver, postDiscard.gold);
    printf("Pre card makeup, hand: %d, deck: %d, discard: %d\n", controlGame->handCount[currentPlayer], controlGame->deckCount[currentPlayer], controlGame->discardCount[currentPlayer]);
    printf("Post card makeup, hand: %d, deck: %d, discard: %d\n\n", testGame->handCount[currentPlayer], testGame->deckCount[currentPlayer], testGame->discardCount[currentPlayer]);
    failureFound = 1;
  }

  if ((testGame->numActions - controlGame->numActions) != expectedActionCountDelta) {
    printf("\nTEST 2: adventurer results in consumption of exactly 1 action\n\n");
    printf("Test 2 - Number of Actions -1\n");
    printf("Grade: FAILURE\n");
    printf("Pre action count: %d\n", controlGame->numActions);
    printf("Post action count: %d\n", testGame->numActions);
    printf("Action count delta: %d\n", (testGame->numActions - controlGame->numActions));
    printf("Expected change: %d\n\n", expectedActionCountDelta);
    failureFound = 1;
  }

  if (((testGame->handCount[currentPlayer] - controlGame->handCount[currentPlayer]) - (postAdventurerTreasureCount - preAdventurerTreasureCount)) != 0){

    printf("\nTEST 3: hand remains unchanged when playing %s aside from addition of 2 treasure cards\n\n", CARDUNDERTEST);
    printf("Test 3 Grade: FAILURE\n");
    printf("Hand count pre: %d\n", controlGame->handCount[currentPlayer]);
    printf("Hand count post: %d\n", testGame->handCount[currentPlayer]);
    printf("Hand count delta: %d\n", (testGame->handCount[currentPlayer] - controlGame->handCount[currentPlayer]));
    printf("Hand treasure delta: %d\n", (postAdventurerTreasureCount - preAdventurerTreasureCount));
    printf("Change in hand count not accounted for by treasure card change: %d, expected: %d\n\n", ((testGame->handCount[currentPlayer] - controlGame->handCount[currentPlayer]) - (postAdventurerTreasureCount - preAdventurerTreasureCount)), 0);
    printf("Hand treasure pre: %d (%d copper, %d silver, %d gold)\n", preAdventurerTreasureCount, preHand.copper, preHand.silver, preHand.gold);
    printf("Hand treasure post: %d (%d copper, %d silver, %d gold)\n\n", postAdventurerTreasureCount, postHand.copper, postHand.silver, postHand.gold);
    printf("Deck treasure pre: %d (%d copper, %d silver, %d gold)\n", preDeck.total, preDeck.copper, preDeck.silver, preDeck.gold);
    printf("Discard treasure pre: %d (%d copper, %d silver, %d gold)\n\n", preDiscard.total, preDiscard.copper, preDiscard.silver, preDiscard.gold);
    printf("Deck treasure post: %d (%d copper, %d silver, %d gold)\n", postDeck.total, postDeck.copper, postDeck.silver, postDeck.gold);
    printf("Discard treasure post: %d (%d copper, %d silver, %d gold)\n\n", postDiscard.total, postDiscard.copper, postDiscard.silver, postDiscard.gold);
    printf("Pre card makeup, hand: %d, deck: %d, discard: %d\n", controlGame->handCount[currentPlayer], controlGame->deckCount[currentPlayer], controlGame->discardCount[currentPlayer]);
    printf("Post card makeup, hand: %d, deck: %d, discard: %d\n\n", testGame->handCount[currentPlayer], testGame->deckCount[currentPlayer], testGame->discardCount[currentPlayer]);
    failureFound = 1;
  }

  //validate proper behavior when player has no treasure cards to draw to (unknown to player at time)
  if ((preDeck.total + preDiscard.total) ==  0) {
    if ((postAdventurerTreasureCount - preAdventurerTreasureCount) != noTreasureRemainingExpectedDelta) {

      printf("\nTEST 4: Player playing %s while deck contains no treasure cards results in 0 treasure added to hand and NO infinite loop\n\n", CARDUNDERTEST);
      printf("Test 4 Grade: FAILURE\n");
      printf("Hand treasure pre: %d (%d copper, %d silver, %d gold)\n", preAdventurerTreasureCount, preHand.copper, preHand.silver, preHand.gold);
      printf("Hand treasure post: %d (%d copper, %d silver, %d gold)\n", postAdventurerTreasureCount, postHand.copper, postHand.silver, postHand.gold);
      printf("Hand treasure delta: %d\n", (postAdventurerTreasureCount - preAdventurerTreasureCount));
      printf("Expected hand treasure delta: %d\n\n", noTreasureRemainingExpectedDelta);
      printf("Deck treasure pre: %d (%d copper, %d silver, %d gold)\n", preDeck.total, preDeck.copper, preDeck.silver, preDeck.gold);
      printf("Deck treasure post: %d (%d copper, %d silver, %d gold)\n\n", postDeck.total, postDeck.copper, postDeck.silver, postDeck.gold);
      printf("Discard treasure pre: %d (%d copper, %d silver, %d gold)\n", preDiscard.total, preDiscard.copper, preDiscard.silver, preDiscard.gold);
      printf("Discard treasure post: %d (%d copper, %d silver, %d gold)\n\n", postDiscard.total, postDiscard.copper, postDiscard.silver, postDiscard.gold);
      printf("Pre card makeup, hand: %d, deck: %d, discard: %d\n", controlGame->handCount[currentPlayer], controlGame->deckCount[currentPlayer], controlGame->discardCount[currentPlayer]);
      printf("Post card makeup, hand: %d, deck: %d, discard: %d\n\n", testGame->handCount[currentPlayer], testGame->deckCount[currentPlayer], testGame->discardCount[currentPlayer]);
      failureFound = 1;
    }
  }
  return failureFound;
}
