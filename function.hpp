#pragma once
#include <memory>
#include <type_traits>
#include <utility>
#include <functional>
#include <concepts>
template <typename callable, typename ret, typename ...params> concept checkArgumentsAndReturnType = requires(callable obj, params... args) { {std::invoke(std::forward<callable>(obj), std::forward<params>(args)...)} -> std::convertible_to<ret>; };

template <typename T> class myAwesomeFunction;
template <typename ret, typename ...params> class myAwesomeFunction<ret(params...)> {
public:
	template <typename C> requires checkArgumentsAndReturnType<C,ret,params...> myAwesomeFunction(C&& callable) : c(std::make_unique<callableCarrier<std::decay_t<C>>>(std::forward<C>(callable))) { }
	ret operator()(params... args) {

		return c->call(std::forward<params>(args)...);
	}
private:
	struct abstractCallable {
		virtual ret call(params...) = 0;
		virtual ~abstractCallable() = default;
	};

	template <typename actualCallable> struct callableCarrier : public abstractCallable {
		actualCallable _callable;
		template <typename T> callableCarrier(T&& callable) : _callable(std::forward<T>(callable)) {} //make_unique above explicitly defines template parameter so non template constructor actualCallable&& becomes constructor with rvalue reference argument instead of forward reference
		ret call(params... args) override {
			return std::invoke(_callable, std::forward<params>(args)...);
		}
	};
	template <typename T> callableCarrier(T&&)->callableCarrier<std::decay_t<T>>;

	std::unique_ptr<abstractCallable> c;
};
//todo: Finish abstractCallable&callableCarrier rule of 5 and clone idiom to create copy/move of myAwesomeFunction
//How to convert to C++11 standard:
//remove C++17's deduction guide of callableCarrier: With existing code this deduction guide is not needed as callableCarrier is created only with explicit decay type parameter in myAwesomeFunction constructor
//replace "std::decay_t<C>" with "typename std::decay<C>::type" as decay_t had been added in C++14
//replace C++17's std::invoke
//remove C++20's concept checkArgumentsAndReturnType