#include <emscripten.h>
#include <vector>

extern "C" {

EMSCRIPTEN_KEEPALIVE
std::vector<int> get_primes(int nth) {
  std::vector<int> primes;
  primes.push_back(2);
  int number = 3;
  while (primes.size() < nth) {
    bool is_prime = true;
    for (int prime : primes) {
      if (prime * prime > number) {
        break;
      }
      if (number % prime == 0) {
        is_prime = false;
        break;
      }
    }
    if (is_prime) {
      primes.push_back(number);
    }
    number += 2;
  }
  return primes;
}

} // extern "C"