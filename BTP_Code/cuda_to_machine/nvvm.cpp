#include <nvvm.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <chrono>

// Function to compile PTX code to machine code
bool compilePTXtoMachineCode(const std::string& ptxFileName, std::vector<char>& machineCode) {
    // Initialize NVVM
    nvvmResult result = nvvmInit();
    if (result != NVVM_SUCCESS) {
        std::cerr << "NVVM initialization failed" << std::endl;
        return false;
    }

    // Read PTX file
    std::ifstream ptxFile(ptxFileName);
    if (!ptxFile.is_open()) {
        std::cerr << "Failed to open PTX file" << std::endl;
        return false;
    }
    std::stringstream buffer;
    buffer << ptxFile.rdbuf();
    std::string ptxCode = buffer.str();

    // Create NVVM program
    nvvmProgram prog;
    result = nvvmCreateProgram(&prog);
    if (result != NVVM_SUCCESS) {
        std::cerr << "Failed to create NVVM program" << std::endl;
        return false;
    }

    // Add PTX code to the NVVM program
    result = nvvmAddModuleToProgram(prog, ptxCode.c_str(), ptxCode.size(), "ptx");
    if (result != NVVM_SUCCESS) {
        std::cerr << "Failed to add PTX module to NVVM program" << std::endl;
        nvvmDestroyProgram(&prog);
        return false;
    }

    // Compile the NVVM program to machine code
    result = nvvmCompileProgram(prog, 0, NULL);
    if (result != NVVM_SUCCESS) {
        std::cerr << "Failed to compile NVVM program" << std::endl;
        nvvmDestroyProgram(&prog);
        return false;
    }

    // Get the machine code
    size_t codeSize;
    result = nvvmGetCompiledResultSize(prog, &codeSize);
    if (result != NVVM_SUCCESS) {
        std::cerr << "Failed to get compiled result size" << std::endl;
        nvvmDestroyProgram(&prog);
        return false;
    }

    machineCode.resize(codeSize);
    result = nvvmGetCompiledResult(prog, machineCode.data());
    if (result != NVVM_SUCCESS) {
        std::cerr << "Failed to get compiled result" << std::endl;
        nvvmDestroyProgram(&prog);
        return false;
    }

    // Destroy the NVVM program
    nvvmDestroyProgram(&prog);

    // Shutdown NVVM
    nvvmResult finalResult = nvvmShutdown();
    if (finalResult != NVVM_SUCCESS) {
        std::cerr << "NVVM shutdown failed" << std::endl;
        return false;
    }

    return true;
}

int main() {
    // Start timing
    auto start = std::chrono::high_resolution_clock::now();

    // Read PTX code from output.ptx
    std::ifstream ptxFile("output.ptx");
    if (!ptxFile.is_open()) {
        std::cerr << "Failed to open PTX file" << std::endl;
        return 1;
    }
    std::stringstream buffer;
    buffer << ptxFile.rdbuf();
    std::string ptxCode = buffer.str();

    // Compile PTX code to machine code
    std::vector<char> machineCode;
    if (compilePTXtoMachineCode("output.ptx", machineCode)) {
        // Write machine code to machine.txt
        std::ofstream machineFile("machine.txt", std::ios::binary);
        if (!machineFile.is_open()) {
            std::cerr << "Failed to open machine.txt" << std::endl;
            return 1;
        }
        machineFile.write(machineCode.data(), machineCode.size());
        machineFile.close();
        std::cout << "Machine code generated successfully and written to machine.txt." << std::endl;
    } else {
        std::cerr << "Failed to generate machine code." << std::endl;
        return 1;
    }

    // End timing
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "Execution time: " << duration.count() << " seconds." << std::endl;

    return 0;
}
