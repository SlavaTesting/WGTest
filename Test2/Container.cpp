#include "Container.h"

CustomAssociativeContainer::CustomAssociativeContainer()
{
}

CustomAssociativeContainer::CustomAssociativeContainer(const CustomAssociativeContainer& other)
{
    Copy(_root, other._root);
}

CustomAssociativeContainer& CustomAssociativeContainer::operator=(const CustomAssociativeContainer& other)
{
    if (this != &other)
    {
        Copy(_root, other._root);
    }
    return *this;
}

CustomAssociativeContainer::~CustomAssociativeContainer()
{
    Clear(_root);
}

CustomAssociativeContainer::CustomAssociativeContainer(const std::initializer_list<StringPair>& pairs)
{
    for (const auto& pair : pairs)
    {
        Insert(pair);
    }
}

void CustomAssociativeContainer::EqualData(Node* node, const StringPair& pair)
{
    node->data._values.insert(node->data._values.cend(), pair._values.cbegin(), pair._values.cend());
}

std::vector<std::string> CustomAssociativeContainer::SearchFiles(const std::string& data)
{
    auto node = SearchNode({ data });
    if (node)
    {
        return node->data._values;
    }
    return std::vector<std::string>{};
}

void CustomAssociativeContainer::ClearAll()
{
    Clear(_root);
}

