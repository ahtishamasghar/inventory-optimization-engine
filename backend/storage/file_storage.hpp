#ifndef FILE_STORAGE_HPP
#define FILE_STORAGE_HPP
#include <iterator>
#include <exception>
#include <fstream>
#include <unordered_set>
#include <cstdio>
#include <stdexcept>
#include <vector>
#include <string>
#include "crow.h"
#include <cmath>
#include <climits>
#include "../models/product.hpp"

using namespace std;

class FileStorage {
private:
    string filename;

public:
    FileStorage(const string& file = "storage/products.json") {
        filename = file;
    }


    bool save(const vector<Product>& products) {
        crow::json::wvalue data;
        data["products"] = crow::json::wvalue::list();

        for (int i = 0; i < (int)products.size(); i++) {
            data["products"][i]["id"] = products[i].id;
            data["products"][i]["name"] = products[i].name;
            data["products"][i]["category"] = products[i].category;
            data["products"][i]["price"] = products[i].price;
            data["products"][i]["quantity"] = products[i].quantity;
            data["products"][i]["minStock"] = products[i].minStock;
        }

        string tempFile = filename + ".tmp";

        ofstream file(tempFile, ios::trunc);

        if (!file.is_open()) {
            return false;
        }

        file << data.dump();
        file.close();

        if (file.fail()) {
            std::remove(tempFile.c_str());
            return false;
        }

        if (std::rename(tempFile.c_str(), filename.c_str()) != 0) {
            std::remove(tempFile.c_str());
            return false;
        }

        return true;
    }


    vector<Product> load() {
        vector<Product> products;

        ifstream file(filename);

        if (!file.is_open()) {
            throw runtime_error("Cannot open storage file: " + filename);
        }

        string content(
            (istreambuf_iterator<char>(file)),
            istreambuf_iterator<char>()
        );

        if (file.bad()) {
            throw runtime_error("Error reading storage file");
        }

        auto data = crow::json::load(content);

        if (!data || !data.has("products") ||
            data["products"].t() != crow::json::type::List) {
            throw runtime_error("Invalid storage JSON");
        }

        try {
            unordered_set<int> usedIds;
            for (const auto& item : data["products"]) {
                if (item.t() != crow::json::type::Object ||
                    !item.has("id") || !item.has("name") ||
                    !item.has("category") || !item.has("price") ||
                    !item.has("quantity") || !item.has("minStock")) {
                    throw runtime_error("Invalid product record");
                }


                if (item["id"].t() != crow::json::type::Number ||
                    item["name"].t() != crow::json::type::String ||
                    item["category"].t() != crow::json::type::String ||
                    item["price"].t() != crow::json::type::Number ||
                    item["quantity"].t() != crow::json::type::Number ||
                    item["minStock"].t() != crow::json::type::Number) {
                    throw runtime_error("Invalid product field types");
                }

                double id = item["id"].d();
                double quantity = item["quantity"].d();
                double minStock = item["minStock"].d();

                if (!isfinite(id) || !isfinite(quantity) ||
                    !isfinite(minStock) ||
                    floor(id) != id ||
                    floor(quantity) != quantity ||
                    floor(minStock) != minStock ||
                    id < 0 || id > INT_MAX ||
                    quantity < 0 || quantity > INT_MAX ||
                    minStock < 0 || minStock > INT_MAX) {
                    throw runtime_error("Invalid integer values");
                }

                Product product;

                product.id = static_cast<int>(id);
                product.name = item["name"].s();
                product.category = item["category"].s();
                product.price = item["price"].d();
                product.quantity = static_cast<int>(quantity);
                product.minStock = static_cast<int>(minStock);


                
                if (product.id < 0 || product.name.empty() ||
                    !isfinite(product.price) || product.price < 0 ||
                    product.quantity < 0 || product.minStock < 0) {
                    throw runtime_error("Invalid product values");
                }

                if (!usedIds.insert(product.id).second) {
                    throw runtime_error("Duplicate product ID");
                }
                products.push_back(product);
            }
        }
        catch (const exception& e) {
            throw runtime_error(
                string("Storage validation failed: ") + e.what()
            );
        }

        return products;
    }


};

#endif
