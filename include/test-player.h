#ifndef TEST_PLAYER_H
#define TEST_PLAYER_H

#include "stat-player.h"

class TestPlayer : public StatPlayer {
public:
  TestPlayer(uint64_t cardLimit, uint64_t playerNumber);
  bool play(
    Cards::PlayedCardInfo &cardStackTop,
    uint64_t const &passes,
    std::multiset<Card> const &playedCards,
    std::vector<Opponent> const &opponentsInfo
  ) override;

private:
  bool getCardValueToPlay(
    CardValue &cardValue,
    std::vector<CardValue> const &cardValues,
    Cards::PlayedCardInfo const &cardStackTop
  );

  CardValue getCardValue(
    Card const &card,
    uint64_t const &jesters,
    std::multiset<Card> const &playedCards,
    std::vector<Opponent> const &opponentsInfo
  ) override;
};

#endif
