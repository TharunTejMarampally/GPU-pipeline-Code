
#include "ptx_to_machine.cpp"
// #include "hazards.cpp"


int main() {
    
    std::string cudaFile = "input.cu"; 
    std::string ptxFile = "output.ptx"; 
    std::string compileCommand = "nvcc -ptx " + cudaFile + " -o " + ptxFile;
    int compileResult = system(compileCommand.c_str());
    if (compileResult != 0) {
        std::cerr << "Error generating PTX code." << std::endl;
        return 1;
    }
    std::cout << "PTX code generated successfully." << std::endl;
    std::vector<PTXInstruction> ptxInstructions = parsePTXFromFile(ptxFile);

    // HazardType hazard = checkHazards(ptxInstructions);
    // Correct function signature
    HazardType checkHazards(const std::vector<PTXInstruction>& ptxInstructions)

    if (hazard == DATA_HAZARD) {
        std::cerr << "Data hazard detected!" << std::endl;
        
    } else if (hazard == CONTROL_HAZARD) {
        std::cerr << "Control hazard detected!" << std::endl;
      
          }

    //Translate to machine code
    std::vector<std::string> machineCode = translateToMachineCode(ptxInstructions);

    // machine code to file
    std::ofstream outputFile("machine.mc");
    if (outputFile.is_open()) {
        for (const auto& instr : machineCode) {
            outputFile << instr << std::endl;
        }
        outputFile.close();
        std::cout << "Machine code written to machine.txt" << std::endl;
    } else {
        std::cerr << "Unable to open output file." << std::endl;
        return 1;
    }

    return 0;
}