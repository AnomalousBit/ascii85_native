#include <stdio.h>
#include <string.h>
#include "../ascii85_native/ascii85_native.c"

int main() {

    char *original_text = "Man is distinguished, not only by his reason, but by this singular passion from other animals, which is a lust of the mind, that by a perseverance of delight in the continued and indefatigable generation of knowledge, exceeds the short vehemence of any carnal pleasure.";

    char encoded[a85_encoded_size(strlen(original_text), 1)];

    a85_encode(original_text, strlen(original_text), encoded, 1);

    printf("original_text:\n%s\n", original_text);
    printf("encoded:\n%s\n", encoded);

    return 0;
}
