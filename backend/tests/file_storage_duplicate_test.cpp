#include <iostream>
#include <fstream>
#include "../storage/file_storage.hpp"

using namespace std;

int main() {
    ofstream file("storage/duplicate_test.json");

    file << R"({"products":[
        {"id":101,"name":"Keyboard","category":"Electronics",
         "price":2500,"quantity":20,"minStock":5},
        {"id":101,"name":"Mouse","category":"Electronics",
         "price":1200,"quantity":15,"minStock":5}
    ]})";

    file.close();

    FileStorage storage("storage/duplicate_test.json");

    try {
        vector<Product> products = storage.load();
        cout << "Unexpected success" << endl;
        return 1;
    }
    catch (const exception& e) {
        cout << "Validation passed: " << e.what() << endl;
    }

    return 0;
}
