#pragma once
#ifndef _DYNAMICARRAY_
#define _DYNAMICARRAY_
#include <initializer_list>
#include <stdexcept>
#include <memory>


template <class _Category, class _Ty, class _Diff = ptrdiff_t, class _Pointer = _Ty*, class _Reference = _Ty&>
struct iterator {
	using iterator_category = _Category;
	using value_type = _Ty;
	using difference_type = _Diff;
	using pointer = _Pointer;
	using reference = _Reference;
};

template<class T>
class DA_const_iterator : public iterator<std::random_access_iterator_tag, T>
{
protected:
	typename DA_const_iterator::pointer p;
public:
	DA_const_iterator(typename DA_const_iterator::pointer p) : p(p) {}
	DA_const_iterator() : p(nullptr) {}
	DA_const_iterator(const DA_const_iterator& other) : p(other.p) {}
	DA_const_iterator(DA_const_iterator&& other) noexcept : p(other.p) { other.p = nullptr; }

	DA_const_iterator& operator=(typename DA_const_iterator::pointer p) { this->p = p; return *this; }
	DA_const_iterator& operator=(const DA_const_iterator& other) { this->p = other.p; return *this; }
	DA_const_iterator& operator=(DA_const_iterator&& other) noexcept { this->p = other.p; other.p = nullptr; return *this; }

	ptrdiff_t operator-(const DA_const_iterator& other) const { return p - other.p; }

	bool operator != (const DA_const_iterator& other) const { return !(p == other.p); }
	bool operator == (const DA_const_iterator& other) const { return p == other.p; }
	bool operator >  (const DA_const_iterator& other) const { return p - other.p > 0; }
	bool operator <  (const DA_const_iterator& other) const { return other > * this; }
	bool operator >= (const DA_const_iterator& other) const { return !(*this < other); }
	bool operator <= (const DA_const_iterator& other) const { return !(*this > other); }

	const typename DA_const_iterator::reference operator*  () const { if (p) return *p; throw std::exception("pointer is unitializated"); }
	const typename DA_const_iterator::pointer   operator-> () const { return p; }

	DA_const_iterator& operator+= (int n) { p += n; return *this; }
	DA_const_iterator  operator+  (int n) { return DA_const_iterator(p + n); }
	DA_const_iterator& operator-= (int n) { p -= n; return *this; }
	DA_const_iterator  operator-  (int n) { return DA_const_iterator(p - n); }

	const typename DA_const_iterator::reference operator[](int n) const { if (p) return *(p + n); throw std::exception("pointer is unitializated"); }

	DA_const_iterator& operator++ () { ++p; return *this; }
	DA_const_iterator  operator++ (int) { DA_const_iterator it(p); ++p; return it; }
	DA_const_iterator& operator-- () { --p; return *this; }
	DA_const_iterator  operator-- (int) { DA_const_iterator it(p); --p; return it; }
};

template<class T>
class DA_iterator : public DA_const_iterator<T>
{
public:
	DA_iterator(typename DA_iterator::pointer p) : DA_const_iterator<T>(p) {}
	DA_iterator() : DA_const_iterator<T>() {}
	DA_iterator(const DA_iterator& other) : DA_const_iterator<T>(other) {}
	DA_iterator(DA_iterator&& other) noexcept : DA_const_iterator<T>(other) {}

	DA_iterator& operator=(typename DA_iterator::pointer p) { this->p = p; return *this; }
	DA_iterator& operator=(const DA_iterator& other) { this->p = other.p; return *this; }
	DA_iterator& operator=(DA_iterator&& other) noexcept { this->p = other.p; other.p = nullptr; return *this; }

	ptrdiff_t operator-(const DA_iterator& other) const { return this->p - other.p; }

	bool operator != (const DA_iterator& other) const { return !(this->p == other.p); }
	bool operator == (const DA_iterator& other) const { return this->p == other.p; }
	bool operator >  (const DA_iterator& other) const { return this->p - other.p > 0; }
	bool operator <  (const DA_iterator& other) const { return other > * this; }
	bool operator >= (const DA_iterator& other) const { return !(*this < other); }
	bool operator <= (const DA_iterator& other) const { return !(*this > other); }

	typename DA_iterator::reference operator*  () { if (this->p) return *this->p; throw std::exception("pointer is unitializated"); }
	typename DA_iterator::pointer   operator-> () { return this->p; }

