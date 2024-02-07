#ifndef CARDS_H
#define CARDS_H

#include <cstdint>

typedef uint8_t Card;

namespace Cards {
inline bool isValidCard(Card card) { return card >= 1 && card <= 12; }

struct PlayedCardInfo {
  Card card;
  uint64_t amount;
};

}; // namespace Cards

#endif