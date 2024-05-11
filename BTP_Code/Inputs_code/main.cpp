#include <iostream>
#include <fstream>
#include <sstream>
#include "Grid.cpp"

using namespace std;

int main() {
    int gridDimX, gridDimY, gridDimZ;
    int numBlocks;
    int numThreads;
    string filename;

    
    cout << "Enter grid dimensions (X Y Z): ";
    cin >> gridDimX >> gridDimY >> gridDimZ;

    cout << "Enter number of blocks: ";
    cin >> numBlocks;

    cout << "Enter number of threads per block: ";
    cin >> numThreads;

   
    cout << "Enter filename: ";
    cin >> filename;

    Grid grid(gridDimX, gridDimY, gridDimZ, numBlocks);

   
    ifstream inputFile(filename);

    if (inputFile.is_open()) {
        string line;
        int blockID = 0; // Initialize block ID

        // Read each line from the file
        while (getline(inputFile, line)) {
            stringstream ss(line);
            string instruction;
            char op;
            char comma;
            int values[3];

            // Parse the instruction from the line
            ss >> instruction >> op >> comma >> values[0] >> comma >> values[1] >> comma >> values[2];

            // Construct the instruction string
            string instructionString = instruction + " " + to_string(values[0]) + "," + to_string(values[1]) + "," + to_string(values[2]);

            // Set the instruction string to each thread in the current block
            for (int threadID = 0; threadID < numThreads; ++threadID) {
                grid.setThreadData(blockID, threadID, instructionString);
            }

            // Move to the next block if the current block is filled with threads
            if ((blockID + 1) % numBlocks == 0) {
                blockID = 0; // Reset block ID
            } else {
                ++blockID; // next block
            }
        }

        inputFile.close();
    } else {
        cout << "Unable to open file." << endl;
        return 1;
    }

for (int blockID = 0; blockID < numBlocks; ++blockID) {
    for (int threadID = 0; threadID < numThreads; ++threadID) {
        cout << "Block " << blockID << ", Thread " << threadID << ": " << grid.getThreadData(blockID, threadID) << endl;
    }
}

    return 0;
}
