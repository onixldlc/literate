#pragma once

#define AK_MAKE_NONCOPYABLE(c) \
private: \
  c(c const&) = delete; \
  c& operator=(c const&) = delete

#define AK_MAKE_NONMOVEABLE(c) \
private: \
  c(c&&) = delete; \
  c& operator=(c&&) = delete
