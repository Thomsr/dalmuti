#include "player.h"

const double TOL = 1E-12;

Player::Player(uint64_t cardLimit, uint64_t playerNumber, PlayerType playerType)
  : playerNumber(playerNumber), playerType(playerType), cardLimit(cardLimit) {}

void Player::resetPlayer() {
  cardsInHand.clear();
}

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
    if (cardsInHand.count(card) + cardsInHand.count(cardLimit + 1) >=
        cardStackTop.amount)
      return true;
  }
  return false;
}

unsigned long long Player::combination(int n, int k) {
  // std::cout << n << " " << k << std::endl;
  if (n < 0 || k < 0)
    return 0;

  if (k == 0 || k == n)
    return 1;
  else {
    if (combinations[n][k] == 0)
      combinations[n][k] = combination(n - 1, k - 1) + combination(n - 1, k);
    return combinations[n][k];
  }
}

double Player::hypergeometricProbability(int n, int x, int N, int M) {
  double numerator = combination(M, x) * combination(N - M, n - x);
  double denominator = combination(N, n);
  return numerator / denominator;
}

/**
 * @returns The value of amount times card, higher is better.
 */
double Player::getPlayableChance(
  Card card,
  uint64_t amount,
  std::multiset<Card> const cards,
  playersInfo players
) {
  double playChance = 0;
  uint64_t totalCardsLeft = 80 - cards.size() - players.playersHand.size();

  for (uint64_t currentCard = card + 1; currentCard < cardLimit;
       currentCard++) {
    for (uint64_t player = 0; player < players.playersHandSize.size();
         player++) {
      uint64_t cardsLeft = currentCard - cards.count(currentCard) -
                           players.playersHand.count(currentCard);
      if (cardsLeft > 0)
        cardsLeft += 2 - cards.count(cardLimit + 1) -
                     players.playersHand.count(cardLimit + 1);

      if (cardsLeft < amount)
        continue;

      for (uint64_t j = cardsLeft; j > cardsLeft - amount; j--) {
        playChance +=
          (hypergeometricProbability(
             players.playersHandSize[player], amount, totalCardsLeft, cardsLeft
           ) /
           (player + 1));
      }
    }
  }
  return playChance;
}

/**
 * @returns The value of amount x card of closing the round,
 * lower is better.
 * @example card = 1, amount = 1 closeValue = 0.0. Since no card can be played
 * over 1x 1.
 */
double Player::getRoundCloseChance(
  Card card,
  uint64_t amount,
  std::multiset<Card> const cards,
  playersInfo players
) {
  double roundCloseChance = 0;
  uint64_t totalCardsLeft = 80 - cards.size() - players.playersHand.size();

  for (uint64_t currentCard = amount; currentCard < card; currentCard++) {
    for (uint64_t player = 0; player < players.nrPlayers - 1; player++) {
      uint64_t cardsLeft = currentCard - cards.count(currentCard) -
                           players.playersHand.count(currentCard);
      if (cardsLeft < amount)
        continue;

      for (uint64_t j = cardsLeft; j > cardsLeft - amount; j--) {
        roundCloseChance += hypergeometricProbability(
          players.playersHandSize[player], amount, totalCardsLeft, cardsLeft
        );
      }
    }
  }
  return roundCloseChance;
}

double
Player::getHandValue(std::multiset<Card> const cards, playersInfo players) {
  double handValue = 0.0;
  for (int i = 1; i <= 12; i++)
    if (cardsInHand.count(i) > 0)
      handValue += getPlayableChance(i, cardsInHand.count(i), cards, players);

  return handValue;
}

void Player::printCardValues() {
  std::cout << "Values: " << std::endl;
  for (int i = 1; i <= 12; i++) {
    std::cout << i << " " << cardsInHand.count(i) << "x"
              << ":";
    // if (cardsInHand.count(i) > 0)
    // std::cout << std::setprecision(2) << cardValue(i,
    // cardsInHand.count(i));
    // std::cout << std::endl;
  }
  std::cout << std::endl;
}
