#pragma once

#include "Hash.h"
#include "Node.h"

#ifdef _DEBUG
#define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
#endif // DEBUG

/*
* Hash map class template
* 키(Key), 해시 함수(Hash Function), 해시(Hash), 값(value), 저장소(Bucket, Slot)
* Collision Resolution: Separate Chaining
*/
template <typename KEY, typename VALUE, size_t TABLE_SIZE = 8, typename HASH_FUNC = SimpleHash<KEY, TABLE_SIZE>>
class HashMap
{
	static_assert(0 < TABLE_SIZE, "Table size must be greater than 0.");
	static_assert(65536 > TABLE_SIZE, "Table size must be less than 65536.");

public:
	HashMap( ) :
		m_Table( ),
		m_HashFunction( )
	{ }

	~HashMap( )
	{
		// destroy all slots one by one
		for ( size_t index = 0; index < TABLE_SIZE; ++index )
		{
			Node<KEY, VALUE>* pEntry = m_Table[index];
			if ( nullptr == pEntry )
			{
				continue;
			}

			Node<KEY, VALUE>* pPrev = nullptr;

			while ( nullptr != pEntry )
			{
				pPrev = pEntry;
				pEntry = &pEntry->getNextNode( );

				delete pPrev;
				pPrev = nullptr;
			}

			m_Table[index] = nullptr;
		}
	}

	bool get(const KEY& key, VALUE& value)
	{
		unsigned long slot = m_HashFunction(key);

		Node<KEY, VALUE>* pEntry = m_Table[slot];
		while ( nullptr != pEntry )
		{
			if ( pEntry->getKey( ) == key )
			{
				value = pEntry->getValue( );
				return true;
			}

			pEntry = &pEntry->getNextNode( );
		}

		return false;
	}

	void insert(const KEY& key, const VALUE& value)
	{
		unsigned long slot = m_HashFunction(key);
		Node<KEY, VALUE>* pPrev = nullptr;
		Node<KEY, VALUE>* pEntry = m_Table[slot];

		while ( nullptr != pEntry && pEntry->getKey( ) != key )
		{
			pPrev = pEntry;
			pEntry = &pEntry->getNextNode( );
		}

		if ( nullptr == pEntry )
		{
			pEntry = new Node<KEY, VALUE>(key, value);
			assert(nullptr != pEntry);

			if ( nullptr == pPrev )
			{
				// insert as first slot
				m_Table[slot] = pEntry;
			}
			else
			{
				pPrev->setNextNode(*pEntry);
			}
		}
		else
		{
			// just update the value
			pEntry->setValue(value);
		}
	}

	void remove(const KEY& key)
	{
		unsigned long slot = m_HashFunction(key);
		Node<KEY, VALUE>* pPrev = nullptr;
		Node<KEY, VALUE>* pEntry = m_Table[slot];
		assert(nullptr != pEntry); // key not found

		while ( nullptr != pEntry && pEntry->getKey( ) != key )
		{
			pPrev = pEntry;
			pEntry = &pEntry->getNextNode( );
		}

		if ( nullptr == pPrev )
		{
			// remove first slot of the list
			m_Table[slot] = &pEntry->getNextNode( );
		}
		else
		{
			pPrev->setNextNode(pEntry->getNextNode( ));
		}

		delete pEntry;
		pEntry = nullptr;
	}

private:
	HashMap(const HashMap&) = delete;
	const HashMap& operator=(const HashMap&) = delete;

private:
	Node<KEY, VALUE>* m_Table[TABLE_SIZE];
	HASH_FUNC m_HashFunction;
};
