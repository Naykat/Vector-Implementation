#include <memory>
#include <utility>
#include <new>
#include <stdexcept>
#include <iterator>
#include <algorithm>
#include <limits>

namespace custom {

template <typename T>
class Vector {
private:
    T* data_pointer;
    size_t size_value;
    size_t capacity_value;

    void reallocate(size_t new_capacity) {
        T* new_data = static_cast<T*>(::operator new(new_capacity * sizeof(T)));
        size_t copy_limit = this->size_value;
        if (new_capacity < copy_limit) {
            copy_limit = new_capacity;
        }
        for (size_t i = 0; i < copy_limit; ++i) {
            ::new (static_cast<void*>(new_data + i)) T(std::move(this->data_pointer[i]));
            (this->data_pointer + i)->~T();
        }
        for (size_t i = copy_limit; i < this->size_value; ++i) {
            (this->data_pointer + i)->~T();
        }
        ::operator delete(this->data_pointer);
        this->data_pointer = new_data;
        this->capacity_value = new_capacity;
        this->size_value = copy_limit;
    }

public:
    Vector() {
        this->data_pointer = nullptr;
        this->size_value = 0;
        this->capacity_value = 0;
    }

    ~Vector() {
        for (size_t i = 0; i < this->size_value; ++i) {
            (this->data_pointer + i)->~T();
        }
        ::operator delete(this->data_pointer);
    }

    void reserve(size_t new_capacity) {
        if (new_capacity > this->capacity_value) {
            this->reallocate(new_capacity);
        }
    }

    void push_back(const T& value) {
        if (this->size_value == this->capacity_value) {
            if (!this->capacity_value) {
                this->reallocate(1);
            } else {
                this->reallocate(this->capacity_value * 2);
            }
        }
        ::new (static_cast<void*>(this->data_pointer + this->size_value)) T(value);
        ++this->size_value;
    }

    void push_back(T&& value) {
        if (this->size_value == this->capacity_value) {
            if (!this->capacity_value) {
                this->reallocate(1);
            } else {
                this->reallocate(this->capacity_value * 2);
            }
        }
        ::new (static_cast<void*>(this->data_pointer + this->size_value)) T(std::move(value));
        ++this->size_value;
    }

    void pop_back() {
        if (this->size_value > 0) {
            (this->data_pointer + (this->size_value - 1))->~T();
            --this->size_value;
        }
    }

    size_t get_size() const {
        return this->size_value;
    }

    size_t get_capacity() const {
        return this->capacity_value;
    }

    bool empty() const {
        return this->size_value == 0;
    }

    T& operator[](size_t index) {
        if (index >= this->size_value) {
            throw std::out_of_range("Index is out of range!");
        }
        return this->data_pointer[index];
    }

    const T& operator[](size_t index) const {
        if (index >= this->size_value) {
            throw std::out_of_range("Index is out of range!");
        }
        return this->data_pointer[index];
    }

    T& at(size_t index) {
        if (index >= this->size_value) {
            throw std::out_of_range("Index is out of range!");
        }
        return this->data_pointer[index];
    }

    const T& at(size_t index) const {
        if (index >= this->size_value) {
            throw std::out_of_range("Index is out of range!");
        }
        return this->data_pointer[index];
    }

    T& front() {
        if (this->size_value == 0) {
            throw std::out_of_range("Vector is empty!");
        }
        return this->data_pointer[0];
    }

    const T& front() const {
        if (this->size_value == 0) {
            throw std::out_of_range("Vector is empty!");
        }
        return this->data_pointer[0];
    }

    T& back() {
        if (this->size_value == 0) {
            throw std::out_of_range("Vector is empty!");
        }
        return this->data_pointer[this->size_value - 1];
    }

    const T& back() const {
        if (this->size_value == 0) {
            throw std::out_of_range("Vector is empty!");
        }
        return this->data_pointer[this->size_value - 1];
    }

