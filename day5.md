### REFERENCE

// Online C++ compiler to run C++ program online
//References: second name of the variable. 
```cpp

#include <iostream>

int main() {
    int number = 42;
    //&reference is memory address
    //pointed by reference is value
    
    int& reference_to_num  = number;
    
    // to print value: use without & 
    // to print memoryaddress: use &
    std::cout << &reference_to_num;

    return 0;
}

### Pointers and deferences

```cpp
// Online C++ compiler to run C++ program online
//pointers- let us move through these arrays quickly.
//1. when we process a 16x16 macroblock ,we will use a pointer to jump directly to the start of the block;s data in memory.


#include <iostream>

int main() {
    int number = 42;
    int* ptr_to_number  = &number;
    
    // 2. defrencing the value pointed by the pointer
    // Write C++ code here
    std::cout << *ptr_to_number;

    return 0;
}

### cstdint potatble variable declararion based on the architecture of the hardware the decoder is running
``` cpp
// Online C++ compiler to run C++ program online
//Introduction to cstdint. 

#include <iostream>
#include <cstdint>

int main() {
    //for 2 bytes of 8 bits
    uint16_t = 
    //for 1 byte of 8bits
    uint8_t = 
    //for 4 bytes each of 8bits
    uint32_t = 
    
    

    return 0;
}

##### for stroing the final pixel vlaue uint8_t is required
##### for calculating the IDCT pixel value uint16_t is required

#### std::vector array
// Online C++ compiler to run C++ program online
```cpp

//Introduction to std:vector. 

#include <iostream>
#include <cstdint>

int main() {
    //std::vector is a dynamic array that can change size,often used for parts of video stream whose length can vary.
    //safer memory handling capability.
    //cons: can introduce a small performance overhaed compared to raw uint8_t and uint16_t arrays., but we are going to go with std::vector as it is dyanmic  for now. 
   
   std::vector<MotionVector> motion_vectors;
    

    return 0;
}



### struct data model

```cpp
//struct for data modeling. 
//16x16 macroblock (unit  of one frame)
#include <iostream>


struct Macrobloack{
    //this is output:
    //decoded luminance data (4 seperaet 8x8 block)
    uint8_t Y_pixels[16][16];
    
    // decoded chrominanace data (one 8x8)
    uint8_t Cb_pixels[8][8];
    uint8_t Cr_pixels[8][8];
    
    //motion vector used for prediction
    uint16_t motion_vector_x;
    uint16_t motion_vector_y;
    
    
}

### Class for managed structure of code:

```cpp

//class based declaration of private properties and variables
#include <iostream>
class BitReader{
    private:
        std::vector<uint8_t> compressed_data;
        size_t current_bit_position;
    
    public:
    //some function which operatos on data
        uint32_t read_bits(int num_bits){
            
            //complex logic to read 1 to 32 bits from the stream.
            // kind of entropy decoding.
        }
}

### std::ios::binary / std::vector<uint8_t>

1. video data has to be converted to -> binary bites
     tesko lagi we use: std::ios::binary


2. encoded data should be stored on the -> memory buffer (like uint8_t or something like that buffer )
        so that decoder logic can work witout waiting for hardware 


### std::ios::in - telling that open the file in input mode

### std::string& temporarry storing the value of the file into that reference variable

## file vitra get pointer hunxa which is also called read pointer (seekg() and tellg() )

### std::ios::end -> telling the read pointer(which file has and yeslai control garna ko lagi seekg() vanne method hunxa and tellg() returns the current position of that get pointer) to go back to the end of the file to measure the file size.

### file.seekg(0,std::ios::beg); tell that pointer to go to the beginning .

### std::vector<uint8_t> buffer(file_size); file_size jati xa teti nai vako buffer namae vako uint8_t memort buffer create gareko.

```cpp

        file.read(reinterpret_cast<char*>(buffer.data()),file_size);

### casting is done from uint8_t* to char* because read function of ifstream expects that.
### file_size specifies how many butes to read from the current file position.
                                                        ## meaning read function exactly file_size file ma jati ko xa, teti nai copy garera buffer ma rakhxa startign from buffer.data() [so buffer.data() is the position of the starting part].

### reads the entirefile content ( since the pointer was previously set to the start with seekg(0,std::ios::beg)) in vector memory;

``cpp
#include <iostream>
#include <fstream>


std::vector<uint8_t> read_file_to_buffer(const std::string& filename){
    
    //open file in input and binary mode.
    std::ifstream file(filename, std::ios::in | std::ios::binary);
    
    //check if the file is opened sucessfully
    if(!file.is_open()){
        
        //handle error logic
        return {};
        
    }
    
    // 2.seek to the end for finding the file size
    
    file.seekg(0,std::ios::end);
    size_t file_size = file.tellg();
    
    //3, seek back to the beginning
    
    file.seekg(0,std::ios::beg);
    
    //4.create a buffer (std::vector to hold the data)
    
    std::vector<uint8_t> buffer(file_size);
    
    //5. Read the entire file content into the buffer
    
    file.read(reinterpret_cast<char*>(buffer.data()),file_size)
    
    return buffer;
    
}