#pragma once

#include <AK/Format.h>
#include <AK/StringView.h>
#include <AK/Types.h>

namespace AK {
class SourceLocation {
public:
  [[nodiscard]] constexpr StringView function_name() const { return { m_function, __builtin_strlen(m_function) }; }
  [[nodiscard]] constexpr StringView filename() const { return { m_file, __builtin_strlen(m_file) }; }
  [[nodiscard]] constexpr u32 line_number() const { return m_line; }

  [[nodiscard]] static constexpr SourceLocation current(char const* const file = __builtin_FILE(), u32 line = __builtin_LINE(), char const* const function = __builtin_FUNCTION()) {
    return SourceLocation(file, line, function);
  }

  constexpr SourceLocation() = default;

private:
  constexpr SourceLocation(char const* const file, u32 line, char const* const function)
    : m_function(function)
    , m_file(file)
    , m_line(line) {
      // TODO: implemented
    }
  
  char const* const m_function { nullptr };
  char const* const m_file { nullptr };
  const u32 m_line { 0 };
};

}

template<>
struct AK::Formatter<AK::SourceLocation> : AK::Formatter<FormatString> {
  ErrorOr<void> format(FormatBuilder& builder, AK::SourceLocation location) {
    return AK::Formatter<FormatString>::format(builder, "[\x1b[34m{}\x1b[0m @ {}:{}]"sv, location.function_name(), location.filename(), location.line_number());
  }
};

using AK::SourceLocation;
