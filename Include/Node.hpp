#ifndef NODE_HPP
#define NODE_HPP

#include <vector>
#include <string>

namespace WinApi
{

struct Node
{
	Node()
		: m_value(), m_children(), m_userContext(nullptr)
	{}
	Node(const std::string& v, const void* con = nullptr)
		: m_value(v), m_userContext(con)
	{}
	Node(const std::string& v, const std::vector<Node>& c, const void* con = nullptr)
		: m_value(v), m_children(c), m_userContext(con)
	{}
	Node& operator=(const Node& p_other)
	{
		m_value = p_other.m_value;
		m_children = p_other.m_children;
		m_userContext = p_other.m_userContext;
		return *this;
	}

	std::string m_value;
	std::vector<Node> m_children;
	const void* m_userContext;
};

} // namespacec WinApi

#endif // NODE_HPP
