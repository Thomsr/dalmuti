#include "worst-stat-card-player.h"

WorstStatCardPlayer::WorstStatCardPlayer(
  uint64_t cardLimit, uint64_t playerNumber
)
  : Player(cardLimit, playerNumber, PlayerType::WORSTSTAT) {
  startingHandValue = getHandValue(std::multiset<Card>(), playersInfo());
}

bool canPlayCard(
  Card card, uint64_t amount, Cards::PlayedCardInfo cardStackTop
) {
  return card < cardStackTop.card &&
         amount == cardStackTop.amount + cardStackTop.jesters;
}

bool isFirstInRound(Cards::PlayedCardInfo cardStackTop) {
  return cardStackTop.card == 0;
}

bool sortDesc(const std::pair<int, int> &a, const std::pair<int, int> &b) {
  return (a.first > b.first);
}

bool WorstStatCardPlayer::play(
  Cards::PlayedCardInfo &cardStackTop,
  std::multiset<Card> const cards,
  playersInfo players
) {
  if (!canPlay(cardStackTop))
    return false;

  removeCardsFromHand(cardLimit + 1, cardsInHand.count(cardLimit + 1));

  // printCardsInHand();
  std::vector<std::pair<double, Card>> cardValues;

  // Allow more cards to be selected when there are less cards in hand
  uint64_t cardDifference = 12;

  // Change the range off cards to full when we are the first player in the
  // round
  Card fromCard = isFirstInRound(cardStackTop) ? 1
                  : cardStackTop.card - cardDifference > 0
                    ? cardStackTop.card - cardDifference
                    : cardStackTop.amount;
  Card tillCard =
    isFirstInRound(cardStackTop) ? cardStackTop.card - 1 : cardStackTop.card;

  // Enter the card values in a vector
  for (Card card = 1; card <= cardLimit; card++) {
    if (cardsInHand.count(card) != 0) {
      // std::cout << "Card: " << int(card) << std::endl;
      // std::cout << "PlayableChance: "
      //           << getPlayableChance(
      //                card, cardsInHand.count(card), cards, players
      //              )
      //           << std::endl;
      // std::cout << "Roundclose: "
      //           << getRoundCloseChance(
      //                card, cardsInHand.count(card), cards, players
      //              )
      //           << std::endl;
      double currentCardValue =
        getPlayableChance(card, cardsInHand.count(card), cards, players);
      // getRoundCloseChance(card, cardsInHand.count(card), cards, players);

      cardValues.push_back({currentCardValue, card});
    }
  }
  std::sort(cardValues.begin(), cardValues.end());

  // for (auto cardValue: cardValues) {
  //   std::cout << "Card: " << int(cardValue.second)
  //             << ", Value: " << cardValue.first << std::endl;
  // }

  if (isFirstInRound(cardStackTop)) {
    Card card = cardValues[0].second;
    uint64_t amount = cardsInHand.count(card);
    cardStackTop = {card, amount, 0};
    removeCardsFromHand(card, amount);
    return true;
  }

  for (auto cardValue: cardValues) {
    if (canPlayCard(
          cardValue.second, cardsInHand.count(cardValue.second), cardStackTop
        )) {
      cardStackTop = {cardValue.second, cardStackTop.amount, 0};
      removeCardsFromHand(cardValue.second, cardStackTop.amount);
      return true;
    }
  }

  return false;
}
