#ifndef PLAYER_H
#define PLAYER_H

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <random>
#include <set>
#include <stdexcept>

#include "cards.h"

enum PlayerType { WORSTCARD, BESTCARD, STAT, USER };

class Player {
public:
  Player(uint64_t cardLimit, uint64_t playerNumber, PlayerType PlayerType);

  virtual bool play(Cards::PlayedCardInfo &cardStackTop,
                    std::multiset<Card> const cards) = 0;

  void resetPlayer();
  void addCardsToHand(Card card);
  void removeCardsFromHand(Card card, uint64_t amount);
  void printCardsInHand();
  bool canPlay(Cards::PlayedCardInfo const &cardStackTop);
  size_t getAmountOfCardsInHand() { return cardsInHand.size(); }
  uint64_t getPlayerNumber() const { return playerNumber; }
  PlayerType getPlayerType() const { return playerType; }
  Card getBestCard() { return *(cardsInHand.begin()); }
  Card getWorstCard() { return *(cardsInHand.rbegin()); }

  double cardValue(Card card, uint64_t amount, std::multiset<Card> const cards);
  double getHandValue(std::multiset<Card> const cards);
  void printCardValues();

  virtual ~Player(){};

protected:
  inline bool isValidCard(Card card) {
    return card >= 1 && card <= cardLimit + 1;
  }

  uint64_t playerNumber;
  PlayerType playerType;
  std::multiset<Card> cardsInHand;
  uint64_t cardLimit;
};

#endif