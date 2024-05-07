#ifndef WORST_STAT_CARD_PLAYER_H
#define WORST_STAT_CARD_PLAYER_H

#include "stat-player.h"

class WorstStatCardPlayer : public StatPlayer {
public:
  WorstStatCardPlayer(uint64_t cardLimit, uint64_t playerNumber);
  bool play(
    Cards::PlayedCardInfo &cardStackTop,
    uint64_t const &passes,
    std::multiset<Card> const &playedCards,
    std::vector<size_t> const &opponentsHandSizes
  ) override;

private:
  CardValue getWorstCardValue(
    std::vector<CardValue> const cardValues,
    Cards::PlayedCardInfo const &cardStackTop
  );

  void getCardValues(
    std::vector<CardValue> &cardValues,
    Cards::PlayedCardInfo const &cardStackTop,
    std::multiset<Card> const &playedCards,
    std::vector<size_t> const &opponentsHandSizes
  );
};

#endif
