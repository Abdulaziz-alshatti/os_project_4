#include "MemoryManager.h"
#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
MemoryManager::MemoryManager(size_t memorySize, size_t pageSize, std::ofstream& outFile)
: totalMemorySize(memorySize), pageSize(pageSize), mainMemory(memorySize, 0), freeFrames(memorySize / pageSize, true), outputStream(outFile) {
    initializeMemory();
}

MemoryManager::~MemoryManager() {}

void MemoryManager::initializeMemory() {
    std::fill(mainMemory.begin(), mainMemory.end(), 0);
}

void MemoryManager::initializeFreeFrames() {
    std::fill(freeFrames.begin(), freeFrames.end(), true);
}

char MemoryManager::readMemory(size_t physicalAddress) {
    if (physicalAddress >= totalMemorySize) {
        outputStream << "Error: Physical address out of bounds." << std::endl;
        return -1;  // Make sure to handle error appropriately
    }
    return mainMemory[physicalAddress];
}

void MemoryManager::writeMemory(size_t physicalAddress, char value) {
    if (physicalAddress >= totalMemorySize) {
        outputStream << "Error: Physical address out of bounds." << std::endl;
        return;
    }
    mainMemory[physicalAddress] = value;
}

int MemoryManager::allocateFrame() {
    for (size_t i = 0; i < freeFrames.size(); i++) {
        if (freeFrames[i]) {
            freeFrames[i] = false;
             
            return i;
        }
    }
    return -1;
}

void MemoryManager::freeFrame(int frameIndex) {
    if (frameIndex >= 0 && frameIndex < static_cast<int>(freeFrames.size())) {
        freeFrames[frameIndex] = true;
    } else {
        outputStream << "Error: Invalid frame index." << std::endl;
    }
}

void MemoryManager::initializeFrame(int frameIndex) {
    std::srand (time(NULL));
    int start = frameIndex * pageSize;
    for (int i = 0; i < pageSize; i++) {
        mainMemory[start + i] = std::rand() ;  // Randomly initialize the frame
    }
     
}

size_t MemoryManager::getFreeFrameCount() const {
    return std::count(freeFrames.begin(), freeFrames.end(), true);
}
