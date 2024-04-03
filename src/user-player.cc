#include "user-player.h"

UserPlayer::UserPlayer(uint64_t cardLimit, uint64_t playerNumber)
    : Player(cardLimit, playerNumber, PlayerType::USER) {}

bool UserPlayer::play(Cards::PlayedCardInfo &cardStackTop,
                      std::multiset<Card> const cards) {
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
  } while (cardsInHand.count(uint64_t(std::stoi(card)) < amount));

  cardStackTop = {Card(std::stoi(card)), amount, 0};
  removeCardsFromHand(std::stoi(card), amount);
  return true;
}