#include <iostream>
#include <algorithm>
#include <numeric>
#include <map>
#include <vector>
#include <string>
#include <memory>


using i8 = char;
using i32 = std::int32_t;
using u32 = std::uint32_t;
using u64 = std::uint64_t;

struct Node;
using NodePtr = std::unique_ptr<Node>;

struct Node {
  u32 visit;
  u32 count;
  bool is_final;
  NodePtr children[26];

  Node(u32 p_visit = 0, u32 p_count = 0, bool p_is_final = false)
    : visit(p_visit), count(p_count), is_final(p_is_final), children()
  {}

  template <typename Str>
  auto append(Str && word) -> void {
    auto state = 0ul;
    auto node = this;

    for (auto i = 0ul; i < std::size(word); ++i) {
      auto nth = word[i] - 'a';

      if (node->children[nth] == nullptr) {
        node->children[nth] = std::make_unique<Node>();
        node->count++;
      }

      node = node->children[nth].get();
      node->visit++;
    }

    node->is_final = true;
  }
};

auto count(Node* const node) -> u64 {
  auto value = 0ull;

  for (auto i = 0ul; i < std::size(node->children); ++i) {
    if (node->children[i]) {
      value += count(node->children[i].get());
    }
  }

  value += (node->count > 1 || node->is_final)
            * node->visit - node->is_final;

  return value;
}

auto main(int argc, char* const argv[]) -> int {
  auto buffer = std::string{};

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