#include <iostream>
#include "../storage/file_storage.hpp"

using namespace std;

int main() {
    FileStorage storage("storage/test_products.json");

    vector<Product> products = {
        {101, "Keyboard", "Electronics", 2500, 20, 5},
        {102, "Mouse", "Electronics", 1200, 15, 5}
    };

    cout << "Save Products: " << storage.save(products) << endl;

    return 0;
}
