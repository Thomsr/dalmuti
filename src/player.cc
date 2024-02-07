#include "player.h"

Player::Player(uint64_t cardLimit, uint64_t playerNumber)
    : playerNumber(playerNumber), cardLimit(cardLimit) {}

void Player::resetPlayer() { cardsInHand.clear(); }

void Player::addCardsToHand(Card card) {
  if (!Cards::isValidCard(card))
    throw std::range_error("Card " + std::to_string(card) + " is not valid");
  cardsInHand.insert(card);
}

void Player::removeCardsFromHand(Card card, uint64_t amount) {
  if (!Cards::isValidCard(card))
    throw std::range_error("Card " + std::to_string(card) + " is not valid");

  if (cardsInHand.count(card) < amount)
    throw std::runtime_error(
        "Provided remove amount exceeds the amount of removable cards");

  if (cardsInHand.count(card) == amount) {
    cardsInHand.erase(card);
    return;
  }

  for (uint64_t count = 0; count < amount; count++)
    cardsInHand.erase(cardsInHand.find(card));
}

void Player::printCardsInHand() {
  for (int card = 1; card <= 12; card++)
    std::cout << card << ":" << cardsInHand.count(card) << " ";

  std::cout << std::endl;
}
