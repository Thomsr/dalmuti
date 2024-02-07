#ifndef TABLE_H
#define TABLE_H

#include <chrono> // For std::chrono::milliseconds
#include <set>
#include <stack>
#include <thread>
#include <vector>

#include "basic-player.h"

class Table {
public:
  Table();
  Table(uint64_t totalPlayers, uint64_t cardLimit);
  void printCards();
  void printPlayerCards(uint64_t player);
  void printAllPlayerCards();
  void printPlayersRanking();
  void distributeCards();
  void swapCards();
  void reset();
  std::vector<BasicPlayer> play();

private:
  void setPlayers(uint64_t totalPlayers);
  void setCardLimit(uint64_t cardLimit);

  bool allPlayersDone();

  std::vector<BasicPlayer> players;
  std::vector<BasicPlayer> nextPlayers;
  std::multiset<Card> cards;
  Cards::PlayedCardInfo cardStackTop;
  uint64_t cardLimit;
};

#endif