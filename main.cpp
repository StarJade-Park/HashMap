#include <iostream>
#include <string>
#include <assert.h>
#include "HashMap.h"

using namespace std;

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
	HashMap<int, string, MAX_TABLE_SIZE, MyKeyHash> hmap;
	hmap.put(1, "1");
	hmap.put(2, "2");
	hmap.put(3, "3");

	string value;
	bool result = hmap.get(2, value);
	assert(result);
	assert(value == "2");

	result = hmap.get(3, value);
	assert(result);
	assert(value == "3");

	hmap.remove(3);
	result = hmap.get(3, value);
	assert(!result);
}