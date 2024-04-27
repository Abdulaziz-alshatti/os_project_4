#include "MemoryManager.h"
#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <ctime>

MemoryManager::MemoryManager(size_t memorySize, size_t pageSize)
    : totalMemorySize(memorySize), pageSize(pageSize), mainMemory(memorySize, 0), freeFrames(memorySize / pageSize, true) {
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
        std::cerr << "Error: Physical address out of bounds." << std::endl;
        return -1;
    }
    return mainMemory[physicalAddress];
}

void MemoryManager::writeMemory(size_t physicalAddress, char value) {
    if (physicalAddress >= totalMemorySize) {
        std::cerr << "Error: Physical address out of bounds." << std::endl;
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
        std::cerr << "Error: Invalid frame index." << std::endl;
    }
}

void MemoryManager::initializeFrame(int frameIndex) {
    std::srand(static_cast<unsigned int>(std::time(nullptr) + frameIndex)); // Seed based on time and frame index
    int start = frameIndex * pageSize;
    for (int i = 0; i < pageSize; i++) {
        mainMemory[start + i] = std::rand() % 256;
        
    }
}


size_t MemoryManager::getFreeFrameCount() const {
    return std::count(freeFrames.begin(), freeFrames.end(), true);
}
