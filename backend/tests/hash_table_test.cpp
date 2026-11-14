//hash table test

#include <iostream>
#include "../dsa/hash_table.hpp"
using namespace std;

int main() {
    HashTable inventory;

    Product p1 = {101, "Keyboard", "Electronics", 2500, 20, 5};
    Product p2 = {102, "Mouse", "Electronics", 1200, 15, 5};

    cout << inventory.insert(p1) << endl;
    cout << inventory.insert(p2) << endl;
    cout << inventory.insert(p1) << endl;

    Product* result = inventory.search(102);

    if (result != nullptr) {
        cout << "Product Found!" << endl;
        cout << "ID: " << result->id << endl;
        cout << "Name: " << result->name << endl;
        cout << "Quantity: " << result->quantity << endl;
    } else {
        cout << "Product Not Found!" << endl;
    }

    cout << "Delete Mouse: " << inventory.remove(102) << endl;

    Product* deleted = inventory.search(102);

    if (deleted == nullptr) {
        cout << "Mouse no longer exists!" << endl;
    } else {
        cout << "Mouse still exists!" << endl;
    }

    Product updated = {101, "Keyboard", "Electronics", 2500, 35, 5};

    cout << "Update Keyboard: " << inventory.update(updated) << endl;

    Product* result2 = inventory.search(101);

    if (result2 != nullptr) {
        cout << "Updated Quantity: " << result2->quantity << endl;
    }

    Product p3 = {202, "Monitor", "Electronics", 15000, 10, 3};
    Product p4 = {303, "Printer", "Electronics", 20000, 7, 2};

    cout << "Insert Monitor: " << inventory.insert(p3) << endl;
    cout << "Insert Printer: " << inventory.insert(p4) << endl;

    Product* monitor = inventory.search(202);
    Product* printer = inventory.search(303);

    if (monitor != nullptr && printer != nullptr) {
        cout << "Collision handled successfully!" << endl;
        cout << "Product 202: " << monitor->name << endl;
        cout << "Product 303: " << printer->name << endl;
    }


    return 0;
}
