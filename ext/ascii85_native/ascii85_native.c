// a85.cpp
//
// Ascii85 C++ implementation pulled from https://github.com/larskholte/a85
// Original C++ Code License: Copy and use as you please. No attribution necessary.
//
// Adapted to C language

#include "ascii85_native.h"

int size_for_a85(int binlen, bool append_null) {
    return (binlen * 5 + 3) / 4 + !!append_null;
}

void to_a85(const u8* data, int binlen, char* text, bool append_null) {
    // Go to end of data and text buffers
    data += binlen;
    text += size_for_a85(binlen,append_null);
    // Append null if requested
    if (append_null) {
        *(--text) = 0;
    }
    // If number of bytes is not divisible by 4, act as if null bytes were added to end of buffer
    int rem = binlen & 3;
    if (rem) {
        u32 val = 0;
        for (int i = 4 - rem; i < 4; i++) {
            val |= *(--data) << (8 * i);
        }
        int i;
        for (i = 0; i < 4 - rem; i++) {
            val /= 85;
        }
        for (; i <= 4; i++) {
            *(--text) = val % 85 + 33;
            val /= 85;
        }
        binlen &= ~3;
    }
    while (binlen) {
        // Process chunks of 4 bytes as 32-bit values
        u32 val = *(--data);
        val |= *(--data) << 8;
        val |= *(--data) << 16;
        val |= *(--data) << 24;
        // Convert to base 85
        *(--text) = val % 85 + 33;
        val /= 85;
        *(--text) = val % 85 + 33;
        val /= 85;
        *(--text) = val % 85 + 33;
        val /= 85;
        *(--text) = val % 85 + 33;
        val /= 85;
        *(--text) = val % 85 + 33;
        binlen -= 4;
    }
}

int size_for_bin(int textlen) {
    return (textlen * 4) / 5;
}

void from_a85(const char* text, int textlen, u8* data) {
    while (textlen) {
        if (textlen < 5) {
            // Determine represented value in base 85
            u32 val = 0;
            int factor = 52200625; // 85^4
            int i;
            for (i = 0; i < textlen; i++) {
                val += (*(text++) - 33) * factor;
                factor /= 85;
            }
            for (; i < 5; i++) {
                val += 'u' * factor;
                factor /= 85;
            }
            int shift = 24;
            for (i = 0; i < textlen - 1; i++) {
                *(data++) = val >> shift;
                shift -= 8;
            }
            break;
        }
        // Determine represented value in base 85
        u32 val = (*(text++) - 33) * 52200625; // 85^4
        val += (*(text++) - 33) * 614125; // 85^3
        val += (*(text++) - 33) * 7225; // 85^2
        val += (*(text++) - 33) * 85; // 85^1
        val += (*(text++) - 33); // 85^0
        // Write out in big-endian order
        *(data++) = val >> 24;
        *(data++) = val >> 16;
        *(data++) = val >> 8;
        *(data++) = val;
        textlen -= 5;
    }
}
