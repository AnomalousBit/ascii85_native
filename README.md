# ascii85_native
Ruby Ascii85 Encoder / Decoder with Native C Extensions.

ascii85_native intends to be a substitution for the pure ruby implementation [ascii85 gem](https://github.com/DataWraith/ascii85gem), for performance improvements.

## Installation
To install the gem, use `gem install ascii85_native`

## Usage

### Decoding
Following the example from the [ascii85 wikipedia](https://en.wikipedia.org/wiki/Ascii85) page:

```
encoded_str = <<-'ENCODED'
<~9jqo^BlbD-BleB1DJ+*+F(f,q/0JhKF<GL>Cj@.4Gp$d7F!,L7@<6@)/0JDEF<G%<+EV:2F!,
O<DJ+*.@<*K0@<6L(Df-\0Ec5e;DffZ(EZee.Bl.9pF"AGXBPCsi+DGm>@3BB/F*&OCAfu2/AKY
i(DIb:@FD,*)+C]U=@3BN#EcYf8ATD3s@q?d$AftVqCh[NqF<G:8+EV:.+Cf>-FD5W8ARlolDIa
l(DId<j@<?3r@:F%a+D58'ATD4$Bl@l3De:,-DJs`8ARoFb/0JMK@qB4^F!,R<AKZ&-DfTqBG%G
>uD.RTpAKYo'+CT/5+Cei#DII?(E,9)oF*2M7/c~>
ENCODED

Ascii85Native::decode(encoded_str)
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
You will need to install the `rake-compiler` gem (`gem install rake-compiler`) for manual extension compilation.

Run `rake compile` to generate the required ascii85_native.so shared library before running tests.

To run the minitest suite (currently based on the coverage provided by ascii85 gem), run `cd spec/lib ; ruby ascii85_native_spec.rb`.

There are a few tests currently failing. Most failed tests are around Ascii85 delimiters with extraneous content to be ignored after the delimiters.

I recommend only sending the actual Ascii85 encoded stream to this library, not whole files with the expectation that the library will find and decode only the Ascii85 portion of the file (it won't).

I can review a failing test if there is a demand, please feel free to open an issue or pull request.

There are also C programs that can be used for inspection of the C code in `ext/ascii85_native_testing/`

## Manually Build the Gem
Use `gem build` to manually create the gem, then `gem install ascii85_native-x.y.z.gem` to install the generated gem.

## Why ascii85_native?
This gem is being used for performance improvements in my fork of the [pdf-reader gem](https://github.com/AnomalousBit/pdf-reader)

After generating a flamegraph for a performance analysis of parsing several hundred PDF files, this gem improved execution times by nearly 60x when compared to the ascii85 gem used by [pdf-reader gem](https://github.com/yob/pdf-reader). YMMV.

## License
MIT Licensed, please review the LICENSE file for details and rights.

