#ifndef DSA_H
#define DSA_H

#include <queue>
#include <stack>
#include <vector>

// Stack: stores application IDs so the most recent application can be undone.
template <typename T>
class UndoStack {
public:
    void push(const T& value) { data.push(value); }
    bool empty() const { return data.empty(); }
    T pop() {
        T value = data.top();
        data.pop();
        return value;
    }
private:
    std::stack<T> data;
};

// Queue: pending applications are processed in FIFO order.
template <typename T>
class PendingQueue {
public:
    void enqueue(const T& value) { data.push(value); }
    bool empty() const { return data.empty(); }
    T dequeue() {
        T value = data.front();
        data.pop();
        return value;
    }
    std::size_t size() const { return data.size(); }
private:
    std::queue<T> data;
};

// Binary Search Tree: stores job IDs for insertion and lookup demonstrations.
class JobIdBST {
private:
    struct Node {
        int key;
        Node* left;
        Node* right;
        explicit Node(int value) : key(value), left(nullptr), right(nullptr) {}
    };
    Node* root = nullptr;

    Node* insertNode(Node* node, int key) {
        if (!node) return new Node(key);
        if (key < node->key) node->left = insertNode(node->left, key);
        else if (key > node->key) node->right = insertNode(node->right, key);
        return node;
    }

    bool searchNode(Node* node, int key) const {
        if (!node) return false;
        if (node->key == key) return true;
        return key < node->key ? searchNode(node->left, key)
                               : searchNode(node->right, key);
    }

    void inorderNode(Node* node, std::vector<int>& result) const {
        if (!node) return;
        inorderNode(node->left, result);
        result.push_back(node->key);
        inorderNode(node->right, result);
    }

    void destroy(Node* node) {
        if (!node) return;
        destroy(node->left);
        destroy(node->right);
        delete node;
    }

public:
    JobIdBST() = default;
    ~JobIdBST() { destroy(root); }
    JobIdBST(const JobIdBST&) = delete;
    JobIdBST& operator=(const JobIdBST&) = delete;

    void insert(int key) { root = insertNode(root, key); }
    bool search(int key) const { return searchNode(root, key); }
    std::vector<int> inorder() const {
        std::vector<int> result;
        inorderNode(root, result);
        return result;
    }
};

#endif
