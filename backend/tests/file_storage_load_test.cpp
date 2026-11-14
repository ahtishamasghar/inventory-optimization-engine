#include <iostream>
#include "../storage/file_storage.hpp"

using namespace std;

int main() {
    FileStorage storage("storage/test_products.json");

    vector<Product> products = storage.load();

    cout << "Loaded Products: " << products.size() << endl;

    for (const Product& p : products) {
        cout << "ID: " << p.id << endl;
        cout << "Name: " << p.name << endl;
        cout << "Price: " << p.price << endl;
        cout << "Quantity: " << p.quantity << endl;
        cout << "----------------" << endl;
    }

    return 0;
}
