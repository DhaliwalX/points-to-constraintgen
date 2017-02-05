#ifndef RECURSIVE_SET_H_
#define RECURSIVE_SET_H_

#include <set>
#include <algorithm>
#include <cassert>

namespace ptsto {

template <typename T>
class RecursiveSet {
public:
    using SetIterator = typename std::set<RecursiveSet<T>*>::iterator;
    using OurSetIterator = typename std::set<T>::iterator;
    using reference = T&;
    using pointer = T*;
    using const_reference = const T&;
    using const_pointer = const T*;

    class iterator {
    public:
        iterator(SetIterator start, SetIterator end)
            : start_{start}, seek_{start}, end_{ end }
        {
            if (start_ != end_) {
                if ((*seek_)->empty()) {
                    seek_ = findFirstNotEmpty(++seek_);
                }

                if (seek_ != end_)
                    current_ = (*seek_)->getOurSet().begin();
            }

        }

        iterator(const iterator &it) = default;
        iterator &operator=(const iterator &it) = default;

        // returns the iterator to the first non empty set
        // starting from `from`
        SetIterator findFirstNotEmpty(SetIterator from) {
            return std::find_if(from, end_,
                    [](RecursiveSet<T> *a) {
                        return !a->empty();
                    });
        }

        bool operator==(const iterator &rhs) const {
            bool result = rhs.current_ == current_;

            if (rhs.start_ == rhs.end_) {
                // we are comparing with end iterator
                if (seek_ == end_ && rhs.end_ == end_) {
                    return true;
                }
            }

            // little debug check
            if (result) {
                assert(seek_ == rhs.seek_);
            }
            return result;
        }

        bool operator!=(const iterator &rhs) const {
            return !(*this == rhs);
        }

        // pre increment operator, incrementing iterator already at
        // end will cause runtime error
        iterator &operator++() {
            assert(seek_ != end_);
            if (current_ == (*seek_)->getOurSet().end()) {
                // if current_ is already at end()
                seek_ = findFirstNotEmpty(++seek_);
                if (seek_ != end_) {
                    current_ = (*seek_)->getOurSet().begin();
                }
            } else {
                ++current_;
                if (current_ == (*seek_)->getOurSet().end()) {
                    // we have reached the end of this set
                    // try to move to next set
                    seek_ = findFirstNotEmpty(++seek_);

                    if (seek_ != end_) {
                        current_ = (*seek_)->getOurSet().begin();
                    }
                }
            }
            return *this;
        }

        // post increment operator
        iterator operator++(int) {
            auto save = *this;
            ++(*this);
            return save;
        }

        // dereference operator
        // FIXME
        T operator*() {
            return *current_;
        }

        // support arrow operator
        pointer operator->() {
            return current_;
        }
    private:
        OurSetIterator current_;
        SetIterator start_;
        SetIterator seek_;
        SetIterator end_;
    };

    iterator begin() {
        // we will create iterator just at the beginning where we
        // see the first element, it may miss the empty sets
        // FIXME
        for (auto it = sets_.begin(), e = sets_.end();
                    it != e; ++it) {
            if (it == sets_.begin()) {
                if (getOurSet().empty())
                    continue;
                return iterator(it, sets_.end());
            }

            RecursiveSet<T> *set = *it;
            if (set->empty()) continue;

            return iterator(it, sets_.end());
        }
        return end();
    }

    iterator end() {
        return iterator(sets_.end(), sets_.end());
    }

    // constructors
    RecursiveSet() {
        // add ourself
        sets_.insert(this);
    }

    RecursiveSet(const RecursiveSet &rhs) = delete;
    RecursiveSet &operator=(const RecursiveSet&) = delete;

    // returns the set owned by us
    std::set<T> &getOurSet() {
        return our_set_;
    }

    // merges two sets, O(log n) operation
    void mergeSet(RecursiveSet<T> *set) {
        if (sets_.count(set)) {
            return;
        }
        sets_.insert(set);
    }

    // inserts the element to our_set_
    void insert(const T &t) {
        our_set_.insert(t);
    }

    // returns true of this is empty, not a constant time operation
    // can be made by hacking into insert operation
    bool empty() {
        bool result = our_set_.empty() && (sets_.size() == 1);

        if (result)
            return result;

        if (!our_set_.empty())
            return false;

        auto it = ++sets_.begin();
        auto end = sets_.end();

        while (it != end) {
            RecursiveSet<T> *set = *it;

            if (!set->empty())
                return false;

            ++it;
        }
        return true;
    }
private:
    std::set<T> our_set_;
    std::set<RecursiveSet<T>*> sets_;
};

}

#endif
