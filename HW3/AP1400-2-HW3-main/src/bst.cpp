#include "bst.h"

#include <iomanip>
#include <queue>
#include <string>
#include <vector>

std::ostream &operator<<(std::ostream &os, const BST::Node &node)
{
    os << "\u001B[32m" << std::left << std::setw(17) << &node << "\u001B[0m=> value:\u001B[32m"
       << std::setw(10) << node.value << "\u001B[0mleft:\u001B[32m" << std::setw(16) << node.left
       << "\u001B[0mright:\u001B[32m" << node.right << "\u001B[0m";

    return os;
}

bool BST::add_node(int value)
{
    if (!root)
    {
        root = new Node(value, nullptr, nullptr);
        return true;
    }

    Node *temp{root};
    bool right_child{};
    Node *parrent{nullptr};

    while (true)
    {
        if (*temp == value)
            return false;

        else if (*temp > value)
        {
            right_child = false;
            parrent = temp;
            temp = temp->left;
        }

        else
        {
            right_child = true;
            parrent = temp;
            temp = temp->right;
        }

        if (!temp)
        {
            temp = new Node(value, nullptr, nullptr);

            if (right_child)
                parrent->right = temp;
            else
                parrent->left = temp;

            return true;
        }
    }
}

void BST::bfs(std::function<void(Node *&node)> func) const
{
    if (root)
    {
        std::queue<Node *> nodes{};
        nodes.push(root);
        while (!nodes.empty())
        {
            if (nodes.front()->left)
                nodes.push(nodes.front()->left);

            if (nodes.front()->right)
                nodes.push(nodes.front()->right);

            func(nodes.front());
            nodes.pop();
        }
    }
}

size_t BST::length() const
{
    size_t cnt{};

    if (root)
    {
        std::queue<Node *> nodes{};
        nodes.push(root);

        while (!nodes.empty())
        {
            if (nodes.front()->left)
                nodes.push(nodes.front()->left);

            if (nodes.front()->right)
                nodes.push(nodes.front()->right);

            cnt++;
            nodes.pop();
        }
    }

    return cnt;
}

std::ostream &operator<<(std::ostream &os, const BST &bst)
{
    std::string s(80, '*');

    os << s << "\n";

    bst.bfs([&os](BST::Node *&node)
            { os << *node << "\n"; });

    os << "binary search tree size: \u001B[32m" << bst.length() << "\u001B[0m\n";
    os << s << std::endl;

    return os;
}

BST::Node **BST::find_node(int value)
{
    if (!root)
        return nullptr;

    Node **temp{&root};

    while (true)
    {
        if (**temp == value)
            return temp;

        if (**temp > value)
            temp = &((*temp)->left);

        else
            temp = &((*temp)->right);

        if (*temp == nullptr)
            return nullptr;
    }
}

BST::Node **BST::find_parrent(int value)
{
    if (!root)
        return nullptr;

    if (!find_node(value))
        return nullptr;

    Node **temp{&root};
    Node **parrent{nullptr};

    while (true)
    {
        if (**temp == value)
            return parrent;

        parrent = temp;
        if (**temp > value)
            temp = &((*temp)->left);

        else
            temp = &((*temp)->right);
    }
}

BST::Node **BST::find_successor(int value)
{
    Node **node{find_node(value)};

    if (!node)
        return nullptr;

    Node **successor{};
    if ((*node)->left)
    {
        successor = &((*node)->left);

        while ((*successor)->right)
            successor = &((*successor)->right);
    }
    else if ((*node)->right)
    {
        successor = &((*node)->right);

        while ((*successor)->left)
            successor = &((*successor)->left);
    }
    else
        successor = nullptr;

    return successor;
}

bool BST::delete_node(int value)
{
    Node **node{find_node(value)};

    if (!node)
        return false;

    if (!(*node)->left && !(*node)->right)
    {
        Node **parrent{find_parrent((*node)->value)};
        if (parrent)
            if ((*parrent)->value > (*node)->value)
            {
                delete *node;
                (*parrent)->left = nullptr;
            }
            else
            {
                delete *node;
                (*parrent)->right = nullptr;
            }

        else // It will be root, because only root has no parrents
        {
            delete *node;
            root = nullptr;
        }
    }

    else if (!(*node)->left || !(*node)->right)
    {
        Node **parrent{find_parrent((*node)->value)};
        if (parrent)
            if ((*parrent)->value > (*node)->value)
            {

                if ((*node)->left)
                {
                    Node *temp{(*node)->left};
                    delete *node;
                    (*parrent)->left = temp;
                }
                else
                {
                    Node *temp{(*node)->right};
                    delete *node;
                    (*parrent)->left = temp;
                }
            }

            else
            {
                if ((*node)->left)
                {
                    Node *temp{(*node)->left};
                    delete *node;
                    (*parrent)->right = temp;
                }
                else
                {
                    Node *temp{(*node)->right};
                    delete *node;
                    (*parrent)->right = temp;
                }
            }

        else // It will be root, because only root has no parrents
        {
            if ((*node)->left)
            {
                Node *temp{(*node)->left};
                delete *node;
                root = temp;
            }
            else
            {
                Node *temp{(*node)->right};
                delete *node;
                root = temp;
            }
        }
    }

    else
    {
        Node **successor{find_successor(value)};
        Node **successor_parrent{find_parrent((*successor)->value)};
        if ((*successor_parrent)->value > (*successor)->value)
        {
            (*node)->value = (*successor)->value;
            delete *successor;
            (*successor_parrent)->left = nullptr;
        }
        else
        {
            (*node)->value = (*successor)->value;
            delete *successor;
            (*successor_parrent)->right = nullptr;
        }
    }

    return true;
}

BST::~BST()
{
    std::vector<Node *> nodes;
    bfs([&nodes](Node *&node)
        { nodes.push_back(node); });
    for (auto &node : nodes)
        delete node;
}

BST::BST(const BST &bst) : root{nullptr}
{
    std::vector<Node *> nodes;
    bst.bfs([&nodes](Node *&node)
            { nodes.push_back(node); });
    for (auto &node : nodes)
        add_node(node->value);
}

BST &BST::operator=(const BST &bst)
{
    if (this != &bst)
    {
        std::vector<Node *> old_nodes;
        bfs([&old_nodes](Node *&node)
            { old_nodes.push_back(node); });
        for (auto &node : old_nodes)
            delete node;

        root = nullptr;

        std::vector<Node *> new_nodes;
        bst.bfs([&new_nodes](Node *&node)
                { new_nodes.push_back(node); });
        for (auto &node : new_nodes)
            add_node(node->value);
    }

    return *this;
}

BST &BST::operator=(BST &&bst)
{
    if (this != &bst)
    {
        std::vector<Node *> nodes;
        bfs([&nodes](Node *&node)
            { nodes.push_back(node); });
        for (auto &node : nodes)
            delete node;

        root = bst.root;
        bst.root = nullptr;
    }

    return *this;
}

BST::BST(BST &&bst)
{
    root = bst.root;
    bst.root = nullptr;
}

BST::BST(std::initializer_list<int> args) : root{nullptr}
{
    for (auto node : args)
        add_node(node);
}

const BST &BST::operator++()
{
    bfs([](Node *&node)
        { node->value++; });

    return *this;
}

const BST BST::operator++(int)
{
    BST bst{*this};

    bfs([&bst](Node *&node)
        { node->value++; });

    return bst;
}
