#include "stat-player.h"

double StatPlayer::combinations[80][80] = {};

StatPlayer::StatPlayer(
  uint64_t cardLimit, uint64_t playerNumber, PlayerType type
)
  : Player(cardLimit, playerNumber, type) {}

double StatPlayer::getHandValue(
  std::multiset<Card> const &playedCards,
  std::vector<size_t> const &opponentsHandSizes
) {
  double handValue = 0.0;
  for (int i = 1; i <= 12; i++)
    if (cardsInHand.count(i) > 0)
      handValue += getRoundCloseChance(
        i, cardsInHand.count(i), playedCards, opponentsHandSizes
      );

  return handValue;
}

double StatPlayer::getPlayableChance(
  Card card,
  uint64_t amount,
  std::multiset<Card> const &playedCards,
  std::vector<size_t> const &opponentsHandSizes
) {
  double playChance = 0;
  uint64_t totalCardsLeft = 80 - playedCards.size() - cardsInHand.size();

  for (uint64_t currentCard = card + 1; currentCard < cardLimit;
       currentCard++) {
    for (uint64_t player = 0; player < opponentsHandSizes.size(); player++) {
      uint64_t cardsLeft = currentCard - playedCards.count(currentCard) -
                           cardsInHand.count(currentCard);
      if (cardsLeft > 0) {
        uint64_t jokersLeft = 2 - playedCards.count(cardLimit + 1) -
                              cardsInHand.count(cardLimit + 1);
        cardsLeft += jokersLeft;
      }

      if (cardsLeft < amount)
        continue;

      for (uint64_t j = cardsLeft; j > cardsLeft - amount; j--) {
        playChance +=
          (hypergeometricProbability(
             opponentsHandSizes[player], amount, totalCardsLeft, cardsLeft
           ) /
           (player + 1));
      }
    }
  }
  return playChance;
}

double StatPlayer::getRoundCloseChance(
  Card card,
  uint64_t amount,
  std::multiset<Card> const &playedCards,
  std::vector<size_t> const &opponentsHandSizes
) {
  double roundCloseChance = 0;
  uint64_t totalCardsLeft = 80 - playedCards.size() - cardsInHand.size();

  for (uint64_t currentCard = amount; currentCard < card; currentCard++) {
    for (uint64_t player = 0; player < opponentsHandSizes.size(); player++) {
      uint64_t cardsLeft = currentCard - playedCards.count(currentCard) -
                           cardsInHand.count(currentCard);

      if (cardsLeft < amount)
        continue;

      for (uint64_t j = cardsLeft; j > cardsLeft - amount; j--) {
        roundCloseChance += hypergeometricProbability(
          opponentsHandSizes[player], amount, totalCardsLeft, cardsLeft
        );
      }
    }
  }
  return roundCloseChance;
}

void StatPlayer::getCardValues(
  std::vector<CardValue> &cardValues,
  Cards::PlayedCardInfo const &cardStackTop,
  std::multiset<Card> const &playedCards,
  std::vector<size_t> const &opponentsHandSizes
) {
  // If we have played more than half our cards, we should consider playing the
  // jesters standalone
  Card tillCardLimit =
    cardsInHand.size() > (80 / cardsInHand.size()) / 2 ? cardLimit : jester;

  // Add the card values to the vector
  for (Card card = 1; card <= tillCardLimit; card++) {
    if (cardsInHand.count(card) == 0)
      continue;

    // 0 if we are calculating the value of jesters
    uint64_t amountOfJesters = card == jester ? 0 : cardsInHand.count(jester);
    for (uint64_t jesters = 0; jesters <= amountOfJesters; jesters++) {
      if (!isFirstInRound(cardStackTop) && !canPlayCard(card, cardsInHand.count(card) + jesters, cardStackTop))
        continue;

      cardValues.push_back(
        getCardValue(card, jesters, playedCards, opponentsHandSizes)
      );
    }
  }
}

void StatPlayer::printCardValues(std::vector<CardValue> cardValues) {
  for (auto cardValue: cardValues) {
    std::cout << "Card: " << int(cardValue.card)
              << ",\t\t totalChance: " << cardValue.totalChance
              << ",\t\t Playable chance: " << cardValue.playableChance
              << ",\t\t Round close chance: " << cardValue.roundCloseChance
              << ",\t jesters: " << cardValue.jesters << std::endl;
  }
}

unsigned long long StatPlayer::combination(const int n, const int k) {
  if (n < 0 || k < 0)
    return 0;

  if (k == 0 || k == n)
    return 1;
  else {
    if (StatPlayer::combinations[n][k] == 0)
      combinations[n][k] = combination(n - 1, k - 1) + combination(n - 1, k);
    return StatPlayer::combinations[n][k];
  }
}

double StatPlayer::hypergeometricProbability(
  const int n, const int x, const int N, const int M
) {
  double numerator = combination(M, x) * combination(N - M, n - x);
  double denominator = combination(N, n);
  return numerator / denominator;
}
