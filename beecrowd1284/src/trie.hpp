#include <memory>
#include "automaton.hpp"


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

  if (node->count > 1) {
    value += node->visit;
    if (node->is_final) {
      --value;
    }
  }
  else if (node->is_final) {
    value += node->visit - 1;
  }

  return value;
}