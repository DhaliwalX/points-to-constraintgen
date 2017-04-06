#include "Gpb.h"
#include <algorithm>
#include <iostream>
#include <set>
#include "Gpu.h"
#include "GpuFlow.h"

// Add pointer to PrevGpg
void Gpb::addPrevGpb(Gpb* p) {
    prevGpb_.insert(p);
}

std::set<Gpb*> &getPrevGpbSet() {
    return prevGpb_;
}

// Add pointer to NextGpg
void Gpb::addNextGpb(Gpb* p) {
    nextGpb_.insert(p);
}

std::set<Gpb*> &getNextGpbSet() {
    return nextGpb_;
}

// Add element to the Set of GPUs in this GPB
void Gpb::insertGpuSet(Gpu& arg1) {
    gpuSet_.insert(arg1);
}

std::set<GpuIDType> getGpuSet() {
    return gpuSet_;
}

// to check whether this GPB is empty or not
bool isEmpty() {
    return (gpuSet_.empty());
}

// Remove the GPB if empty by connecting its prev to next GPBs
void Gpb::removeEmptyGpb() {
    for (std::set<Gpb*>::iterator it1 = prevGpb_.begin(); it1 != prevGpb_.end();
         it1++) {
        for (std::set<Gpb*>::iterator it2 = nextGpb_.begin();
             it2 != nextGpb_.end(); it2++) {
            (*it1).addNextGpb(*it2);
            (*it2).addPrevGpb(*it1);
        }
    }
    // Here free the current GPB
}

// Insert into In
void Gpb::insertIn(const GpuIDType& arg1) {
    in_.insert(arg1);
}

std::set<GpuIDType> &getIn() {
    return in_;
}

std::set<GpuIDType> &getOut() {
    return out_;
}

// Insert into Out
void Gpb::insertOut(const GpuIDType& arg1) {
    out_.insert(arg1);
}

// This function will return a set of pair (DestinationId, Destinationlist),
// where finds a match to the SourceId and SourceList
std::set<std::pair<NodeIndex, IndirectionList*>> Gpb::matchSource(
    NodeIndex id, IndirectionList *list) {
    return match(id, list);
}

// This function will return a set of pair (SourceId, Sourcelist), where finds a
// match to the DestinationId and DestinationList
std::set<std::pair<NodeIndex, IndirectionList*>> Gpb::matchDestination(
    NodeIndex id, IndirectionList* list) {
    return match(id, list);
}

std::set<std::pair<NodeIndex, IndirectionList*>> Gpb::match(NodeIndex id, IndirectionList *list) {
    for (auto &id : in_) {
        // TODO
    }
}
