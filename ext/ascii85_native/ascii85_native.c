// ascii85_native.c

#include "stdlib.h"
#include "ascii85_native.h"

int a85_encoded_size(int input_length, bool append_null) {
    return (input_length * 5 + 3) / 4 + !!append_null;
}

void a85_encode(const u8* input, int input_length, char* output, bool append_null) {
    // Go to end of input and output buffers
    input += input_length;
    output += a85_encoded_size(input_length,append_null);
    // Append null if requested
    if (append_null) {
        *(--output) = 0;
    }
    // If number of bytes is not divisible by 4, act as if null bytes were added to end of buffer
    int rem = input_length & 3;
    if (rem) {
        u32 val = 0;
        for (int i = 4 - rem; i < 4; i++) {
            val |= *(--input) << (8 * i);
        }
        int i;
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
    return ((input_length * 4) / 5) + 1;
}

void a85_filter_before_decode(const char* input, int input_length, char* output) {
	int input_remaining = input_length;
	while (input_remaining) {
        if ( !(*input == 0 || (*input >= 10 && *input <= 13)) ) {
			*output = *input;
			output++;
		}
		input++;
		input_remaining--;
	}
	*output = '\0';
}

void a85_decode(const char* input, int input_length, u8* output) {
	char* filtered_input;
	filtered_input = (char*)malloc(input_length*sizeof(char));

	a85_filter_before_decode(input, input_length, filtered_input);

	int filtered_length = strlen(filtered_input);

    while (filtered_length) {

		if (*filtered_input == 0) { break; }

        if (*filtered_input >= 10 && *filtered_input <= 13) { filtered_input++; filtered_length--; continue; }

        if (filtered_length < 5) {
            // Determine represented value in base 85
            u32 val = 0;
            int factor = 52200625; // 85^4
            int i;
            for (i = 0; i < filtered_length; i++) {
                val += (*(filtered_input++) - 33) * factor;
                factor /= 85;
            }
            for (; i < 5; i++) {
                val += 'u' * factor;
                factor /= 85;
            }
            int shift = 24;
            for (i = 0; i < filtered_length - 1; i++) {
                *(output++) = val >> shift;
                shift -= 8;
            }
            break;
        }

        // Determine represented value in base 85
        u32 val = (*(filtered_input++) - 33) * 52200625; // 85^4
        val += (*(filtered_input++) - 33) * 614125; // 85^3
        val += (*(filtered_input++) - 33) * 7225; // 85^2
        val += (*(filtered_input++) - 33) * 85; // 85^1
        val += (*(filtered_input++) - 33); // 85^0

        // Write out in big-endian order
        *(output++) = val >> 24;
        *(output++) = val >> 16;
        *(output++) = val >> 8;
        *(output++) = val;
        filtered_length -= 5;
    }

	*(output++) = '\0';
}
