#pragma once

#include "Hash.h"
#include "Node.h"

#ifdef _DEBUG
#define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
#endif // DEBUG

 // Hash map class template
template <typename KEY, typename VALUE, size_t TABLE_SIZE = 8, typename HASH_FUNC = SimpleHash<KEY, TABLE_SIZE>>
class HashMap
{
	static_assert(0 < TABLE_SIZE, "Table size must be greater than 0.");
	static_assert(65536 > TABLE_SIZE, "Table size must be less than 65536.");

public:
	HashMap( ) :
		m_Tables( ),
		m_HashFunction( )
	{ }

	~HashMap( )
	{
		// destroy all buckets one by one
		for ( size_t index = 0; index < TABLE_SIZE; ++index )
		{
			Node<KEY, VALUE>* pEntry = m_Tables[index];
			Node<KEY, VALUE>* pPrev = nullptr;

			while ( nullptr != pEntry )
			{
				pPrev = pEntry;
				pEntry = &pEntry->getNextNode( );

				delete pPrev;
				pPrev = nullptr;
			}

			m_Tables[index] = nullptr;
		}
	}

	bool get(const KEY& key, VALUE& value)
	{
		unsigned long hashValue = m_HashFunction(key);

		Node<KEY, VALUE>* pEntry = m_Tables[hashValue];
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

	void put(const KEY& key, const VALUE& value)
	{
		unsigned long hashValue = m_HashFunction(key);
		Node<KEY, VALUE>* pPrev = nullptr;
		Node<KEY, VALUE>* pEntry = m_Tables[hashValue];

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
				// insert as first bucket
				m_Tables[hashValue] = pEntry;
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

	void remove(const KEY &key)
	{
		unsigned long hashValue = m_HashFunction(key);
		Node<KEY, VALUE>* pPrev = nullptr;
		Node<KEY, VALUE>* pEntry = m_Tables[hashValue];

		while ( nullptr != pEntry && pEntry->getKey( ) != key )
		{
			pPrev = pEntry;
			pEntry = &pEntry->getNextNode( );
		}

		if (nullptr == pEntry )
		{
			// key not found
			assert(false);
			return;
		}
		else
		{
			if ( nullptr == pPrev )
			{
				// remove first bucket of the list
				m_Tables[hashValue] = &pEntry->getNextNode( );
			}
			else
			{
				pPrev->setNextNode(pEntry->getNextNode( ));
			}

			delete pEntry;
			pEntry = nullptr;
		}
	}

private:
	HashMap(const HashMap&) = delete;
	const HashMap& operator=(const HashMap&) = delete;

private:
	Node<KEY, VALUE>* m_Tables[TABLE_SIZE];
	HASH_FUNC m_HashFunction;
};