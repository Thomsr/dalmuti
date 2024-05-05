#ifndef WORST_STAT_CARD_PLAYER_H
#define WORST_STAT_CARD_PLAYER_H

#include "player.h"

class WorstStatCardPlayer : public Player {
public:
  WorstStatCardPlayer(uint64_t cardLimit, uint64_t playerNumber);
  bool play(
    Cards::PlayedCardInfo &cardStackTop,
    std::multiset<Card> const cards,
    playersInfo players
  ) override;

private:
  double startingHandValue;
};

#endif