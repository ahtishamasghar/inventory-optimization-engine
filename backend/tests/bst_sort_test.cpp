#include <cassert>
#include "../inventory_engine.hpp"

int main() {
    InventoryEngine inventory;
    assert(inventory.getProductsSortedById().empty());
    assert(inventory.addProduct({50, "Root", "Test", 10, 8, 2}));
    assert(inventory.addProduct({20, "Left", "Test", 30, 4, 2}));
    assert(inventory.addProduct({70, "Right", "Test", 20, 6, 2}));
    assert(inventory.addProduct({40, "Middle", "Test", 5, 3, 2}));
    auto products = inventory.getProductsSortedById();
    assert(products.size() == 4);
    assert(products[0].id == 20 && products[1].id == 40);
    assert(products[2].id == 50 && products[3].id == 70);
    assert(inventory.updateProduct({40, "Updated", "Test", 15, 1, 2}));
    assert(inventory.removeProduct(50));
    products = inventory.getProductsSortedById();
    assert(products.size() == 3);
    assert(products[0].id == 20 && products[1].id == 40 && products[2].id == 70);
    assert(products[1].name == "Updated" && products[1].quantity == 1);
    assert(inventory.removeProduct(20));
    assert(inventory.removeProduct(40));
    assert(inventory.removeProduct(70));
    assert(inventory.getProductsSortedById().empty());
}
