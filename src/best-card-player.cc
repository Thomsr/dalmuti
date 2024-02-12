#include "best-card-player.h"

BestCardPlayer::BestCardPlayer(uint64_t cardLimit, uint64_t playerNumber)
    : Player(cardLimit, playerNumber) {}

bool BestCardPlayer::play(Cards::PlayedCardInfo &cardStackTop) {
  if (cardsInHand.size() == 0)
    return false;

  if (cardStackTop.card == 0) {
    Card card = getBestCard();
    uint64_t amount = cardsInHand.count(card);
    cardStackTop = {card, amount};
    removeCardsFromHand(card, amount);
    return true;
  }

  for (Card card = 1; card <= cardStackTop.card - 1; card++) {
    if (cardsInHand.count(card) >= cardStackTop.amount) {
      cardStackTop = {card, cardStackTop.amount};
      removeCardsFromHand(card, cardStackTop.amount);
      return true;
    }
  }
  return false;
}