	DA_iterator& operator+= (int n) { this->p += n; return *this; }
	DA_iterator  operator+  (int n) { return DA_iterator(this->p + n); }
	DA_iterator& operator-= (int n) { this->p -= n; return *this; }
	DA_iterator  operator-  (int n) { return DA_iterator(this->p - n); }

	typename DA_iterator::reference operator [] (int n) { if (this->p) return *(this->p + n); throw std::exception("pointer is unitializated"); }

	DA_iterator& operator++ () { ++this->p; return *this; }
	DA_iterator  operator++ (int) { DA_iterator it(this->p); ++this->p; return it; }
	DA_iterator& operator-- () { --this->p; return *this; }
	DA_iterator  operator-- (int) { DA_iterator it(this->p); --this->p; return it; }
};

template<class T,class Allocator = std::allocator<T>>
class std::vector
{
private:
	ptrdiff_t _difference = sizeof(T);
	const size_t _additional_size = 4096 / _difference;
	size_t _capacity = _additional_size;
	size_t _size = 0;
	T* _array = nullptr;
	Allocator _allocator;
	typedef std::allocator_traits<Allocator> allocator_t;

	

	size_t capacity_of_size(size_t size) { return (size / _additional_size + 1) * _additional_size; }
	size_t current_min_capacity() { return capacity_of_size(_size); }
	void destroy_array(T*& _array, size_t& _constructed_count) {
		for (size_t i = 0; i < _constructed_count; ++i) {
			allocator_t::destroy(_allocator, _array + i);
		}
		_constructed_count = 0;
	}
	void destroy_current_array() { destroy_array(_array, _size); }
	void deallocate_array(T*& _array, const size_t& _capacity) {
		if (_array) {
			allocator_t::deallocate(_allocator, _array, _capacity);
			_array = nullptr;
		}
	}
	void deallocate_current_array() {
		deallocate_array(_array, _capacity);
		_capacity = _additional_size;
	}
	void delete_current_array() {
		destroy_current_array();
		deallocate_current_array();
	}
public:
	using const_iterator = DA_const_iterator<T>;
	using iterator = DA_iterator<T>;

	using const_reverse_iterator = std::reverse_iterator<const_iterator>;
	using reverse_iterator = std::reverse_iterator<iterator>;

	// Constructors
	std::vector() {
		try {
			_array = allocator_t::allocate(_allocator, _capacity);
		}
		catch (const std::bad_alloc & ex) {
			throw std::bad_alloc(ex);
		}
	}
	std::vector(const std::initializer_list<T>& list) {
		size_t list_size = list.size();

		if (list_size >= _capacity) {
			_capacity = capacity_of_size(list_size);
		}

		try {
			_array = allocator_t::allocate(_allocator, _capacity);
			for (auto&& t : list) {
				allocator_t::construct(_allocator, _array + _size, t);
				++_size;
			}
		}
		catch (const std::exception & _ex) {
			delete_current_array();
			throw std::exception(_ex);
		}
	}

	std::vector(const std::vector& other) :
		_capacity(other._capacity),
		_size(other._size)
	{
		try {
			_array = allocator_t::allocate(_allocator, _capacity);
			for (size_t i = 0; i < _size; ++i) {
				allocator_t::construct(_allocator, _array + i, other[i]);
			}
		}
		catch (const std::exception & _ex) {
			delete_current_array();
			throw std::exception(_ex);
		}
	}

	std::vector(std::vector&& other) noexcept :
		_size(other._size),
		_capacity(other._capacity),
		_array(other._array)
	{
		other._array = nullptr;
	}

	// Destructor
	~std::vector() {
		delete_current_array();
	}

	// operator =
	std::vector& operator=(const std::vector& other) {
		if (&other == this) {
			return *this;
		}

		T* new_array = nullptr;
		size_t new_size = other._size;
		size_t new_capacity = capacity_of_size(new_size);
		size_t constructed_count = 0;
		try {
			new_array = allocator_t::allocate(_allocator, new_capacity);
			for (size_t i = 0; i < new_size; ++i) {
				allocator_t::construct(_allocator, new_array + i, other[i]);
				++constructed_count;
			}
			delete_current_array();
			_capacity = new_capacity;
			_array = new_array;
			_size = new_size;
		}
		catch (const std::exception & _ex) {
			if (new_array) {
				destroy_array(new_array, constructed_count);
				deallocate_array(new_array, new_capacity);
			}
			throw std::exception(_ex);
		}
		return *this;
	}

