#include "table.h"

Table::Table() : cardStackTop({0, 0, 0}), cardLimit(12) { setCardLimit(12); }

Table::Table(uint64_t cardLimit)
    : cardStackTop({0, 0, 0}), cardLimit(cardLimit)
{
  setCardLimit(cardLimit);
}

void Table::addPlayer(Player *player) { players.push_back(player); }

void Table::printCards()
{
  for (uint64_t card = 1; card <= cardLimit; card++)
    std::cout << card << ":" << cards.count(card) << " ";

  std::cout << std::endl;
}

void Table::printPlayerCards(uint64_t player)
{
  std::cout << player << "\t";
  players[player]->printCardsInHand();
}

void Table::printAllPlayerCards()
{
  for (size_t player = 0; player < players.size(); player++)
  {
    std::cout << players[player]->getPlayerNumber() << "\t";
    players[player]->printCardsInHand();
  }
}

void Table::printPlayersRanking()
{
  for (size_t player = 0; player < players.size(); player++)
    std::cout << players[player]->getPlayerNumber() << " ";
  std::cout << std::endl;
}

void Table::printPlayersHandValue()
{
  for (size_t currentPlayer = 0; currentPlayer < players.size();
       currentPlayer++)
  {
    std::vector<size_t> playersHandSize;
    for (size_t player = 0; player < players.size(); player++)
    {
      if (player != currentPlayer)
        playersHandSize.push_back(players[player]->getAmountOfCardsInHand());
    }
    playersInfo playersInfo = {players[currentPlayer]->getCardsInHand(),
                               static_cast<uint64_t>(players.size()),
                               playersHandSize};
    std::cout << players[currentPlayer]->getHandValue(cards, playersInfo)
              << std::endl;
  }
}

void Table::distributeCards()
{
  // Shuffle the cards
  std::vector<Card> shuffledCards(cards.begin(), cards.end());
  std::random_device rd;
  std::default_random_engine rng(rd());
  std::shuffle(shuffledCards.begin(), shuffledCards.end(), rng);

  // Distribute cards to players
  size_t playerIndex = 0;
  for (Card card : shuffledCards)
  {
    players[playerIndex]->addCardsToHand(card);
    playerIndex = (playerIndex + 1) % players.size();
  }

  // Clear the cards from the table after distribution
  cards.clear();
}

void Table::swapCards()
{
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

void Table::reset()
{
  cardStackTop = {0, 0, 0};
  for (auto player : nextPlayers)
    player->resetPlayer();
  players.clear();
  players = nextPlayers;
  nextPlayers.clear();
  setCardLimit(12);
  distributeCards();
}

std::vector<Player *> Table::play()
{
  uint64_t currentPlayer = 0;
  uint64_t passes = 0;

  while (!allPlayersDone())
  {
    std::vector<size_t> playersHandSize;
    for (size_t player = 0; player < players.size(); player++)
    {
      if (player != currentPlayer)
        playersHandSize.push_back(players[player]->getAmountOfCardsInHand());
    }
    playersInfo playersInfo = {
        players[currentPlayer]->getCardsInHand(),
        static_cast<uint64_t>(players.size()),
        playersHandSize};

    if (players[currentPlayer]->play(cardStackTop, cards, playersInfo))
    {
      passes = 0;
      // std::cout << currentPlayer << " played: " << int(cardStackTop.card) <<
      // " "
      //           << int(cardStackTop.amount) << "x " << cardStackTop.jesters
      //           << std::endl;
      for (uint64_t i = 0; i < cardStackTop.amount; i++)
        cards.insert(cardStackTop.card);
    }
    else
    {
      // std::cout << currentPlayer << " passed" << std::endl;
      passes++;
    }

    // std::chrono::milliseconds timespan(1000);
    // std::this_thread::sleep_for(timespan);

    // players[3]->printCardsInHand();

    if (passes == uint64_t(players.size() - 1))
      cardStackTop = {0, 0, 0};

    currentPlayer = (currentPlayer + 1) % players.size();
  }

  // printPlayersRanking();
  reset();
  swapCards();
  return players;
}

void Table::setCardLimit(uint64_t cardLimit)
{
  cards.clear();
  for (uint64_t card = 1; card <= cardLimit; card++)
    for (uint64_t amount = 0; amount < card; amount++)
      cards.insert(card);

  // Add jesters
  cards.insert(cardLimit + 1);
  cards.insert(cardLimit + 1);
}

bool Table::allPlayersDone()
{
  for (size_t player = 0; player < players.size();)
  {
    if (players[player]->getAmountOfCardsInHand() == 0)
    {
      nextPlayers.push_back(players[player]);
      // std::cout << players[player]->getPlayerNumber() << " is done"
      //           << std::endl;
      players.erase(players.begin() + player);
    }
    else
    {
      player++; // Only increment if the player is not removed
    }
  }

  if (players.size() == 1)
  {
    nextPlayers.push_back(players[0]);
    players.erase(players.begin());
    return true;
  }
  return false;
}

Table::~Table()
{
  for (auto player : players)
    delete player;
}