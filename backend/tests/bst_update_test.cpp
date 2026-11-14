
#include <iostream>
#include "../dsa/bst.hpp"

using namespace std;

int main() {
    BST bst;

    Product p1 = {101, "Keyboard", "Electronics", 2500, 20, 5};
    Product p2 = {102, "Mouse", "Electronics", 1200, 15, 5};

    bst.insert(p1);
    bst.insert(p2);

    p1.quantity = 5;
    p1.price = 2000;

    cout << "Update Keyboard: " << bst.updateProduct(p1) << endl;

    Product* result = bst.search(101);

    if (result != nullptr) {
        cout << "Name: " << result->name << endl;
        cout << "Quantity: " << result->quantity << endl;
        cout << "Price: " << result->price << endl;
    }

    return 0;
}
