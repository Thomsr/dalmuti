#ifndef STAT_PLAYER_H
#define STAT_PLAYER_H

#include "player.h"

struct CardValue {
  double totalChance;
  double playableChance;
  double roundCloseChance;
  Card card;
  uint64_t jesters;
};

class StatPlayer : public Player {
public:
  StatPlayer(uint64_t cardLimit, uint64_t playerNumber, PlayerType playerType);

  double getHandValue(
    std::multiset<Card> const &playedCards,
    std::vector<size_t> const &opponentsHandSizes
  );

protected:
  /**
   * @returns The value of amount times card, higher is better.
   */
  double getPlayableChance(
    Card const &card,
    uint64_t const &amount,
    std::multiset<Card> const &playedCards,
    std::vector<size_t> const &opponentsHandSizes
  );

  /**
   * @returns The value of amount x card of closing the round,
   * lower is better.
   * @example card = 1, amount = 1 closeValue = 0.0. Since no card can be played
   * over 1x 1.
   */
  double getRoundCloseChance(
    Card const &card,
    uint64_t const &amount,
    std::multiset<Card> const &playedCards,
    std::vector<size_t> const &opponentsHandSizes
  );

  static bool sortCardValue(const CardValue &a, const CardValue &b) {
    return a.totalChance < b.totalChance;
  }

  void getCardValues(
    std::vector<CardValue> &cardValues,
    Cards::PlayedCardInfo const &cardStackTop,
    std::multiset<Card> const &playedCards,
    std::vector<size_t> const &opponentsHandSizes
  );

  virtual CardValue getCardValue(
    Card const &card,
    uint64_t const &jesters,
    std::multiset<Card> const &playedCards,
    std::vector<size_t> const &opponentsHandSizes
  ) = 0;

  void printCardValues(std::vector<CardValue> cardValues);

private:
  static double combinations[80][80];
  unsigned long long combination(const int n, const int k);
  double hypergeometricProbability(
    const int &n, const int &x, const int &N, const int &M
  );
  uint64_t
  getCardsLeft(Card const &card, std::multiset<Card> const &playedCards);
};

#endif
