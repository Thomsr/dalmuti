#include "best-card-player.h"

BestCardPlayer::BestCardPlayer(uint64_t cardLimit, uint64_t playerNumber)
    : Player(cardLimit, playerNumber, PlayerType::BESTCARD) {}

bool BestCardPlayer::play(Cards::PlayedCardInfo &cardStackTop,
                          std::multiset<Card> const cards,
                          playersInfo playersInfo) {
  if (cardsInHand.size() == 0)
    return false;

  if (cardStackTop.card == 0) {
    Card card = getBestCard();
    uint64_t amount = cardsInHand.count(card);
    cardStackTop = {card, amount, 0};
    removeCardsFromHand(card, amount);
    return true;
  }

  for (Card card = 1; card <= cardStackTop.card - 1; card++) {
    if (cardsInHand.count(card) >= cardStackTop.amount + cardStackTop.jesters) {
      cardStackTop = {card, cardStackTop.amount, 0};
      removeCardsFromHand(card, cardStackTop.amount);
      return true;
    }
    if (cardsInHand.count(card) > 0) {
      // Jesters
      if (cardsInHand.count(cardLimit + 1) == 1 &&
          cardsInHand.count(card) + 1 ==
              cardStackTop.amount + cardStackTop.jesters) {
        removeCardsFromHand(card, cardStackTop.amount - 1);
        removeCardsFromHand(cardLimit + 1, 1);
        cardStackTop = {card, cardStackTop.amount - 1, 1};
        return true;
      }
      if (cardsInHand.count(cardLimit + 1) == 2 &&
          cardsInHand.count(card) + 2 ==
              cardStackTop.amount + cardStackTop.jesters) {
        removeCardsFromHand(card, cardStackTop.amount - 2);
        removeCardsFromHand(cardLimit + 1, 2);
        cardStackTop = {card, cardStackTop.amount - 2, 2};
        return true;
      }
    }
  }
  return false;
}