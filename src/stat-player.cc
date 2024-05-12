#include "stat-player.h"

double StatPlayer::combinations[80][80] = {};

StatPlayer::StatPlayer(
  uint64_t cardLimit, uint64_t playerNumber, PlayerType type
)
  : Player(cardLimit, playerNumber, type) {}

double StatPlayer::getHandValue(
  std::multiset<Card> const &playedCards,
  std::vector<Opponent> const &opponentsInfo
) {
  double handValue = 0.0;
  for (int i = 1; i <= 12; i++)
    if (cardsInHand.count(i) > 0)
      handValue += getRoundCloseChance(
        i, cardsInHand.count(i), playedCards, opponentsInfo
      );

  return handValue;
}

double StatPlayer::getPlayableChance(
  Card const &card,
  uint64_t const &amount,
  std::multiset<Card> const &playedCards,
  std::vector<Opponent> const &opponentsInfo
) {
  double playChance = 0;
  uint64_t totalCardsLeft = 80 - playedCards.size() - cardsInHand.size();

  for (uint64_t currentCard = card + 1; currentCard < cardLimit;
       currentCard++) {
    uint64_t timesPlayed = 0;
    for (uint64_t player = 0; player < opponentsInfo.size(); player++) {
      // If the player has already played the card, we assume that the player
      // does not have the card anymore
      if (opponentsInfo[player].playedCards.count(currentCard) > 0) {
        timesPlayed++;
        if (timesPlayed == opponentsInfo.size() - 1)
          playChance += 1;
        continue;
      }

      uint64_t cardsLeft = getCardsLeft(currentCard, playedCards);

      if (cardsLeft < amount)
        continue;

      playChance +=
        hypergeometricProbability(
          opponentsInfo[player].handSize, amount, totalCardsLeft, cardsLeft
        ) /
        (player + 1);
    }
  }
  return playChance;
}

double StatPlayer::getRoundCloseChance(
  Card const &card,
  uint64_t const &amount,
  std::multiset<Card> const &playedCards,
  std::vector<Opponent> const &opponentsInfo
) {
  double roundCloseChance = 0;
  uint64_t totalCardsLeft = 80 - playedCards.size() - cardsInHand.size();

  for (uint64_t player = 0; player < opponentsInfo.size(); player++) {
    for (uint64_t currentCard = 1; currentCard < card; currentCard++) {
      if (opponentsInfo[player].playedCards.count(currentCard) > 0)
        continue;
      uint64_t cardsLeft = getCardsLeft(currentCard, playedCards);

      if (cardsLeft < amount)
        continue;

      roundCloseChance += hypergeometricProbability(
        opponentsInfo[player].handSize, amount, totalCardsLeft, cardsLeft
      );
    }
  }
  return roundCloseChance;
}

void StatPlayer::getCardValues(
  std::vector<CardValue> &cardValues,
  Cards::PlayedCardInfo const &cardStackTop,
  std::multiset<Card> const &playedCards,
  std::vector<Opponent> const &opponentsInfo
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
      if (!isFirstInRound(cardStackTop) &&
          !canPlayCard(card, cardsInHand.count(card) + jesters, cardStackTop))
        continue;

      CardValue newCardValue =
        getCardValue(card, jesters, playedCards, opponentsInfo);
      cardValues.push_back(newCardValue);
    }
  }
}

CardValue StatPlayer::getRoundCloseZeroChanceCardValue(
  std::vector<CardValue> const &cardValues
) {
  for (auto cardValue: cardValues)
    if (cardValue.roundCloseChance == 0)
      return cardValue;
  return cardValues[0];
}

bool StatPlayer::hasOnlyOneNonZeroRoundCloseChance(
  std::vector<CardValue> const &cardValues
) {
  uint64_t nonZeroCount = 0;
  for (auto cardValue: cardValues)
    if (cardValue.roundCloseChance > 0) {
      nonZeroCount++;
      if (nonZeroCount > 1)
        return false;
    }

  return true;
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
  const int &n, const int &x, const int &N, const int &M
) {
  double numerator = combination(M, x) * combination(N - M, n - x);
  double denominator = combination(N, n);
  return numerator / denominator;
}

uint64_t StatPlayer::getCardsLeft(
  Card const &card, std::multiset<Card> const &playedCards
) {
  int cardsLeft = card - playedCards.count(card) - cardsInHand.count(card);
  if (cardsLeft < 0)
    throw std::runtime_error("Negative cards left");

  if (cardsLeft == 0)
    return 0;

  uint64_t jestersLeft =
    2 - playedCards.count(jester) - cardsInHand.count(jester);

  return cardsLeft + jestersLeft;
}
