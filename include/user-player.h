#ifndef USER_PLAYER_H
#define USER_PLAYER_H

#include "player.h"

class UserPlayer : public Player {
public:
  UserPlayer(uint64_t cardLimit, uint64_t playerNumber);
  bool play(
    Cards::PlayedCardInfo &cardStackTop,
    uint64_t const passes,
    std::multiset<Card> const &playedCards,
    std::vector<size_t> const &opponentsHandSizes
  ) override;

private:
};

#endif
