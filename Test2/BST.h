#pragma once

#define BLACK false
#define RED true
#define MAX_STRING_LENGTH 256000

#include <initializer_list>
#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>

struct StringPair
{
    std::string _key;
    std::vector<std::string> _values;

    StringPair() : _key(), _values() {}

    StringPair(const std::string& key, const std::string& value)
    {
        if (key.size() > MAX_STRING_LENGTH)
        {
            _key = std::move(std::string(key.c_str(), MAX_STRING_LENGTH));
        }
        else
        {
            _key = key;
        }
        if (value.size() > MAX_STRING_LENGTH)
        {
            _values.emplace_back(value.c_str(), MAX_STRING_LENGTH);
        }
        else
        {
            _values.emplace_back(value);
        }
    }

    StringPair(const std::initializer_list<std::string>& strs)
    {
        auto key = *strs.begin();
        if (key.size() > MAX_STRING_LENGTH)
        {
            _key = std::move(std::string(key.c_str(), MAX_STRING_LENGTH));
        }
        else
        {
            _key = key;
        }
        std::for_each(strs.begin() + 1, strs.end(), [=](const std::string& str) {
            if (str.size() > MAX_STRING_LENGTH)
            {
                _values.emplace_back(str.c_str(), MAX_STRING_LENGTH);
            }
            else
            {
                _values.emplace_back(str);
            }
        });
    }

    StringPair& operator=(const StringPair& other)
    {
        if (this != &other)
        {
            if (other._key.size() > MAX_STRING_LENGTH)
            {
                _key = std::move(std::string(other._key.c_str(), MAX_STRING_LENGTH));
            }
            else
            {
                _key = other._key;
            }
            std::for_each(other._values.begin(), other._values.end(), [=](const std::string& str) {
                if (str.size() > MAX_STRING_LENGTH)
                {
                    _values.emplace_back(str.c_str(), MAX_STRING_LENGTH);
                }
                else
                {
                    _values.emplace_back(str);
                }
            });
        }
        return *this;
    }

    bool operator < (const StringPair& other)
    {
        return _key < other._key;
    }

    friend bool operator < (const StringPair& first, const StringPair& second)
    {
        return first._key < second._key;
    }

    bool operator > (const StringPair& other)
    {
        return _key > other._key;
    }

    friend bool operator > (const StringPair& first, const StringPair& second)
    {
        return first._key > second._key;
    }

    friend bool operator == (const StringPair& first, const StringPair& second)
    {
        return first._key == second._key;
    }

    friend std::ostream& operator << (std::ostream& os, const StringPair& pair)
    {
        os << "{" << pair._key.c_str() << "," << pair._values[0].c_str() << "}";
        return os;
    }
};

template <typename T>
class BST
{
public:
    BST();
    BST(const std::initializer_list<T>& list);
    BST(const BST& other);
    virtual ~BST();

    BST& operator=(const BST& other);

public:
    bool IsEmpty() const;
    void Insert(const T& data);
    void Remove(const T& data);
    void Postorder();

protected:
    struct Node
    {
        T data;
        Node* _parent;
        Node* _left, *_right;
        bool color;
    }*_root;

protected:
    void Copy(Node*& root, Node* other);
    void Clear(Node*& root);
    void InsertFixUp(Node* node);
    void LeftRotate(Node* x);
    void RightRotate(Node* x);
    void Postorder(Node* p, int indent);
    void Transplant(Node*& u, Node*& v);
    void Remove(Node* z);
    Node* TreeMinimum(Node* subTree) const;
    void DeleteFixup(Node* x);
    Node* SearchNode(const T& data) const;
    virtual void EqualData(Node* node, const T& data);
};

template <typename T>
BST<T>::BST() : _root(nullptr) {}

template <typename T>
BST<T>::BST(const BST& other) : _root(nullptr)
{
    if (other._root)
    {
        _root = new Node;
        _root->_left = nullptr;
        _root->_right = nullptr;
        _root->_parent = nullptr;
        Copy(_root, other._root);
    }
}

template <typename T>
BST<T>::BST(const std::initializer_list<T>& list)
{
    for (const auto& data : list)
    {
        Insert(data);
    }
}

template <typename T>
BST<T>::~BST()
{
    Clear(_root);
}

template <typename T>
BST<T>& BST<T>::operator=(const BST& other)
{
    if (this != &other)
    {
        Clear(_root);
        Copy(_root, other._root);
    }
    return *this;
}

