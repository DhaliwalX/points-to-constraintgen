#include "IndirectionListMap.h"

bool IndirectionListMap::has(const IndirectionList *list) const {
    return map_.count(list);
}

IndirectionList *IndirectionListMap::find(IndirectionList *list) {
    auto it = map_.bucket(list);
    
    auto bucket_begin = map_.begin(it);

    if (bucket_begin == map_.end())
        return nullptr;

    for (; bucket_begin != map_.end(); bucket_begin++) {
        if (*list == **bucket_begin) {
            return *bucket_begin;
        }
    }
    return nullptr;
}


IndirectionList *findOrInsert(IndirectionList *list) {
    auto l = find(list);

    if (l == nullptr) {
        map_.insert(list);
        return list;
    }

    return l;
}
