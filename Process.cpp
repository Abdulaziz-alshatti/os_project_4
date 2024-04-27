#include "Process.h"
#include <iostream>
#include <sstream>
#include <iomanip>

Process::Process(int id, const std::string& filename, MemoryManager& memManager)
    : processID(id), inputFilename(filename), memoryManager(memManager) {
    pageTable.resize(256);  // Assuming 256 pages max for simplicity
}

void Process::run() {
    loadMemoryAccesses();
}

void Process::loadMemoryAccesses() {
    std::ifstream file(inputFilename);
    std::string line;

    if (!file) {
        std::cerr << "Failed to open process file: " << inputFilename << std::endl;
        return;
    }

    while (getline(file, line)) {
        performMemoryAccess(line);
    }
}

void Process::performMemoryAccess(const std::string& access) {
    std::istringstream iss(access);
    char op;
    std::string reg;
    unsigned int virtualAddr;

    if (!(iss >> op >> reg >> virtualAddr)) {
        std::cerr << "Invalid access format: " << access << std::endl;
        return;
    }

    unsigned int physicalAddr = translateAddress(virtualAddr, op == 'W');
    char value = memoryManager.readMemory(physicalAddr);

    std::cout << "P" << processID << " OPERATION: " << op << " " << reg << " 0x"
              << std::setw(8) << std::setfill('0') << std::hex << virtualAddr << std::endl;

    if (op == 'R') {
        std::cout << "P" << processID << ": " << reg << " = 0x"
                  << std::setw(8) << std::setfill('0') << std::hex << static_cast<unsigned int>(value)
                  << " (mem at virtual addr 0x"
                  << std::setw(8) << std::setfill('0') << std::hex << virtualAddr << ")" << std::endl;
    } else {
         
    }
}

unsigned int Process::translateAddress(unsigned int virtualAddress, bool isWrite) {
    unsigned int pageIndex = virtualAddress / memoryManager.getPageSize();
    unsigned int offset = virtualAddress % memoryManager.getPageSize();
    auto& entry = pageTable[pageIndex];

    if (!entry.first) {  // If the page is not resident
        int frameIndex = memoryManager.allocateFrame();
        if (frameIndex == -1) {
            std::cerr << "No free frames available." << std::endl;
            throw std::runtime_error("No free frames available.");
        }
        entry.first = true;
        entry.second = frameIndex;
        memoryManager.initializeFrame(frameIndex);  // Initialize the newly allocated frame
        std::cout << "Frame " << frameIndex << " initialized for page " << pageIndex << std::endl;
    }

    if (isWrite) {
         
    }

    return entry.second * memoryManager.getPageSize() + offset;
}
