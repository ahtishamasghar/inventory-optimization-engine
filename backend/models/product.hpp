#ifndef PRODUCT_H
#define PRODUCT_H
#include <string>
using namespace std;
struct Product {
    int id;
    string name;
    string category;
    double price;
    int quantity;
    int minStock;
};
#endif