    T* data_ptr() {
        return this->data_pointer;
    }

    const T* data_ptr() const {
        return this->data_pointer;
    }

    T* data() {
        return this->data_ptr();
    }

    const T* data() const {
        return this->data_ptr();
    }

    T* begin() {
        return this->data_pointer;
    }

    const T* begin() const {
        return this->data_pointer;
    }

    const T* cbegin() const {
        return this->data_pointer;
    }

    T* end() {
        return this->data_pointer + this->size_value;
    }

    const T* end() const {
        return this->data_pointer + this->size_value;
    }

    const T* cend() const {
        return this->data_pointer + this->size_value;
    }

    using reverse_iterator = std::reverse_iterator<T*>;
    using const_reverse_iterator = std::reverse_iterator<const T*>;

    reverse_iterator rbegin() {
        return reverse_iterator(this->end());
    }
    const_reverse_iterator rbegin() const {
        return const_reverse_iterator(this->end());
    }
    const_reverse_iterator crbegin() const {
        return const_reverse_iterator(this->end());
    }
    reverse_iterator rend() {
        return reverse_iterator(this->begin());
    }
    const_reverse_iterator rend() const {
        return const_reverse_iterator(this->begin());
    }
    const_reverse_iterator crend() const {
        return const_reverse_iterator(this->begin());
    }

    size_t size_() const { return this->size_value; }
    size_t capacity_() const { return this->capacity_value; }

    size_t size() const { return this->size_value; }
    size_t capacity() const { return this->capacity_value; }

    size_t max_size() const {
        return std::numeric_limits<size_t>::max() / sizeof(T);
    }

    void shrink_to_fit() {
        if (this->capacity_value > this->size_value) {
            this->reallocate(this->size_value);
        }
    }

    void clear() {
        for (size_t i = 0; i < this->size_value; ++i) {
            (this->data_pointer + i)->~T();
        }
        this->size_value = 0;
    }

    T* insert(T* position, const T& value) {
        size_t index = static_cast<size_t>(position - this->data_pointer);
        if (index > this->size_value) {
            throw std::out_of_range("Insert position is out of range!");
        }
        if (this->size_value == this->capacity_value) {
            size_t new_capacity;
            if (this->capacity_value) {
                new_capacity = this->capacity_value * 2;
            } else {
                new_capacity = 1;
            }
            this->reserve(new_capacity);
            position = this->data_pointer + index;
        }
        if (index < this->size_value) {
            ::new (static_cast<void*>(this->data_pointer + this->size_value)) T(std::move(this->data_pointer[this->size_value - 1]));
            for (size_t i = this->size_value - 1; i > index; --i) {
                this->data_pointer[i] = std::move(this->data_pointer[i - 1]);
            }
            this->data_pointer[index] = value;
        } else {
            ::new (static_cast<void*>(this->data_pointer + this->size_value)) T(value);
        }
        ++this->size_value;
        return this->data_pointer + index;
    }

    T* insert(T* position, T&& value) {
        size_t index = static_cast<size_t>(position - this->data_pointer);
        if (index > this->size_value) {
            throw std::out_of_range("Insert position is out of range!");
        }
        if (this->size_value == this->capacity_value) {
            size_t new_capacity;
            if (this->capacity_value) {
                new_capacity = this->capacity_value * 2;
            } else {
                new_capacity = 1;
            }
            this->reserve(new_capacity);
            position = this->data_pointer + index;
        }
        if (index < this->size_value) {
            ::new (static_cast<void*>(this->data_pointer + this->size_value)) T(std::move(this->data_pointer[this->size_value - 1]));
            for (size_t i = this->size_value - 1; i > index; --i) {
                this->data_pointer[i] = std::move(this->data_pointer[i - 1]);
            }
            this->data_pointer[index] = std::move(value);
        } else {
            ::new (static_cast<void*>(this->data_pointer + this->size_value)) T(std::move(value));
        }
        ++this->size_value;
        return this->data_pointer + index;
    }

