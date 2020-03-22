#include <iostream>
#include <string>
#include <assert.h>
#include "HashMap.h"

constexpr auto MAX_TABLE_SIZE = 65536 - 1;

struct MyKeyHash
{
	unsigned long operator()(const int& k) const
	{
		return k % MAX_TABLE_SIZE;
	}
};

int main( )
{
	HashMap<int, std::string, MAX_TABLE_SIZE, MyKeyHash> hashMap;
	hashMap.insert(1, "1");
	hashMap.insert(2, "2");
	hashMap.insert(3, "3");

	std::string value;
	bool result = hashMap.get(4, value);
	assert(result);
	assert(value == "2");

	result = hashMap.get(3, value);
	assert(result);
	assert(value == "3");

	hashMap.remove(3);
	result = hashMap.get(3, value);
	assert(!result);
}