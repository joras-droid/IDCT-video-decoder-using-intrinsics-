## C/C++ SIMD Instrinsics. 
let's go with first c/c++ without writing assembly to not comlicate the learning process.
it will let us write super-fast , low level code without the extreme diffculty of pure assembly

### What are simd instrinsics?
instrisics are special c/c++ functions that acts as a direct, ont to one mapping to a specific assembly instruction.

1. allows us to write simd refisters and instructions while being in c/c++ programming environment.
2. c compiler treats intrinsics differently than normal functions, instead of generatiing a function call, it subsititures the instrinsic with the single, corresponding assembly instruction.

#### II. key concepts for SIMD Instrinsics.
To use instrinsics effectively for IDCT, we have to understand three things:
1. SIMD Registers (The Wide ScrathPads)
Instead of standard 32-bit or 64 bit general purpose registers we discussed in assembly SIMD instructions use wider registers (128-bit XMM registers or 256-bit YMM registers for AVX)
- we must load multiple data items (like four 32 bit numbers or 16 8-bit pixel values)into these wide registers simultaneiously.
            128/256 ko hunxa re --- ki 4 ota 32 bit numbers store garnu paryo
                                       16 ota 8 bit pixel values store garnu paryo

2. Data Types (The Magic of __m128 and __m256):
In c/c++ we handle these wide SIMD registers using special data types:

single precision: 32bit as a single packet.

__m128: Represents the 128-bit XMM register, holding 4 single-precision floating-point numbers.
__m256: Represents the 256-bit YMM register, bolding 8 single-precision floating point numbers.

3. Intrinsics Naming Convention:
Most instrinsics follow a predictable naming structure.

prefix: _mm: stands for Intel MMX/SSE(the instruction set)
operation: add 
Data Type : ps (packed single precision floating point which is of 32 bit)
----------------------------------------------------------------------------------------------------------
FUll instrinsic: 
```
_mm_add_ps 
```
(add four single-precision floating point numbers in parallel)
-----------------------------------------------------------------------------------------------------------

### IDCT Relevent Example for Instrinsics:
Since IDCT involves many parallel multiplications and additions, lets look at the basic steps for parallel addition of four numbers:
GOAL: Add Vectors A=[1,2,3,4] and B=[5,6,7,8] to get c=[6,8,10,12]

C++ instrinsics code        Description
__m128 a,b,c;               declare special 128-bit variable

a= _mm_set_ps(4,3,2,1);     Load Data(Parallel Load): load the 4 numbers of vector A into the __m128 variable a;
b= _mm_set_ps(5,6,7,8);     Load Data(Parallel Load): load the 4 numbers of vector B into hte __m128 variable b;

c= _mm_add_ps(a,b);         single line executes one assembly instruction that add all the 4 pairs of numbers at the same time