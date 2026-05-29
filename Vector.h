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
    // ======================================================
   // assign
   // ======================================================

    void assign(size_type count, const T& value) {
        Vector tmp(count, value);
        swap(tmp);
    }
    template <typename InputIt,
        typename = std::enable_if_t<!std::is_integral<InputIt>::value>>
        void assign(InputIt first, InputIt last) {
        Vector tmp(first, last);
        swap(tmp);
    }
    void assign(std::initializer_list<T> init) {
        Vector tmp(init);
        swap(tmp);
    }

    // ======================================================
    // Elementų prieiga
    // ======================================================

    /// Prieiga su ribų tikrinimu — meta std::out_of_range.
    reference at(size_type pos) {
        if (pos >= size_) throw std::out_of_range("Vector::at: indeksas uz ribu");
        return data_[pos];
    }
    const_reference at(size_type pos) const {
        if (pos >= size_) throw std::out_of_range("Vector::at: indeksas uz ribu");
        return data_[pos];
    }
    /// Prieiga be ribų tikrinimo.
    reference       operator[](size_type pos) { return data_[pos]; }
    const_reference operator[](size_type pos) const { return data_[pos]; }

    reference       front() { return data_[0]; }
    const_reference front() const { return data_[0]; }
    reference       back() { return data_[size_ - 1]; }
    const_reference back()  const { return data_[size_ - 1]; }

    pointer       data()       noexcept { return data_; }
    const_pointer data() const noexcept { return data_; }

    // ======================================================
   // Iteratoriai (paprastos rodyklės = random access iteratoriai)
   // ======================================================

    iterator       begin()        noexcept { return data_; }
    const_iterator begin()  const noexcept { return data_; }
    const_iterator cbegin() const noexcept { return data_; }

    iterator       end()        noexcept { return data_ + size_; }
    const_iterator end()  const noexcept { return data_ + size_; }
    const_iterator cend() const noexcept { return data_ + size_; }

    reverse_iterator       rbegin()        noexcept { return reverse_iterator(end()); }
    const_reverse_iterator rbegin()  const noexcept { return const_reverse_iterator(end()); }
    const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(end()); }

    reverse_iterator       rend()        noexcept { return reverse_iterator(begin()); }
    const_reverse_iterator rend()  const noexcept { return const_reverse_iterator(begin()); }
    const_reverse_iterator crend() const noexcept { return const_reverse_iterator(begin()); }

    // ======================================================
   // Talpa
   // ======================================================

    bool      empty()    const noexcept { return size_ == 0; }
    size_type size()     const noexcept { return size_; }
    size_type capacity() const noexcept { return capacity_; }
    size_type max_size() const noexcept {
        return std::numeric_limits<size_type>::max() / sizeof(T);
    }

    /// Užtikrina bent new_cap talpą (jei mažiau — nieko nedaro).
    void reserve(size_type new_cap) {
        if (new_cap > capacity_) reallocate(new_cap);
    }

    /// Sumažina talpą iki size().
    void shrink_to_fit() {
        if (capacity_ > size_) reallocate(size_);
    }
    // ======================================================
   // Modifikatoriai
   // ======================================================

   /// Pašalina visus elementus (talpa nekinta).
    void clear() noexcept {
        destroy_elements();
        size_ = 0;
    }

    /// Prideda elementą gale (kopija).
    void push_back(const T& value) {
        if (size_ == capacity_)
            reserve(capacity_ == 0 ? 1 : capacity_ * 2);
        new (data_ + size_) T(value);
        ++size_;
    }
    /// Prideda elementą gale (perkėlimas).
    void push_back(T&& value) {
        if (size_ == capacity_)
            reserve(capacity_ == 0 ? 1 : capacity_ * 2);
        new (data_ + size_) T(std::move(value));
        ++size_;
    }

    /// Sukonstruoja elementą gale vietoje (be tarpinės kopijos).
    template <typename... Args>
    reference emplace_back(Args&&... args) {
        if (size_ == capacity_)
            reserve(capacity_ == 0 ? 1 : capacity_ * 2);
        new (data_ + size_) T(std::forward<Args>(args)...);
        ++size_;
        return back();
    }

    /// Pašalina paskutinį elementą.
    void pop_back() {
        --size_;
        data_[size_].~T();
    }

    /// Įterpia value prieš pos. Grąžina iteratorių į įterptą elementą.
    iterator insert(const_iterator pos, const T& value) {
        size_type idx = static_cast<size_type>(pos - data_);
        if (size_ == capacity_)
            reserve(capacity_ == 0 ? 1 : capacity_ * 2);
        // pastumiame elementus nuo galo
        new (data_ + size_) T(std::move(data_[size_ - 1]));
        for (size_type i = size_ - 1; i > idx; --i)
            data_[i] = std::move(data_[i - 1]);
        data_[idx] = value;
        ++size_;
        return data_ + idx;
    }
    iterator insert(const_iterator pos, T&& value) {
        size_type idx = static_cast<size_type>(pos - data_);
        if (size_ == capacity_)
            reserve(capacity_ == 0 ? 1 : capacity_ * 2);
        if (size_ > 0)
            new (data_ + size_) T(std::move(data_[size_ - 1]));
        for (size_type i = size_; i > idx; --i)
            if (i < size_) data_[i] = std::move(data_[i - 1]);
        data_[idx] = std::move(value);
        ++size_;
        return data_ + idx;
    }

    /// Sukonstruoja elementą vietoje prieš pos.
    template <typename... Args>
    iterator emplace(const_iterator pos, Args&&... args) {
        size_type idx = static_cast<size_type>(pos - data_);
        if (size_ == capacity_)
            reserve(capacity_ == 0 ? 1 : capacity_ * 2);
        if (idx < size_) {
            new (data_ + size_) T(std::move(data_[size_ - 1]));
            for (size_type i = size_ - 1; i > idx; --i)
                data_[i] = std::move(data_[i - 1]);
            data_[idx] = T(std::forward<Args>(args)...);
        }
        else {
            new (data_ + idx) T(std::forward<Args>(args)...);
        }
        ++size_;
        return data_ + idx;
    }

    /// Pašalina elementą pozicijoje pos. Grąžina iteratorių į kitą elementą.
    iterator erase(const_iterator pos) {
        size_type idx = static_cast<size_type>(pos - data_);
        for (size_type i = idx; i + 1 < size_; ++i)
            data_[i] = std::move(data_[i + 1]);
        --size_;
        data_[size_].~T();
        return data_ + idx;
    }

    /// Pašalina intervalą [first, last).
    iterator erase(const_iterator first, const_iterator last) {
        size_type idx = static_cast<size_type>(first - data_);
        size_type count = static_cast<size_type>(last - first);
        for (size_type i = idx; i + count < size_; ++i)
            data_[i] = std::move(data_[i + count]);
        for (size_type i = 0; i < count; ++i)
            data_[--size_].~T();
        return data_ + idx;
    }