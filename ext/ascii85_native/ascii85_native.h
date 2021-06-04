// ascii85_native.h

#include <stdint.h>

typedef _Bool bool;
typedef uint8_t u8;
typedef uint32_t u32;

// Returns size of char buffer required for output of a85_encode().
int a85_encoded_size(int input_length, bool append_null);

// Translates the given binary data of the given size to Ascii85.
// Can translate in-place.
// Optionally appends a null character.
void a85_encode(const u8* input, int input_length, char* output, bool append_null);

// Returns the size of u8 buffer required for the output of a85_decode().
// In reality, the number of decoded characters could be far less due to
// ignored characters being thrown out (see a85_filter_before_decode())
int a85_decoded_size(int input_length);

// Translates the given Ascii85 input to binary output.
// Can translate in-place.
void a85_decode(const char* input, int input_length, u8* output);

// Removes unused / ignored characters from the ascii85 input
// TODO: identify and remove <~ and ~> delimiters
void a85_filter_before_decode(const char* input, int input_length, char* output);
