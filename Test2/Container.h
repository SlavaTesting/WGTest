#pragma once 

#include "BST.h"

class CustomAssociativeContainer : public BST<StringPair>
{
public:
    CustomAssociativeContainer();
    CustomAssociativeContainer(const CustomAssociativeContainer& other);
    CustomAssociativeContainer& operator=(const CustomAssociativeContainer& other);
    ~CustomAssociativeContainer();
    CustomAssociativeContainer(const std::initializer_list<StringPair>& pairs);

public:
    std::vector<std::string> SearchFiles(const std::string& data);
    void ClearAll();

private:
    void EqualData(Node* node, const StringPair& pair) override;

private:
    using BST<StringPair>::_root;
};