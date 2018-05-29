//This code is under MIT licence, you can find the complete file here: https://github.com/kwiato88/WinApi/blob/master/LICENSE
#include "Node.hpp"

namespace WinApi
{

Node::Node()
	: m_value(), m_children(), m_userContext(nullptr)
{}

Node::Node(const std::string& v, const void* con)
	: m_value(v), m_userContext(con)
{}

Node::Node(const std::string& v, const std::vector<Node>& c, const void* con)
	: m_value(v), m_children(c), m_userContext(con)
{}

Node& Node::operator=(const Node& p_other)
{
	m_value = p_other.m_value;
	m_children = p_other.m_children;
	m_userContext = p_other.m_userContext;
	return *this;
}

NodeExport::NodeExport(const Copy& p_data)
	: indentLvl(0), data(p_data)
{}

void NodeExport::append(const Node& p_node)
{
	appendIndent();
	out += p_node.m_value;
	out += data.itemSepartor;
	++indentLvl;
	for (const auto& node : p_node.m_children)
		append(node);
	--indentLvl;
}

std::string NodeExport::result()
{
	return out;
}

void NodeExport::appendIndent()
{
	for (int i = 0; i < indentLvl; ++i)
		out += data.indent;
}

}
