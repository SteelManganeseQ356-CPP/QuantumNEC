module;
#include <concepts>
#include <iostream>
#include <utility>
#include <type_traits>
export module api;
export namespace QuantumNEC {

template<template<typename> class T, typename R>
concept is_any = requires(T<R> t) {
	{ t.any_thing_ } -> std::same_as<R>;
};
template<typename T>
class any {
private:
    

public:
	explicit any(auto &&_any)
		requires is_any<any, T>
	{
		this->any_thing_ = std::move(_any.any_thing_);
	}
	explicit any(T &&_thing) {
		this->any_thing_ = std::move(_thing);
	}

private:
	T any_thing_;

public:
	T
	_cast_(void) {
		return this->any_thing_;
	}
};
template<typename T>
T
any_cast(const any<T> &_any) {
	return _any._cast_();
}
}	 // namespace QuantumNEC
export auto
main(void) -> int {
	using namespace QuantumNEC;
	any<int> a{0};
	int b = any_cast<int>(a);
std::is_empty< int >;
std::enable_if< int >;
std::is_nothrow_constructible_v< int, int >;
std::make_index_sequence< int >;
return 0;
}