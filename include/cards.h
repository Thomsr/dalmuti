#ifndef CARDS_H
#define CARDS_H

#include <cstdint>

typedef uint8_t Card;

namespace Cards {
struct PlayedCardInfo {
  Card card;
  uint64_t amount;
  uint8_t jesters;
};

}; // namespace Cards

#endif