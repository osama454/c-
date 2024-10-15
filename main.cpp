#include <iostream>
#include <mpi.h>
#include <vector>
#include <iomanip>
#include <cstdint>

using namespace std;

// S-AES Implementation (Simplified Example - Replace with your actual implementation)
uint8_t sbox(uint8_t input) {
    static const uint8_t sbox_table[16] = {
        0x9, 0x4, 0xA, 0xB, 0xD, 0x1, 0x8, 0x5,
        0x6, 0x2, 0x0, 0x3, 0xC, 0xE, 0xF, 0x7
    };
    return sbox_table[input];
}

uint8_t inv_sbox(uint8_t input) {
    static const uint8_t inv_sbox_table[16] = {
        0xA, 0x5, 0x9, 0xB, 0x1, 0x7, 0x8, 0xF,
        0x6, 0x0, 0x2, 0x3, 0xC, 0x4, 0xD, 0xE
    };
    return inv_sbox_table[input];
}

uint16_t round_function(uint16_t input, uint16_t key) {
    uint8_t nibbles[4];
    nibbles[0] = (input >> 12) & 0xF;
    nibbles[1] = (input >> 8) & 0xF;
    nibbles[2] = (input >> 4) & 0xF;
    nibbles[3] = input & 0xF;

    uint8_t key_nibbles[4];
    key_nibbles[0] = (key >> 12) & 0xF;
    key_nibbles[1] = (key >> 8) & 0xF;
    key_nibbles[2] = (key >> 4) & 0xF;
    key_nibbles[3] = key & 0xF;

    nibbles[1] ^= key_nibbles[0];
    nibbles[3] ^= key_nibbles[2];
    nibbles[0] ^= key_nibbles[1];
    nibbles[2] ^= key_nibbles[3];

    return (nibbles[0] << 12) | (nibbles[1] << 8) | (nibbles[2] << 4) | nibbles[3];
}

uint16_t s_aes_encrypt(uint16_t plaintext, uint16_t key) {
    uint16_t round1_key = (key >> 8) | (key << 8);
    uint16_t round2_key = key;

    uint16_t round1_output = round_function(plaintext, round1_key);
    uint16_t ciphertext = round_function(round1_output, round2_key);
    return ciphertext;
}

uint16_t s_aes_decrypt(uint16_t ciphertext, uint16_t key) {
    uint16_t round1_key = (key >> 8) | (key << 8);
    uint16_t round2_key = key;

    uint16_t round1_output = round_function(ciphertext, round2_key);
    uint16_t plaintext = round_function(round1_output, round1_key);

    return plaintext;
}

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Known plaintext and ciphertext (Example - Replace with your actual values)
    uint16_t plaintext = 0x0123; 
    uint16_t ciphertext = s_aes_encrypt(plaintext, 0x1234); // Example key 0x1234 (used for testing)

    // Key space division
    uint32_t  key_space_size = 65536;
    uint16_t keys_per_process = key_space_size / size;
    uint16_t start_key = rank * keys_per_process;
    uint16_t end_key = (rank == size - 1) ? key_space_size : start_key + keys_per_process;

    double start_time = MPI_Wtime(); // Start timer

    uint16_t found_key = 0;
    bool key_found = false;

    // Each process checks its range of keys
    for (uint16_t key = start_key; key < end_key; ++key) {
        uint16_t decrypted_text = s_aes_decrypt(ciphertext, key); 

        if (decrypted_text == plaintext) {
            found_key = key;
            key_found = true;
            break; 
        }
    }

    // Gather the results from all processes
    int global_key_found = 0;
    MPI_Allreduce(&key_found, &global_key_found, 1, MPI_INT, MPI_LOR, MPI_COMM_WORLD);

    if (global_key_found) {
        // If any process found the key, broadcast the key
        if (key_found) {
            MPI_Bcast(&found_key, 1, MPI_UNSIGNED_SHORT, rank, MPI_COMM_WORLD);
        }
        MPI_Bcast(&found_key, 1, MPI_UNSIGNED_SHORT, MPI_ANY_SOURCE, MPI_COMM_WORLD);

        if (rank == 0) { // Rank 0 prints the result and time
            double end_time = MPI_Wtime();
            cout << "Key found: " << hex << found_key << endl;
            cout << "Time taken: " << end_time - start_time << " seconds" << endl;
        }
    } else if (rank == 0) { // If no key is found
        double end_time = MPI_Wtime();
        cout << "Key not found." << endl;
        cout << "Time taken: " << end_time - start_time << " seconds" << endl;
    }

    MPI_Finalize();
    return 0;
}
