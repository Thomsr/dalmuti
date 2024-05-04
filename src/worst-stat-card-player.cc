#include "worst-stat-card-player.h"

WorstStatCardPlayer::WorstStatCardPlayer(
  uint64_t cardLimit, uint64_t playerNumber
)
  : Player(cardLimit, playerNumber, PlayerType::WORSTSTAT) {}

bool WorstStatCardPlayer::play(
  Cards::PlayedCardInfo &cardStackTop,
  std::multiset<Card> const cards,
  playersInfo players
) {
  if (!canPlay(cardStackTop))
    return false;

  removeCardsFromHand(cardLimit + 1, cardsInHand.count(cardLimit + 1));
  if (true)
    return true;
}
