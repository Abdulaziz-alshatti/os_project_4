#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <thread>
#include "MemoryManager.h"
#include "Process.h"

int main(int argc, char** argv) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <input_file> <output_file> [seed]" << std::endl;
        return 1;
    }

    std::string inputFile = argv[1];
    std::string outputFile = argv[2];
    int seed = (argc > 3) ? std::stoi(argv[3]) : -1;

    std::ifstream file(inputFile);
    if (!file) {
        std::cerr << "Error opening input file: " << inputFile << std::endl;
        return 1;
    }

    size_t mainMemorySize, pageSize;
    int processCount;
    file >> mainMemorySize >> pageSize >> processCount;

    if (!file) {
        std::cerr << "Error reading configuration from input file" << std::endl;
        return 1;
    }

    std::vector<std::string> threadFiles(processCount);
    for (int i = 0; i < processCount; ++i) {
        if (!(file >> threadFiles[i])) {
            std::cerr << "Error reading thread file name for process " << i << std::endl;
            return 1;
        }
    }

    file.close();

    MemoryManager memoryManager(mainMemorySize, pageSize);

    std::cout << "Main Memory Size: " << mainMemorySize << " bytes" << std::endl;
    std::cout << "Page Size: " << pageSize << " bytes" << std::endl;
    std::cout << "Number of Processes: " << processCount << std::endl;
    for (const auto& fileName : threadFiles) {
        std::cout << "Thread File: " << fileName << std::endl;
    }

    std::vector<std::thread> threads;
    for (int i = 0; i < processCount; i++) {
        threads.emplace_back([&memoryManager, &threadFiles, i]() {
            Process process(i, threadFiles[i], memoryManager);
            process.run();
        });
    }

    for (auto& thread : threads) {
        thread.join();
    }

    std::cout << "Simulation completed." << std::endl;
    return 0;
}
