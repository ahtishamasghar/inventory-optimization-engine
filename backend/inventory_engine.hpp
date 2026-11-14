#ifndef INVENTORY_ENGINE_HPP
#define INVENTORY_ENGINE_HPP
#include "dsa/hash_table.hpp"
#include "dsa/bst.hpp"
#include "dsa/min_heap.hpp"
#include "dsa/merge_sort.hpp"
#include <cmath>
#include <algorithm>
#include <cctype>

using namespace std;

class InventoryEngine {
private:
    HashTable hashTable;
    BST bst;
    MinHeap minHeap;

public:
    InventoryEngine() {
    }

    vector<Product> getAllProducts() {
        return hashTable.getAllProducts();
    }

    vector<Product> getProductsSortedByPrice() {
        vector<Product> products = hashTable.getAllProducts();

        MergeSort::sortByPrice(products);

        return products;
    }

    vector<Product> getProductsSortedById() const {
        return bst.getProductsInorder();
    }

    // Case-insensitive substring matching; scans all hash-table products.
    vector<Product> searchProductsByName(string name) {
        auto lower = [](unsigned char ch) { return static_cast<char>(std::tolower(ch)); };
        transform(name.begin(), name.end(), name.begin(), lower);
        vector<Product> matches;
        for (const auto& product : hashTable.getAllProducts()) {
            string candidate = product.name;
            transform(candidate.begin(), candidate.end(), candidate.begin(), lower);
            if (candidate.find(name) != string::npos) matches.push_back(product);
        }
        return matches;
    }

    bool addProduct(const Product& product) {
        if (product.id < 0 || product.name.empty() ||
            !isfinite(product.price) || product.price < 0 || product.quantity < 0 ||
            product.minStock < 0) {
            return false;
        }

        if (!hashTable.insert(product)) {
            return false;
        }

        bst.insert(product);
        minHeap.insert(product);

        return true;
    }

    Product* getProduct(int id) {
        return hashTable.search(id);
    }


    bool updateProduct(const Product& product) {
        if (product.id < 0 || product.name.empty() ||
            !isfinite(product.price) || product.price < 0 ||
            product.quantity < 0 || product.minStock < 0) {
            return false;
        }

        if (hashTable.search(product.id) == nullptr ||
            bst.search(product.id) == nullptr ||
            !minHeap.contains(product.id)) {
            return false;
        }

        bool hashUpdated = hashTable.update(product);
        bool bstUpdated = bst.updateProduct(product);
        bool heapUpdated = minHeap.updateProduct(product);

        return hashUpdated && bstUpdated && heapUpdated;
    }


    bool removeProduct(int id) {
        if (hashTable.search(id) == nullptr) {
            return false;
        }

        bst.remove(id);
        minHeap.remove(id);
        hashTable.remove(id);

        return true;
    }

    const Product* getLowestStock() {
        return minHeap.getMin();
    }

};

#endif
