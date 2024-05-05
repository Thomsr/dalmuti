#include "table.h"
#include <algorithm>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <vector>

#define PBSTR "||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||"

// Define a custom comparator to sort positions by their numerical order
bool comparePositions(
  const std::pair<int, int> &a, const std::pair<int, int> &b
) {
  return a.first < b.first;
}

// Define a custom comparator to sort player positions by player number
bool comparePlayers(
  const std::pair<int, std::unordered_map<int, int>> &a,
  const std::pair<int, std::unordered_map<int, int>> &b
) {
  return a.first < b.first;
}

void printProgress(double percentage) {
  int val = (int)(percentage * 100);
  int lpad = (int)(percentage * 60);
  int rpad = 60 - lpad;
  printf("\r%3d%% [%.*s%*s]", val, lpad, PBSTR, rpad, "");
  fflush(stdout);
}

void getStatistics(int numberOfRounds, Table &t, int nrPlayers) {
  std::vector<std::vector<Player *>> playerRounds;
  try {
    for (int i = 0; i < numberOfRounds; i++) {
      playerRounds.push_back(t.play());
      printProgress(static_cast<double>(i) / numberOfRounds);
    }
    printProgress(1);
  } catch (std::runtime_error &e) {
    std::cout << e.what() << std::endl;
  }

  // Count player positions in each round
  std::unordered_map<int, std::unordered_map<int, int>> playerPositions;
  for (const auto &round: playerRounds) {
    int position = 1;
    for (const auto &player: round) {
      int playerNumber = player->getPlayerNumber();
      playerPositions[playerNumber][position]++;
      position++;
    }
  }

  // Sort player positions by player number
  std::vector<std::pair<int, std::unordered_map<int, int>>>
    sortedPlayerPositions(playerPositions.begin(), playerPositions.end());
  std::sort(
    sortedPlayerPositions.begin(), sortedPlayerPositions.end(), comparePlayers
  );

  std::cout << std::endl
            << static_cast<double>(numberOfRounds / nrPlayers) /
                 numberOfRounds * 100.0
            << "%" << std::endl;

  // Print the summary
  for (const auto &entry: sortedPlayerPositions) {
    std::cout << "Player " << entry.first << ":" << std::endl;
    std::vector<std::pair<int, int>> sortedPositions(
      entry.second.begin(), entry.second.end()
    );
    std::sort(sortedPositions.begin(), sortedPositions.end(), comparePositions);
    for (const auto &positionEntry: sortedPositions) {
      std::cout << "Position " << positionEntry.first << ": "
                << positionEntry.second << " times ("
                << static_cast<double>(positionEntry.second) / numberOfRounds *
                     100.0
                << "%)" << std::endl;
    }
    std::cout << std::endl;
  }
}

int main(int argc, char *argv[]) {
  if (argc < 4) {
    std::cerr << "Usage: " << argv[0]
              << " <rounds> <players> <debug> [highestcard]" << std::endl;
    return 1;
  }

  int cardLimit = 12;
  bool debug = std::atoi(argv[3]) & 1;

  if (argc == 5)
    cardLimit = std::atoi(argv[4]);

  int numberOfRounds = std::atoi(argv[1]);
  Table table(cardLimit, debug);

  for (int i = 1; i <= std::atoi(argv[2]); i++) {
    int playerType;
    std::cin >> playerType;
    switch (playerType) {
      case 0:
        table.addPlayer(new WorstCardPlayer(cardLimit, i));
        break;
      case 1:
        table.addPlayer(new BestCardPlayer(cardLimit, i));
        break;
      case 2:
        table.addPlayer(new StatPlayer(cardLimit, i));
        break;
      case 3:
        table.addPlayer(new WorstStatCardPlayer(cardLimit, i));
        break;
    }
  }

  std::cout << "Starting game with " << std::atoi(argv[2]) << " players."
            << std::endl;

  table.distributeCards();
  getStatistics(numberOfRounds, table, std::atoi(argv[2]));
  return 0;
}
