#pragma once

#include <AK/ByteBuffer.h>
#include <AK/Error.h>
#include <AK/String.h>
#include <AK/StringView.h>

namespace AK {
[[nodiscard]] size_t calculate_base64_decoded_length(StringView);
[[nodiscard]] size_t calculate_base64_encoded_length(ReadonlyBytes);
[[nodiscard]] ErrorOr<ByteBuffer> decoded_base64(StringView);

[[nodiscard]] String encode_base64(ReadonlyBytes)
}
using AK::decode_base64;
using AK::encode_base64;
