
#ifndef PJ1_RBTREE_FOR_SET_H
#define PJ1_RBTREE_FOR_SET_H
#include "RBtree.h"
template <class K>//set only has key
class RBset{
    typedef K ValueType;
    struct keyOfValue
    {
        const K& operator()(const ValueType& val) // val = key
        {
            return val;
        }
    };

    typedef RBTree<ValueType, K, keyOfValue> RBT;
public:
    typedef typename RBT::iterator iterator;
    RBset():_t() {}
    iterator begin() {
        return _t.Begin();
    }
    iterator end(){
        return _t.End();
    }
    ValueType& operator[](const K& key){
        return *(_t.Find(key));
    }
    bool empty()const{
        return _t.Empty();
    }
    size_t size()const{
        return _t.Size();
    }
    std::pair<iterator, bool> insert(const ValueType& val){
        return _t.Insert(val);
    }
    void clear(){
        _t.Clear();
    }
    iterator find(const K& key){
        return _t.Find(key);
    }
    bool check(){
        return _t.checkRBTree();
    }
    void Delete(const K& key) {
        return _t.Delete(key);
    }
    void print(){
        return _t.print_by_order();
    }
    void update(const K& key, ValueType val){
        return _t.Update(key,val);
    }
    ValueType search(const K& key){
        return _t.Find(key)->value;
    }

private:
    RBT _t;
};
#endif //PJ1_RBTREE_FOR_SET_H
