#include "thread.cpp"

class Thread_Block : public CustomThread {
public:
    vector<CustomThread> tb_data;
    int tb_id;
    int tb_dimx, tb_dimy, tb_dimz;
    int numThreads; 

   Thread_Block() : tb_id(0), tb_dimx(0), tb_dimy(0), tb_dimz(0), numThreads(0) {}

    Thread_Block(int tb_id, int numThreads, int tb_dimx, int tb_dimy, int tb_dimz)
    : tb_id(tb_id), tb_dimx(tb_dimx), tb_dimy(tb_dimy), tb_dimz(tb_dimz), numThreads(numThreads) {
    tb_data.resize(numThreads); // Resize tb_data vector to the specified number of threads per block
    for (int i = 0; i < numThreads; ++i) {
        tb_data.emplace_back(tb_dimx, tb_dimy, tb_dimz);
    }
    }


    void set_tb_id(int tb_id) {
        this->tb_id = tb_id;
    }

    int get_tb_id() const {
        return tb_id;
    }

    int get_size() const {
        return tb_data.size();
    }

    void set_tb_data(int threadID, const std::string& data) {
        if (threadID >= 0 && threadID < tb_data.size()) {
            tb_data[threadID].set_data(data);
        }
    }

    void set_tb_ID(int threadID, int t_id) {
        if (threadID >= 0 && threadID < tb_data.size()) {
            tb_data[threadID].set_ID(t_id);
        }
    }

    tuple<int, int, int> get_block_dim() const {
        return make_tuple(tb_dimx, tb_dimy, tb_dimz);
    }

    string get_t_data(int threadID) const {
        if (threadID >= 0 && threadID < tb_data.size()) {
            return tb_data[threadID].t_data; // Directly access t_data
        }
        return "";
    }

    int get_t_id(int threadID) const {
        if (threadID >= 0 && threadID < tb_data.size()) {
            return tb_data[threadID].get_ID();
        }
        return -1;
    }
};
