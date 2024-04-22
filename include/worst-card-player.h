#ifndef WORST_CARD_PLAYER_H
#define WORST_CARD_PLAYER_H

#include "player.h"

class WorstCardPlayer : public Player {
public:
  WorstCardPlayer(uint64_t cardLimit, uint64_t playerNumber);
  bool play(Cards::PlayedCardInfo &cardStackTop,
            std::multiset<Card> const cards, playersInfo players) override;
};

#endif