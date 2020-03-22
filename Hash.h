#pragma once

#include <cassert>

// reinterpret_cast<unsigned long> 가능한 경우에만 이 해시 함수를 사용할 것
template <typename KEY, size_t TABLE_SIZE>
struct SimpleHash
{
	static_assert(0 < TABLE_SIZE, "Table size must be greater than 0.");

	unsigned long operator()(const KEY& key) const
	{
		return reinterpret_cast<unsigned long>(key) % TABLE_SIZE;
	}
};