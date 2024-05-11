#include <iostream>
#include <vector>
#include <queue>

using namespace std;

class Wrap {
public:
    vector<vector<string>> threads; 
    Wrap(int numThreadsPerWrap) {
        threads.resize(numThreadsPerWrap, vector<string>());
    }
};

queue<Wrap> createWraps(const vector<vector<vector<string>>>& gridThreads, int numThreadsPerWrap) {
    queue<Wrap> wraps;
    int threadsInWrap = 0;
    Wrap currentWrap(numThreadsPerWrap);

    for (const auto& block : gridThreads) {
        for (const auto& thread : block) {
            for (const auto& instruction : thread) {
                currentWrap.threads[threadsInWrap].push_back(instruction);
                threadsInWrap++;
                if (threadsInWrap >= numThreadsPerWrap) {
                    wraps.push(currentWrap);
                    currentWrap = Wrap(numThreadsPerWrap);
                    threadsInWrap = 0;
                }
            }
        }
    }

    // If there are remaining threads, fill the last wrap with empty threads
    if (threadsInWrap > 0) {
        while (threadsInWrap < numThreadsPerWrap) {
            currentWrap.threads[threadsInWrap].push_back(""); // Empty thread
            threadsInWrap++;
        }
        wraps.push(currentWrap);
    }

    return wraps;
}

