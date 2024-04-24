#include <iostream>
#include <algorithm>
#include <numeric>
#include <vector>
#include <chrono>
#include <random>
#include <limits>

template <typename T>
class Optional {
private:
    bool hasValue;
    T value;

public:
    Optional() : hasValue(false), value() {}
    Optional(const T& val) : hasValue(true), value(val) {}

    bool has_value() const { return hasValue; }
    T get_value_or(const T& defaultValue) const { return hasValue ? value : defaultValue; }
};

void parallelReduction(const std::vector<int>& array) {
    int min = std::accumulate(array.begin(), array.end(), std::numeric_limits<int>::max(), [](int a, int b) { return std::min(a, b); });
    int max = std::accumulate(array.begin(), array.end(), std::numeric_limits<int>::min(), [](int a, int b) { return std::max(a, b); });
    long long sum = std::accumulate(array.begin(), array.end(), 0LL);
    Optional<double> average = array.empty() ? Optional<double>() : Optional<double>(static_cast<double>(sum) / array.size());

    std::cout << "Parallel Min: " << min << std::endl;
    std::cout << "Parallel Max: " << max << std::endl;
    std::cout << "Parallel Sum: " << sum << std::endl;
    std::cout << "Parallel Average: " << (average.has_value() ? std::to_string(average.get_value_or(0.0)) : "NaN") << std::endl;
}

void sequentialReduction(const std::vector<int>& array) {
    auto minMax = std::minmax_element(array.begin(), array.end());
    int min = minMax.first == array.end() ? std::numeric_limits<int>::max() : *minMax.first;
    int max = minMax.second == array.end() ? std::numeric_limits<int>::min() : *minMax.second;
    long long sum = std::accumulate(array.begin(), array.end(), 0LL);
    Optional<double> average = array.empty() ? Optional<double>() : Optional<double>(static_cast<double>(sum) / array.size());

    std::cout << "Sequential Min: " << min << std::endl;
    std::cout << "Sequential Max: " << max << std::endl;
    std::cout << "Sequential Sum: " << sum << std::endl;
    std::cout << "Sequential Average: " << (average.has_value() ? std::to_string(average.get_value_or(0.0)) : "NaN") << std::endl;
}

int main() {
    // Generate a random array
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(0, 9999);
    std::vector<int> array(5000000);
    for (int& num : array) {
        num = dis(gen);
    }

    // Perform parallel reduction and measure time
    auto startTime = std::chrono::high_resolution_clock::now();
    parallelReduction(array);
    auto parallelTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - startTime).count();
    std::cout << "Parallel Time: " << parallelTime << "ms" << std::endl;

    // Perform sequential reduction and measure time
    startTime = std::chrono::high_resolution_clock::now();
    sequentialReduction(array);
    auto sequentialTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - startTime).count();
    std::cout << "Sequential Time: " << sequentialTime << "ms" << std::endl;

    return 0;
}
