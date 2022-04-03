#ifndef BST_H
#define BST_H

#include <iostream>
#include <functional>
#include <initializer_list>
class BST
{
public:
    class Node
    {
    public:
        Node(int value, Node *left, Node *right) : value{value}, left{left}, right{right} {}
        Node() : Node{0, nullptr, nullptr} {}
        Node(const Node &node) : value{node.value}, left{node.left}, right{node.right} {}

        bool operator<(const int &v) const { return value < v; }
        bool operator>(const int &v) const { return value > v; }
        bool operator==(const int &v) const { return value == v; }
        bool operator<=(const int &v) const { return value <= v; }
        bool operator>=(const int &v) const { return value >= v; }

        friend bool operator<(const int &v, const BST::Node &node) { return v < node.value; }
        friend bool operator>(const int &v, const BST::Node &node) { return v > node.value; }
        friend bool operator==(const int &v, const BST::Node &node) { return v == node.value; }
        friend bool operator<=(const int &v, const BST::Node &node) { return v <= node.value; }
        friend bool operator>=(const int &v, const BST::Node &node) { return v >= node.value; }

        int value;
        Node *left;
        Node *right;
    };

    Node *&get_root() { return root; };
    void bfs(std::function<void(Node *&node)> func) const;
    size_t length() const;
    bool add_node(int value);
    Node **find_node(int value);
    Node **find_parrent(int value);
    Node **find_successor(int value);
    bool delete_node(int value);

private:
    Node *root;
};

std::ostream &operator<<(std::ostream &os, const BST::Node &node);
std::ostream &operator<<(std::ostream &os, const BST &bst);
#endif // BST_H
