#include <iostream>
#include <random>


auto main(int argc, char* const argv[]) -> int {
  if (argc > 3) {
    auto count_words = std::stoul(argv[1]);
    auto mean = std::stod(argv[2]);
    auto std_dev = std::stod(argv[3]);

    auto seed = argc == 5 ? std::stoul(argv[4]) : std::random_device{}();
    auto rng = std::mt19937{seed};

    auto alphabet_dist = std::uniform_int_distribution<char>('a', 'z');
    auto len_dist = std::normal_distribution<double>(mean, std_dev);

    std::cout << count_words << std::endl;

    for (auto i = 0ul; i < count_words; ++i) {
      auto len = 0;
      do {
        len = static_cast<std::uint32_t>(len_dist(rng));
      } while (len <= 0 || len >= 100);

      for (auto j = 0ul; j < len; ++j) {
        std::cout << alphabet_dist(rng);
      }

      std::cout << std::endl;
    }

    return 0;
  }

  return 1;
}