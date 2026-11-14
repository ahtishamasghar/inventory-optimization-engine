#ifndef BST_HPP
#define BST_HPP
#include <iostream>
#include <vector>
#include "../models/product.hpp"
using namespace std;

struct BSTNode {
    Product product;
    BSTNode* left;
    BSTNode* right;

    BSTNode(const Product& p) {
        product = p;
        left = nullptr;
        right = nullptr;
    }
};

class BST {
    private:
        BSTNode* root;

        BSTNode* findMin(BSTNode* node) {
            while (node->left != nullptr) {
                node = node->left;
            }
            return node;
        }

        BSTNode* deleteNode(BSTNode* node, int id) {
            if (node == nullptr)
                return nullptr;

            if (id < node->product.id) {
                node->left = deleteNode(node->left, id);
            }
            else if (id > node->product.id) {
                node->right = deleteNode(node->right, id);
            }
            else {
                if (node->left == nullptr) {
                    BSTNode* temp = node->right;
                    delete node;
                    return temp;
                }

                if (node->right == nullptr) {
                    BSTNode* temp = node->left;
                    delete node;
                    return temp;
                }

                BSTNode* temp = findMin(node->right);
                node->product = temp->product;
                node->right = deleteNode(node->right, temp->product.id);
            }

            return node;
        }

        BSTNode* searchNode(BSTNode* node, int id) {
            if (node == nullptr || node->product.id == id)
                return node;

            if (id < node->product.id)
                return searchNode(node->left, id);

            return searchNode(node->right, id);
        }

        

        BSTNode* insertNode(BSTNode* node, const Product& product) {
            if (node == nullptr) {
                return new BSTNode(product);
            }

            if (product.id < node->product.id) {
                node->left = insertNode(node->left, product);
            }
            else if (product.id > node->product.id) {
                node->right = insertNode(node->right, product);
            }

            return node;
        }

    void inorderNode(BSTNode* node) {
        if (node == nullptr)
            return;

        inorderNode(node->left);
        cout << node->product.id << " ";
        inorderNode(node->right);
    }


        void collectInorder(const BSTNode* node, vector<Product>& products) const {
            if (node == nullptr) return;
            collectInorder(node->left, products);
            products.push_back(node->product);
            collectInorder(node->right, products);
        }

        void clear(BSTNode* node) {
            if (node == nullptr)
                return;

            clear(node->left);
            clear(node->right);
            delete node;
        }

    public:
        BST() {
            root = nullptr;
        }

        bool updateProduct(const Product& product) {
            Product* existing = search(product.id);

            if (existing == nullptr) {
                return false;
            }

            *existing = product;
            return true;
        }


        bool remove(int id) {
            if (search(id) == nullptr)
                return false;

            root = deleteNode(root, id);
            return true;
        }

        Product* search(int id) {
            BSTNode* result = searchNode(root, id);

            if (result == nullptr)
                return nullptr;

            return &result->product;
        }


        ~BST() {
            clear(root);
        }

        void insert(const Product& product) {
            root = insertNode(root, product);
        }

        vector<Product> getProductsInorder() const {
            vector<Product> products;
            collectInorder(root, products);
            return products;
        }

        void inorder() {
            inorderNode(root);
            cout << endl;
        }

        BST(const BST&) = delete;
        BST& operator=(const BST&) = delete;
};

#endif
