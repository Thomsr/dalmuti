#ifndef BEST_POSSIBLE_CARD_PLAYER_H
#define BEST_POSSIBLE_CARD_PLAYER_H

#include "player.h"

class BestCardPlayer : public Player {
public:
  BestCardPlayer(uint64_t cardLimit, uint64_t playerNumber);
  bool play(Cards::PlayedCardInfo &cardStackTop,
            std::multiset<Card> const cards) override;
};

#endif