#ifdef INDIRECTION_LIST_H_
#define INDIRECTION_LIST_H_

class IndirectionList {
public:
    using iterator = int8_t*;
    using const iterator = const int8_t*;

    IndirectionList() 
    : star_count_{ 0 }, length_{ 0 }
    { }

    void insertField(NodeIndex field) {
        assert(sizeof(field_list_) != length_ + 1);
        field_list_[length_] = field;
        length_++;
    }

    bool isFieldListEmpty() {
        return !length_;
    }

    iterator begin() {
        return field_list_;
    }

    iterator end() {
        return field_list_ + length_;
    }

    int64_t getRefCount() {
        return star_count_;
    }

    void setRefCount(int64_t star_count) {
        star_count_ = star_count;
    }

private:
    int32_t star_count_;
    int32_t length_;
    int8_t field_list_[8];
};

namespace detail {
// hashing utilities, copied from boost

inline void HashCombine(std::size_t& seed, const char& v) {
    std::hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed<<6) + (seed>>2);
}

std::size_t HashCharString(const char *str, const size_t len) {
    std::size_t seed = 0;
    for (size_t i = 0; i < len; i++) {
        HashCombine(seed, str[i]);
    }

    return seed;
}

}

// hash implementation for IndirectionList
namespace std {
    template<> struct hash<IndirectionList> {
        using argument_type = IndirectionList;
        using result_type = std::size_t;

        result_type operator()(argument_type const &list) const {
            return HashCharString((const char*)&list, sizeof(argument_type));
        }
    }
}

#endif
