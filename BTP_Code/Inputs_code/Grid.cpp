#include<bits/stdc++.h>
#include "thread.cpp"
#include "thread_block.cpp"

class Grid {
    vector<Thread_Block> blocks;
    int gridDimX,gridDimY,gridDimZ;
    int numBlocks;
public:
   
    Grid(int gridDimX, int gridDimY, int gridDimZ, int numBlocks) 
        : gridDimX(gridDimX), gridDimY(gridDimY), gridDimZ(gridDimZ) {
        blocks.resize(numBlocks);  // Resize the blocks vector to the specified number of blocks
    }
    
   
    void addBlock(const Thread_Block& block) {
        blocks.push_back(block);
    }

    // Method to set data for a specific thread in a specific block
    void setThreadData(int blockID, int threadID, const string& data) {
        if (blockID >= 0 && blockID < blocks.size()) {
            blocks[blockID].set_tb_data(threadID, data);
        }
    }

    // Method to set ID for a specific thread in a specific block
    void setThreadID(int blockID, int threadID, int t_id) {
        if (blockID >= 0 && blockID < blocks.size()) {
            blocks[blockID].set_tb_ID(threadID, t_id);
        }
    }
    
    // Method to get block dimensions of a specific block
    tuple<int, int, int> getBlockDim(int blockID) const {
        if (blockID >= 0 && blockID < blocks.size()) {
            return blocks[blockID].get_block_dim();
        }
        return std::make_tuple(-1, -1, -1); // Return invalid dimensions if block ID is out of range
    }

    // Method to get thread data of a specific thread in a specific block
    std::string getThreadData(int blockID, int threadID) const {
        if (blockID >= 0 && blockID < blocks.size()) {
            return blocks[blockID].get_t_data(threadID);
        }
        return ""; // Return empty string if block ID is out of range
    }

    // Method to get thread ID of a specific thread in a specific block
    int getThreadID(int blockID, int threadID) const {
        if (blockID >= 0 && blockID < blocks.size()) {
            return blocks[blockID].get_t_id(threadID);
        }
        return -1; // Return -1 if block ID is out of range
    }

    // Method to calculate total threads across all blocks
    int getTotalThreads() const {
        int totalThreads = 0;
        for (const auto& block : blocks) {
            totalThreads += block.get_size();
        }
        return totalThreads;
    }
};
