#include "Process.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <mutex>
Process::Process(int id, const std::string& filename, MemoryManager& memManager, std::ofstream& outFile,std::mutex& mutex)
    : processID(id), inputFilename(filename), memoryManager(memManager), outputStream(outFile),outFileMutex(mutex) {
    pageTable.resize(256);  // Assuming 256 pages max for simplicity
}

void Process::run() {
    loadMemoryAccesses();

}

void Process::loadMemoryAccesses() {
    std::ifstream file(inputFilename);
    std::string line;
    

    if (!file) {
        outputStream << "Failed to open process file: " << inputFilename << std::endl;
        return;
    }
    
     outputStream << "Process " << processID << "  Started" << std::endl;
    if (getline(file, line)) {
        std::istringstream iss(line);
        if (!(iss >> virtualMemorySize)) {
            outputStream << "Failed to read virtual memory size from process file: " << inputFilename << std::endl;
            return;
        }
         
    } else {
        outputStream << "Empty process file or unable to read virtual memory size: " << inputFilename << std::endl;
        return;
    }

    
    while (getline(file, line)) {
        if (!line.empty()) {
            performMemoryAccess(line);
        }
    }
}

unsigned int Process::translateAddress(unsigned int virtualAddress, bool isWrite) {
    unsigned int pageIndex = virtualAddress / memoryManager.getPageSize();
    unsigned int offset = virtualAddress % memoryManager.getPageSize();
    auto& entry = pageTable[pageIndex];
    std::ostringstream message;

    if (!entry.first) {  // If the page is not resident
        message << "P" << processID << ": page " << pageIndex << " not resident in memory\n";
        int frameIndex = memoryManager.allocateFrame();
        if (frameIndex == -1) {
            message << "No free frames available.\n";
            logOutput(message.str());
            throw std::runtime_error("No free frames available.");
        }
        entry.first = true;
        entry.second = frameIndex;
        memoryManager.initializeFrame(frameIndex);  // Initialize the newly allocated frame
        message << "P" << processID << ": using free frame " << frameIndex << "\n";
        message << "P" << processID << ": new translation from page " << pageIndex << " to frame " << frameIndex << "\n";
        logOutput(message.str());
    } else {
        message << "P" << processID << ": valid translation from page " << pageIndex << " to frame " << entry.second << "\n";
        logOutput(message.str());
    }

    unsigned int physicalAddress = entry.second * memoryManager.getPageSize() + offset;
    message << "P" << processID << ": translated VA 0x" 
            << std::setw(8) << std::setfill('0') << std::hex << virtualAddress
            << " to PA 0x" 
            << std::setw(8) << std::setfill('0') << std::hex << physicalAddress << "\n";
    logOutput(message.str());
    if (isWrite) {
        message << "P" << processID << ": write flag set for page " << pageIndex << "\n";
        logOutput(message.str()); 
    }

    
    return physicalAddress;
}


void Process::performMemoryAccess(const std::string& access) {
    std::istringstream iss(access);
    char op;
    std::string reg;
    unsigned int virtualAddr;
    std::ostringstream message;

    if (!(iss >> op >> reg >> virtualAddr)) {
        message << "Invalid access format: " << access << "\n";
        logOutput(message.str());
        return;
    }
    
    message << "P" << processID << " OPERATION: " << op << " " << reg << " 0x"
            << std::setw(8) << std::setfill('0') << std::hex << virtualAddr << "\n";
    logOutput(message.str());
    unsigned int physicalAddr = translateAddress(virtualAddr, op == 'W');
    if (op == 'R') {
        char value = memoryManager.readMemory(physicalAddr);
        message << "P" << processID << ": " << reg << " = 0x"
                << std::setw(8) << std::setfill('0') << std::hex << static_cast<unsigned int>(value)
                << " (mem at virtual addr 0x"
                << std::setw(8) << std::setfill('0') << std::hex << virtualAddr << ")\n";
                logOutput(message.str()); 
    } else if (op == 'W') {
        char valueToWrite = static_cast<char>(std::stoi(reg.substr(1), nullptr, 16));  // Example: extracting value from register notation
        memoryManager.writeMemory(physicalAddr, valueToWrite);
        message << "P" << processID << ": wrote 0x"
                << std::setw(8) << std::setfill('0') << std::hex << static_cast<unsigned int>(valueToWrite)
                << " from " << reg << " to virtual addr 0x"
                << std::setw(8) << std::setfill('0') << std::hex << virtualAddr << "\n";
                logOutput(message.str()); 
    }

     
}




