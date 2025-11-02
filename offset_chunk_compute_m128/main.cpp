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
		__m128 vec_A1 = _mm_loadu_ps(A+REMAINDER_START_INDEX);
		__m128 vec_B1 = _mm_loadu_ps(B+REMAINDER_START_INDEX);
		
		__m128 vec_c1= _mm_add_ps(vec_A1, vec_B1);
		
		_mm_storeu_ps(C+REMAINDER_START_INDEX,vec_c1);
	}
}


int main() {
   float A[9] = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f};
    float B[9] = {0.1f, 0.2f, 0.3f, 0.4f, 0.5f, 0.6f, 0.7f, 0.8f, 0.9f};
    
    float C[9]; 
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
