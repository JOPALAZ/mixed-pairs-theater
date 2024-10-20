#include <iostream>
#include <random>
#include <ctime>
#include <thread>
#include <vector>
#include <algorithm>
#include <memory>
#include <mutex>
#include <condition_variable>

struct Params {
    int men;
    int women;
    int iterations;
    unsigned long long counter = 0;
    bool ready = false;
    std::string output;
    std::mutex mtx;
    std::condition_variable cv;
    Params(const Params&) = delete;
    Params& operator=(const Params&) = delete;
    Params(int m, int w, int iter) : men(m), women(w), iterations(iter) {}
};

int gcd(int a, int b) {
    if (a == 0)
        return b;
    if (b == 0)
        return a;
    if (a == b)
        return a;
    if (a > b)
        return gcd(a - b, b);
    return gcd(a, b - a);
}

int calculatePairs(const std::string& row) {
    int pairs = 0;
    for (size_t i = 0; i < row.size() - 1; ++i) {
        if (row[i] != row[i + 1]) {
            pairs++;
        }
    }
    return pairs;
}

int iterate(int men, int women, std::string& output) {
    std::string row(men, 'M');
    row.append(women, 'W');

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(row.begin(), row.end(), g);

    int pairs = calculatePairs(row);
    output += row + "\tPairs: " + std::to_string(pairs) + "\n";
    return pairs;
}

void threadFunction(std::shared_ptr<Params> params) {
    for (int i = 0; i < params->iterations; ++i) {
        params->counter += iterate(params->men, params->women, params->output);
    }
    {
        std::lock_guard<std::mutex> lock(params->mtx);
        params->ready = true;
    }
    params->cv.notify_one();
}

int main() {
    int men, women, iterations;
    std::string answer;
    std::vector<std::shared_ptr<Params>> paramVector;
    std::vector<std::thread> threadVector;
    unsigned long long totalCounter = 0;
    const int processor_count = std::thread::hardware_concurrency();

    std::cout << "Amount of men: ";
    while (!(std::cin >> men) || men < 0) {
        std::cout << "Please enter a valid number of men: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    std::cout << "Amount of women: ";
    while (!(std::cin >> women) || women < 0) {
        std::cout << "Please enter a valid number of women: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    std::cout << "Amount of iterations: ";
    while (!(std::cin >> iterations) || iterations <= 0) {
        std::cout << "Please enter a valid number of iterations: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    for (int i = 0; i < processor_count * 2; ++i) {
        int currentIterations = iterations / (processor_count * 2);
        if (i == processor_count * 2 - 1) {
            currentIterations += iterations % (processor_count * 2);
        }
        auto params = std::make_shared<Params>(men, women, currentIterations);
        paramVector.push_back(params);
        threadVector.emplace_back(std::thread(threadFunction, params));
    }

    for (auto& params : paramVector) {
        std::unique_lock<std::mutex> lock(params->mtx);
        params->cv.wait(lock, [&params]() { return params->ready; });
        totalCounter += params->counter;
    }

    for (auto& thread : threadVector) {
        if (thread.joinable()) {
            thread.join();
        }
    }

    std::cout << "Average pairs: " << totalCounter / static_cast<double>(iterations) << std::endl;
    std::cout << "Output all iterations? [y/N]: ";
    std::cin >> answer;
    if (!answer.empty() && tolower(answer[0]) == 'y') {
        for (const auto& params : paramVector) {
            std::cout << params->output;
        }
    }

    return 0;
}
