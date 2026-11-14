
#include <iostream>
#include "../dsa/min_heap.hpp"
using namespace std;

int main() {
    MinHeap inventory;

    Product p1 = {101, "Keyboard", "Electronics", 2500, 20, 5};
    Product p2 = {102, "Mouse", "Electronics", 1200, 15, 5};
    Product p3 = {103, "Monitor", "Electronics", 15000, 5, 3};
    Product p4 = {104, "Printer", "Electronics", 20000, 8, 2};

    inventory.insert(p1);
    inventory.insert(p2);
    inventory.insert(p3);
    inventory.insert(p4);

    Product* lowest = inventory.getMin();

    if (lowest != nullptr) {
        cout << "Lowest Stock Product:" << endl;
        cout << "ID: " << lowest->id << endl;
        cout << "Name: " << lowest->name << endl;
        cout << "Quantity: " << lowest->quantity << endl;
    }

    Product removed;

    if (inventory.extractMin(removed)) {
        cout << "Removed Product: " << removed.name << endl;
        cout << "Removed Quantity: " << removed.quantity << endl;
    }

    Product* next = inventory.getMin();

    if (next != nullptr) {
        cout << "New Lowest Stock Product: " << next->name << endl;
        cout << "Quantity: " << next->quantity << endl;
    }

    cout << "Update Keyboard: "
        << inventory.updateQuantity(101, 2) << endl;

    Product* lowestAfterUpdate = inventory.getMin();

    if (lowestAfterUpdate != nullptr) {
        cout << "Lowest Stock After Update: "
            << lowestAfterUpdate->name << endl;

        cout << "Quantity: "
            << lowestAfterUpdate->quantity << endl;
    }

    cout << "Increase Keyboard Quantity: "
        << inventory.updateQuantity(101, 30) << endl;

    Product* lowestAfterIncrease = inventory.getMin();


    if (lowestAfterIncrease != nullptr) {
        cout << "New Lowest Stock: "
            << lowestAfterIncrease->name << endl;

        cout << "Quantity: "
            << lowestAfterIncrease->quantity << endl;
    }


    cout << "Update Missing Product: "
        << inventory.updateQuantity(999, 10) << endl;

    cout << "Update Negative Quantity: "
        << inventory.updateQuantity(101, -5) << endl;

    return 0;
}
