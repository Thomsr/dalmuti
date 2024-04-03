#include "player.h"

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

double combination(int n, int r){
  int nFac = n, rFac = r, nrFac = n - r;
  for(int i = n-1; i > 0; i--)
    nFac *= i;
  
  for(int i = r-1; i > 0; i--)
    rFac *= i;
  
  for(int i = n-r-1; i > 0; i--)
    nrFac *= i;

  return double(nFac / double(rFac * nrFac));
}

double Player::cardValue(Card card, uint64_t amount,
                         std::multiset<Card> const cards, std::multiset<Card> playersHand, uint64_t nrPlayers, std::vector<size_t> playersHandSize) {
                          // Eigen hand toevoegen
  double cardValue = 0;
  for (uint64_t currentCard = amount; currentCard < card; currentCard++) {
    for(uint64_t player = 0; player < nrPlayers - 1; player++){
      uint64_t cardsLeft = card - cards.count(currentCard) - playersHand.count(currentCard);
      if (cardsLeft < amount)
        continue;

      // std::cout << "cardsLeft: " << cardsLeft << " "
      //           << "amount: " << amount << " "
      //           << "currentCard: " << currentCard << std::endl;
      for (uint64_t j = cardsLeft; j > cardsLeft - amount; j--)
        cardValue += (combination(cardsLeft, amount) * combination(80-cards.size()-cardsLeft - playersHand.size() + playersHand.count(currentCard), playersHandSize[player] - amount)) / combination(80-cards.size()-playersHand.size(), playersHandSize[player]);
    }
  }

  // std::cout << cardValue << std::endl;
  return cardValue;
}

double Player::getHandValue(std::multiset<Card> const cards) {
  double handValue = 0.0;
  for (int i = 1; i <= 12; i++)
    if (cardsInHand.count(i) > 0)
      handValue += cardValue(i, cardsInHand.count(i), cards);

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