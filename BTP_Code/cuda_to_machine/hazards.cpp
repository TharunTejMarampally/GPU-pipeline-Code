


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
        if (currentInstr.opcode == "add" || currentInstr.opcode == "mul") { // Assuming ALU operations
            
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

