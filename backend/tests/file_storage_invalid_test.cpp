#include <iostream>
#include <fstream>
#include "../storage/file_storage.hpp"

using namespace std;

int main() {
    ofstream file("storage/invalid_test.json");

    file << R"({"products":[{"id":201,"name":"Test",
    "category":"Electronics","price":500,
    "quantity":2.5,"minStock":1}]})";

    file.close();

    FileStorage storage("storage/invalid_test.json");

    try {
        vector<Product> products = storage.load();
        cout << "Unexpected success" << endl;
    }
    catch (const exception& e) {
        cout << "Validation passed: "
             << e.what() << endl;
    }

    return 0;
}
