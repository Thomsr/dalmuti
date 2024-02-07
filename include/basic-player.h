#ifndef BASIC_PLAYER_H
#define BASIC_PLAYER_H

#include "player.h"

class BasicPlayer : public Player {
public:
  BasicPlayer(uint64_t cardLimit, uint64_t playerNumber);
  bool play(Cards::PlayedCardInfo &cardStackTop) override;
};

#endif