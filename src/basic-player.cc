#include "basic-player.h"

BasicPlayer::BasicPlayer(uint64_t cardLimit, uint64_t playerNumber)
    : Player(cardLimit, playerNumber) {}

bool BasicPlayer::play(Cards::PlayedCardInfo &cardStackTop) {
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
  }
  return false;
}