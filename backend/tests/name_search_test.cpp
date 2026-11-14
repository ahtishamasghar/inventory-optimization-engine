#include <cassert>
#include "../inventory_engine.hpp"

int main() {
    InventoryEngine engine;
    assert(engine.searchProductsByName("keyboard").empty());
    assert(engine.addProduct({101, "Keyboard", "Electronics", 2500, 25, 5}));
    assert(engine.addProduct({102, "Wireless Keyboard", "Electronics", 3000, 10, 3}));
    assert(engine.addProduct({103, "LED Monitor", "Electronics", 18000, 12, 4}));
    assert(engine.searchProductsByName("KEYBOARD").size() == 2);
    assert(engine.searchProductsByName("board").size() == 2);
    assert(engine.searchProductsByName("wireless keyboard").size() == 1);
    assert(engine.searchProductsByName("missing").empty());
    assert(engine.getProduct(101)->name == "Keyboard");
    assert(engine.updateProduct({101, "USB Cable", "Electronics", 500, 2, 2}));
    assert(engine.searchProductsByName("keyboard").size() == 1);
    assert(engine.removeProduct(102));
    assert(engine.searchProductsByName("keyboard").empty());
}
