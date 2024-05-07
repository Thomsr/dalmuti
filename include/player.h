#ifndef PLAYER_H
#define PLAYER_H

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <random>
#include <set>
#include <stdexcept>

#include "cards.h"

enum PlayerType {
  WORSTCARD,
  BESTCARD,
  USER,
  WORSTSTAT
};

struct play {
  uint64_t nrPlayers;
  std::vector<size_t> playersHandSize;
  uint64_t passes;
};

class Player {
public:
  Player(uint64_t cardLimit, uint64_t playerNumber, PlayerType playerType);

  virtual bool play(
    Cards::PlayedCardInfo &cardStackTop,
    uint64_t const &passes,
    std::multiset<Card> const &playedCards,
    std::vector<size_t> const &opponentsHandSizes
  ) = 0;

  bool canPlay(Cards::PlayedCardInfo const &cardStackTop);
  bool canPlayCard(
    Card const &card,
    uint64_t const &amount,
    Cards::PlayedCardInfo const &cardStackTop
  ) const;
  void addCardsToHand(Card const &card);
  void removeCardsFromHand(Card const &card, uint64_t const &amount);
  void printCardsInHand();

  inline size_t getAmountOfCardsInHand() { return cardsInHand.size(); }
  inline uint64_t getPlayerNumber() const { return playerNumber; }
  inline PlayerType getPlayerType() const { return playerType; }
  inline Card getBestCard() { return *(cardsInHand.begin()); }
  inline Card getWorstCard() { return *(cardsInHand.rbegin()); }
  inline std::multiset<Card> getCardsInHand() { return cardsInHand; }

  virtual void resetPlayer();
  virtual ~Player(){};

protected:
  inline bool isValidCard(Card card) {
    return card >= 1 && card <= cardLimit + 1;
  }

  inline bool isFirstInRound(Cards::PlayedCardInfo const &cardStackTop) {
    return cardStackTop.card == 0;
  }

  Card const jester;
  uint64_t playerNumber;
  PlayerType playerType;
  std::multiset<Card> cardsInHand;
  const uint64_t cardLimit;
};

#endif
