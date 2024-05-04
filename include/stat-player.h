#ifndef STAT_PLAYER_H
#define STAT_PLAYER_H

#include "player.h"

class StatPlayer : public Player {
public:
  StatPlayer(uint64_t cardLimit, uint64_t playerNumber);
  bool play(
    Cards::PlayedCardInfo &cardStackTop,
    std::multiset<Card> const cards,
    playersInfo players
  ) override;
};

#endif