Gem::Specification.new "ascii85_native", "1.0" do |s|
  s.name        = 'ascii85_native'
  s.version     = '1.0.3'
  s.summary     = "Ascii85 Encoder / Decoder with Native C Extensions"
  s.description = "A faster implementation of the Ascii85 Encode & Decode methods using native C extensions to do the heavy lifting."
  s.authors     = ["Jason Crossfield"]
  s.email       = 'jason.crossfield@gmail.com'
  s.files       = ['lib/ascii85_native.rb', 'ext/ascii85_native/ascii85_native.c', 'ext/ascii85_native/ascii85_native.h']
  s.homepage    = 'https://github.com/AnomalousBit/ascii85_native'
  s.license     = 'MIT'
  s.require_paths = ['lib', 'ext/ascii85_native']
  s.extensions  = %w[ext/ascii85_native/extconf.rb]
  s.add_runtime_dependency 'ffi', '~> 1.15', '>= 1.15.0'
  s.add_development_dependency 'rake', '~> 13.0', '>= 13.0.1'
end
