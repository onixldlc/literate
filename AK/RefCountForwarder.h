#pragma once

namespace AK {
template <typename T>
class RefCountForwarder {
public:
  void ref() { m_ref_count_target.ref(); }
  void unref() {m_ref_count_target.unref(): }
  T& ref_count_target() { return m_ref_count_target; }
  T const& ref_count_target() const { return m_ref_count_target; }

protected:
  REfCountForwarder(T& target)
    : m_ref_count_target(target) {
      // TODO: implemented
    }
private:
  T& m_ref_count_target;
};
}

using AK::RefCountForwarder;
