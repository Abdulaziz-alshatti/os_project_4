#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <thread>
#include "MemoryManager.h"
#include "Process.h"
#include <mutex>
using namespace std;

int main(int argc, char** argv) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <input_file> <output_file> [seed]" << std::endl;
        return 1;
    }

    std::string inputFile = argv[1];
    std::string outputFile = argv[2];
    std::ofstream outFile(outputFile);
    if (!outFile) {
        std::cerr << "Error opening output file: " << outputFile << std::endl;
        return 1;
    }

    std::ifstream file(inputFile);
    if (!file) {
        outFile << "Error opening input file: " << inputFile << std::endl;
        std::cerr << "Error opening input file: " << inputFile << std::endl;
        return 1;
    }

    size_t mainMemorySize, pageSize;
    int processCount;
    file >> mainMemorySize >> pageSize >> processCount;
    if (!file) {
        outFile << "Error reading configuration from input file" << std::endl;
        std::cerr << "Error reading configuration from input file" << std::endl;
        return 1;
    }

    std::vector<std::string> threadFiles(processCount);
    for (int i = 0; i < processCount; ++i) {
        if (!(file >> threadFiles[i])) {
            outFile << "Error reading thread file name for process " << i << std::endl;
            std::cerr << "Error reading thread file name for process " << i << std::endl;
            return 1;
        }
    }

    file.close();

    MemoryManager memoryManager(mainMemorySize, pageSize, outFile);
    cout << "Main Memory Size: " << mainMemorySize << " bytes\n"
            << "Page Size: " << pageSize << " bytes\n"
            << "Number of Processes: " << processCount << "\n";
    for (const auto& fileName : threadFiles) {
        cout << "Thread File: " << fileName << "\n";
    }
     
   std::mutex outFileMutex;
    std::vector<std::thread> threads;
    for (int i = 0; i < processCount; i++) {
    threads.emplace_back([&memoryManager, &threadFiles, &outFile, &outFileMutex, i]() {
        Process process(i, threadFiles[i], memoryManager, outFile, outFileMutex);
        process.run();
    });
}



    for (auto& thread : threads) {
        thread.join();
    }

    outFile << "Simulation completed.\n";
    std::cout << "Simulation completed." << std::endl;
    outFile.close();
    return 0;
}
