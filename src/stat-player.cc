#include "stat-player.h"

StatPlayer::StatPlayer(uint64_t cardLimit, uint64_t playerNumber)
  : Player(cardLimit, playerNumber, PlayerType::STAT) {}

bool StatPlayer::play(
  Cards::PlayedCardInfo &cardStackTop,
  std::multiset<Card> const cards,
  playersInfo players
) {
  if (cardsInHand.empty())
    return false;

  removeCardsFromHand(
    cardLimit + 1,
    cardsInHand.count(cardLimit + 1)
  ); // Remove jesters from hand (if any)

  std::pair<Card, double> bestCard = {0, 5.0};

  if (cardStackTop.card == 0) {
    bestCard.second = 0;
    for (Card card = getWorstCard(); card >= 1; card--) {
      if (cardsInHand.count(card) != 0) {
        if (cardValue(card, cardsInHand.count(card), cards, players) > bestCard.second)
          bestCard = {
            card, cardValue(card, cardsInHand.count(card), cards, players)
          };
      }
    }
    if (bestCard.first == 0)
      return false;

    cardStackTop = {bestCard.first, cardsInHand.count(bestCard.first), 0};
    removeCardsFromHand(bestCard.first, cardsInHand.count(bestCard.first));
    return true;
  }

  for (Card card = cardStackTop.card - 1; card >= 1; card--) {
    // std::cout << "Card: " << int(card) << std::endl;
    if (cardsInHand.count(card) != 0) {
      if (cardsInHand.count(card) >= cardStackTop.amount) {
        if (cardValue(card, cardStackTop.amount, cards, players) < bestCard.second)
          bestCard = {
            card, cardValue(card, cardStackTop.amount, cards, players)
          };
      }
    }
  }

  if (bestCard.first == 0)
    return false;

  cardStackTop = {bestCard.first, cardStackTop.amount, 0};
  removeCardsFromHand(bestCard.first, cardStackTop.amount);
  return true;
}
