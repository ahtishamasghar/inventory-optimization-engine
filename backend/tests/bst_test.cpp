#include <iostream>
#include "../dsa/bst.hpp"
using namespace std;

int main() {
    BST inventory;

    Product p1 = {50, "Keyboard", "Electronics", 2500, 20, 5};
    Product p2 = {30, "Mouse", "Electronics", 1200, 15, 5};
    Product p3 = {70, "Monitor", "Electronics", 15000, 10, 3};
    Product p4 = {20, "Printer", "Electronics", 20000, 7, 2};
    Product p5 = {40, "Speaker", "Electronics", 3000, 12, 4};

    inventory.insert(p1);
    inventory.insert(p2);
    inventory.insert(p3);
    inventory.insert(p4);
    inventory.insert(p5);
    
    cout << "Five products inserted into BST!" << endl;
    cout << "BST Inorder Traversal: ";
    inventory.inorder();

    Product* result = inventory.search(40);

    if (result != nullptr) {
        cout << "Product Found!" << endl;
        cout << "ID: " << result->id << endl;
        cout << "Name: " << result->name << endl;
    } else {
        cout << "Product Not Found!" << endl;
    }

    cout << "Delete 20: " << inventory.remove(20) << endl;

    cout << "BST after deletion: ";
    inventory.inorder();

    cout << "Search deleted product: ";
    if (inventory.search(20) == nullptr)
        cout << "Not Found" << endl;
    else
        cout << "Found" << endl;

    cout << "Delete 30: " << inventory.remove(30) << endl;

    cout << "BST after deleting 30: ";
    inventory.inorder();

    cout << "Delete 50: " << inventory.remove(50) << endl;

    cout << "BST after deleting 50: ";
    inventory.inorder();

    if (inventory.search(50) == nullptr) {
        cout << "Product 50 successfully deleted!" << endl;
    }

    return 0;
}
