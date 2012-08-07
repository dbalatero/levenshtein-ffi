require 'ffi'

module Levenshtein
  class << self
    extend FFI::Library

    # Try loading in order.
    library = File.dirname(__FILE__) + "/../ext/levenshtein/levenshtein"
    candidates = ['.bundle', '.so', '.dylib', ''].map { |ext| library + ext }
    ffi_lib(candidates)

    # Safe version of distance, checks that arguments are really strings.
    def distance(str1, str2)
      validate(str1)
      validate(str2)
      ffi_distance(str1, str2)
    end

    # Unsafe version. Results in a segmentation fault if passed nils!
    attach_function :ffi_distance, :levenshtein, [:string, :string], :int

    private
    def validate(arg)
      unless arg.kind_of?(String)
        raise TypeError, "wrong argument type #{arg.class} (expected String)"
      end
    end
  end
end
