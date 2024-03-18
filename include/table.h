#ifndef TABLE_H
#define TABLE_H

#include <chrono> // For std::chrono::milliseconds
#include <set>
#include <stack>
#include <thread>
#include <vector>

#include "best-card-player.h"
#include "user-player.h"
#include "worst-card-player.h"

class Table {
public:
  Table();
  Table(uint64_t totalPlayers, uint64_t cardLimit);
  void printCards();
  void printPlayerCards(uint64_t player);
  void printAllPlayerCards();
  void printPlayersRanking();
  void printPlayersHandValue();
  void distributeCards();
  void swapCards();
  void reset();
  std::vector<Player *> play();

  ~Table();

private:
  void setPlayers(uint64_t totalPlayers);
  void setCardLimit(uint64_t cardLimit);

  bool allPlayersDone();

  std::vector<Player *> players;
  std::vector<Player *> nextPlayers;
  std::multiset<Card> cards;
  Cards::PlayedCardInfo cardStackTop;
  uint64_t cardLimit;
};

#endif