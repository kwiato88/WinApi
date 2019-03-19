//This code is under MIT licence, you can find the complete file here: https://github.com/kwiato88/WinApi/blob/master/LICENSE
#ifndef NODE_HPP
#define NODE_HPP

#include <vector>
#include <string>

namespace WinApi
{

struct Node
{
	Node();
	Node(const std::string& v, const void* con = nullptr);
	Node(const std::string& v, const std::vector<Node>& c, const void* con = nullptr);
	Node& operator=(const Node& p_other);

	std::string m_value;
	std::vector<Node> m_children;
	const void* m_userContext;
};

class NodeExport
{
public:
	struct Copy
	{
		std::string indent;
		std::string itemSepartor;
	};
	NodeExport(const Copy& p_data = Copy{ "\t", "\n" });

	void append(const Node& p_node);
	std::string result() const;

private:
	void appendIndent();

	int indentLvl;
	Copy data;
	std::string out;
};

} // namespacec WinApi

#endif // NODE_HPP
