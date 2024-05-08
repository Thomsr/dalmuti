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

  CardValue worstCardValue;
  if (!getCardValueToPlay(worstCardValue, cardValues, cardStackTop))
    return false;
  Card card = worstCardValue.card;
  uint64_t amount = cardsInHand.count(card);
  uint64_t jesters = worstCardValue.jesters;
  removeCardsFromHand(card, amount);
  removeCardsFromHand(jester, jesters);
  cardStackTop = {card, amount, jesters};
  return true;
}

bool WorstStatCardPlayer::getCardValueToPlay(
  CardValue &cardValue,
  std::vector<CardValue> const &cardValues,
  Cards::PlayedCardInfo const &cardStackTop
) {
  if (isFirstInRound(cardStackTop) && hasOnlyOneNonZeroRoundCloseChance(cardValues)) {
    cardValue = getRoundCloseZeroChanceCardValue(cardValues);
    return true;
  }

  if (isFirstInRound(cardStackTop) && cardValues[0].card == jester && cardValues.size() > 1) {
    cardValue = cardValues[1];
    return true;
  }

  // if (cardValues[0].card < 5 && cardValues[0].roundCloseChance < 10.0)
  //   return false;
  cardValue = cardValues[0];
  return true;
}

CardValue WorstStatCardPlayer::getCardValue(
  Card const &card,
  uint64_t const &jesters,
  std::multiset<Card> const &playedCards,
  std::vector<size_t> const &opponentsHandSizes
) {
  double playableChance = getPlayableChance(
    card, cardsInHand.count(card) + jesters, playedCards, opponentsHandSizes
  );
  double roundCloseChance = getRoundCloseChance(
    card, cardsInHand.count(card) + jesters, playedCards, opponentsHandSizes
  );

  return (CardValue{
    playableChance * 2 - card * .5,
    playableChance,
    roundCloseChance,
    card,
    jesters
  });
}
