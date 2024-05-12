#include "test-player.h"

TestPlayer::TestPlayer(uint64_t cardLimit, uint64_t playerNumber)
  : StatPlayer(cardLimit, playerNumber, TEST) {}

bool TestPlayer::play(
  Cards::PlayedCardInfo &cardStackTop,
  uint64_t const &passes,
  std::multiset<Card> const &playedCards,
  std::vector<Opponent> const &opponentsInfo
) {
  (void)passes;

  if (!canPlay(cardStackTop))
    return false;

  std::vector<CardValue> cardValues;
  getCardValues(cardValues, cardStackTop, playedCards, opponentsInfo);
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

bool TestPlayer::getCardValueToPlay(
  CardValue &cardValue,
  std::vector<CardValue> const &cardValues,
  Cards::PlayedCardInfo const &cardStackTop
) {
  if (isFirstInRound(cardStackTop) &&
      hasOnlyOneNonZeroRoundCloseChance(cardValues)) {
    cardValue = getRoundCloseZeroChanceCardValue(cardValues);
    return true;
  }

  for (auto cardValueCurrent: cardValues) {
    if (!canPlayCard(
          cardValueCurrent.card,
          cardsInHand.count(cardValueCurrent.card),
          cardStackTop
        ))
      continue;

    if (isFirstInRound(cardStackTop)) {
      cardValue = cardValueCurrent;
      return true;
    }

    // if (cardValues[0].card < 5 && cardValues[0].roundCloseChance < 10.0)
    //   return false;
    cardValue = cardValueCurrent;
    return true;
  }
  return false;
}

CardValue TestPlayer::getCardValue(
  Card const &card,
  uint64_t const &jesters,
  std::multiset<Card> const &playedCards,
  std::vector<Opponent> const &opponentsInfo
) {
  double playableChance = getPlayableChance(
    card, cardsInHand.count(card) + jesters, playedCards, opponentsInfo
  );
  double roundCloseChance = getRoundCloseChance(
    card, cardsInHand.count(card) + jesters, playedCards, opponentsInfo
  );

  return (CardValue{
    playableChance * 2 - card * .5,
    playableChance,
    roundCloseChance,
    card,
    jesters
  });
}
