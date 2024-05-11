#ifndef CUSTOM_THREAD_H
#define CUSTOM_THREAD_H
#include<bits/stdc++.h>
using namespace std;

class CustomThread{
    public:
    string t_data;
    unsigned t_id;
    int t_dimx;
    int t_dimy;
    int t_dimz;
   
    CustomThread() : t_dimx(0), t_dimy(0), t_dimz(0), t_id(0) {
    }
    
    CustomThread(int t_dimx,int t_dimy,int t_dimz){
        this->t_dimx=t_dimx;
        this->t_dimy=t_dimy;
        this->t_dimz=t_dimz;
    }
    void set_data(const string& t_data){
     this->t_data=t_data;
    }
    void set_ID(unsigned int t_id){
        this->t_id=t_id;
    }
    unsigned get_ID() const { 
        return t_id; 
    }
   tuple<int, int, int> t_dim() const {
        return make_tuple(t_dimx, t_dimy, t_dimz);
    }
};
#endif