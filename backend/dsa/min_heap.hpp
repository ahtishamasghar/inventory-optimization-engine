#ifndef MIN_HEAP_HPP
#define MIN_HEAP_HPP

#include "../models/product.hpp"
#include <vector>
#include <utility>
using namespace std;

class MinHeap {
private:
    vector<Product> heap;

public:
    MinHeap() {
    }
    bool contains(int id) const {
        for (const Product& product : heap) {
            if (product.id == id) {
                return true;
            }
        }

        return false;
    }

    bool remove(int id) {
        for (int i = 0; i < (int)heap.size(); i++) {
            if (heap[i].id == id) {
                heap[i] = heap.back();
                heap.pop_back();

                if (i < (int)heap.size()) {
                    int parent = (i - 1) / 2;

                    if (i > 0 &&
                        heap[i].quantity < heap[parent].quantity) {

                        while (i > 0) {
                            parent = (i - 1) / 2;

                            if (heap[parent].quantity <= heap[i].quantity) {
                                break;
                            }

                            swap(heap[parent], heap[i]);
                            i = parent;
                        }
                    } else {
                        while (true) {
                            int left = 2 * i + 1;
                            int right = 2 * i + 2;
                            int smallest = i;

                            if (left < (int)heap.size() &&
                                heap[left].quantity < heap[smallest].quantity) {
                                smallest = left;
                            }

                            if (right < (int)heap.size() &&
                                heap[right].quantity < heap[smallest].quantity) {
                                smallest = right;
                            }

                            if (smallest == i) {
                                break;
                            }

                            swap(heap[i], heap[smallest]);
                            i = smallest;
                        }
                    }
                }

                return true;
            }
        }

        return false;
    }


    bool updateProduct(const Product& product) {
        if (product.quantity < 0) {
            return false;
        }

        for (int i = 0; i < (int)heap.size(); i++) {
            if (heap[i].id == product.id) {
                heap[i] = product;

                while (i > 0) {
                    int parent = (i - 1) / 2;

                    if (heap[parent].quantity <= heap[i].quantity) {
                        break;
                    }

                    swap(heap[parent], heap[i]);
                    i = parent;
                }

                while (true) {
                    int left = 2 * i + 1;
                    int right = 2 * i + 2;
                    int smallest = i;

                    if (left < (int)heap.size() &&
                        heap[left].quantity < heap[smallest].quantity) {
                        smallest = left;
                    }

                    if (right < (int)heap.size() &&
                        heap[right].quantity < heap[smallest].quantity) {
                        smallest = right;
                    }

                    if (smallest == i) {
                        break;
                    }

                    swap(heap[i], heap[smallest]);
                    i = smallest;
                }

                return true;
            }
        }

        return false;
    }


    bool updateQuantity(int id, int newQuantity) {
        if (newQuantity < 0)
            return false;

        int i = -1;

        for (int j = 0; j < (int)heap.size(); j++) {
            if (heap[j].id == id) {
                i = j;
                break;
            }
        }

        if (i == -1)
            return false;

        heap[i].quantity = newQuantity;

        while (i > 0) {
            int parent = (i - 1) / 2;

            if (heap[parent].quantity <= heap[i].quantity)
                break;

            swap(heap[parent], heap[i]);
            i = parent;
        }

        int n = heap.size();

        while (true) {
            int left = 2 * i + 1;
            int right = 2 * i + 2;
            int smallest = i;

            if (left < n && heap[left].quantity < heap[smallest].quantity)
                smallest = left;

            if (right < n && heap[right].quantity < heap[smallest].quantity)
                smallest = right;

            if (smallest == i)
                break;

            swap(heap[i], heap[smallest]);
            i = smallest;
        }

        return true;
    }

    bool extractMin(Product& removed) {
        if (heap.empty())
            return false;

        removed = heap[0];

        heap[0] = heap.back();
        heap.pop_back();

        int i = 0;
        int n = heap.size();

        while (true) {
            int left = 2 * i + 1;
            int right = 2 * i + 2;
            int smallest = i;

            if (left < n && heap[left].quantity < heap[smallest].quantity)
                smallest = left;

            if (right < n && heap[right].quantity < heap[smallest].quantity)
                smallest = right;

            if (smallest == i)
                break;

            swap(heap[i], heap[smallest]);
            i = smallest;
        }

        return true;
    }

    void insert(const Product& product) {
        heap.push_back(product);

        int i = heap.size() - 1;

        while (i > 0) {
            int parent = (i - 1) / 2;

            if (heap[parent].quantity <= heap[i].quantity)
                break;

            swap(heap[parent], heap[i]);
            i = parent;
        }
    }

    Product* getMin() {
        if (heap.empty())
            return nullptr;

        return &heap[0];
    }
};

#endif
