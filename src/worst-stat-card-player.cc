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

struct CardValue {
  double playableChance;
  double roundCloseChance;
  Card card;
  uint64_t jesters;
};

bool sort(const CardValue &a, const CardValue &b) {
  if (a.playableChance != b.playableChance)
    return a.playableChance < b.playableChance;

  if (a.roundCloseChance != b.roundCloseChance)
    return a.roundCloseChance > b.roundCloseChance;

  if (a.card != b.card)
    return a.card < b.card;

  return a.jesters < b.jesters;
}

bool WorstStatCardPlayer::play(
  Cards::PlayedCardInfo &cardStackTop,
  std::multiset<Card> const cards,
  playersInfo players
) {
  if (!canPlay(cardStackTop))
    return false;

  std::vector<CardValue> cardValues;

  // removeCardsFromHand(cardLimit + 1, cardsInHand.count(cardLimit + 1));

  // Enter the card values in a vector
  for (Card card = 1; card <= cardLimit + 1; card++) {
    if (cardsInHand.count(card) != 0) {
      uint64_t amountOfJesters =
        card == cardLimit + 1 ? 0 : cardsInHand.count(cardLimit + 1);
      for (uint64_t jesters = 0; jesters <= amountOfJesters; jesters++) {
        // double currentCardValue =
        // getPlayableChance(card, cardsInHand.count(card), cards, players);
        // getRoundCloseChance(card, cardsInHand.count(card), cards, players);

        cardValues.push_back(
          {getPlayableChance(card, cardsInHand.count(card), cards, players),
           getRoundCloseChance(card, cardsInHand.count(card), cards, players),
           card,
           jesters}
        );
      }
    }
  }
  std::sort(cardValues.begin(), cardValues.end(), sort);

  // for (auto cardValue: cardValues) {
  //   std::cout << "Card: " << int(cardValue.card)
  //             << ", Playable chance: " << cardValue.playableChance
  //             << ", Round close chance: " << cardValue.roundCloseChance
  //             << ", Jesters: " << cardValue.jesters << std::endl;
  // }

  if (isFirstInRound(cardStackTop)) {
    Card card = cardValues[0].card;
    uint64_t jesters = cardValues[0].jesters;
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
