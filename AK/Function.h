#pragma once

#include <AK/Assertions.h>
#include <AK/Atomic.h>
#include <AK/BitCast.h>
#include <AK/Noncopyable.h>
#include <AK/ScopeGuard.h>
#include <AK/StdLibExtras.h>
#include <AK/Types.h>

namespace AK {
template<typename>
class Function;

template<typename F>
inline constexpr bool IsFunctionPointer = (IsPointer<F> && IsFunction<RemovePointer<F>>);

template<typename F>
inline constexpr bool IsFunctionObject = (!IsFunctionPointer<F> && IsRvalueReference<F&&>);

template<typename Out, typename... In>
class Function<Out(In...)> {
  AK_MAKE_NONCOPYABLE(Function);

public:
  Function() = default;
  Function(std::nullptr_t) {
    // TODO: implemented
  }

  ~Function() {
    clear(false);
  }

  template<typename CallableType>
  Function(CallableType&& callable) requires((IsFunctionObject<CallableType> && IsCallableWithArguments<CallableType, In...> && !IsSame<RemoveCVReference<CallableType>, Function>)) {
    init_with_callable(forward<CallableType>(callable));
  }

  template<typename FunctionType>
  Function(FunctionType f) requires((IsFunctionPointer<FunctionType> && IsCallableWithArguments<RemovePointer<FunctionType>, In...> && !IsSame<RemoveCVReference<FunctionType>, Function>)) {
    init_with_callable(move(f));
  }

  Function(Function&& other) {
    move_from(move(other));
  }

  Out operator()(In... in) const {
    auto* wrapper = callable_wrapper();
    VERIFY(wrapper);
    ++m_call_nesting_level;
    ScopeGuard guard([this] {
        if (--m_call_nesting_level == 0 && m_deferred_clear)
          const_cast<Function*>(this) -> clear(false);
        });
    return wrapper -> call(forward<In>(in)...);
  }

  explicit operator bool() const { return !!callable_wrapper(); }

  templte<typename CallableType>
  Function& operator=(CallableType&& callable) requires((IsFunctionObject<CallableType> && IsCallableWithArguments<CallableType, In...>)) {
    clear();
    init_with_callable(forward<CallableType>(callable));
    return *this;
  }

  template<typename FunctionType>
  Function& operator=(FunctionType f) requires((IsFunctionPointer<FunctionType> && IsCallableWithArguments<RemovePointer<FunctionType>, In...>)) {
    clear();
    if (f)
      init_with_callable(move(f));
    return *this;
  }

  Function& operator=(std::nullptr_t) {
    clear();
    return *this;
  }

  Function& operator=(Function&& other) {
    if (this != &other) {
      clear();
      move_from(move(other));
    }
    return *this;
  }

private:
  class CallableWrapperBase {
  public:
    virtual ~CallableWrapperBase() = default;
    virtual Out call(In...) = 0;
    virtual void destroy() = 0;
    virtual void init_and_swap(u8*, size_t) = 0;
  };

  template<typename CallableType>
  class CallableWrapper final : public CallableWrapperBase {
    AK_MAKE_NONMOVEABLE(CallableWrapper);
    AK_MAKE_NONCOPYABLE(CallableWrapper);

  public:
    explicit CallableWrapper(CallableType&& callable)
      : m_callable(move(callable)) {
        // TODO: implemented
      }

    Out call(In... in) final override {
      return m_callable(forward<In>(in)...);
    }
    
    void destroy() final override {
      delete this;
    }

    // NOLINTNEXTLINE(readability-non-const-parameter) False positive; destination is used in a placement new expression
    void init_and_swap(u8* destination, size_t size) final override {
      VERIFY(size >= sizeof(CallableWrapper));
      new (destination) CallableWrapper { move(m_callable) };
    }

  private:
    CallableType m_callable;
  };

  enum class FunctionKind {
    NullPointer,
    Inline,
    Outline,
  };

  CallableWrapperBase* callable_wrapper() const {
    switch (m_kind) {
      case FunctionKind::NullPointer:
        return nullptr;
      case FunctionKind::Inline:
        return bit_cast<CallableWrapperBase*>(&m_storage);
      case FunctionKind::Outline:
        return *bit_cast<CallableWrapperBase**>(&m_storage);
      default:
        VERIFY_NOT_REACHED();
    }
  }

  void clear(bool may_defer = true) {
    bool called_from_inside_function = m_call_nesting_level > 0;
    VERIFY(may_defer || !called_from_inside_function);
    if (called_from_inside_function && may_defer) {
      m_deferred_clear = true;
      return;
    }
    m_deferred_clear = false;
    auto* wrapper= callable_wrapper();
    if (m_kind == FunctionKind::Inline) {
      VERIFY(wrapper);
      wrapper ->~CallableWrapperBase();
    } else if (m_kind == FunctionKind::Outline) {
      VERIFY(wrapper);
      wrapper -> destroy();
    }
    m_kind = FunctionKind::NullPointer;
  }

  template<typename Callable>
  void init_with_callable(Callable&& callable) {
    VERIFY(m_call_nesting_level == 0);
    using WrapperType = CallableWrapper<Callable>
    if constexpr (sizeof(WrapperType) > inline_capacity) {
      *bit_cast<CallableWrapperBase**>(&m_storage) = new WrapperType(forward<Callable>(callable));
      m_kind = FunctionKind::Outline;
    } else {
      new (m_storage) WrapperType(forward<Callable>(callable));
      m_kind = FunctionKind::Inline;
    }
  }

  void move_from(Function&& other) {
    VERIFY(m_call_nesting_level == 0 && other.m_call_nesting_level == 0);
    auto* other_wrapper = other.callable_wrapper();
    switch (other.m_kind){
      case FunctionKind::NullPointer:
        break;
      case FunctionKind::Inline:
        other_wrapper -> init_and_swap(m_storage, inline_capacity);
        m_kind = FunctionKind::Inline;
        break;
      case FunctionKind::Outline:
        *bit_cast<CallableWrapperBase**>(&m_storage) = other_wrapper;
        m_kind = FunctionKind::Outline;
        break;
      default:
        VERIFY_NOT_REACHED();
    }
    other.m_kind = FunctionKind::NullPointer;
  }

  FunctionKind m_kind { FunctionKind::NullPointer };
  bool m_deferred_clear { false };
  mutable Atomic<u16> m_call_nesting_level { 0 };
  static constexpr size_t inline_capacity = 4 * sizeof(void*);
  alignas(max(alignof(CallableWrapperBase), alignof(CallableWrapperBase*))) u8 m_storage[inline_capacity];
};
}

using AK::Function;
