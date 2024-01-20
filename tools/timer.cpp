#include <ctime>
#include <iostream>
#include <string>
#include <vector>

/**
 * 借助RAII机制的算法计时器
 */
class Timer {
private:
    std::string  hint;
    std::clock_t start = 0;

public:
    Timer(const std::string& str = "") : hint(str) {
        start = std::clock();
    }

    // 可以自定义输出方式
    ~Timer() {
        std::clock_t end = std::clock();
        std::cout << hint;
        std::cout << (end - start) << std::endl;
    }
};

void bubbleSort(std::vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n - 1 - i; j++) {
            if (arr[j] > arr[j + 1]) {
                std::swap(arr[j], arr[j + 1]);
            }
        }
    }
}

void quickSort(std::vector<int>& arr, const int start, const int end) {
    if (start >= end) {
        return;
    }

    int left  = start;
    int right = end;

    int pivotIdx = rand() % (right - left + 1) + left;
    std::swap(arr[pivotIdx], arr[left]);

    int pivot = arr[left];
    while (left < right) {
        while (left < right && arr[right] >= pivot) {
            right--;
        }
        arr[left] = arr[right];
        while (left < right && arr[left] <= pivot) {
            left++;
        }
        arr[right] = arr[left];
    }
    arr[left] = pivot;

    pivotIdx = left;
    quickSort(arr, start, pivotIdx - 1);
    quickSort(arr, pivotIdx + 1, end);
}

int main() {
    std::vector<int> arr;
    for (int i = 0; i < 50000; i += 1) {
        arr.push_back(rand() % 1000);
    }

    // 测试冒泡排序时间
    {
        auto  nums = arr;
        Timer timer("bubbleSort use ");
        bubbleSort(nums);
    }

    // 测试快速排序时间
    {
        auto  nums = arr;
        Timer timer("quickSort use ");
        quickSort(nums, 0, nums.size() - 1);
    }

    return 0;
}