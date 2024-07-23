#pragma once

#include <cstddef>
#include <cstring>
#include <new>


template <typename T>
class Vector {
private:
  T* _arr{};
  size_t _size{};
  size_t _capacity{};
public:
  Vector() = default;
  
  Vector(size_t size, const T& value = T()) : _size(size), _capacity(2 * size) {
    _arr = reinterpret_cast<T*>(new char[_capacity * sizeof(T)]);
    for (size_t idx = 0; idx < _size; idx++) {
      new (_arr + idx) T(value);
    }
  }
  
  Vector(const Vector& iVector): _size(iVector._size), _capacity(iVector._capacity) {
    _arr = reinterpret_cast<T*>(new char[_capacity * sizeof(T)]);
    std::memcpy(_arr, iVector._arr, _capacity * sizeof(T));
  };
  
  Vector(Vector&& iVector) {
    std::swap(_size, iVector._size);
    std::swap(_capacity, iVector._capacity);
    std::swap(_arr, iVector._arr);
  };

  Vector& operator=(const Vector& iVector) {
    _size = iVector._size;
    _capacity = iVector._capacity;
    std::memcpy(_arr, iVector._arr, _capacity * sizeof(T));    
  }
  
  Vector& operator=(Vector&& iVector) {
    std::swap(_size, iVector._size);
    std::swap(_capacity, iVector._capacity);
    std::swap(_arr, iVector._arr);
    iVector._size = 0;
    iVector._arr = nullptr;
    iVector._capacity = 0;
  }

  void reserve(size_t newCapacity) {
    if (newCapacity < _capacity) {
      return;
    }
    T* newArray = reinterpret_cast<T*>(new char[newCapacity * sizeof(T)]);
    size_t lastIdx = 0;
    try
    {
      for (; lastIdx < _size; lastIdx++) {
        new (newArray + lastIdx) T(_arr[lastIdx]);
      }
    }
    catch (...)
    {
      for (size_t i = 0; i < lastIdx; i++) {
        (newArray +i)->~T();
      }
      delete[] reinterpret_cast<char*>(newArray);
      throw;
    }
    
    for (size_t i = 0; i < _size; i++) {
      _arr[i].~T();
    }

    delete[] _arr;
    _arr = newArray;
    _capacity = newCapacity;
  } 
  
  void push_back(const T& iValue) {
    try
    {
      if (_size == _capacity) {
        if (_capacity > 0)
          reserve(_capacity * 2);
        else
          reserve(1);    
      }
      new(_arr + _size) T(iValue);
    }
    catch(...)
    {
      return;
    }
    _size++;
  }

  void push_back(T&& iValue) {
    try
    {
      if (_size == _capacity) {
        if (_capacity > 0)
          reserve(_capacity * 2);
        else
          reserve(1);    
      }
      new(_arr + _size) T(std::move(iValue));
    }
    catch(...)
    {
      return;
    }
    _size++;   
  }

  void pop_back() {
    if (empty()) {
        return;
    }
    (_arr + _size)->~T();
    _size--;
  }

  template <class... Args>
  void emplace_back(Args&&... args) {
    push_back(T(std::forward<Args>(args)...));  
  }

  T& operator[](size_t iPose) {
    return _arr[iPose];
  }

  const T& operator[](size_t iPose) const {
    return _arr[iPose];
  }

  T& at(size_t iPose) {
    if (iPose >= _size) {
        throw std::out_of_range("");
    }
    return _arr[iPose]; 
  }

  const T& at(size_t iPose) const {
    if (iPose >= _size) {
      throw std::out_of_range("");
    }
    return _arr[iPose]; 
  }


  bool empty() const noexcept {
    return _size == 0;
  }

  size_t size() const noexcept {
    return _size;
  }

  T* data() noexcept { 
    return _arr;
  }

  const T* data() const noexcept {
    return _arr;
  }

  void swap(Vector& iVector) noexcept {
    std::swap(_size, iVector._size);
    std::swap(_capacity, iVector._capacity);
    std::swap(_arr, iVector._arr);
  }

};