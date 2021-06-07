#include <stdio.h>
#include <string.h>
#include "../ascii85_native/ascii85_native.c"

int main() {

    char *ascii85_encoded = "9jqo^BlbD-BleB1DJ+*+F(f,q/0JhKF<GL>Cj@.4Gp$d7F!,L7@<6@)/0JDEF<G%<+EV:2F!,O<DJ+*.@<*K0@<6L(Df-\\0Ec5e;DffZ(EZee.Bl.9pF\"AGXBPCsi+DGm>@3BB/F*&OCAfu2/AKYi(DIb:@FD,*)+C]U=@3BN#EcYf8ATD3s@q?d$AftVqCh[NqF<G:8+EV:.+Cf>-FD5W8ARlolDIal(DId<j@<?3r@:F%a+D58'ATD4$Bl@l3De:,-DJs`8ARoFb/0JMK@qB4^F!,R<AKZ&-DfTqBG%G>uD.RTpAKYo'+CT/5+Cei#DII?(E,9)oF*2M7/c";

	int encoded_length = 350;
    //int encoded_length = strlen(ascii85_encoded);
    printf("strlen(ascii85_encoded): %i\n", encoded_length);

    /*
    for(int i = 0; i <= encoded_length; i++) {
      printf("encoded_text[%i]: %c : %i\n", i, ascii85_encoded[i], ascii85_encoded[i]);
    }
    */

    int decoded_size = a85_decoded_size(encoded_length);
    //printf("a85_decoded_size(decoded_size): %i\n", decoded_size);

    uint8_t decoded_text[decoded_size];

    a85_decode(ascii85_encoded, encoded_length, decoded_text);

    //int decoded_length = strlen(decoded_text);
    //printf("strlen(decoded_text): %i\n", decoded_length);

    /*
    for(int i = 0; i <= decoded_length; i++) {
      printf("decoded_text[%i]: %c : %i\n", i, decoded_text[i], decoded_text[i]);
    }
    */

    printf("encoded_original:\n%s\n", ascii85_encoded);
    printf("decoded:\n%s\n", decoded_text);

    return 0;
}
