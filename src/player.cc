#include "player.h"

#include <cfenv>

const double TOL = 1E-9;

Player::Player(uint64_t cardLimit, uint64_t playerNumber, PlayerType playerType)
    : playerNumber(playerNumber), playerType(playerType), cardLimit(cardLimit) {
}

void Player::resetPlayer() { cardsInHand.clear(); }

void Player::addCardsToHand(Card card) {
  if (!isValidCard(card))
    throw std::range_error("Card " + std::to_string(card) + " is not valid");
  cardsInHand.insert(card);
}

void Player::removeCardsFromHand(Card card, uint64_t amount) {
  if (!isValidCard(card))
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
  for (uint64_t card = 1; card <= cardLimit + 1; card++)
    std::cout << card << ":" << cardsInHand.count(card) << " ";

  std::cout << std::endl;
}

bool Player::canPlay(Cards::PlayedCardInfo const &cardStackTop) {
  for (Card card = cardStackTop.card - 1; card >= cardStackTop.amount; card--) {
    if (cardsInHand.count(card) + cardsInHand.count(cardLimit + 1) >=
        cardStackTop.amount)
      return true;
  }
  return false;
}

double Player::cardValue(Card card, uint64_t amount,
                         std::multiset<Card> const cards, playersInfo players) {
  double cardValue = 0;
  for (uint64_t currentCard = amount; currentCard < card; currentCard++) {
    for (uint64_t player = 0; player < players.nrPlayers - 1; player++) {
      uint64_t cardsLeft = card - cards.count(currentCard) -
                           players.playersHand.count(currentCard);
      if (cardsLeft < amount)
        continue;

      for (uint64_t j = cardsLeft; j > cardsLeft - amount; j--) {
        
      }
    }
  }

  std::cout << cardValue << std::endl;
  return cardValue;
}

double Player::getHandValue(std::multiset<Card> const cards,
                            playersInfo players) {
  double handValue = 0.0;
  for (int i = 1; i <= 12; i++)
    if (cardsInHand.count(i) > 0)
      handValue += cardValue(i, cardsInHand.count(i), cards, players);

  return handValue;
  return 0.0;
}

void Player::printCardValues() {
  std::cout << "Values: " << std::endl;
  for (int i = 1; i <= 12; i++) {
    std::cout << i << " " << cardsInHand.count(i) << "x"
              << ":";
    if (cardsInHand.count(i) > 0)
      // std::cout << std::setprecision(2) << cardValue(i,
      // cardsInHand.count(i));
      std::cout << std::endl;
  }
  std::cout << std::endl;
}