template <typename T>
void BST<T>::Copy(Node*& root, Node* other)
{
    if (other)
    {
        if (!root)
        {
            root = new Node;
        }
        root->data = other->data;
        root->color = other->color;
        if (other->_left)
        {
            root->_left = new Node;
            root->_left->_parent = root;
            Copy(root->_left, other->_left);
        }
        else
        {
            root->_left = nullptr;
        }
        if (other->_right)
        {
            root->_right = new Node;
            root->_right->_parent = root;
            Copy(root->_right, other->_right);
        }
        else
        {
            root->_right = nullptr;
        }
    }
}

template <typename T>
void BST<T>::Clear(Node*& root)
{
    if (root)
    {
        Clear(root->_left);
        Clear(root->_right);
        root->_left = nullptr;
        root->_right = nullptr;
        root->_parent = nullptr;
        delete root;
        root = nullptr;
    }
}

template <typename T>
bool BST<T>::IsEmpty() const
{
    return nullptr == _root;
}

template<typename T>
void BST<T>::EqualData(Node* node, const T& data)
{

}

template <typename T>
void BST<T>::Insert(const T& data)
{
    auto z = new Node;
    z->data = data;
    Node* y = nullptr;
    Node* x = _root;
    while (x)
    {
        y = x;
        if (data < x->data)
        {
            x = x->_left;
        }
        else if (data > x->data)
        {
            x = x->_right;
        }
        else
        {
            EqualData(x, data);
            return;
        }
    }
    z->_parent = y;
    if (!y)
    {
        _root = z;
    }
    else if (data < y->data)
    {
        y->_left = z;
    }
    else
    {
        y->_right = z;
    }
    z->_left = nullptr;
    z->_right = nullptr;
    z->color = RED;
    InsertFixUp(z);
}

template <typename T>
void BST<T>::InsertFixUp(Node* node)
{
    if (node)
    {
        while (node->_parent && node->_parent->color == RED)
        {
            if (node->_parent == node->_parent->_parent->_left)
            {
                auto uncle = node->_parent->_parent->_right;
                if (uncle && uncle->color == RED)
                {
                    node->_parent->color = BLACK;
                    uncle->color = BLACK;
                    node->_parent->_parent->color = RED;
                    node = node->_parent->_parent;
                }
                else if (node == node->_parent->_right)
                {
                    node = node->_parent;
                    LeftRotate(node);
                }
                else if (node->_parent)
                {
                    node->_parent->color = BLACK;
                    node->_parent->_parent->color = RED;
                    RightRotate(node->_parent->_parent);
                }
            }
            else
            {
                auto uncle = node->_parent->_parent->_left;
                if (uncle && uncle->color == RED)
                {
                    node->_parent->color = BLACK;
                    uncle->color = BLACK;
                    node->_parent->_parent->color = RED;
                    node = node->_parent->_parent;
                }
                else if (node == node->_parent->_left)
                {
                    node = node->_parent;
                    RightRotate(node);
                }
                else if (node->_parent)
                {
                    node->_parent->color = BLACK;
                    node->_parent->_parent->color = RED;
                    LeftRotate(node->_parent->_parent);
                }
            }
        }
    }
    _root->color = BLACK;
}

template <typename T>
void BST<T>::LeftRotate(Node* x)
{
    Node* y = x->_right;
    if (x && y)
    {
        x->_right = y->_left;
        if (y->_left)
        {
            y->_left->_parent = x;
        }
        y->_parent = x->_parent;
        if (!x->_parent)
        {
            _root = y;
        }
        else if (x == x->_parent->_left)
        {
            x->_parent->_left = y;
        }
        else
        {
            x->_parent->_right = y;
        }
        y->_left = x;
        x->_parent = y;
    }
}

template <typename T>
void BST<T>::RightRotate(Node* x)
{
    Node* y = x->_left;
    if (y)
    {
        x->_left = y->_right;
        if (y->_right)
        {
            y->_right->_parent = x;
        }
        y->_parent = x->_parent;
        if (!x->_parent)
        {
            _root = y;
        }
        else if (x == x->_parent->_left)
        {
            x->_parent->_left = y;
        }
        else
        {
            x->_parent->_right = y;
        }
        y->_right = x;
        x->_parent = y;
    }
}

