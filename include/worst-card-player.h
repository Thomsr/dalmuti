#ifndef WORST_CARD_PLAYER_H
#define WORST_CARD_PLAYER_H

#include "player.h"

class WorstCardPlayer : public Player {
public:
  WorstCardPlayer(uint64_t cardLimit, uint64_t playerNumber);
  bool play(
    Cards::PlayedCardInfo &cardStackTop,
    uint64_t const &passes,
    std::multiset<Card> const &playedCards,
    std::vector<size_t> const &opponentsHandSizes
  ) override;
};

#endif
