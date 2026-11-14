
#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP

#include "../models/product.hpp"
#include <list>
#include <vector>
using namespace std;

class HashTable {
    private:
        static const int SIZE = 101;
        vector<list<Product>> table;

        int hashFunction(int id) {
            return id % SIZE;
        }

    public:
        HashTable() {
            table.resize(SIZE);
        }

        vector<Product> getAllProducts() {
            vector<Product> products;

            for (const auto& bucket : table) {
                for (const auto& product : bucket) {
                    products.push_back(product);
                }
            }

            return products;
        }

        bool insert(const Product& product) {
            if (product.id < 0)
                return false;

            int index = hashFunction(product.id);

            for (const Product& p : table[index]) {
                if (p.id == product.id) {
                    return false;
                }
            }

            table[index].push_back(product);
            return true;
        }

        Product* search(int id) {
            if (id < 0)
                return nullptr;

            int index = hashFunction(id);

            for (Product& p : table[index]) {
                if (p.id == id) {
                    return &p;
                }
            }

            return nullptr;
        }

        bool remove(int id) {
            if (id < 0)
                return false;

            int index = hashFunction(id);

            for (auto it = table[index].begin(); it != table[index].end(); ++it) {
                if (it->id == id) {
                    table[index].erase(it);
                    return true;
                }
            }

            return false;
        }

        bool update(const Product& product) {
            Product* existing = search(product.id);

            if (existing == nullptr) {
                return false;
            }

            *existing = product;
            return true;
        }

};

#endif
