#include <vector>

// Function to check if a number is prime
bool is_prime(int num) {
    if (num < 2) return false;
    for (int i = 2; i * i <= num; ++i) {
        if (num % i == 0) return false;
    }
    return true;
}

// Function to compute the first n primes
extern "C" {
    int* compute_primes(int n) {
        static std::vector<int> primes;
        primes.clear();
        
        int count = 0;
        int num = 2;
        
        while (count < n) {
            if (is_prime(num)) {
                primes.push_back(num);
                count++;
            }
            num++;
        }

        return primes.data();  // Return the pointer to the first element of the array
    }
}
