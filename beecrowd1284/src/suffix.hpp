#include <fstream>
#include "common.hpp"


namespace sxa {

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

auto append_word(Automaton& sam, std::string const& word) -> Automaton {
  auto last = 0ul;
  auto i = 0ul;

  while (i < std::size(word) && sam.q[last].edges.count(word[i])) {
    last = sam.q[last].edges[word[i]];
    ++i;
  }

  /* For each letter */
  for (; i < std::size(word); ++i) {
    auto const produce = word[i];

    auto cur = sam.add_state(State{ .len = sam[last].len + 1, .link = -1, .is_final = true });
    
    auto p = last;
    while (p != -1 && !sam[p].edges.count(produce)) {
      sam[p].edges[produce] = cur;
      p = sam[p].link;
    }

    if (p == -1) {
      sam[cur].link = 0;
    }
    else {
      auto q = sam[p].edges[produce];

      if (sam[p].len + 1 == sam[q].len) {
        sam[cur].link = q;
      }
      else {
        auto clone = sam.add_state(sam[q]);

        sam[clone].len = sam[p].len + 1;
        sam[cur].link = clone;
        sam[q].link = clone;

        while (p != -1 && sam[p].edges[produce] == q) {
          sam[p].edges[produce] = clone;
          p = sam[p].link;
        }
      }
    }

    last = cur;
  }

  return sam;
}

auto generate_suffix_automaton(std::vector<std::string> const& words) -> Automaton {
  auto sam = Automaton{};
  sam.q.reserve(std::size(words));

  /* Add q0 */
  sam.add_state(State{ .len = 0, .link = -1 });

  for (auto const& word: words) {
    append_word(sam, word);
  }

  return sam;
}

auto count_steps(Automaton const& sam, std::string const& word) -> u32 {
  auto count = 0ul;
  auto state = 0ul;

  for (auto i = 0ul; i < std::size(word); ++i) {
    auto available = std::count_if(std::cbegin(sam.q[state].edges), std::cend(sam.q[state].edges),
      [&sam, state](auto const& p) { return sam.q[state].len + 1 == sam.q[p.second].len; }
    );
    count += available > 1 || sam.q[state].is_final || i == 0 ? 1 : 0;
    state = sam.q[state].edges.at(word[i]);
  }

  return count;
}

auto run_words(Automaton const& sam, std::vector<std::string> const& words) -> double {
  auto steps = std::vector<u32>{};
  steps.reserve(std::size(words));

  std::transform(std::cbegin(words), std::cend(words), std::back_inserter(steps), [&sam](std::string const& word){
    return count_steps(sam, word);
  });

  auto sum = std::reduce(std::cbegin(steps), std::cend(steps), 0, std::plus<u32>{});

  return sum / static_cast<double>(std::size(steps));
}

auto save_automaton(Automaton const& sam, std::string const& name) -> void {
  auto builder = std::ofstream(name);

  auto space = [](u32 level) -> std::string {
    return std::string(2 * level, ' ');
  };

  builder << R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?><!--Created with JFLAP 7.1.-->)" << '\n';

  builder << "<structure>\n"
    << space(2) << "<type>fa</type>\n"
    << space(4) << "<automaton>\n";

  for (auto i = 0ul; i < std::size(sam.q); ++i) {
    builder << space(6) << "<state "
      << "id=\"" << i << "\" "
      << "name=\"q" << i << "\">\n";

    builder << space(8) << "<x>" << 0 << "</x>\n";
    builder << space(8) << "<y>" << 0 << "</y>\n";
    
    if (i == 0) {
      builder << space(8) << "<initial/>\n";
    }

    if (sam.q[i].is_final) {
      builder << space(8) << "<final/>\n";
    }

    builder << space(6) << "</state>\n";
  }

  for (auto i = 0ul; i < std::size(sam.q); ++i) {
    for (auto [l, r]: sam.q[i].edges) {
      builder << space(6) << "<transition>\n";
      builder << space(8) << "<from>" << i << "</from>\n";
      builder << space(8) << "<to>"   << r << "</to>\n";
      builder << space(8) << "<read>" << l << "</read>\n";
      builder << space(6) << "</transition>\n";
    }
  }

  builder << space(2) << "</automaton>\n"
    << "</structure>\n";
}

} // sxa