	std::vector& operator=(std::vector&& other) noexcept {
		if (&other == this) {
			return *this;
		}

		delete_current_array();

		_size = other._size;
		_capacity = other._capacity;
		_array = other._array;
		other._array = nullptr;
		return *this;
	}

	// Member functions
	void assign(size_t count, const T& value) {
		T* new_array = nullptr;
		size_t new_capacity = capacity_of_size(count);
		size_t constructed_count = 0;
		try {
			new_array = allocator_t::allocate(_allocator, new_capacity);
			for (size_t i = 0; i < count; ++i) {
				allocator_t::construct(_allocator, new_array + i, value);
				++constructed_count;
			}
			delete_current_array();
			_capacity = new_capacity;
			_array = new_array;
			_size = count;
		}
		catch (const std::exception & _ex) {
			if (new_array) {
				destroy_array(new_array, constructed_count);
				deallocate_array(new_array, new_capacity);
			}
			throw std::exception(_ex);
		}
	}

	template <class _InputIt>
	void assign(_InputIt first, _InputIt last) {
		typename std::iterator_traits<_InputIt>::difference_type n = std::distance(first, last);
		T* new_array = nullptr;
		size_t new_capacity = capacity_of_size(n);
		size_t constructed_count = 0;
		try {
			new_array = allocator_t::allocate(_allocator, new_capacity);
			for (size_t i = 0; i < n; ++i) {
				allocator_t::construct(_allocator, new_array + i, (*first)++);
				++constructed_count;
			}
			delete_current_array();
			_capacity = new_capacity;
			_array = new_array;
			_size = n;
		}
		catch (const std::exception & _ex) {
			if (new_array) {
				destroy_array(new_array, constructed_count);
				deallocate_array(new_array, new_capacity);
			}
			throw std::exception(_ex);
		}
	}
	Allocator get_allocator() const { return _allocator; }

	// Elements access
	T& at(size_t index) {
		if (!(index < _size)) throw std::out_of_range("Array index is out of range");
		return *(_array + index);
	}
	const T& at(size_t index) const {
		if (!(index < _size)) throw std::out_of_range("Array index is out of range");
		return *(_array + index);
	}
	T& operator[](size_t index) {
		return *(_array + index);
	}
	const T& operator[](size_t index) const {
		return *(_array + index);
	}

	// Iterators
	iterator                begin()        noexcept { return iterator(_array); }
	const_iterator          begin()  const noexcept { return const_iterator(_array); }
	const_iterator          cbegin()  const noexcept { return const_iterator(_array); }

	iterator                end()        noexcept { return iterator(_array + _size); }
	const_iterator          end()  const noexcept { return const_iterator(_array + _size); }
	const_iterator          cend()  const noexcept { return const_iterator(_array + _size); }

	reverse_iterator        rbegin()        noexcept { return reverse_iterator(begin()); }
	const_reverse_iterator  rbegin()  const noexcept { return const_reverse_iterator(cbegin()); }
	const_reverse_iterator  crbegin()  const noexcept { return const_reverse_iterator(cbegin()); }


	reverse_iterator        rend()        noexcept { return reverse_iterator(end()); }
	const_reverse_iterator  rend()  const noexcept { return const_reverse_iterator(cend()); }
	const_reverse_iterator  crend()  const noexcept { return const_reverse_iterator(cend()); }

	// Capacity
	bool   empty()  const noexcept { return !_size; }
	size_t size()  const noexcept { return _size; }
	size_t max_size()  const noexcept { return allocator_t::max_size(_allocator); }
	void reserve(size_t _CAPACITY) {
		if (_CAPACITY < _size) {
			while (_size > _CAPACITY) {
				--_size;
				allocator_t::destroy(_allocator, _array + _size);
			}
		}
		T* new_array = nullptr;
		size_t new_size = _CAPACITY;
		size_t new_capacity = capacity_of_size(_CAPACITY);
		try {
			new_array = allocator_t::allocate(_allocator, new_capacity);
			for (size_t i = 0; i < new_size; ++i) {
				allocator_t::construct(_allocator, new_array + i, std::move(_array[i]));
				allocator_t::destroy(_allocator, _array + i);
			}
			delete_current_array();
			_capacity = new_capacity;
			_array = new_array;
			_size = new_size;
		}
		catch (const std::exception & _ex) {
			throw std::exception(_ex);
		}
	}
	size_t capacity()  const noexcept { return _capacity; }
	void shrink_to_fit() {
		try {
			size_t new_capacity = current_min_capacity();
			T* new_array = allocator_t::allocate(_allocator, new_capacity);
			for (size_t i = 0; i < _size; ++i) {
				allocator_t::construct(_allocator, new_array + i, std::move(_array[i]));
				allocator_t::destroy(_allocator, _array + i);
			}
			allocator_t::deallocate(_allocator, _array, _capacity);
			_capacity = new_capacity;
			_array = new_array;
		}
		catch (const std::exception & ex)
		{
			
			throw std::exception(ex);
		}
	}

