#ifndef GPU_H
#define GPU_H


#include "IndirectionList.h"

using GpuIDType = std::pair<int, int>;

//header file for GPU
class Gpu {
    private :

        //considering type of NodeId: NodeIndex
        NodeIndex sourceId_;
        NodeIndex destinationId_;

        //considering IndirectionListId: IndirectionList
        IndirectionList *sourceList_;
        IndirectionList *destinationList_;

        //SequenceNo will be unique for each GPU
        std::pair<int, int> id_;
   
    public :
        Gpu(NodeIndex arg1, NodeIndex arg2, IndirectionList arg3, IndirectionList arg4);
        //Constructor

        bool equals(Gpu &);
        //GPU comparator function

        bool operator<(const Gpu&)
        //declaration of operator<() 
};

#endif
