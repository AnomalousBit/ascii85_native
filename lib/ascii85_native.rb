# frozen_string_literal: true

require 'ffi'

module Ascii85Native
  extend FFI::Library

  ffi_lib File.join(File.dirname(__FILE__), 'ascii85_native.so')

  #void a85_encode(const u8* data, int binlen, char* text, bool append_null);
  attach_function :a85_encode, [:buffer_in, :int, :buffer_out, :bool], :void

  #int a85_encoded_size(int binlen, bool append_null);
  attach_function :a85_encoded_size, [:int, :bool], :int

  #int a85_decoded_size(int textlen)
  attach_function :a85_decoded_size, [:int], :int

  #void a85_decode(const char* text, int textlen, u8* data);
  attach_function :a85_decode, [:buffer_in, :int, :buffer_out], :void

  def self.encode(input, include_delimiter=false)
    if input.nil? || input.size == 0 
      return '<~~>' if include_delimiter
      return ''
    end

    if input.class == String
      input_data = []
      input.each_byte { |byte| input_data << byte }
    else
      input_data = input
    end

    FFI::MemoryPointer.new(:uint8, input_data.size) do |in_uint8|
      in_uint8.write_array_of_type(FFI::TYPE_UINT8, :put_uint8, input_data)
      out_size = self.a85_encoded_size(input_data.size, true)

      FFI::MemoryPointer.new(:uint8, out_size) do |output|
        self.a85_encode(in_uint8, input_data.size, output, true)
        if include_delimiter
          return '<~' + (output.read_string() || '') + '~>'
        else
          return output.read_string()
        end
      end
    end
  end


  def self.decode(input, force_delimiter=false)
    return "" if input.nil? || input.size == 0

    # Array slicing in ruby 3.0.1 appears to be constant time O(1): no performance hit based on array size.
    # No reason to implement this code segment in C if this holds true.
    if force_delimiter
      input = input[2..-3]
    else
      stream_start = find_stream_start(input)
      stream_end = find_stream_end(input)
      input = input[stream_start..stream_end] if stream_start != 0 || stream_end != -1
    end

    FFI::MemoryPointer.new(:char, input.size) do |in_char|
      in_char.write_string(input)
      out_size = self.a85_decoded_size(input.size)

      FFI::MemoryPointer.new(:uint8, out_size) do |output|
        self.a85_decode(in_char, input.size, output)
        return output.read_string()
      end
    end
  end

  def self.find_stream_start(input)
    start_slice = 0
    cursor = 0 

    input.size.times do |i|
      if ["\n", "\r", ' '].include?(input[cursor])
        cursor += 1
        next
      elsif input[cursor] == '<' && input[cursor+1] == '~'
        start_slice = cursor + 2
        break
      else
        break # input is not delimited
      end
    end

    return start_slice
  end

  def self.find_stream_end(input)
    end_slice = -1
    cursor = -1

    input.size.times do |i|
      if ["\n", "\r", ' '].include?(input[cursor])
        cursor -= 1
        next
      elsif input[cursor] == '>' && input[cursor-1] == '~'
        end_slice = cursor - 2
        break
      else
        break # input is not delimited
      end
    end

    return end_slice
  end
end