	// Modificators
	void clear() noexcept { destroy_current_array(); }

	// insert ????

	/*template<class... Args>
	iterator emplace(const_iterator pos, Args&&... args);*/

	void erase(const size_t& index) {
		if (!(index < _size)) throw std::out_of_range("Array index is out of range");
		for (size_t i = index; i < _size - 1; ++i) {
			_array[i] = std::move(_array[i + 1]);
		}
		--_size;
		allocator_t::destroy(_allocator, _array + _size);
	}

	void erase(const_iterator pos) {
		typename std::iterator_traits<const_iterator>::difference_type n = std::distance(begin().operator->(), pos.operator->());
		erase(n - 1);
	}

	void erase(const_iterator first, const_iterator last) { // Not recommended to use, This function is unchecked
		typename std::iterator_traits<const_iterator>::difference_type diff = std::distance(first.operator->(), last.operator->());
		typename std::iterator_traits<const_iterator>::difference_type begin_i = std::distance(begin().operator->(), first.operator->()) - 1;
		for (size_t i = begin_i; i < _size - diff; ++i) {
			_array[i] = std::move(_array[i + diff]);
		}
		
		for (size_t i = _size - diff; i < _size; ++i) {
			allocator_t::destroy(_allocator, _array + i);
		}
		_size -= diff;
	}

	void push_back(const T& value) {
		emplace_back(value);
	}
	void push_back(T&& value) {
		emplace_back(std::move(value));
	}

	template<class... Args>
	void emplace_back(Args&&... args) {
		try {
			if (_size + 1 > _capacity) {
				size_t new_capacity = _capacity + _additional_size;
				T* new_array = allocator_t::allocate(_allocator, new_capacity);
				for (size_t i = 0; i < _size; ++i) {
					allocator_t::construct(_allocator, new_array + i, std::move(_array[i]));
					allocator_t::destroy(_allocator, _array + i);
				}
				allocator_t::deallocate(_allocator, _array, _capacity);
				_capacity = new_capacity;
				_array = new_array;
			}
			allocator_t::construct(_allocator, _array + _size, args...);
			++_size;
		}
		catch (const std::exception & ex)
		{
			throw std::exception(ex);
		}
	}

	void pop_back() {
		if (_size) {
			--_size;
			allocator_t::destroy(_allocator, _array + _size);
		}
	}

	void resize(size_t SIZE) {
		try {
			if (SIZE > _size) {
				for (size_t i = 0; i < SIZE - _size; ++i) {
					this->push_back(T());
				}
			}
			if (SIZE < _size) {
				while (_size > SIZE) {
					--_size;
					allocator_t::destroy(_allocator, _array + _size);
				}
			}
		}
		catch (const std::exception & ex)
		{
			throw std::exception(ex);
		}
	}

	template<class T, class Allocator>
	friend bool operator == (const std::vector& _Left, const std::vector& _Right);
};


#endif

template<class T, class Allocator>
inline bool operator==(const typename std::vector<T,Allocator>& _Left, const typename std::vector<T, Allocator>& _Right)
{
	if (_Left.size() != _Right.size()) {
		return false;
	}

	typename std::vector < T, Allocator>::const_iterator left_begin = _Left.begin();
	typename std::vector < T, Allocator>::const_iterator left_end = _Left.end();
	typename std::vector < T, Allocator>::const_iterator right_begin = _Right.begin();

	while (left_begin != left_end)
	{
		if ((*left_begin) != (*right_begin)) {
			return false;
		}
		++left_begin;
		++right_begin;
	}

	return true;
}
