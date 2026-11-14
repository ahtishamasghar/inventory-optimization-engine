
#include <iostream>
#include "../dsa/min_heap.hpp"

using namespace std;

int main() {
    MinHeap heap;

    Product p1 = {101, "Keyboard", "Electronics", 2500, 20, 5};
    Product p2 = {102, "Mouse", "Electronics", 1200, 15, 5};
    Product p3 = {103, "Monitor", "Electronics", 15000, 5, 2};

    heap.insert(p1);
    heap.insert(p2);
    heap.insert(p3);

    cout << "Remove Monitor: " << heap.remove(103) << endl;

    Product* lowest = heap.getMin();

    if (lowest != nullptr) {
        cout << "Lowest Stock: " << lowest->name << endl;
        cout << "Quantity: " << lowest->quantity << endl;
    }

    cout << "Remove Missing Product: " << heap.remove(999) << endl;

    return 0;
}
