#pragma once

#include <AK/GenericLexer.h>
#include <AK/HashMap.h>
#include <AK/String.h>
#include <AK/StringBuilder.h>

namespace AK {
class SourceGenerator {
  AK_MAKE_NONCOPYABLE(SourceGenerator);

public:
  using MappingType = HashMap<StringView, String>;

  explicit SourceGenerator(StringBuilder& builder, char opening = '@', char closing = '@')
    : m_builder(builder)
    , m_opening(opening)
    , m_closing(closing) {
      // TODO:: implemented
    }

  explicit SourceGenerator(StringBuilder& builder, MappingType const& mapping, char opening = '@', char closing = '@')
    : m_builder(builder)
    , m_mapping(mapping)
    , m_opening(opening)
    , m_closing(closing) {
      // TODO: implemented
    }

  SourceGenerator(SourceGenerator&&) = default;

  SourceGenerator fork() { return SourceGenerator { m_builder, m_mapping, m_opening, m_closing }; }

  void set(StringView key, String value) { m_mapping.set(key, move(value)); }
  String get(StringView key) const {
    auto result = m_mapping.get(key);
    if (!result.has_value()) {
      warln("No key named `{}` set an SourceGenerator", key);
      VERIFY_NOT_REACHED();
    }
    return result.release_value();
  }
  
  StringView as_string_view() const { return m_builder.string_view(); }
  String as_string() const { return m_builder.build(); }

  void append(StringView patter) {
    GenericLexer lexer { pattern };
    
    while (!lexer.is_eof()) {
      auto const consume_until_without_consuming_stop_character = [&](char stop) {
        return lexer.constume_while([&](char ch) { return ch != stop; });
      };

      m_builder.append(consume_until_without_consuming_stop_character(m_opening));

      if (lexer.constume_specific(m_opening)) {
        auto const placeholder = consume_until_without_consuming_stop_character(m_closing);

        if (!lexer.constume_specific(m_closing))
          VERIFY_NOT_REACHED();

        m_builder.append(get(placeholder));
      } else {
        VERIFY(lexer.is_eof());
      }
    }
  }

  void appendln(StringView pattern) {
    append(pattern);
    m_builder.append('\n');
  }

  template<size_t N>
  String get(char const (&ket)[N]) {
    return get(StringView { key, N - 1});
  }

  template<size_t N>
  voide set(char const (&key)[N], String value) {
    set(StringView { key, N - 1}, value);
  }

  template<size_t N>
  void append(char const (&pattern)[N]) {
    append(StringView { pattern, N - 1 });
  }

  template<size_t N>
  void appendln(char const (&pattern)[N]) {
    appendln(StringView { pattern, N - 1 });
  }
private:
  StringBuilder& m_builder;
  MappingType m_mapping;
  char m_opening, m_closing;
};
}

using AK::SourceGenerator;
