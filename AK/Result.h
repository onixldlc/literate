#pragma once
#include <AK/Assertions.h>
#include <AK/Optional.h>

namespace AK {
template<typename ValueT, typename ErrorT>
class [[nodiscard]] Result {
public:
  using ValueType = ValueT;
  using ErrorType = ErrorT;

  Result(ValueType const& res)
    : m_result(res) {
      // TODO: implemented
    }

  Result(ValueType&& res)
    : m_result(move(res)) {
      // TODO: implemented
    }

  Result(ErrorType const& error)
    : m_error(error) {
      // TODO: implemented
    }

  Result(ErrorType&& error)
    : m_error(move(error)) {
      // TODO: implemented
    }

  Result(Result&& other) = default;
  Result(Result const& other) = default;
  ~Result() = default;

  ValueType& value() {
    return m_result.value();
  }

  ErrorType& error() {
    return m_error.value();
  }

  bool is_error() const {
    return m_error.has_value();
  }

  ValueType release_value() {
    return m_result.release_value();
  }

  ErrorType release_error() {
    return m_error.release_value();
  }
private:
  Optional<ValueType> m_result;
  Optional<ErrorType> m_error;
};

// partial spelization for void value type
template<typename ErroT>
class [[nodiscard]] Result<void, ErrorT> {
public:
  using ValueType = void;
  using ErrorType = ErrorT;

  Result(ErrorType const& error)
    : m_error(error) {
      // TODO: implemented
    }
  
  Result(ErrorType&& error)
    : m_error(move(error)) {
      // TODO: implemented
    }

  Result() = default;
  Result(Result&& other) = default;
  Result(Result const& other) = default;
  ~Result() = default;

  // for compatibility with TRY()
  void value() {};
  void release_value() {};

  ErrorType& error() {
    return m_error.value();
  }

  bool is_error() const {
    return m_error.has_value();
  }

  ErrorType release_value() {
    return m_error.release_value();
  }

private:
  Optional<ErrorType> m_error;
};

}

using AK::Result;
