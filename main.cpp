#include <iostream>
#include "function.hpp"

int function(int arg) {
	return arg;
}

struct foo {
	foo(foo&&) {
		std::cout << "foo move constructor\n";
	}
	foo() = default;
	/*explicit*/ operator int() { //uncomment explicit to disallow object6
		return 1;
	}
};

struct callableStruct {
	int operator()(foo&& arg) {
		return 1;
	}
};

int main()
{
	int toCapture{ 0 };
	myAwesomeFunction<int(int)> object1{ &function };
	myAwesomeFunction<int(int)> object2{ [&toCapture](int arg) {toCapture += arg; return arg; } };
	myAwesomeFunction<int(foo&&)> object3{ callableStruct{} };
	callableStruct cs;
	myAwesomeFunction<int(foo&&)> object4{ cs };
	auto lambda = [](int arg) {return arg; };
	myAwesomeFunction<int(int)> object5{ lambda };
	myAwesomeFunction<int(int)> object6{ [](int arg) { return foo{}; } }; //lambda returns object implicitly convertible to int
	std::cout << object1(100) << '\n' << object2(200) << '\n' << object3(foo{}) << '\n' << object4(foo{}) << '\n'<< object5(300) << '\n' << object6(100) << '\n';
}
