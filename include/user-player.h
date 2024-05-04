#ifndef USER_PLAYER_H
#define USER_PLAYER_H

#include "player.h"

class UserPlayer : public Player {
public:
  UserPlayer(uint64_t cardLimit, uint64_t playerNumber);
  bool play(
    Cards::PlayedCardInfo &cardStackTop,
    std::multiset<Card> const cards,
    playersInfo players
  ) override;

private:
};

#endif
