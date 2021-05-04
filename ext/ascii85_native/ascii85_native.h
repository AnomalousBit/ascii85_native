// a85.h
//
// Ascii85 C++ implementation pulled from https://github.com/larskholte/a85
// Original C++ Code License: Copy and use as you please. No attribution necessary.
//
// Adapted to C language

#include <stdint.h>

typedef _Bool bool;
typedef uint8_t u8;
typedef uint32_t u32;

// Returns size of buffer required for to_a85 function.
int size_for_a85(int binlen, bool append_null);

// Translates the given binary data of the given size to Ascii85.
// Can translate in-place.
// Optionally appends a null character.
void to_a85(const u8* data, int binlen, char* text, bool append_null);

// Returns the size of buffer required for from_a85 function.
int size_for_bin(int textlen);

// Translates the given Ascii85 text to binary data.
// Can translate in-place.
void from_a85(const char* text, int textlen, u8* data);