    T* insert(T* position, size_t count, const T& value) {
        size_t index = static_cast<size_t>(position - this->data_pointer);
        if (index > this->size_value) {
            throw std::out_of_range("Insert position is out of range!");
        }
        if (count == 0) {
            return this->data_pointer + index;
        }
        if (this->size_value + count > this->capacity_value) {
            size_t new_capacity;
            if (this->capacity_value) {
                new_capacity = this->capacity_value;
            } else {
                new_capacity = 1;
            }
            while (new_capacity < this->size_value + count) {
                new_capacity *= 2;
            }
            T* new_data = static_cast<T*>(::operator new(new_capacity * sizeof(T)));
            for (size_t i = 0; i < index; ++i) {
                ::new (static_cast<void*>(new_data + i)) T(std::move(this->data_pointer[i]));
                (this->data_pointer + i)->~T();
            }
            for (size_t j = 0; j < count; ++j) {
                ::new (static_cast<void*>(new_data + index + j)) T(value);
            }
            for (size_t i = index; i < this->size_value; ++i) {
                ::new (static_cast<void*>(new_data + count + i)) T(std::move(this->data_pointer[i]));
                (this->data_pointer + i)->~T();
            }
            ::operator delete(this->data_pointer);
            this->data_pointer = new_data;
            this->capacity_value = new_capacity;
            this->size_value += count;
            return this->data_pointer + index;
        } else {
            for (size_t i = 0; i < count; ++i) {
                ::new (static_cast<void*>(this->data_pointer + this->size_value + i)) T(std::move(this->data_pointer[this->size_value - 1 + i]));
            }
            for (size_t i = this->size_value; i > index; --i) {
                this->data_pointer[i + count - 1] = std::move(this->data_pointer[i - 1]);
            }
            for (size_t j = 0; j < count; ++j) {
                this->data_pointer[index + j] = value;
            }
            this->size_value += count;
            return this->data_pointer + index;
        }
    }

    template <typename... Args>
    T* emplace(T* position, Args&&... args) {
        size_t index = static_cast<size_t>(position - this->data_pointer);
        if (index > this->size_value) {
            throw std::out_of_range("Emplace position is out of range!");
        }
        if (this->size_value == this->capacity_value) {
            size_t new_capacity;
            if (this->capacity_value) {
                new_capacity = this->capacity_value * 2;
            } else {
                new_capacity = 1;
            }
            this->reserve(new_capacity);
            position = this->data_pointer + index;
        }
        if (index < this->size_value) {
            ::new (static_cast<void*>(this->data_pointer + this->size_value)) T(std::move(this->data_pointer[this->size_value - 1]));
            for (size_t i = this->size_value - 1; i > index; --i) {
                this->data_pointer[i] = std::move(this->data_pointer[i - 1]);
            }
            this->data_pointer[index].~T();
            ::new (static_cast<void*>(this->data_pointer + index)) T(std::forward<Args>(args)...);
        } else {
            ::new (static_cast<void*>(this->data_pointer + this->size_value)) T(std::forward<Args>(args)...);
        }
        ++this->size_value;
        return this->data_pointer + index;
    }

    T* erase(T* position) {
        size_t index = static_cast<size_t>(position - this->data_pointer);
        if (index >= this->size_value) {
            throw std::out_of_range("Erase position is out of range!");
        }
        for (size_t i = index; i + 1 < this->size_value; ++i) {
            this->data_pointer[i] = std::move(this->data_pointer[i + 1]);
        }
        (this->data_pointer + (this->size_value - 1))->~T();
        --this->size_value;
        return this->data_pointer + index;
    }

