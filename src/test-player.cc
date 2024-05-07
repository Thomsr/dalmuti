#include "test-player.h"

TestPlayer::TestPlayer(uint64_t cardLimit, uint64_t playerNumber)
  : StatPlayer(cardLimit, playerNumber, WORSTCARD), startingHandValue(0) {}

bool TestPlayer::play(
  Cards::PlayedCardInfo &cardStackTop,
  uint64_t const &passes,
  std::multiset<Card> const &playedCards,
  std::vector<size_t> const &opponentsHandSizes
) {
  if (startingHandValue == 0) {
    startingHandValue = getHandValue(playedCards, opponentsHandSizes);
  }
  if (!canPlay(cardStackTop))
    return false;

  double const aggressiveness =
    ((passes / double(opponentsHandSizes.size())) + (startingHandValue / 10));

  (void)aggressiveness;

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

CardValue TestPlayer::getWorstCardValue(
  std::vector<CardValue> const &cardValues,
  Cards::PlayedCardInfo const &cardStackTop
) {
  if (isFirstInRound(cardStackTop) && cardValues[0].card == jester && cardValues.size() > 1)
    return cardValues[1];

  return cardValues[0];
}

CardValue TestPlayer::getCardValue(
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