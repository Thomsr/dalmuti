#include "table.h"

Table::Table() : cardStackTop({0, 0}), cardLimit(12) {
  setPlayers(8);
  setCardLimit(12);
}

Table::Table(uint64_t totalPlayers, uint64_t cardLimit)
    : cardStackTop({0, 0}), cardLimit(cardLimit) {
  setPlayers(totalPlayers);
  setCardLimit(cardLimit);
}

void Table::printCards() {
  for (uint64_t card = 1; card <= cardLimit; card++)
    std::cout << card << ":" << cards.count(card) << " ";

  std::cout << std::endl;
}

void Table::printPlayerCards(uint64_t player) {
  std::cout << player << "\t";
  players[player]->printCardsInHand();
}

void Table::printAllPlayerCards() {
  for (size_t player = 0; player < players.size(); player++) {
    std::cout << players[player]->getPlayerNumber() << "\t";
    players[player]->printCardsInHand();
  }
}

void Table::printPlayersRanking() {
  for (size_t player = 0; player < players.size(); player++)
    std::cout << players[player]->getPlayerNumber() << " ";
  std::cout << std::endl;
}

void Table::printPlayersHandValue() {
  for (size_t player = 0; player < players.size(); player++) {
    std::cout << players[player]->getHandValue() << std::endl << std::endl;
  }
}

void Table::distributeCards() {
  // Shuffle the cards
  std::vector<Card> shuffledCards(cards.begin(), cards.end());
  std::random_device rd;
  std::default_random_engine rng(rd());
  std::shuffle(shuffledCards.begin(), shuffledCards.end(), rng);

  // Distribute cards to players
  size_t playerIndex = 0;
  for (Card card : shuffledCards) {
    players[playerIndex]->addCardsToHand(card);
    playerIndex = (playerIndex + 1) % players.size();
  }

  // Clear the cards from the table after distribution
  cards.clear();
}

void Table::swapCards() {
  if (players.size() < 4)
    throw std::runtime_error("Table must contain at least 4 players");

  players[players.size() - 1]->addCardsToHand(players[0]->getWorstCard());
  players[0]->removeCardsFromHand(players[0]->getWorstCard(), 1);
  players[players.size() - 1]->addCardsToHand(players[0]->getWorstCard());
  players[0]->removeCardsFromHand(players[0]->getWorstCard(), 1);

  players[0]->addCardsToHand(players[players.size() - 1]->getBestCard());
  players[players.size() - 1]->removeCardsFromHand(
      players[players.size() - 1]->getBestCard(), 1);
  players[0]->addCardsToHand(players[players.size() - 1]->getBestCard());
  players[players.size() - 1]->removeCardsFromHand(
      players[players.size() - 1]->getBestCard(), 1);

  players[players.size() - 2]->addCardsToHand(players[1]->getWorstCard());
  players[1]->removeCardsFromHand(players[1]->getWorstCard(), 1);
  players[1]->addCardsToHand(players[players.size() - 2]->getBestCard());
  players[players.size() - 2]->removeCardsFromHand(
      players[players.size() - 2]->getBestCard(), 1);
}

void Table::reset() {
  cardStackTop = {0, 0};
  for (auto player : nextPlayers)
    player->resetPlayer();
  players.clear();
  players = nextPlayers;
  nextPlayers.clear();
  setCardLimit(12);
  distributeCards();
}

std::vector<Player *> Table::play() {
  uint64_t currentPlayer = 0;
  uint64_t passes = 0;
  while (!allPlayersDone()) {
    if (currentPlayer >= uint64_t(players.size()))
      currentPlayer = 0;

    if (players[currentPlayer]->play(cardStackTop)) {
      passes = 0;
    } else {
      passes++;
    }
    if (passes == uint64_t(players.size() - 1))
      cardStackTop = {0, 0};

    currentPlayer++;
  }
  printPlayersRanking();
  reset();
  swapCards();
  return players;
}

void Table::setPlayers(uint64_t totalPlayers) {
  for (uint64_t player = 0; player < totalPlayers / 2; player++)
    players.push_back(new BestCardPlayer(cardLimit, player));

  for (uint64_t player = totalPlayers / 2; player < totalPlayers; player++)
    players.push_back(new WorstCardPlayer(cardLimit, player));
}

void Table::setCardLimit(uint64_t cardLimit) {
  for (uint64_t card = 1; card <= cardLimit; card++)
    for (uint64_t amount = 0; amount < card; amount++)
      cards.insert(card);

  // Add jesters
  cards.insert(cardLimit + 1);
  cards.insert(cardLimit + 1);
}

bool Table::allPlayersDone() {
  for (size_t player = 0; player < players.size();) {
    if (players[player]->getAmountOfCardsInHand() == 0) {
      nextPlayers.push_back(players[player]);
      players.erase(players.begin() + player);
    } else {
      player++; // Only increment if the player is not removed
    }
  }

  if (players.size() == 1) {
    nextPlayers.push_back(players[0]);
    players.erase(players.begin());
    return true;
  }
  return false;
}

Table::~Table() {
  for (auto player : players)
    delete player;
}