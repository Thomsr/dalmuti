#ifndef TABLE_H
#define TABLE_H

#include <chrono>
#include <set>
#include <stack>
#include <thread>
#include <vector>

#include "best-card-player.h"
#include "stat-player.h"
#include "test-player.h"
#include "user-player.h"
#include "worst-card-player.h"
#include "worst-stat-card-player.h"

class Table {
public:
  Table(uint64_t cardLimit, bool debug);
  void addPlayer(Player *player);
  void printCards();
  void printPlayerCards(uint64_t player);
  void printAllPlayerCards();
  void printPlayersRanking();
  void distributeCards();
  void swapCards();
  void reset();
  std::vector<Player *> play();

  ~Table();

private:
  void setCardLimit(uint64_t cardLimit);
  bool allPlayersDone();
  std::vector<size_t> getOpponentsHandSizes(int player);
  void addCardsToPlayedCards(Card card, uint64_t amount);

  std::vector<Player *> players;
  std::vector<Player *> nextPlayers;
  std::multiset<Card> cards;
  std::multiset<Card> playedCards;
  Cards::PlayedCardInfo cardStackTop;
  uint64_t cardLimit;

  bool debug;
};

#endif
