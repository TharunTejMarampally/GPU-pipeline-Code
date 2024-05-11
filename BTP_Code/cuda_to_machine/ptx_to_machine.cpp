#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include "hazards.cpp" 

// Class to represent each PTX instruction
class PTXInstruction {
public:
    std::string opcode;
    std::vector<std::string> operands;
    PTXInstruction(std::string op, std::vector<std::string> ops) : opcode(op), operands(ops) {}
};

//  parse PTX code from a file and return a vector of PTX instructions
std::vector<PTXInstruction> parsePTXFromFile(const std::string& filename) {
    std::ifstream file(filename);
    std::vector<PTXInstruction> instructions;
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string opcode;
            iss >> opcode;
            std::vector<std::string> operands;
            std::string operand;
            while (iss >> operand) {
                operands.push_back(operand);
            }
            instructions.push_back(PTXInstruction(opcode, operands));
        }
        file.close();
    } else {
        std::cerr << "Unable to open file: " << filename << std::endl;
    }
    return instructions;
}

// PTX instructions to machine code
// PTX instructions to machine code with opcodes
std::vector<std::string> translateToMachineCode(const std::vector<PTXInstruction>& ptxInstructions) {
    std::unordered_map<std::string, std::string> opcodeMap = {
        {"add", "0000"},
        {"sub", "0001"},
        {"mul", "0010"},
        {"div", "0011"}
        // Add more opcodes as needed
    };


    //hazards

    


enum HazardType {
    NO_HAZARD,
    DATA_HAZARD,
    CONTROL_HAZARD,
    STRUCTURAL_HAZARD
};

HazardType checkHazards(const std::vector<PTXInstruction>& ptxInstructions) {
    bool hasBranchDivergence = false;
    bool hasRAW = false;
    bool hasWAW = false;
    bool hasStructural = false;

    // Map to store memory locations written by previous instructions
    std::unordered_map<std::string, size_t> writeIndices;
    for (size_t i = 0; i < ptxInstructions.size(); ++i) {
        const PTXInstruction& currentInstr = ptxInstructions[i];
        // Branch Divergence (Control Hazard)
        if (currentInstr.opcode == "bra" || currentInstr.opcode == "bra.uni") {
            hasBranchDivergence = true;
           
            for (const std::string& operand : currentInstr.operands) {
                if (operand == "threadIdx.x") {
                    
                    break;
                }
            }
        }

        // Read-After-Write (RAW) and Write-After-Write (WAW) Hazards
        if (currentInstr.opcode == "ld") { // Assuming load operation
            std::string memoryLocation = currentInstr.operands[0]; 
            if (writeIndices.find(memoryLocation) != writeIndices.end()) {
                size_t writeIndex = writeIndices[memoryLocation];
                if (writeIndex > i) {
                    hasRAW = true;
                }
            }
        }
        // Write-After-Write (WAW) Hazard
        if (currentInstr.opcode == "st") { 
            std::string memoryLocation = currentInstr.operands[0]; 
            if (writeIndices.find(memoryLocation) != writeIndices.end()) {
                hasWAW = true;    
            }    
            writeIndices[memoryLocation] = i;
        }
        // Structural Hazard 
        if (currentInstr.opcode == "add" || currentInstr.opcode == "mul") {    
            hasStructural = true;
        }
    }

    if (hasBranchDivergence) {
        return CONTROL_HAZARD;
    } else if (hasRAW || hasWAW) {
        return DATA_HAZARD;
    } else if (hasStructural) {
        return STRUCTURAL_HAZARD;
    } else {
        return NO_HAZARD;
    }
}


    std::vector<std::string> machineCode;

    for (const auto& instr : ptxInstructions) {
        std::string machineInstr;
        // Check if the opcode is supported
        if (opcodeMap.find(instr.opcode) != opcodeMap.end()) {
            machineInstr = opcodeMap[instr.opcode] + " "; // Add the opcode
            // Concatenate the operands
            for (const auto& operand : instr.operands) {
                machineInstr += operand + " ";
            }
            // Remove the last space
            if (!machineInstr.empty()) {
                machineInstr.pop_back();
            }
        } else {
            machineInstr = "UNSUPPORTED";
        }
        machineCode.push_back(machineInstr);
    }

    return machineCode;
}

