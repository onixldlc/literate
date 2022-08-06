#pragma once

#define TRY(expression) \
  ({\
    auto _temporary_result = (expression); \
    if (_temporary_result.is_error()) [[unlikely]] \
      return _temporary_result.release_error(); \
    _temporary_result.release_value(); \
  })

#define MUST(expression) \
  ({\
    auto _temporary_result = (expression); \
    VERIFY(!_temporary_result.is_error()); \
    _temporary_result.release_value(); \
  })
