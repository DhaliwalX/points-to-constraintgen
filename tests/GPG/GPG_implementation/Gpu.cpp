#include<iostream>
#include "Gpu.h"
using namespace std;

//constructor defination
Gpu::Gpu(NodeIndex arg1, NodeIndex arg2, IndirectionList *arg3, IndirectionList *arg4, std::pair<int, int> &val)
{
        sourceId_ = arg1;
        destinationId_ = arg2;
        sourceList_ = arg3;
        destinationList_ = arg4;
        id_ = val;
}
        
//GPU comparator function defination 
bool Gpu::equals(Gpu& gpu)
{
        return id_ = gpu.id_;
}

bool Gpu :: operator<(Gpu)
{
        // just to enable set operations
        return id_ < gpu.id_;
}
        