    T* erase(T* first, T* last) {
        size_t index_first = static_cast<size_t>(first - this->data_pointer);
        size_t index_last  = static_cast<size_t>(last  - this->data_pointer);
        if (index_first > index_last || index_last > this->size_value) {
            throw std::out_of_range("Erase range is out of range!");
        }
        size_t count = index_last - index_first;
        if (count == 0) return this->data_pointer + index_first;
        for (size_t i = index_first; i + count < this->size_value; ++i) {
            this->data_pointer[i] = std::move(this->data_pointer[i + count]);
        }
        for (size_t i = 0; i < count; ++i) {
            (this->data_pointer + (this->size_value - 1 - i))->~T();
        }
        this->size_value -= count;
        return this->data_pointer + index_first;
    }

    template <typename... Args>
    void emplace_back(Args&&... args) {
        if (this->size_value == this->capacity_value) {
            if (!this->capacity_value) {
                this->reallocate(1);
            } else {
                this->reallocate(this->capacity_value * 2);
            }
        }
        ::new (static_cast<void*>(this->data_pointer + this->size_value)) T(std::forward<Args>(args)...);
        ++this->size_value;
    }

    void resize(size_t count, const T& value) {
        if (count < this->size_value) {
            for (size_t i = count; i < this->size_value; ++i) {
                (this->data_pointer + i)->~T();
            }
            this->size_value = count;
        } else if (count > this->size_value) {
            if (count > this->capacity_value) {
                size_t new_capacity;
                if (this->capacity_value) {
                    new_capacity = this->capacity_value * 2;
                } else {
                    new_capacity = 1;
                }
                while (new_capacity < count) {
                    new_capacity *= 2;
                }
                this->reserve(new_capacity);
            }
            for (size_t i = this->size_value; i < count; ++i) {
                ::new (static_cast<void*>(this->data_pointer + i)) T(value);
            }
            this->size_value = count;
        }
    }

    void resize(size_t count) {
        if (count < this->size_value) {
            for (size_t i = count; i < this->size_value; ++i) {
                (this->data_pointer + i)->~T();
            }
            this->size_value = count;
        } else if (count > this->size_value) {
            if (count > this->capacity_value) {
                size_t new_capacity;
                if (this->capacity_value) {
                    new_capacity = this->capacity_value * 2;
                } else {
                    new_capacity = 1;
                }
                while (new_capacity < count) {
                    new_capacity *= 2;
                }
                this->reserve(new_capacity);
            }
            for (size_t i = this->size_value; i < count; ++i) {
                ::new (static_cast<void*>(this->data_pointer + i)) T();
            }
            this->size_value = count;
        }
    }

    void swap(Vector& other) {
        T* temp_data = this->data_pointer;
        this->data_pointer = other.data_pointer;
        other.data_pointer = temp_data;
        size_t temp_size = this->size_value;
        this->size_value = other.size_value;
        other.size_value = temp_size;
        size_t temp_capacity = this->capacity_value;
        this->capacity_value = other.capacity_value;
        other.capacity_value = temp_capacity;
    }

    bool operator==(const Vector& other) const {
        if (this->size_value != other.size_value) {
            return false;
        }
        for (size_t i = 0; i < this->size_value; ++i) {
            if (this->data_pointer[i] != other.data_pointer[i]) {
                return false;
            }
        }
        return true;
    }

    bool operator!=(const Vector& other) const {
        return !(*this == other);
    }

    bool operator<(const Vector& other) const {
        size_t limit = std::min(this->size_value, other.size_value);
        for (size_t i = 0; i < limit; ++i) {
            if (this->data_pointer[i] < other.data_pointer[i]) return true;
            if (other.data_pointer[i] < this->data_pointer[i]) return false;
        }
        return this->size_value < other.size_value;
    }

    bool operator<=(const Vector& other) const {
        return !(*this > other);
    }

    bool operator>(const Vector& other) const {
        return other < *this;
    }

    bool operator>=(const Vector& other) const {
        return other <= *this;
    }
};

}  // namespace custom
