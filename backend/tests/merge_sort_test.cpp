
#include <iostream>
#include "../dsa/merge_sort.hpp"
using namespace std;

int main() {
    vector<Product> products = {
        {101, "Keyboard", "Electronics", 2500, 20, 5},
        {102, "Mouse", "Electronics", 1200, 15, 5},
        {103, "Monitor", "Electronics", 15000, 5, 3},
        {104, "Printer", "Electronics", 20000, 8, 2}
    };

    MergeSort::sortByPrice(products);

    cout << "Products Sorted by Price:" << endl;

    for (const Product& p : products) {
        cout << p.name << " - " << p.price << endl;
    }

    return 0;
}
