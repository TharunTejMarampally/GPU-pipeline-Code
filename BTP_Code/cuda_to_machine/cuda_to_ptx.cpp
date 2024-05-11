#include <iostream>
#include <cstdlib>
#include <chrono>

int main() {
    
    auto start = std::chrono::high_resolution_clock::now();

    std::string cudaFile = "input.cu"; 
    std::string ptxFile = "output.ptx"; 

    
    std::string command = "nvcc -ptx " + cudaFile + " -o " + ptxFile;

    // Execute the command
    int result = system(command.c_str());

    // Check if compilation was successful
    if (result == 0) {
        std::cout << "PTX code generated successfully." << std::endl;
    } else {
        std::cerr << "Error generating PTX code." << std::endl;
    }

    
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "Execution time: " << duration.count() << " seconds." << std::endl;

    return 0;
}
