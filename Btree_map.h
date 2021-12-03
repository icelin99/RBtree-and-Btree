
#ifndef PJ1_BTREE_MAP_H
#define PJ1_BTREE_MAP_H

#include "BTree.h"

template <class K, class ValueType>
class Bmap {
    typedef std::pair<K, ValueType> DataType;  //pair
public:
    struct keyOfValue //get key by value
    {
        const K& operator()(const DataType& val)//val is pair
        {
            return val.first; //get key
        }
    };
    struct dataOfValue{
        const ValueType& operator^(const DataType& val)//val is pair
        {
            return val.second;//get key
        }
    };

    typedef BTree<DataType, K, keyOfValue> BT;


public:
    typedef typename BT::iterator iterator;
    typedef typename BT::Node Node;
    Bmap():_t() {}
//    ~Bmap(){
//        _t.Clear();
//    }
    iterator begin() {
        return _t.Begin();
    }
    iterator end() {
        return _t.End();
    }
    size_t size() const{
        return _t.Size();
    }
    bool empty() const {
        return _t.Empty();
    }
    ValueType& operator[](const K& key){
        //return (*(_t.Insert(std::pair<K, ValueType>(key, ValueType())).first)).second;
        return (*(_t.Find(key))).second;
        //second is value
    }
    K& operator - (const DataType& dt){ //postposition
        return dt.first;
    }


    Node* getROOT(){
        return _t.getRoot();
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
    void Delete(const K& key){
        return _t.Delete(key);
    }
    void print_by_order(){
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
        int knum = _t.Find_key_of_x(x,key);
        DataType d = x->keys[knum];
        return d;
    }
    void print_value(Node* x, int knum){
        std::cout<<x->keys[knum].first<<" "<<x->keys[knum].second<<std::endl;
        getROOT();
    }
    void print(){
        return _t.Print(_t.getRoot(), 2);
    }
private:
    BT _t;

};

#endif //PJ1_BTREE_MAP_H
