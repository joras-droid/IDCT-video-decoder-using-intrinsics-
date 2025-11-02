#include <iostream>
#include <iomanip>
#include <emmintrin.h> // Header for SSE/__m128 intrinsics

const int SIMD_CHUNK_SIZE = 4;

void add_6_floats(float* A, float* B, float* C) {
	
	const int NO_OF_CHUNKS_FULL = 9/SIMD_CHUNK_SIZE;
	// 9/4 = 2 ho. malai ta 2*4=8 samma we are good. very good if we don't have remainder.
	// but if we have reaminder which is tyo point ma chai we have to point through float* pointer.
	// 2*4=8 vanepaxi at 8th index our pointer should point for that last rotation of operation to be performed in m128 by _mm_loadu_ps.
	
	const int REMAINDER_START_INDEX = NO_OF_CHUNKS_FULL * SIMD_CHUNK_SIZE;
	
	for(int i=0;i<NO_OF_CHUNKS_FULL;i++){
		
		int offset = i * SIMD_CHUNK_SIZE;
		
		
		__m128 vec_A = _mm_loadu_ps(A + offset);
		__m128 vec_B = _mm_loadu_ps(B + offset);
		
		__m128 vec_c = _mm_add_ps(vec_A,vec_B);
		
		_mm_storeu_ps(C+offset, vec_c);//c0,c4,... for full sized chunks.
	}
	
	if(REMAINDER_START_INDEX < 9 ){
		__m128 vec_A1 = _mm_load_ps(A+REMAINDER_START_INDEX);
		__m128 vec_B1 = _mm_load_ps(B+REMAINDER_START_INDEX);
		
		__m128 vec_c1= _mm_add_ps(vec_A1, vec_B1);
		
		_mm_storeu_ps(C+REMAINDER_START_INDEX,vec_c1);
	}
    // ----------------------------------------------------
    // STEP 1: Process the first 4 elements (Full Vectorization)
    // ----------------------------------------------------

//    // Load the first 4 floats of A and B into two 128-bit registers
//    __m128 vec_A1 = _mm_loadu_ps(A);
//    __m128 vec_B1 = _mm_loadu_ps(B);
//
//    // Perform the parallel addition on the first 4 elements
//    __m128 vec_C1 = _mm_add_ps(vec_A1, vec_B1);
//
//    // Store the results back into the first 4 slots of C
//    _mm_storeu_ps(C, vec_C1);
//
//    // ----------------------------------------------------
//    // STEP 2: Process the remaining 2 elements (Partial Vectorization)
//    // ----------------------------------------------------
//
//    // Pointers for the remaining 2 elements (starting at index 4)
//    float* A_partial = A + 4;
//    float* B_partial = B + 4;
//    float* C_partial = C + 4;
//
//    // Load the next 4 floats starting from index 4. 
//    // IMPORTANT: The last two slots will load garbage or data you don't care about.
//    __m128 vec_A2 = _mm_loadu_ps(A_partial);
//    __m128 vec_B2 = _mm_loadu_ps(B_partial);
//	
//	float* A_partial1 = A + 8;
//    float* B_partial1 = B + 8;
//    float* C_partial1 = C + 8;
//
//    // Load the next 4 floats starting from index 4. 
//    // IMPORTANT: The last two slots will load garbage or data you don't care about.
//    __m128 vec_A3 = _mm_loadu_ps(A_partial1);
//    __m128 vec_B3 = _mm_loadu_ps(B_partial1);
//    
////A_partial le hold point gare dekhi tespaxi ko 4 ota floating values read garxa.
//
//    // Perform the parallel addition
//    __m128 vec_C2 = _mm_add_ps(vec_A2, vec_B2);
//	__m128 vec_C3 = _mm_add_ps(vec_A3, vec_B3);
//    // Store the results back starting at index 4.
//    // NOTE: We only care about the first 2 results stored here (C[4] and C[5]).
//    _mm_storeu_ps(C_partial, vec_C2); 
//       _mm_storeu_ps(C_partial1, vec_C3); 
//    
    
    // Since we only had 6 elements total, the values stored in C[6] and C[7] are invalid 
    // if C was allocated for only 6 elements. If C was padded to 8, then C[6] and C[7]
    // will contain the sums of the garbage data loaded in A[6]/B[6] and A[7]/B[7].
    // The code relies on the calling function only reading the first 6 elements of C.
}


int main() {
    // 6 single-precision floats (32-bit) for A and B
   
   float A[9] = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f};
    float B[9] = {0.1f, 0.2f, 0.3f, 0.4f, 0.5f, 0.6f, 0.7f, 0.8f, 0.9f};
    
    float C[9]; // Result array for 6 floats
//	float
    // Run the SIMD operation
    add_6_floats(A, B, C);

    // Output the result
    std::cout << "--- SIMD Addition (C = A + B) ---" << std::endl;
    std::cout << std::fixed << std::setprecision(1);

    for (int i = 0; i < 9; ++i) {
        std::cout << "C[" << i << "] = " << A[i] << " + " << B[i] << " = " << C[i] << std::endl;
    }

    return 0;
}
