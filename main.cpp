#include <iostream>
#include <iomanip>
#include <emmintrin.h> // Header for SSE/__m128 intrinsics

void add_6_floats(float* A, float* B, float* C) {
    // ----------------------------------------------------
    // STEP 1: Process the first 4 elements (Full Vectorization)
    // ----------------------------------------------------

    // Load the first 4 floats of A and B into two 128-bit registers
    __m128 vec_A1 = _mm_loadu_ps(A);
    __m128 vec_B1 = _mm_loadu_ps(B);

    // Perform the parallel addition on the first 4 elements
    __m128 vec_C1 = _mm_add_ps(vec_A1, vec_B1);

    // Store the results back into the first 4 slots of C
    _mm_storeu_ps(C, vec_C1);

    // ----------------------------------------------------
    // STEP 2: Process the remaining 2 elements (Partial Vectorization)
    // ----------------------------------------------------

    // Pointers for the remaining 2 elements (starting at index 4)
    float* A_partial = A + 4;
    float* B_partial = B + 4;
    float* C_partial = C + 4;

    // Load the next 4 floats starting from index 4. 
    // IMPORTANT: The last two slots will load garbage or data you don't care about.
    __m128 vec_A2 = _mm_loadu_ps(A_partial);
    __m128 vec_B2 = _mm_loadu_ps(B_partial);

    // Perform the parallel addition
    __m128 vec_C2 = _mm_add_ps(vec_A2, vec_B2);

    // Store the results back starting at index 4.
    // NOTE: We only care about the first 2 results stored here (C[4] and C[5]).
    _mm_storeu_ps(C_partial, vec_C2); 
    
    // Since we only had 6 elements total, the values stored in C[6] and C[7] are invalid 
    // if C was allocated for only 6 elements. If C was padded to 8, then C[6] and C[7]
    // will contain the sums of the garbage data loaded in A[6]/B[6] and A[7]/B[7].
    // The code relies on the calling function only reading the first 6 elements of C.
}


int main() {
    // 6 single-precision floats (32-bit) for A and B
    float A[6] = {10.0f, 20.0f, 30.0f, 40.0f, 50.0f, 60.0f};
    float B[6] = { 1.5f,  2.5f,  3.5f,  4.5f,  5.5f,  6.5f};
    float C[6]; // Result array for 6 floats

    // Run the SIMD operation
    add_6_floats(A, B, C);

    // Output the result
    std::cout << "--- SIMD Addition (C = A + B) ---" << std::endl;
    std::cout << std::fixed << std::setprecision(1);

    for (int i = 0; i < 6; ++i) {
        std::cout << "C[" << i << "] = " << A[i] << " + " << B[i] << " = " << C[i] << std::endl;
    }

    return 0;
}
