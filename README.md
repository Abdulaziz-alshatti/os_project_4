Group Project 4: Memory Management Simulation

This project simulates various memory management techniques to demonstrate advanced concepts 
in operating systems, particularly focusing on paged memory management with multiple threads 
representing different processes. 
The simulation includes handling page table management, free frame allocation, 
page replacement using a clock algorithm, and synchronized access to shared resources. 
It aims to provide insights into how different memory management strategies affect 
process performance, memory utilization, and system efficiency.
Contributors

Abdullah Alshatti
Abdulaziz Alshatti

Compilation Instructions

To compile the project, a Makefile has been provided for convenience. 
Simply run the following command in the terminal within the project directory:
 

make

This command compiles all necessary source files and generates the executable mem_sim, 
which is used to run the memory management simulation.
Running the Program

After compilation, execute the program with the following command:
 
./mem_sim <input file> <output file>
 File Descriptions

main.cpp: Contains the main function and handles command-line arguments, file I/O.
MemoryManager.cpp: Implements the core memory management logic, including frame allocation, memory reading and writing.
MemoryManager.h: Header file for MemoryManager.cpp, declaring the functions and data structures used for managing memory.
Process.cpp: Represents a process in the simulation, handling memory accesses and address translation.
Process.h: Header file for Process.cpp, declaring the functions and data structures used by the process.
