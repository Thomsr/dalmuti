#include "player.h"

const double TOL = 1E-12;

Player::Player(uint64_t cardLimit, uint64_t playerNumber, PlayerType playerType)
  : jester(cardLimit + 1), playerNumber(playerNumber), playerType(playerType),
    cardLimit(cardLimit) {}

void Player::resetPlayer() {
  cardsInHand.clear();
}

void Player::addCardsToHand(Card card) {
  if (!isValidCard(card))
    throw std::range_error("Card " + std::to_string(card) + " is not valid");
  cardsInHand.insert(card);
}

void Player::removeCardsFromHand(Card card, uint64_t amount) {
  if (amount == 0)
    return;

  if (!isValidCard(card))
    throw std::range_error("Card " + std::to_string(card) + " is not valid");

  if (cardsInHand.count(card) < amount)
    throw std::runtime_error(
      "Provided remove amount exceeds the amount of removable cards " +
      std::to_string(card) + " " + std::to_string(amount) + " " +
      std::to_string(cardsInHand.count(card)) + " " +
      std::to_string(cardsInHand.count(cardLimit + 1))
    );

  if (cardsInHand.count(card) == amount) {
    cardsInHand.erase(card);
    return;
  }

  for (uint64_t count = 0; count < amount; count++)
    cardsInHand.erase(cardsInHand.find(card));
}

void Player::printCardsInHand() {
  for (uint64_t card = 1; card <= cardLimit + 1; card++)
    std::cout << card << ":" << cardsInHand.count(card) << " ";

  std::cout << std::endl;
}

bool Player::canPlay(Cards::PlayedCardInfo const &cardStackTop) {
  for (Card card = cardStackTop.card - 1; card >= cardStackTop.amount; card--) {
    if (cardsInHand.count(card) + cardsInHand.count(cardLimit + 1) >= cardStackTop.amount)
      return true;
  }
  return false;
}

bool Player::canPlayCard(
  const Card card,
  const uint64_t amount,
  const Cards::PlayedCardInfo &cardStackTop
) const {
  return card < cardStackTop.card &&
         amount == cardStackTop.amount + cardStackTop.jesters;
}
