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

  std::vector<CardValue> cardValues;

  // If we have played more than half our cards, we should consider playing
  // jesters standalone
  Card tillCardLimit = cardsInHand.size() > (80 / opponentsHandSizes.size()) / 2
                         ? cardLimit
                         : cardLimit + 1;

  // Enter the card values in a vector
  for (Card card = 1; card <= tillCardLimit; card++) {
    if (cardsInHand.count(card) != 0) {
      uint64_t amountOfjesters =
        card == cardLimit + 1 ? 0 : cardsInHand.count(cardLimit + 1);
      for (uint64_t jesters = 0; jesters <= amountOfjesters; jesters++) {
        double playableChance = getPlayableChance(
          card,
          cardsInHand.count(card) + jesters,
          playedCards,
          opponentsHandSizes
        );
        double roundCloseChance = getRoundCloseChance(
          card,
          cardsInHand.count(card) + jesters,
          playedCards,
          opponentsHandSizes
        );

        cardValues.push_back(
          {playableChance * 2 - card * .5 - aggressiveness,
           playableChance,
           roundCloseChance,
           card,
           jesters}
        );
      }
    }
  }
  std::sort(cardValues.begin(), cardValues.end(), sortCardValue);

  if (isFirstInRound(cardStackTop)) {
    // We dont want to play the cardLimit + 1 as the first card, unless it's the
    // only card left
    auto cardValue =
      cardValues[0].card == cardLimit + 1 && cardValues.size() != 1
        ? cardValues[1]
        : cardValues[0];

    Card card = cardValue.card;
    uint64_t jesters = cardValue.jesters;
    uint64_t amount = cardsInHand.count(card);
    cardStackTop = {card, amount, jesters};
    removeCardsFromHand(card, amount);
    removeCardsFromHand(cardLimit + 1, jesters);
    return true;
  }

  for (auto cardValue: cardValues) {
    if (canPlayCard(
          cardValue.card,
          cardsInHand.count(cardValue.card) + cardValue.jesters,
          cardStackTop
        )) {

      // if (cardValue.playableChance > aggressiveness * 3 &&
      // cardValue.roundCloseChance < 3.0)
      //   continue;

      removeCardsFromHand(
        cardValue.card, cardStackTop.amount - cardValue.jesters
      );
      removeCardsFromHand(cardLimit + 1, cardValue.jesters);
      cardStackTop = {
        cardValue.card,
        cardStackTop.amount - cardValue.jesters,
        cardValue.jesters
      };
      return true;
    }
  }

  return false;
}
