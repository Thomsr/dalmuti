#include "user-player.h"

UserPlayer::UserPlayer(uint64_t cardLimit, uint64_t playerNumber)
    : Player(cardLimit, playerNumber) {}

bool UserPlayer::play(Cards::PlayedCardInfo &cardStackTop) {
  if (!canPlay(cardStackTop))
    return false;

  std::cout << "Top card(s): " << int(cardStackTop.card) << " "
            << cardStackTop.amount << "x" << std::endl;
  printCardsInHand();

  std::string card;
  uint64_t amount;
  do {
    std::cin >> card;
    if (card == "pass")
      return false;
    std::cin >> amount;
  } while (cardsInHand.count(std::stoi(card) < amount));

  cardStackTop = {Card(std::stoi(card)), amount};
  removeCardsFromHand(std::stoi(card), amount);
  return true;
}