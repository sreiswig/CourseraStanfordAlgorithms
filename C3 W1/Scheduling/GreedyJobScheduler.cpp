#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

struct Job {
    long int weight;
    long int length;

    Job(long int weight, long int length) {
        this->weight = weight;
        this->length = length;
    }
};

// Custom Sort Operator for sorting by difference
struct weight_length_difference_comparator {
    inline bool operator() (const Job& lhs, const Job& rhs) {
        if (lhs.weight - lhs.length == rhs.weight - rhs.length) {
            return lhs.weight > rhs.weight;
        }
        return lhs.weight - lhs.length > rhs.weight - rhs.length;
    }
};

// Custom Sort Operator for sorting by difference
struct weight_length_ratio_comparator {
    inline bool operator() (const Job& lhs, const Job& rhs) {
        return (double)lhs.weight / lhs.length > (double)rhs.weight / rhs.length;
    }
};

class Scheduler {
    private:
        std::vector<Job> jobs;
    public:
        void addJob(Job j) {
            jobs.push_back(j);
        }

        // returns the sum of weighted completion times
        long long int scheduleByWeightLengthDifference() {
            long long int sum = 0;
            long long int time = 0;
            std::sort(jobs.begin(), jobs.end(), weight_length_difference_comparator());
            for (auto job : jobs) {
                time += job.length;
                sum += time * job.weight;
            }
            return sum;
        }

        long long int scheduleByWeightLengthRatio() {
            long long int sum = 0;
            long long int time = 0;
            std::sort(jobs.begin(), jobs.end(), weight_length_ratio_comparator());
            for (auto job : jobs) {
                time += job.length;
                sum += time * job.weight;
            }
            return sum;
        }
};

int main() {
    std::string filename;
    std::cout << "Enter File Name: ";
    std::cin >> filename;
    
    std::ifstream file(filename);
    std::string line;
    Scheduler scheduler;

    long long int numberOfJobs;
    std::getline(file, line);
    numberOfJobs = std::stoll(line);

    // read from file and parse into jobs    
    while (std::getline(file, line)) {
        std::string delimiter = " ";
        std::string weightString = line.substr(0, line.find(delimiter));
        line.erase(0, line.find(delimiter) + 1);
        std::string lengthString = line.substr(0, line.find(delimiter));
        scheduler.addJob(Job(std::stol(weightString), std::stol(lengthString)));
    }

    std::cout << "Sum of weighted Completion Times, scheduled by Weight Length Difference: " << scheduler.scheduleByWeightLengthDifference() << "\n";
    std::cout << "Sum of weighted Completion Times, scheduled by Weight over Lenght Ratio: " << scheduler.scheduleByWeightLengthRatio() << "\n";
}