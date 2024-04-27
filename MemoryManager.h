#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include <vector>
#include <cstddef> // for size_t

class MemoryManager {
public:
    MemoryManager(size_t memorySize, size_t pageSize);
    ~MemoryManager();

    void initializeMemory();
    char readMemory(size_t physicalAddress);
    void writeMemory(size_t physicalAddress, char value);

    int allocateFrame();
    void freeFrame(int frameIndex);
    void initializeFrame(int frameIndex);

    size_t getFreeFrameCount() const;
    size_t getPageSize() const { return pageSize; }

private:
    std::vector<char> mainMemory;
    std::vector<bool> freeFrames;
    size_t totalMemorySize;
    size_t pageSize;

    void initializeFreeFrames();
};

#endif // MEMORY_MANAGER_H
