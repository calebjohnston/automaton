#pragma once

#include <array>
#include <iostream>
#include <map>
#include <variant>
#include <vector>

#include "kernel.h"

struct Error {
	int code = 0;
	std::string message;
};

typedef std::variant<KernelRef,Error> MaybeKernelRef;
typedef std::function<KernelRef(KernelRef)> KernelFunction;
typedef std::function<KernelFunction(KernelFunction)> HigherOrderKernelFunction;

KernelFunction compose(KernelFunction a, KernelFunction b)
{
	return [a = std::move(a), b = std::move(b)](KernelRef element) {
		return b(a(std::move(element)));
	};
}

KernelRef operator|(KernelRef element, KernelFunction decorator)
{
	return decorator(std::move(element));
}

std::vector<KernelRef> operator|(std::vector<KernelRef> elements, KernelFunction decorator)
{
	std::vector<KernelRef> output;
	for (auto& it : elements)
		output.push_back(std::move(it) | decorator);

	return output;
}

KernelFunction operator|(KernelFunction a, KernelFunction b)
{
	return compose(a, b);
}

KernelFunction operator|(HigherOrderKernelFunction a, KernelFunction b)
{
	return a(b);
}

// KernelRef setTest10(KernelRef element)
// {
// 	if (element)
// 		bool result = element->updateTest(10);
// 	return element;
// }
//
// KernelRef addTest10(KernelRef element)
// {
// 	if (element)
// 		bool result = element->updateTest(element->test() + 10);
// 	return element;
// }

//---------------------
typedef std::function<MaybeKernelRef(MaybeKernelRef)> MaybeKernelFunction;

MaybeKernelFunction compose(MaybeKernelFunction a, MaybeKernelFunction b)
{
	return [a = std::move(a), b = std::move(b)](MaybeKernelRef element) {
		return b(a(std::move(element)));
	};
}

MaybeKernelRef operator|(MaybeKernelRef element, MaybeKernelFunction decorator)
{
	return decorator(std::move(element));
}

std::vector<MaybeKernelRef> operator|(std::vector<MaybeKernelRef> elements, MaybeKernelFunction decorator)
{
	std::vector<MaybeKernelRef> output;
	for (auto& it : elements)
		output.push_back(std::move(it) | decorator);

	return output;
}

MaybeKernelFunction operator|(MaybeKernelFunction a, MaybeKernelFunction b)
{
	return compose(a, b);
}

// MaybeKernelRef setTest10(MaybeKernelRef element)
// {
// 	try {
//       std::get<KernelRef>(element);
//     }
//     catch (const std::bad_variant_access& ex) {
//         std::cout << std::get<Error>(element).message << std::endl;
// 		return element;
//     }
//
// 	if (std::get<0>(element))
// 		bool result = std::get<0>(element)->updateTest(10);
//
// 	return element;
// }
//
// MaybeKernelRef addTest10(MaybeKernelRef element)
// {
// 	try {
//       std::get<KernelRef>(element);
//     }
//     catch (const std::bad_variant_access& ex) {
//         std::cout << std::get<Error>(element).message << std::endl;
// 		return element;
//     }
//
// 	if (std::get<0>(element)) {
// 		int new_val = std::get<0>(element)->test() + 10;
// 		bool result = std::get<0>(element)->updateTest(new_val);
// 	}
//
// 	return element;
// }

MaybeKernelRef subtractTest20(MaybeKernelRef element)
{
	try {
      std::get<KernelRef>(element);
    }
    catch (const std::bad_variant_access& ex) {
        std::cout << std::get<Error>(element).message << std::endl;
		return element;
    }

	if (std::get<0>(element)) {
		int new_val = std::get<0>(element)->test() - 20;
		bool result = std::get<0>(element)->updateTest(new_val);
		if (!result) {
			Error err = {1, "test value can't be negative."};
			return err;
		}
	}

	return element;
}

KernelRef setTest10(KernelRef element)
{
	if (element) return Kernel::updateTest(element, 10);

	return element;
}

KernelRef addTest10(KernelRef element)
{
	if (element) return Kernel::updateTest(element, element->test() + 10);

	return element;
}

KernelRef policy(KernelRef element)
{
	if (element && element->hasGroups()) {
		return element;
	}

	return KernelRef();
}

KernelFunction applySecurity(KernelFunction filter_policy)
{
	return [fp = std::move(filter_policy)](KernelRef element) {
		if (fp(element))
			return element | setTest10 | addTest10;
		else
			return element;
	};
}
