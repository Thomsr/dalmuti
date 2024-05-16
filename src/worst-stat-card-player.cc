#include "worst-stat-card-player.h"

WorstStatCardPlayer::WorstStatCardPlayer(
  uint64_t cardLimit, uint64_t playerNumber
)
  : StatPlayer(cardLimit, playerNumber, WORSTSTAT) {}

bool WorstStatCardPlayer::play(
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

void getRoundCloseZeroCardValues(
  std::vector<CardValue> &roundCloseZeroCardValues,
  std::vector<CardValue> const &cardValues
) {
  for (auto cardValue: cardValues) {
    if (cardValue.roundCloseChance == 0)
      roundCloseZeroCardValues.push_back(cardValue);
  }
}

bool hasBadCardWithSameSetSize(
  std::vector<CardValue> const &cardValues, uint64_t const &setSize
) {
  for (int i = 0; i < cardValues.size() / 3; i++) {
    CardValue cardValue = cardValues[i];
    if (cardValue.card == setSize)
      return true;
  }
  return false;
}

CardValue getCardValueWithSameSize(
  std::vector<CardValue> const &cardValues, uint64_t const &setSize
) {
  for (auto cardValue: cardValues) {
    if (cardValue.card == setSize)
      return cardValue;
  }
  return cardValues[0];
}

bool WorstStatCardPlayer::getCardValueToPlay(
  CardValue &cardValue,
  std::vector<CardValue> const &cardValues,
  Cards::PlayedCardInfo const &cardStackTop
) {
  if (isFirstInRound(cardStackTop)) {
    if (hasOnlyOneNonZeroRoundCloseChance(cardValues)) {
      cardValue = getRoundCloseZeroChanceCardValue(cardValues);
      return true;
    }

    uint64_t setSize = 0;
    if (hasSetCloser(setSize, cardValues)) {
      if (setSize != 0) {
        cardValue = getCardValueWithSameSize(cardValues, setSize);
        return true;
      }
    }

    // Don't play the jester standalone if we have other cards to play
    if (cardValues[0].card == jester && cardValues.size() > 1) {
      cardValue = cardValues[1];
      return true;
    }
  }

  cardValue = cardValues[0];
  return true;
}

CardValue WorstStatCardPlayer::getCardValue(
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

bool WorstStatCardPlayer::hasSetCloser(
  uint64_t &setSize, std::vector<CardValue> const &cardValues
) {
  std::vector<CardValue> roundCloseZeroCardValues;
  getRoundCloseZeroCardValues(roundCloseZeroCardValues, cardValues);

  if (roundCloseZeroCardValues.size() == 0)
    return false;

  for (auto cardValue: roundCloseZeroCardValues) {
    if (hasBadCardWithSameSetSize(
          cardValues, cardsInHand.count(cardValue.card)
        )) {
      setSize = cardsInHand.count(cardValue.card);
      return true;
    }
  }
  return false;
}
