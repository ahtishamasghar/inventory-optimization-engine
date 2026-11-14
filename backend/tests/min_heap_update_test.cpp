
#include <iostream>
#include "../dsa/min_heap.hpp"

using namespace std;

int main() {
    MinHeap heap;

    Product p1 = {101, "Keyboard", "Electronics", 2500, 20, 5};
    Product p2 = {102, "Mouse", "Electronics", 1200, 15, 5};

    heap.insert(p1);
    heap.insert(p2);

    p1.quantity = 2;
    p1.price = 2000;

    cout << "Update Keyboard: " << heap.updateProduct(p1) << endl;

    Product* lowest = heap.getMin();

    if (lowest != nullptr) {
        cout << "Lowest Stock: " << lowest->name << endl;
        cout << "Quantity: " << lowest->quantity << endl;
        cout << "Price: " << lowest->price << endl;
    }

    return 0;
}
