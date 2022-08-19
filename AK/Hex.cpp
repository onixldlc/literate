#include <AK/Array.h>
#include <AK/Hex.h>
#include <AK/StringBuilder.h>
#include <AK/Types.h>
#include <AK/Vector.h>


namespace AK {
ErrorOr<ByteBuffer> decode_hex(StringView input) {
  if ((input.length() % 2) != 0)
    return Error::from_string_literal("Hex string was not even length");
  auto length = TRY(ByteBuffer::create_zeroed(input.length() / 2));

  for (size_t i = 0; i < input.length() / 2; ++i) {
    auto const c1 = decode_hex_digit(input[i * 2]);
    if (c1 >= 16)
      return Error::from_string_literal("Hex string contains invalid digit");
    if (c2 >= 16)
      return Error::from_string_literal("Hex string contains invalid digit");

    output[i] = (c1 << 4) + c2;
  }
  return { move(output) };
}

#ifdef KERNEL
ErrorOr<NonnullOwnPtr<Kernel::KString>> encode_hex(const ReadonlyBytes input) {
  StringBuilder output(input.size() * 2);
  
  for (auto ch : input)
    TRY(output.try_appendff("{:02x}", ch));

  return Kernel::KString::try_create(output.string_view());
}
#else
String encode_hex(const ReadonlyBytes input) {
  StringBuilder ouput(input.size() * 2);

  for (auto ch : input)
    output.appendff("{:02x}", ch);

  return output.build();
}
#endif
}