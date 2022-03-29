//
//  slot.hpp
//
//  Created by Caleb Johnston on 3/28/2022.
//  Copyright © 2022 Caleb Johnston. All rights reserved.
//

#pragma once

#include <memory>
#include <vector>
#include <type_traits>

/*
template<typename T> struct is_shared_ptr : std::false_type {};
template<typename T> struct is_shared_ptr<std::shared_ptr<T>> : std::true_type {};

template <class T>
typename std::enable_if<is_shared_ptr<decltype(std::declval<T>().value)>::value, void>::type
func( T t )
{
    std::cout << "shared ptr" << std::endl;
}

template <class T>
typename std::enable_if<!is_shared_ptr<decltype(std::declval<T>().value)>::value, void>::type
func( T t )
{
    std::cout << "non shared" << std::endl;
}
*/

template<typename T, typename U, std::size_t I>
class Slot {
public:
	Slot(std::vector<std::shared_ptr<U>>& s) : _storage(s) { _storage.resize(I); }
	~Slot() = default;

	Slot(const Slot& rhs) noexcept = delete;
	Slot(Slot&& rhs) noexcept = delete;
	Slot& operator=(const std::shared_ptr<T>& rhs) { resize(); _storage[I] = rhs; return *this; }
	Slot& operator=(std::shared_ptr<T>&& rhs) { resize(); _storage[I] = rhs; return *this; }

    std::shared_ptr<T> operator->() const {
		// using T_ptr = typename std::pointer_traits<Ptr>::template rebind<block>;
		//return std::dynamic_pointer_cast<T>(std::remove_reference_t<decltype(_storage[I])>(_storage[I]));
		//return std::dynamic_pointer_cast<std::remove_reference<decltype(*T)>::type>(_storage[I]);
		return std::dynamic_pointer_cast<T>(_storage[I]);
	}
	// T& operator*() const {
	// 	return std::dynamic_pointer_cast<std::remove_reference_t<decltype(T)>::element_type>(_storage[I]);
	// }

private:
	inline void resize() {
		if (_storage.size() <= I)
			_storage.resize(I);
	}

	std::vector<std::shared_ptr<U>>& _storage;
};
