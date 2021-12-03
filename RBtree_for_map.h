
#ifndef PJ1_RBTREE_FOR_MAP_H
#define PJ1_RBTREE_FOR_MAP_H

#include "RBtree.h"
#include "BTree.h"

template <class K, class ValueType>
class RBmap {
    typedef std::pair<K, ValueType> DataType;  //pair


    struct keyOfValue //get key by value
    {
        const K& operator()(const DataType& val)//val is pair
        {
            return val.first;//get key
        }
    };
    struct dataOfValue{
        const K& operator()(const DataType& val)//val is pair
        {
            return val.second;//get key
        }
    };

    typedef RBTree<DataType, K, keyOfValue> RBT;
    typedef RBTreeNode<DataType> Node;

public:
    typedef typename RBT::iterator iterator; // 自动改正
    typedef typename RBT::Node node;
    RBmap():_t(RBT()) {}
    iterator begin() {
        return _t.Begin();
    }
    iterator end() {
        iterator end = _t.End();
        return _t.End();
    }
    size_t size() const{
        return _t.Size();
    }
    bool empty() const {
        return _t.Empty();
    }

    ValueType& operator[](const K& key){
        return (*(_t.Insert(std::pair<K, ValueType>(key, ValueType())).first)).second;
        //second is value
    }

    //modify
    std::pair<iterator, bool> insert(const DataType& val)//val is pair
    {
        return _t.Insert(val);
    }

    void clear() {
        _t.Clear();
    }
    iterator find(const K& key) {
        return _t.Find(key);
    }
    bool check(){
        return _t.checkRBTree();
    }
    void Delete(const K& key){
        return _t.Delete(key);
    }
    void print_clor_and_key(){
        return _t.print_by_order();
    }
    void update(const K& key, const DataType& val){
        return _t.Update(key,val);
    }
    DataType search(const K& key){
        iterator iy = _t.Find(key);
        Node* x = iy.getNode();
        if(x == nullptr)
            return DataType();
        return x->value;
    }


private:
    RBT _t;
};

#endif //PJ1_RBTREE_FOR_MAP_H
