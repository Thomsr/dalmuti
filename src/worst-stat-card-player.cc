#include "worst-stat-card-player.h"

WorstStatCardPlayer::WorstStatCardPlayer(
  uint64_t cardLimit, uint64_t playerNumber
)
  : Player(cardLimit, playerNumber, PlayerType::WORSTSTAT),
    startingHandValue(0) {}

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
  double totalChance;
  double playableChance;
  double roundCloseChance;
  Card card;
  uint64_t jesters;
};

bool sort(const CardValue &a, const CardValue &b) {
  return a.totalChance < b.totalChance;

  // if (a.playableChance != b.playableChance)
  //   return a.playableChance < b.playableChance;

  // if (a.roundCloseChance != b.roundCloseChance)
  //   return a.roundCloseChance > b.roundCloseChance;

  // if (a.card != b.card)
  //   return a.card < b.card;

  // return a.jesters < b.jesters;
}

bool WorstStatCardPlayer::play(
  Cards::PlayedCardInfo &cardStackTop,
  std::multiset<Card> const cards,
  playersInfo players
) {
  if (startingHandValue == 0) {
    startingHandValue = getHandValue(cards, players);
    // std::cout << std::endl;
    // printCardsInHand();
    // std::cout << startingHandValue << std::endl;
  }
  if (!canPlay(cardStackTop))
    return false;

  double aggressiveness =
    // ((players.passes / double(players.nrPlayers)) + (startingHandValue / 20))
    // * .5;
    .5;

  // std::cout << "Aggresiveness: " << aggresiveness << std::endl;

  std::vector<CardValue> cardValues;

  // If we have played more than half our cards, we should consider playing
  // jesters standalone
  Card tillCardLimit = cardsInHand.size() > (80 / players.nrPlayers) / 2
                         ? cardLimit
                         : cardLimit + 1;

  // Enter the card values in a vector
  for (Card card = 1; card <= tillCardLimit; card++) {
    if (cardsInHand.count(card) != 0) {
      uint64_t amountOfJesters =
        card == cardLimit + 1 ? 0 : cardsInHand.count(cardLimit + 1);
      for (uint64_t jesters = 0; jesters <= amountOfJesters; jesters++) {
        // double currentCardValue =
        // getPlayableChance(card, cardsInHand.count(card), cards, players);
        // getRoundCloseChance(card, cardsInHand.count(card), cards, players);

        double playableChance = getPlayableChance(
          card, cardsInHand.count(card) + jesters, cards, players
        );

        double roundCloseChance = getRoundCloseChance(
          card, cardsInHand.count(card) + jesters, cards, players
        );

        cardValues.push_back(
          {playableChance * 2 - card * .5,
           playableChance,
           roundCloseChance,
           card,
           jesters}
        );
      }
    }
  }
  std::sort(cardValues.begin(), cardValues.end(), sort);

  // for (auto cardValue: cardValues) {
  //   if (canPlayCard(
  //         cardValue.card,
  //         cardsInHand.count(cardValue.card) + cardValue.jesters,
  //         cardStackTop
  //       )) {
  //     std::cout << "Card: " << int(cardValue.card)
  //               << ",\t\t totalChance: " << cardValue.totalChance
  //               << ",\t\t Playable chance: " << cardValue.playableChance
  //               << ",\t\t Round close chance: " << cardValue.roundCloseChance
  //               << ",\t Jesters: " << cardValue.jesters << std::endl;
  //   }
  // }
  // std::cout << std::endl;
  // for (auto cardValue: cardValues) {
  //   // if (!canPlayCard(
  //   //       cardValue.card,
  //   //       cardsInHand.count(cardValue.card) + cardValue.jesters,
  //   //       cardStackTop
  //   //     )) {
  //   std::cout << "Card: " << int(cardValue.card)
  //             << ",\t\t totalChance: " << cardValue.totalChance
  //             << ",\t\t Playable chance: " << cardValue.playableChance
  //             << ",\t\t Round close chance: " << cardValue.roundCloseChance
  //             << ",\t Jesters: " << cardValue.jesters << std::endl;
  //   // }
  // }

  if (isFirstInRound(cardStackTop)) {
    // We dont want to play the jester as the first card, unless it's the only
    // card left
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
