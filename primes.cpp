#include <emscripten.h>
#include <vector>

extern "C" {

EMSCRIPTEN_KEEPALIVE
std::vector<int> get_primes(int n) {
  std::vector<int> primes;
  primes.push_back(2);
  int num = 3;
  while (primes.size() < n) {
    bool is_prime = true;
    for (int i = 0; i < primes.size() && primes[i] * primes[i] <= num; ++i) {
      if (num % primes[i] == 0) {
        is_prime = false;
        break;
      }
    }
    if (is_prime) {
      primes.push_back(num);
    }
    num += 2;
  }
  return primes;
}

} // extern "C"