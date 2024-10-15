#include <iostream>
#include <mpi.h>
#include <vector>
#include <iomanip>
#include "s_aes.h" // Include your S-AES implementation

using namespace std;

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Known plaintext and ciphertext (Example - Replace with your actual values)
    uint16_t plaintext = 0x0123; 
    uint16_t ciphertext = 0x5678; // The ciphertext corresponding to the plaintext after S-AES encryption

    // Key space division
    uint16_t key_space_size = 65536;
    uint16_t keys_per_process = key_space_size / size;
    uint16_t start_key = rank * keys_per_process;
    uint16_t end_key = (rank == size - 1) ? key_space_size : start_key + keys_per_process;


    double start_time = MPI_Wtime(); // Start timer

    uint16_t found_key = 0;
    bool key_found = false;

    for (uint16_t key = start_key; key < end_key; ++key) {
        uint16_t decrypted_text = s_aes_decrypt(ciphertext, key); // Use your S-AES decryption function

        if (decrypted_text == plaintext) {
            found_key = key;
            key_found = true;
            break; 
        }
    }

    // Broadcast if key is found
    int key_found_int = key_found ? 1 : 0;  // Convert bool to int for MPI
    MPI_Bcast(&key_found_int, 1, MPI_INT, rank, MPI_COMM_WORLD); // First, broadcast the flag

    if (key_found_int) { // If any process found the key
        MPI_Bcast(&found_key, 1, MPI_UNSIGNED_SHORT, rank, MPI_COMM_WORLD); // Broadcast the key itself

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