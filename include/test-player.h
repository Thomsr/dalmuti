#ifndef TEST_PLAYER_H
#define TEST_PLAYER_H

#include "stat-player.h"

class TestPlayer : public StatPlayer {
public:
  TestPlayer(uint64_t cardLimit, uint64_t playerNumber);
  bool play(
    Cards::PlayedCardInfo &cardStackTop,
    uint64_t const passes,
    std::multiset<Card> const &playedCards,
    std::vector<size_t> const &opponentsHandSizes
  ) override;

  void resetPlayer() override {
    cardsInHand.clear();
    startingHandValue = 0;
  }

private:
  double startingHandValue;
};

#endif
