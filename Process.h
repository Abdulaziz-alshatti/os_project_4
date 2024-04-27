#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <fstream>
#include <vector>
#include "MemoryManager.h"
#include <mutex>
class Process {
public:
    Process(int id, const std::string& filename, MemoryManager& memManager, std::ofstream& outputStream,std::mutex& outFileMutex);
    void run();
    void logOutput(const std::string& message) {
        std::lock_guard<std::mutex> lock(outFileMutex);
        outputStream << message;
    }

private:
    int processID;
    std::mutex& outFileMutex;
    std::string inputFilename;
    MemoryManager& memoryManager;
    std::ofstream& outputStream;  // Reference to the output stream for file output
    std::vector<std::pair<char, unsigned int>> pageTable;
    int virtualMemorySize;
    void loadMemoryAccesses();
    void performMemoryAccess(const std::string& access);
    unsigned int translateAddress(unsigned int virtualAddress, bool isWrite);
};

#endif // PROCESS_H
