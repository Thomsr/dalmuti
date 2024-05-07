#include "worst-stat-card-player.h"

WorstStatCardPlayer::WorstStatCardPlayer(
  uint64_t cardLimit, uint64_t playerNumber
)
  : StatPlayer(cardLimit, playerNumber, WORSTSTAT) {}

bool WorstStatCardPlayer::play(
  Cards::PlayedCardInfo &cardStackTop,
  uint64_t const &passes,
  std::multiset<Card> const &playedCards,
  std::vector<size_t> const &opponentsHandSizes
) {
  (void)passes;

  if (!canPlay(cardStackTop))
    return false;

  std::vector<CardValue> cardValues;
  getCardValues(cardValues, cardStackTop, playedCards, opponentsHandSizes);
  if (cardValues.size() == 0)
    return false;

  std::sort(cardValues.begin(), cardValues.end(), sortCardValue);
  // printCardValues(cardValues);

  CardValue worstCardValue = getWorstCardValue(cardValues, cardStackTop);
  Card card = worstCardValue.card;
  uint64_t amount = cardsInHand.count(card);
  uint64_t jesters = worstCardValue.jesters;
  removeCardsFromHand(card, amount);
  removeCardsFromHand(jester, jesters);
  cardStackTop = {card, amount, jesters};
  return true;
}

CardValue WorstStatCardPlayer::getWorstCardValue(
  std::vector<CardValue> const cardValues,
  Cards::PlayedCardInfo const &cardStackTop
) {
  if (isFirstInRound(cardStackTop) && cardValues[0].card == jester && cardValues.size() > 1)
    return cardValues[1];

  return cardValues[0];
}

void WorstStatCardPlayer::getCardValues(
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

      double playableChance = getPlayableChance(
        card, cardsInHand.count(card) + jesters, playedCards, opponentsHandSizes
      );
      double roundCloseChance = getRoundCloseChance(
        card, cardsInHand.count(card) + jesters, playedCards, opponentsHandSizes
      );

      cardValues.push_back(
        {playableChance * 2 - card * .5,
         playableChance,
         roundCloseChance,
         card,
         jesters}
      );
    }
  }
}
