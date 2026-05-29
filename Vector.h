#pragma once
/**
 * @file Vector.h
 * @brief Nuosavas dinaminis masyvas Vector<T> — std::vector atitikmuo.
 *
 * Realizuoja >80% std::vector sąsajos: konstruktorius, Rule of Five,
 * elementų prieigą, iteratorius, talpos valdymą ir modifikatorius.
 * Atmintis valdoma rankiniu būdu (::operator new / placement new),
 * kad teisingai veiktų ir su netrivialiais tipais (pvz. Studentas).
 */

#include <cstddef>            // std::size_t, std::ptrdiff_t
#include <stdexcept>          // std::out_of_range, std::length_error
#include <initializer_list>   // std::initializer_list
#include <iterator>           // std::reverse_iterator, std::distance
#include <utility>            // std::move, std::forward, std::swap
#include <type_traits>        // std::enable_if, std::is_integral
#include <limits>             // std::numeric_limits

/**
 * @class Vector
 * @brief Dinaminis masyvas su automatišku talpos augimu (kaip std::vector).
 * @tparam T saugomų elementų tipas.
 */

template <typename T>
class Vector {
public:
    // ===== Tipų aliasai (kaip std::vector) =====
    using value_type = T;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using iterator = value_type*;
    using const_iterator = const value_type*;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

private:
    pointer   data_ = nullptr;  ///< rodyklė į alokuotą atmintį
    size_type size_ = 0;        ///< elementų skaičius
    size_type capacity_ = 0;        ///< alokuota talpa

    /// Alokuoja žalią atmintį n elementų (be konstravimo).
    static pointer allocate(size_type n) {
        if (n == 0) return nullptr;
        return static_cast<pointer>(::operator new(n * sizeof(T)));
    }
    /// Atlaisvina žalią atmintį (be destrukcijos).
    static void deallocate(pointer p) {
        ::operator delete(p);
    }
    /// Iškviečia visų elementų destruktorius.
    void destroy_elements() {
        for (size_type i = 0; i < size_; ++i) data_[i].~T();
    }
    /// Perskirsto į new_cap talpą, perkeliant esamus elementus (move).
    void reallocate(size_type new_cap) {
        pointer new_data = allocate(new_cap);
        for (size_type i = 0; i < size_; ++i) {
            new (new_data + i) T(std::move(data_[i]));  // perkeliame
            data_[i].~T();                              // sunaikiname seną
        }
        deallocate(data_);
        data_ = new_data;
        capacity_ = new_cap;
    }
public:
    // ======================================================
    // Konstruktoriai
    // ======================================================

    /// Numatytasis — tuščias vektorius.
    Vector() noexcept = default;

    /// Sukuria count elementų, inicializuotų numatytąja reikšme.
    explicit Vector(size_type count) {
        data_ = allocate(count);
        capacity_ = count;
        for (size_type i = 0; i < count; ++i) new (data_ + i) T();
        size_ = count;
    }

    /// Sukuria count kopijų reikšmės value.
    Vector(size_type count, const T& value) {
        data_ = allocate(count);
        capacity_ = count;
        for (size_type i = 0; i < count; ++i) new (data_ + i) T(value);
        size_ = count;
    }

    /// Sukuria iš iteratorių intervalo [first, last).
    /// SFINAE: išjungiama, kai InputIt yra sveikasis skaičius (kad nesusimaišytų
    /// su Vector(count, value) konstruktoriumi).
    template <typename InputIt,
        typename = std::enable_if_t<!std::is_integral<InputIt>::value>>
        Vector(InputIt first, InputIt last) {
        size_type n = static_cast<size_type>(std::distance(first, last));
        data_ = allocate(n);
        capacity_ = n;
        size_type i = 0;
        for (; first != last; ++first, ++i) new (data_ + i) T(*first);
        size_ = n;
    }

    /// Sukuria iš initializer_list: Vector<int> v{1,2,3};
    Vector(std::initializer_list<T> init) {
        data_ = allocate(init.size());
        capacity_ = init.size();
        size_type i = 0;
        for (const T& val : init) new (data_ + i++) T(val);
        size_ = init.size();
    }
    // ======================================================
   // Rule of Five
   // ======================================================

   /// Kopijavimo konstruktorius — gili kopija.
    Vector(const Vector& other) {
        data_ = allocate(other.size_);
        capacity_ = other.size_;
        for (size_type i = 0; i < other.size_; ++i)
            new (data_ + i) T(other.data_[i]);
        size_ = other.size_;
    }

    /// Perkėlimo konstruktorius — pagrobia resursus, originalą palieka tuščią.
    Vector(Vector&& other) noexcept
        : data_(other.data_), size_(other.size_), capacity_(other.capacity_) {
        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }

    /// Destruktorius — sunaikina elementus ir atlaisvina atmintį.
    ~Vector() {
        destroy_elements();
        deallocate(data_);
    }

    /// Kopijavimo priskyrimas (copy-and-swap idioma).
    Vector& operator=(const Vector& other) {
        if (this != &other) {
            Vector tmp(other);
            swap(tmp);
        }
        return *this;
    }

    /// Perkėlimo priskyrimas.
    Vector& operator=(Vector&& other) noexcept {
        if (this != &other) {
            destroy_elements();
            deallocate(data_);
            data_ = other.data_;
            size_ = other.size_;
            capacity_ = other.capacity_;
            other.data_ = nullptr;
            other.size_ = 0;
            other.capacity_ = 0;
        }
        return *this;
    }

    /// Priskyrimas iš initializer_list.
    Vector& operator=(std::initializer_list<T> init) {
        Vector tmp(init);
        swap(tmp);
        return *this;
    }
