#ifndef AIRMAP_OPTIONAL_H_
#define AIRMAP_OPTIONAL_H_

#include <iostream>
#include <type_traits>

namespace airmap {
template <typename T>
class Optional {
 public:
  Optional() : has_value{false} {
  }

  Optional(const Optional& other) : has_value{other.has_value} {
    if (has_value)
      new (&storage.value) T(other.storage.value);
  }

  Optional(Optional<T>&& other) : has_value{other.has_value} {
    if (has_value)
      new (&storage.value) T(other.storage.value);
  }

  Optional(const T& value) : has_value{true} {
    new (&storage.value) T(value);
  }

  Optional(T&& value) : has_value{true} {
    new (&storage.value) T(value);
  }

  ~Optional() {
    reset();
  }

  Optional& operator=(const Optional& rhs) {
    if (rhs.has_value)
      set(rhs.storage.value);
    else
      reset();

    return *this;
  }

  Optional& operator=(const T& rhs) {
    set(rhs);
    return *this;
  }

  Optional& operator=(Optional&& rhs) {
    if (rhs.has_value)
      set(rhs.storage.value);
    else
      reset();

    return *this;
  }

  bool operator==(const Optional& rhs) const {
    if (has_value != rhs.has_value)
      return false;

    return has_value && (storage.value == rhs.storage.value);
  }

  explicit operator bool() const {
    return has_value;
  }

  const T& get() const {
    return storage.value;
  }

  T& get() {
    return storage.value;
  }

  void set(const T& value) {
    reset();

    has_value = true;
    new (&storage.value) T(value);
  }

  void reset() {
    if (has_value)
      (&storage.value)->~T();
    has_value = false;
  }

 private:
  bool has_value;
  union Storage {
    Storage() {
    }
    ~Storage() {
    }
    T value;
  } storage;
};

template <typename T>
inline std::ostream& operator<<(std::ostream& out, const Optional<T>& value) {
  if (value)
    out << value.get();
  else
    out << "not set";
  return out;
}

template <typename T>
using Required = Optional<T>;

}  // namespace airmap

#endif  // AIRMAP_OPTIONAL_H_
