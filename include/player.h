#ifndef PLAYER_H
#define PLAYER_H

#include <algorithm>
#include <iostream>
#include <random>
#include <set>
#include <stdexcept>

#include "cards.h"

class Player {
public:
  Player(uint64_t cardLimit, uint64_t playerNumber);
  void resetPlayer();
  void addCardsToHand(Card card);
  void removeCardsFromHand(Card card, uint64_t amount);
  void printCardsInHand();
  size_t getAmountOfCardsInHand() { return cardsInHand.size(); }
  uint64_t getPlayerNumber() const { return playerNumber; }
  Card getBestCard() { return *(cardsInHand.begin()); }
  Card getWorstCard() { return *(cardsInHand.rbegin()); }
  virtual bool play(Cards::PlayedCardInfo &cardStackTop) = 0;

protected:
  uint64_t playerNumber;
  std::multiset<Card> cardsInHand;
  uint64_t cardLimit;
};

#endif