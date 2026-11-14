
#include <iostream>
#include "../inventory_engine.hpp"

using namespace std;

int main() {
    InventoryEngine inventory;

    inventory.addProduct({101, "Keyboard", "Electronics", 2500, 20, 5});
    inventory.addProduct({102, "Mouse", "Electronics", 1200, 15, 5});
    inventory.addProduct({103, "Monitor", "Electronics", 15000, 8, 2});
    inventory.addProduct({104, "Printer", "Electronics", 9000, 4, 2});

    vector<Product> products = inventory.getProductsSortedByPrice();

    cout << "Products Sorted by Price:" << endl;

    for (const Product& p : products) {
        cout << p.name << " - " << p.price << endl;
    }

    return 0;
}
