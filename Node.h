#pragma once

#include <cassert>

template <typename KEY, typename VALUE>
class Node
{
public:
	Node(const KEY& key, const VALUE& value) :
		m_key(key),
		m_value(value),
		m_pNextNode(nullptr)
	{ }

	~Node( ) = default;

	KEY getKey( ) const
	{
		return m_key;
	}

	void setKey(const KEY& key)
	{
		m_key = key;
	}

	VALUE getValue( ) const
	{
		return m_value;
	}

	void setValue(const VALUE& value)
	{
		m_value = value;
	}

	Node& getNextNode( ) const
	{
		return *m_pNextNode;
	}

	void setNextNode(Node& node)
	{
		m_pNextNode = &node;
	}

private:
	Node(const Node&) = delete;
	Node& operator=(const Node&) = delete;

private:
	KEY m_key;
	VALUE m_value;
	Node* m_pNextNode;
};