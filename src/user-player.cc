#include "user-player.h"

UserPlayer::UserPlayer(uint64_t cardLimit, uint64_t playerNumber)
  : Player(cardLimit, playerNumber, PlayerType::USER) {}

bool UserPlayer::play(
  Cards::PlayedCardInfo &cardStackTop,
  uint64_t const &passes,
  std::multiset<Card> const &playedCards,
  std::vector<Opponent> const &opponentsInfo
) {

  (void)passes;
  (void)playedCards;
  (void)opponentsInfo;

  if (!canPlay(cardStackTop))
    return false;

  std::cout << "Top card(s): " << int(cardStackTop.card) << " "
            << cardStackTop.amount << "x" << std::endl;
  printCardsInHand();

  std::string card;
  uint64_t amount;
  uint64_t jesters;
  do {
    std::cout << "Enter card to play or pass: ";
    std::cin >> card;
    if (card == "pass")
      return false;
    std::cout << "Enter amount of cards: ";
    std::cin >> amount;
    if (cardsInHand.count(cardLimit + 1) > 0) {
      std::cout << "Enter amount of jesters: ";
      std::cin >> jesters;
    } else
      jesters = 0;
  } while (jesters > cardsInHand.count(cardLimit + 1) ||
           cardsInHand.count(uint64_t(std::stoi(card)) + jesters < amount));

  cardStackTop = {Card(std::stoi(card)), amount, jesters};
  removeCardsFromHand(std::stoi(card), amount);
  removeCardsFromHand(cardLimit + 1, jesters);
  return true;
}
