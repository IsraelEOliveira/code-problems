#include <algorithm>
#include <numeric>
#include <map>
#include <vector>
#include <string>
#include <fstream>


using i8 = char;
using i32 = std::int32_t;
using u32 = std::uint32_t;
using u64 = std::uint64_t;

struct State {
  u32 len;
  i32 link;
  bool is_final;
  std::map<i8, u32> edges;
};

struct Automaton {
  std::vector<State> q;

  auto operator[](u32 i) -> State& {
    return q[i];
  }

  template <typename S>
  auto add_state(S && s) -> u32 {
    q.push_back(std::forward<S>(s));
    return std::size(q) - 1;
  }
};

auto save_automaton(Automaton const& sam, std::vector<std::string> const& words) -> void {
  auto name = std::string{};

  for (auto const& word : words) {
    name.append(word);
  }

  auto head = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?><!--Created with JFLAP 7.1.-->
    <structure>
      <type>fa</type>
      <automaton>)";

  auto builder = std::ofstream(name.substr(0, std::min(std::size(name), 10ul)) + ".jff");
  builder << head << '\n';

  auto x = 0.0;
  auto y = 0.0;

  for (auto i = 0ul; i < std::size(sam.q); ++i) {
    builder << std::string(8, ' ') << "<state "
      << "id=" << "\"" << i << "\" "
      << "name=" << "\"q" << i << "\"" << ">\n";

    builder << std::string(10, ' ') << "<x>" << x << "</x>" << '\n';
    builder << std::string(10, ' ') << "<y>" << y << "</y>" << '\n';
    
    if (i == 0) {
      builder << std::string(10, ' ') << "<initial/>" << '\n';
    }

    if (sam.q[i].is_final) {
      builder << std::string(10, ' ') << "<final/>" << '\n';
    }

    builder << std::string(8, ' ') << "</state>" << '\n';

    x += 10.0;
    y += 10.0;
  }

  for (auto i = 0ul; i < std::size(sam.q); ++i) {
    for (auto [l, r]: sam.q[i].edges) {
      builder << std::string(8, ' ') << "<transition>" << '\n';
      builder << std::string(10, ' ') << "<from>" << i << "</from>" << '\n';
      builder << std::string(10, ' ') << "<to>" << r << "</to>" << '\n';
      builder << std::string(10, ' ') << "<read>" << l << "</read>" << '\n';
      builder << std::string(8, ' ') << "</transition>" << '\n';
    }
  }

  builder << std::string(4, ' ') << "</automaton>" << '\n'
    << "</structure>" << '\n';
}