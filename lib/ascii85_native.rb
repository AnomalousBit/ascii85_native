# frozen_string_literal: true

require 'ffi'

module Ascii85Native
	extend FFI::Library

	ffi_lib File.join(File.dirname(__FILE__), 'ascii85_native.so')

	#void to_a85(const u8* data, int binlen, char* text, bool append_null);
	attach_function :to_a85, [:buffer_in, :int, :buffer_out, :bool], :void

	#int size_for_a85(int binlen, bool append_null);
	attach_function :size_for_a85, [:int, :bool], :int

	#void from_a85(const char* text, int textlen, u8* data);
	attach_function :from_a85, [:buffer_in, :int, :buffer_out], :void

	#int size_for_bin(int textlen)
	attach_function :size_for_bin, [:int], :int


	def self.encode(input)
		return '' if input.nil?

		if input.class == String
			input = []
			input.each_byte { |byte| input << byte }
		end

		FFI::MemoryPointer.new(:uint8, input.size) do |in_uint8|
			in_uint8.write_array_of_type(FFI::TYPE_UINT8, :put_uint8, input)
			out_size = self.size_for_a85(input.size, true)

			FFI::MemoryPointer.new(:uint8, out_size) do |output|
				self.to_a85(in_uint8, input.size, output, true)
				return output.read_string()
			end
		end
	end


	def self.decode(input)
		return "" if input.nil?

		if input[0] == '<' && input[1] == '~' && input[-2] == '~' && input[-1] == '>'
			input = input[2..-3]
		end

		FFI::MemoryPointer.new(:char, input.size) do |in_char|
			in_char.write_string(input)
			out_size = self.size_for_bin(input.size)

			FFI::MemoryPointer.new(:uint8, out_size) do |output|
				self.from_a85(in_char, input.size, output)
				return output.read_string()
			end
		end
	end
end
