

#ifndef PJ1_BTREE_SET_H
#define PJ1_BTREE_SET_H

#include "BTree.h"
template <class K>
class Bset{
    typedef K ValueType;
    struct keyOfValue {
        const  K& operator()(const ValueType& val) { // val is key
            return val;
        }
    };

    typedef BTree<ValueType, K, keyOfValue> BT;

public:
    typedef typename BT::iterator iterator;
    Bset():_t() {}
    iterator begin() {
        return _t.Begin();
    }
    iterator end() {
        return _t.End();
    }
    bool empty() {
        return _t.Empty();
    }
    ValueType& operator[](const K& key){
        return *(_t.Find(key));
    }
    size_t size() const {
        return _t.Size();
    }
    std::pair<iterator, bool> insert(const ValueType& val) {
        return _t.Insert(val);
    }
    void clear() {
        _t.Clear();
    }
    iterator find(const K& key) {
        return _t.Find(key);
    }
    void Delete(const K& key) {
        return _t.Delete(key);
    }
    void print_by_order(){
        return _t.print_by_order();
    }
    void update(const K& key, ValueType val) {
        return _t.Update(key, val);
    }
    ValueType search(const K& key){
        return _t.Find(key)->value;
    }
    void print(){
        return _t.Print(_t.getRoot(), 2);
    }
private:
    BT _t;
};



#endif //PJ1_BTREE_SET_H
