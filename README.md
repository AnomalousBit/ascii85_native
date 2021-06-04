# ascii85_native
Ruby Ascii85 Encoder / Decoder with Native C Extensions.

ascii85_native intends to be a substitution for the pure ruby implementation [ascii85 gem](https://github.com/DataWraith/ascii85gem), for performance improvements.

## Installation
To install the gem, use `gem install ascii85_native`

## Usage

### Decoding
Following the example from the [ascii85 wikipedia](https://en.wikipedia.org/wiki/Ascii85) page:

```
encoded_str = "<~9jqo^BlbD-BleB1DJ+*+F(f,q/0JhKF<GL>Cj@.4Gp$d7F!,L7@<6@)/0JDEF<G%<+EV:2F!,\nO<DJ+*.@<*K0@<6L(Df-\0Ec5e;DffZ(EZee.Bl.9pF"AGXBPCsi+DGm>@3BB/F*&OCAfu2/AKY\ni(DIb:@FD,*)+C]U=@3BN#EcYf8ATD3s@q?d$AftVqCh[NqF<G:8+EV:.+Cf>-FD5W8ARlolDIa\nl(DId<j@<?3r@:F%a+D58'ATD4$Bl@l3De:,-DJs`8ARoFb/0JMK@qB4^F!,R<AKZ&-DfTqBG%G\n>uD.RTpAKYo'+CT/5+Cei#DII?(E,9)oF*2M7/c~>"

Ascii85Native::decode(str)
```

### Encoding
The same text example from decoding:
```
str = "Man is distinguished, not only by his reason, but by this singular passion from other animals, which is a lust of the mind, that by a perseverance of delight in the continued and indefatigable generation of knowledge, exceeds the short vehemence of any carnal pleasure."
Ascii85Native::encode(str)
```

To wrap the encoded string with the common ascii85 delimiters (<~ and ~>), use the optional `include_delimiter` argument for `encode(data, include_delimiter=false)`.
```
Ascii85Native::encode(str, true)
```

## Testing
To run the test suite (currently based on the coverage provided by ascii85 gem), run `ruby spec/lib/ascii85_native_spec.rb`.

There are several tests failing still, will review when I have time or please feel free to open a pull request.

## Manually Build the Gem
Use `gem build` to manually create the gem, then `gem install ascii85_native-x.y.z.gem` to install the generated gem.

## Why ascii85_native?
This gem is being used for performance improvements in my fork of the [pdf-reader gem](https://github.com/AnomalousBit/pdf-reader)

After generating a flamegraph for a performance analysis of parsing several hundred PDF files, this gem reduced execution time by over 60% when compared to the ascii85 gem used by [pdf-reader gem](https://github.com/yob/pdf-reader).

## License
MIT Licensed, please review the LICENSE file for details and rights.

