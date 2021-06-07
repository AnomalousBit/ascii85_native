// ascii85_native.c

#include "stdlib.h"
#include "ascii85_native.h"
#include "stdio.h"
#include "string.h"

int a85_encoded_size(int input_length, bool append_null) {
    return (input_length * 5 + 3) / 4 + !!append_null;
}

void a85_encode(const u8* input, int input_length, char* output, bool append_null) {

    // Go to end of input and output buffers
    input += input_length;
    output += a85_encoded_size(input_length,append_null);

    // Append null if requested
    if (append_null) { *(--output) = 0; }

    // If number of bytes is not divisible by 4, act as if null bytes were added to end of buffer
    int rem = input_length & 3;
    if (rem) {
        u32 val = 0;

		int i;
        for (i = 4 - rem; i < 4; i++) {
            val |= *(--input) << (8 * i);
        }
        for (i = 0; i < 4 - rem; i++) {
            val /= 85;
        }
        for (; i <= 4; i++) {
            *(--output) = val % 85 + 33;
            val /= 85;
        }

        input_length &= ~3;
    }

    while (input_length) {
        // Process chunks of 4 bytes as 32-bit values
        u32 val = *(--input);
        val |= *(--input) << 8;
        val |= *(--input) << 16;
        val |= *(--input) << 24;

        // Convert to base 85
        *(--output) = val % 85 + 33;
        val /= 85;
        *(--output) = val % 85 + 33;
        val /= 85;
        *(--output) = val % 85 + 33;
        val /= 85;
        *(--output) = val % 85 + 33;
        val /= 85;
        *(--output) = val % 85 + 33;
        input_length -= 4;
    }
}

int a85_decoded_size(int input_length) {
    return ((input_length * 4) / 5);
}

void a85_decode(const char* input, int input_length, u8* output) {
    while (input_length) {

        while ((*input >= 9 && *input <= 13) || *input == 32 || *input == 0) { input++; input_length--;}

        if (input_length < 5) {
            // Determine represented value in base 85
            u32 val = 0;
            int factor = 52200625; // 85^4

            int i;
            for (i = 0; i < input_length; i++) {
                val += (*(input++) - 33) * factor;
                factor /= 85;
            }
            for (; i < 5; i++) {
                val += 'u' * factor;
                factor /= 85;
            }
            int shift = 24;
            for (i = 0; i < input_length - 1; i++) {
                *(output++) = val >> shift;
                shift -= 8;
            }
            break;
        }

        // Determine represented value in base 85 while throwing out invalid ascii85 characters
        while ((*input >= 9 && *input <= 13) || *input == 32 || *input == 0) { input++; input_length--;}
        u32 val = (*(input++) - 33) * 52200625; // 85^4
        while ((*input >= 9 && *input <= 13) || *input == 32 || *input == 0) { input++; input_length--;}
        val += (*(input++) - 33) * 614125; // 85^3
        while ((*input >= 9 && *input <= 13) || *input == 32 || *input == 0) { input++; input_length--;}
        val += (*(input++) - 33) * 7225; // 85^2
        while ((*input >= 9 && *input <= 13) || *input == 32 || *input == 0) { input++; input_length--;}
        val += (*(input++) - 33) * 85; // 85^1
        while ((*input >= 9 && *input <= 13) || *input == 32 || *input == 0) { input++; input_length--;}
        val += (*(input++) - 33); // 85^0

        // Write out in big-endian order
        *(output++) = val >> 24;
        *(output++) = val >> 16;
        *(output++) = val >> 8;
        *(output++) = val;
        input_length -= 5;
    }
	*(output++) = 0;
}
