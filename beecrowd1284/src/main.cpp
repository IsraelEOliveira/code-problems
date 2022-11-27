/**
 * THEORY OF COMPUTATION
 * OLIVEIRA, I.E.; ZANCANARO, J.C.; 2022
 * 
 * https://youtu.be/73Mm4dR2W1M
 * https://www.inf.ufrgs.br/~svravelo/assets/slides/Desafios____08.pdf
 * https://static.googleusercontent.com/media/research.google.com/en//pubs/archive/35395.pdf
 * 
 */
#include <string_view>
#include <iostream>
#include "trie.hpp"


auto main(int argc, char* const argv[]) -> int {
  auto buffer = std::string{};
  auto save = argc > 1 && argv[1] == std::string_view("--save");

  while (std::getline(std::cin, buffer)) {
    auto amount = std::stoul(buffer);
    auto automata = std::make_unique<Node>();

    for (auto i = 0ul; i < amount; ++i) {
      std::getline(std::cin, buffer);
      automata->append(buffer);
    }

    auto visits = static_cast<double>(count(automata.get()));

    std::printf("%.2lf\n", (amount + visits) / static_cast<double>(amount));
  }
  
  return 0;
}

/**
 * [israel.eo@red beecrowd1284]$ ./run.sh
 * 2.00
 * 1.67
 * 2.71
 */