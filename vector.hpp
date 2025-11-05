#include <memory>
#include <utility>
#include <new>
#include <stdexcept>

namespace custom {

template <typename T>
class Vector {
private:
    T*     data;
    size_t size;
    size_t capacity;

    void resize(size_t new_capacity) {
        T* new_data = static_cast<T*>(::operator new(new_capacity * sizeof(T)));
        size_t limit = this->size;
        if (new_capacity < limit) {
            limit = new_capacity;
        }
        for (size_t i = 0; i < limit; ++i) {
            ::new (static_cast<void*>(new_data + i)) T(std::move(this->data[i]));
            (this->data + i)->~T();
        }
        for (size_t i = limit; i < this->size; ++i) {
            (this->data + i)->~T();
        }
        ::operator delete(this->data);
        this->data = new_data;
        this->capacity = new_capacity;
        this->size = limit;
    }

public:
    Vector() {
        this->data = nullptr;
        this->size = 0;
        this->capacity = 0;
    }

    ~Vector() {
        for (size_t i = 0; i < this->size; ++i) {
            (this->data + i)->~T();
        }
        ::operator delete(this->data);
    }

    void reserve(size_t new_capacity) {
        if (new_capacity > this->capacity) {
            this->resize(new_capacity);
        }
    }

    void push_back(const T& value) {
        if (this->size == this->capacity) {
            if (!this->capacity) {
                this->resize(1);
            } else {
                this->resize(this->capacity * 2);
            }
        }
        ::new (static_cast<void*>(this->data + this->size)) T(value);
        ++this->size;
    }

    void push_back(T&& value) {
        if (this->size == this->capacity) {
            if (!this->capacity) {
                this->resize(1);
            } else {
                this->resize(this->capacity * 2);
            }
        }
        ::new (static_cast<void*>(this->data + this->size)) T(std::move(value));
        ++this->size;
    }

    void pop_back() {
        if (this->size > 0) {
            (this->data + (this->size - 1))->~T();
            --this->size;
        }
    }

    size_t get_size() const {
        return this->size;
    }

    size_t get_capacity() const {
        return this->capacity;
    }

    bool empty() const {
        return this->size == 0;
    }

    T& operator[](size_t index) {
        if (index >= this->size) {
            throw std::out_of_range("Index is out of range!");
        }
        return this->data[index];
    }
};

}  // namespace custom
