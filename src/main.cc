#include "table.h"
#include <algorithm>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <vector>

// Define a custom comparator to sort positions by their numerical order
bool comparePositions(const std::pair<int, int> &a,
                      const std::pair<int, int> &b) {
  return a.first < b.first;
}

// Define a custom comparator to sort player positions by player number
bool comparePlayers(const std::pair<int, std::unordered_map<int, int>> &a,
                    const std::pair<int, std::unordered_map<int, int>> &b) {
  return a.first < b.first;
}

int main(int argc, char *argv[]) {
  if (argc != 4) {
    std::cerr << "Usage: " << argv[0] << " <rounds> <players> <highestcard>"
              << std::endl;
    return 1;
  }

  int numberOfRounds = std::atoi(argv[1]);
  Table t(std::atoi(argv[2]), std::atoi(argv[3]));

  t.distributeCards();
  std::vector<std::vector<Player *>> playerRounds;
  try {
    for (int i = 0; i < numberOfRounds; i++) {
      t.printAllPlayerCards();
      t.printPlayersHandValue();
      playerRounds.push_back(t.play());
    }
  } catch (std::runtime_error &e) {
    std::cout << e.what() << std::endl;
  }

  // Count player positions in each round
  std::unordered_map<int, std::unordered_map<int, int>> playerPositions;
  for (const auto &round : playerRounds) {
    int position = 1;
    for (const auto &player : round) {
      int playerNumber = player->getPlayerNumber();
      playerPositions[playerNumber][position]++;
      position++;
    }
  }

  // Sort player positions by player number
  std::vector<std::pair<int, std::unordered_map<int, int>>>
      sortedPlayerPositions(playerPositions.begin(), playerPositions.end());
  std::sort(sortedPlayerPositions.begin(), sortedPlayerPositions.end(),
            comparePlayers);

  std::cout << static_cast<double>(numberOfRounds / std::atoi(argv[2])) /
                   numberOfRounds * 100.0
            << "%" << std::endl;

  // Print the summary
  for (const auto &entry : sortedPlayerPositions) {
    std::cout << "Player " << entry.first << ":" << std::endl;
    std::vector<std::pair<int, int>> sortedPositions(entry.second.begin(),
                                                     entry.second.end());
    std::sort(sortedPositions.begin(), sortedPositions.end(), comparePositions);
    for (const auto &positionEntry : sortedPositions) {
      std::cout << "Position " << positionEntry.first << ": "
                << positionEntry.second << " times ("
                << static_cast<double>(positionEntry.second) / numberOfRounds *
                       100.0
                << "%)" << std::endl;
    }
    std::cout << std::endl;
  }

  return 0;
}
