#include <iostream>
#include "../inventory_engine.hpp"
using namespace std;

int main() {
    InventoryEngine inventory;

    Product p1 = {101, "Keyboard", "Electronics", 2500, 20, 5};
    Product p2 = {102, "Mouse", "Electronics", 1200, 15, 5};

    cout << "Add Keyboard: "
         << inventory.addProduct(p1) << endl;

    cout << "Add Mouse: "
         << inventory.addProduct(p2) << endl;

    cout << "Add Duplicate Keyboard: "
         << inventory.addProduct(p1) << endl;


    Product* result = inventory.getProduct(102);

    if (result != nullptr) {
        cout << "Product Found!" << endl;
        cout << "ID: " << result->id << endl;
        cout << "Name: " << result->name << endl;
        cout << "Quantity: " << result->quantity << endl;
    } else {
        cout << "Product Not Found!" << endl;
    }

    Product* missing = inventory.getProduct(999);

    if (missing == nullptr) {
        cout << "Product 999 Not Found!" << endl;
    }


    Product updated = {101, "Gaming Keyboard", "Electronics", 3500, 3, 5};

    cout << "Update Keyboard: "
        << inventory.updateProduct(updated) << endl;

    Product* resultAfterUpdate = inventory.getProduct(101);

    if (resultAfterUpdate != nullptr) {
        cout << "Updated Name: " << resultAfterUpdate->name << endl;
        cout << "Updated Price: " << resultAfterUpdate->price << endl;
        cout << "Updated Quantity: " << resultAfterUpdate->quantity << endl;
    }

    Product missingProduct = {999, "Unknown", "Electronics", 1000, 10, 5};

    cout << "Update Missing Product: "
        << inventory.updateProduct(missingProduct) << endl;


    cout << "Remove Mouse: "
        << inventory.removeProduct(102) << endl;

    Product* deleted = inventory.getProduct(102);

    if (deleted == nullptr) {
        cout << "Mouse Successfully Deleted!" << endl;
    }

    cout << "Remove Missing Product: "
        << inventory.removeProduct(999) << endl;

    const Product* lowest = inventory.getLowestStock();

    if (lowest != nullptr) {
        cout << "Lowest Stock Product: " << lowest->name << endl;
        cout << "Quantity: " << lowest->quantity << endl;
    } else {
        cout << "Inventory is Empty!" << endl;
    }

    return 0;
}
