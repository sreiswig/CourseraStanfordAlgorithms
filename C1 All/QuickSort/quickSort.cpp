#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>

int PickMedian(std::vector<int> choices) {
    std::sort(choices.begin(), choices.end());
    return choices.at(1);
}

int Partition(int pivotIndex, std::vector<int> &array, int leftBoundary, int rightBoundary) {
    if (pivotIndex != leftBoundary) {
        // swap pivot element and first element
        int temp = array.at(leftBoundary);
        array.at(leftBoundary) = array.at(pivotIndex);
        array.at(pivotIndex) = temp;
    }

    int pivotElement = array.at(leftBoundary);

    int i = leftBoundary + 1;
    for (int j = i; j <= rightBoundary; j++) {
        if(array.at(j) < pivotElement) {
            // swap new element with leftmost element bigger than pivot
            int temp = array.at(i);
            array.at(i) = array.at(j);
            array.at(j) = temp;
            i++;
        } 
    }

    // swap pivot element and a[i-1]
    array.at(leftBoundary) = array.at(i-1);
    array.at(i-1) = pivotElement;

    return i-1;
}

int QuickSort(std::vector<int> &array, int left, int right) {
    if(left > right) return 0;
    if(right - left == 0) { return 0; }

    int count = (right + 1) - (left + 1);
    if(right == array.size()-1) {count = right - left;}
    int k;
    k = (left + ((right+1) - (left+1))/2);

    int leftVal = array.at(left);
    int rightVal = array.at(right);
    int middleVal = array.at(k);

    std::vector<int> choices = {array.at(left), array.at(right), array.at(k)};

    int partition = PickMedian(choices);
    int medianVal = partition;

    if (partition == array.at(left)) {
        partition = left;
    } else if(partition == array.at(right)) {
        partition = right;
    } else {
        partition = k;
    }

    int partitionIndex = Partition(partition, array, left, right);
    count += QuickSort(array, left, partitionIndex - 1);
    count += QuickSort(array, partitionIndex + 1, right);
    return count;
}

int main() {
    // load values into vector int
    std::string line;
    std::ifstream myfile ("QuickSort.txt");
    std::vector<int> nums;
    
    if (myfile.is_open()) {
        while (std::getline(myfile, line)) {
            nums.push_back(stoi(line));
        }
        myfile.close(); 
    }
    std::vector<int> testArray = {2, 20, 1, 15, 3, 11, 13, 6, 16, 10, 19, 5, 4, 9, 8, 14, 18, 17, 7, 12};
    //std::cout << QuickSort(testArray, 0, testArray.size()-1) << "\n";
    std::cout << QuickSort(nums, 0, nums.size()-1) << "\n";
    for (auto num : nums) {
        std::cout << num << " ";
    }
    std::cout << "\n";
    return 0;
}