template <typename T>
void BST<T>::Transplant(Node*& u, Node*& v)
{
    if (u->_parent == nullptr)
    {
        _root = v;
    }
    else if (u == u->_parent->_left)
    {
        u->_parent->_left = v;
    }
    else
    {
        u->_parent->_right = v;
    }
    if (v)
    {
        v->_parent = u->_parent;
    }
};

template <typename T>
typename BST<T>::Node* BST<T>::TreeMinimum(Node* subTree) const
{
    auto x = subTree;
    Node* res = nullptr;
    while (x)
    {
        res = x;
        x = x->_left;
    }
    return res;
}

template <typename T>
void BST<T>::DeleteFixup(Node* x)
{
    while (x && x != _root && x->color == BLACK)
    {
        if (x == x->_parent->_left)
        {
            auto w = x->_parent->_right;
            if (w->color == RED)
            {
                w->color = BLACK;
                x->_parent->color = RED;
                LeftRotate(x->_parent);
                w = x->_parent->_right;
            }
            if (w->_left && w->_left->color == BLACK && w->_right && w->_right->color == BLACK)
            {
                w->color = RED;
                x = x->_parent;
            }
            else if (w->_right->color == BLACK)
            {
                w->_left->color = BLACK;
                w->color = RED;
                RightRotate(w);
                w = x->_parent->_right;
            }
            w->color = x->_parent->color;
            x->_parent->color = BLACK;
            w->_right->color = BLACK;
            LeftRotate(x->_parent);
            x = _root;
        }
        else
        {
            auto w = x->_parent->_left;
            if (w->color == RED)
            {
                w->color = BLACK;
                x->_parent->color = RED;
                LeftRotate(x->_parent);
                w = x->_parent->_left;
            }
            if (w->_right && w->_right->color == BLACK && w->_left && w->_left->color == BLACK)
            {
                w->color = RED;
                x = x->_parent;
            }
            else if (w->_left->color == BLACK)
            {
                w->_right->color = BLACK;
                w->color = RED;
                RightRotate(w);
                w = x->_parent->_left;
            }
            w->color = x->_parent->color;
            x->_parent->color = BLACK;
            w->_left->color = BLACK;
            LeftRotate(x->_parent);
            x = _root;
        }
    }
    if (x)
    {
        x->color = BLACK;
    }
}

template <typename T>
typename BST<T>::Node* BST<T>::SearchNode(const T& data) const
{
    Node* result = _root;
    while (result)
    {
        if (data == result->data)
        {
            return result;
        }
        else if (data < result->data)
        {
            result = result->_left;
        }
        else
        {
            result = result->_right;
        }
    }
    return result;
}

template <typename T>
void BST<T>::Remove(const T& data)
{
    auto node = SearchNode(data);
    if (node)
    {
        Remove(node);
    }
}

template <typename T>
void BST<T>::Remove(Node* z)
{
    if (!_root)
    {
        return;
    }
    auto y = z;
    auto originalColor = y->color;
    Node* x = nullptr;;
    if (z->_left == nullptr)
    {
        x = z->_right;
        Transplant(z, z->_right);
    }
    else if (!z->_right)
    {
        x = z->_left;
        Transplant(z, z->_left);
    }
    else
    {
        y = TreeMinimum(z->_right);
        originalColor = y->color;
        x = y->_right;
        if (y->_parent == z && x)
        {
            x->_parent = y;
        }
        else
        {
            Transplant(y, y->_right);
            y->_right = z->_right;
            if (y->_right)
            {
                y->_right->_parent = y;
            }
        }
        Transplant(z, y);
        y->_left = z->_left;
        y->_left->_parent = y;
        y->color = z->color;
    }
    if (originalColor == BLACK)
    {
        DeleteFixup(x);
    }
}

template <typename T>
void BST<T>::Postorder(Node* p, int indent)
{
    if (p != nullptr) {
        if (p->_right)
        {
            Postorder(p->_right, indent + 4);
        }
        if (indent) {
            std::cout << std::setw(indent) << ' ';
        }
        if (p->_right)
        {
            std::cout << " /\n" << std::setw(indent) << ' ';
        }
        std::cout << p->data << "\n ";
        if (p->_left)
        {
            std::cout << std::setw(indent) << ' ' << " \\\n";
            Postorder(p->_left, indent + 4);
        }
    }
}

template <typename T>
void BST<T>::Postorder()
{
    Postorder(_root, 1);
}