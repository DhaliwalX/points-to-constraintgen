#ifndef INDIRECTION_LIST_MAP_H_
#define INDIRECTION_LIST_MAP_H_

#include "IndirectionList.h"

#include <unordered_set>

namespace std {

    // override the default implementation for pointers
    template<> struct hash<IndirectionList*> {
        using argument_type = IndirectionList*;
        using result_type = std::size_t;

        result_type operator()(argument_type const &list) const {
            return HashCharString((const char*)list, sizeof(IndirectionList));
        }
    }
}

class IndirectionListMap {
public:
    bool has(const IndirectionList *list) const;

    IndirectionList *find(IndirectionList *list);

    IndirectionList *findOrInsert(IndirectionList *list);

private:
    std::unordered_set<IndirectionList*> map_;
};

#endif
