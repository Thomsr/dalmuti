#include "worst-card-player.h"

WorstCardPlayer::WorstCardPlayer(uint64_t cardLimit, uint64_t playerNumber)
    : Player(cardLimit, playerNumber) {}

bool WorstCardPlayer::play(Cards::PlayedCardInfo &cardStackTop) {
  if (cardsInHand.size() == 0)
    return false;

  if (cardStackTop.card == 0) {
    Card card = *(cardsInHand.rbegin());
    uint64_t amount = cardsInHand.count(card);
    cardStackTop = {card, amount};
    removeCardsFromHand(card, amount);
    return true;
  }

  for (Card card = cardStackTop.card - 1; card >= 1; card--) {
    if (cardsInHand.count(card) >= cardStackTop.amount) {
      cardStackTop = {card, cardStackTop.amount};
      removeCardsFromHand(card, cardStackTop.amount);
      return true;
    }
    // Jesters
    if (cardsInHand.count(cardLimit + 1) == 1 &&
        cardsInHand.count(card) + 1 == cardStackTop.amount) {
      cardStackTop = {card, cardStackTop.amount};
      removeCardsFromHand(card, cardStackTop.amount - 1);
      removeCardsFromHand(cardLimit + 1, 1);
      return true;
    }
    if (cardsInHand.count(cardLimit + 1) == 2 &&
        cardsInHand.count(card) + 2 == cardStackTop.amount) {
      cardStackTop = {card, cardStackTop.amount};
      removeCardsFromHand(card, cardStackTop.amount - 2);
      removeCardsFromHand(cardLimit + 1, 2);
      return true;
    }
  }
  return false;
}