#ifndef GPB_H
#define GPB_H

#include "Gpu.h"

// header file for GPB
class Gpb {
   private:
    // set of pointers to the previous GPBs of the current GPB
    std::set<Gpb *> prevGpb_;

    // set of pointers to the Next GPBs of the current GPB
    std::set<Gpb *> nextGpb_;

    // Set of GPUs in this GPB
    std::set<GpuIDType> gpuSet_;

    // In GpuFlow of this GPB
    std::set<GpuIDType> in_;

    // Out GpuFlow of this GPB
    std::set<GpuIDType> out_;

   public:
    // Add pointer to PrevGpg
    void addPrevGpb(Gpb *);

    // Get set of previous gpbs
    std::set<Gpb *> &getPrevGpbSet();

    // Add pointer to NextGpg
    void addNextGpb(Gpb *);

    // Get set of next gpbs
    std::set<Gpb *> &getNextGpbSet();

    // Add element to the Set of GPUs in this GPB
    void insertGpuSet(const GpuIDType &);

    // Get set of GPUs in this GPB
    std::set<GpuIDType> &getGpuSet();

    // to check whether this GPB is empty or not
    bool isEmpty();

    // Remove the GPB if empty by connecting its prev to next GPBs
    void removeEmptyGpb();

    // Insert into In
    void insertIn(const GpuIDType &);

    // Get In Set
    std::set<GpuIDType> &getIn();

    // Insert into Out
    void insertOut(const GpuIDType &);

    // Get Out Set
    std::set<GpuIDType> &getOut();

    // reyurns true if it is exit block
    bool isExitBlock() {
        return getNextGpbSet().empty();
    }

    // returns true if it is entry block
    bool isEntryBlock() {
        return getPrevGpbSet().empty();
    }

    // This function will return a set of pair (DestinationId, Destinationlist),
    // where finds a match to the SourceId and SourceList
    std::set<std::pair<NodeIndex, IndirectionList *>> matchSource(
        NodeIndex, IndirectionList *);

    // This function will return a set of pair (SourceId, Sourcelist), where
    // finds
    // a match to the DestinationId and DestinationList
    std::set<std::pair<NodeIndex, IndirectionList *>> matchDestination(
        NodeIndex, IndirectionList *);

    // This function will return a set of pair (DestinationId ,
    // DestinationList),
    // where finds a match to the SourceId and SourceList to help
    std::set<std::pair<NodeIndex, IndirectionList *>> match(NodeIndex,
                                                            IndirectionList *);
};

#endif
