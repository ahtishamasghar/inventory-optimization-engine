
#ifndef MERGE_SORT_HPP
#define MERGE_SORT_HPP

#include "../models/product.hpp"
#include <vector>
using namespace std;

class MergeSort {
private:
    static void merge(vector<Product>& arr, int left, int mid, int right) {
        vector<Product> temp;

        int i = left;
        int j = mid + 1;

        while (i <= mid && j <= right) {
            if (arr[i].price <= arr[j].price) {
                temp.push_back(arr[i]);
                i++;
            } else {
                temp.push_back(arr[j]);
                j++;
            }
        }

        while (i <= mid) {
            temp.push_back(arr[i]);
            i++;
        }

        while (j <= right) {
            temp.push_back(arr[j]);
            j++;
        }

        for (int k = 0; k < (int)temp.size(); k++) {
            arr[left + k] = temp[k];
        }
    }

    static void mergeSort(vector<Product>& arr, int left, int right) {
        if (left >= right)
            return;

        int mid = left + (right - left) / 2;

        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);

        merge(arr, left, mid, right);
    }

public:
    static void sortByPrice(vector<Product>& arr) {
        if (arr.empty())
            return;

        mergeSort(arr, 0, (int)arr.size() - 1);
    }
};

#endif
