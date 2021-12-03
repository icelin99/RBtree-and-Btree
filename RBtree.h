//
//Red-black properties:
//1. Every node is either red or black.
//2. The root is black.
//3. Every leaf (NIL) is black.
//4. If a node is red, then both its children are black.
//5. All simple paths from any node x to a descendant leaf have the same number of black nodes.
// 以order为基准，空格数为w， 则同一行其他元素空格数 w +（ order - 2^树深 * 3 -1）* 6

#ifndef PJ1_RBTREE_H
#define PJ1_RBTREE_H
#include <iomanip>
#include <vector>
#include <fstream>
#include <type_traits>

enum COLOR{
    RED, BLACK
};

template <typename ValueType>
struct RBTreeNode {
    RBTreeNode(const ValueType& value = ValueType(), COLOR col = RED)
        :pLeftChild(nullptr),
        pRightChild(nullptr),
        pFather(nullptr),
        value(value),
        color(col)
        {}

    RBTreeNode<ValueType>* pLeftChild;
    RBTreeNode<ValueType>* pRightChild;
    RBTreeNode<ValueType>* pFather;
    ValueType value;
    COLOR color;
};

// add a iterator for encapsulating different types of data
template <class C>
class Iterator{
public:
    typedef RBTreeNode<C> Node;
    typedef Iterator<C> self;

    Iterator(Node* node= nullptr)
        :pNode(node)
    {}

    //overlapping operator
    C& operator * ()
    {
        return pNode->value;
    }
    C* operator -> () {
        return &(operator*());
    }
    // compare
    bool operator !=(const self& s) const {
        return pNode != s.pNode;
    }
    bool operator == (const self& s) {
        return pNode == s.pNode;
    }
    // move
    self& operator ++() { // preposition
        Successor();
        return *this;
    }
    self& operator ++(int) { // postposition
        self tmp(*this); //先保存一份
        Successor();
        return tmp;
    }
    self& operator--() { // preposition
        Predecessor();
        return *this;
    }
    self& operator--(int) { // postposition
        self tmp(*this); //先保存一份
        Predecessor();
        return tmp;
    }

    void Successor(){
        // inorder traversal
        if(pNode->pRightChild) { // have right child
            pNode = pNode->pRightChild;
            while (pNode->pLeftChild) { // find successor
                pNode = pNode->pLeftChild;
            }
        }
        else { // empty right subtree. then the father
            if(pNode->pFather == nullptr)
                pNode = pNode->pFather; // need return null, or endless loop
            else{
                Node* father = pNode->pFather;
                while (father && pNode == father->pRightChild) { // node is right child, find until it is the first left
                    pNode = father;
                    father = pNode->pFather;
                }
                pNode = father;
            }

        }
    }
    void Predecessor() {
        if(pNode->pLeftChild) { // have left child
            pNode = pNode->pLeftChild;
            while (pNode->pRightChild) { // find predeccessor
                pNode = pNode->pRightChild;
            }
        }
        else { // empty left subtree. then the father
            if(pNode->pFather == nullptr)
                ;
            else {
                Node* father = pNode->pFather;
                while (father && pNode == father->pleftChild) {
                    pNode = father;
                    father = pNode->pFather;
                }
                pNode = father;
            }
        }
    }
    Node* getNode(){
        return pNode;
    }

private:
    Node* pNode;
};

//V就代表key,K是key的类型
//如果是set，就是key，但是如果是map，就是<key,value>的键值对
//KeyofValue,获取key
template <class V, class K, class keyOfValue>
class RBTree {
public:
    typedef Iterator<V> iterator;
    typedef RBTreeNode<V> Node;

public:
    RBTree(): pRoot(nullptr), size(0) {
    }
    ~RBTree(){
        Clear();
        delete pRoot;
        pRoot = nullptr;
    }
    iterator Begin(){ //root
        return treeMin(pRoot);
    }
    iterator End() {
        if (treeMax(pRoot) == nullptr)
            return iterator(nullptr);
        return (treeMax(pRoot)->pRightChild); // if treemax, max not belonged
    }

    std::pair<iterator , bool> Insert(const V& data) //  V is map, data is the key-value pair
    {
        size++;
        Node* pNewN = nullptr;
        if(getRoot() == nullptr) {
            pRoot = pNewN = new Node(data, BLACK); // root is black
        }
        else {
            if (Find(keyOfValue()(data)) != nullptr) {
                std::cout << keyOfValue()(data) << " already existed" << std::endl;
                size--;
                return make_pair(iterator(End()), false); // same key
            }
            Node* x = getRoot();
            Node* y = x; // y track the father
            while(x) {
                y = x;
                if(keyOfValue()(data) < keyOfValue()(x->value)){ // is it's map, compare key
                    // new node is smaller, then search from left side
                    x = x->pLeftChild;
                }
                else if(keyOfValue()(data) > keyOfValue()(x->value)) {
                    x = x->pRightChild;
                }
                else {
                    return make_pair(iterator(x), false); // same key
                }
            }
            // x is the position to be inserted, y is its father
            x = pNewN = new Node(data); // RED
            if (keyOfValue()(data) < keyOfValue()(y->value)) {
                y->pLeftChild = x;
            }
            else {
                y->pRightChild = x;
            }
            x->pFather = y;
            Insert_fixup(x);
        }
        return std::pair<iterator,bool>(iterator(pNewN), true);
    }

    size_t Size()const
    {
        return size;
    }

    void Clear()
    {
        Destroy(this->pRoot);//从根开始销毁
        size = 0;
    }

    void print_by_order() {
        isOrdered(pRoot);
        std::cout<<std::endl;
    }

    bool checkRBTree(){
        if (Empty())
            return true;
        // root is black?
        if (pRoot->color != BLACK){
            std::cout << "Violation of red-black tree property 2: root isn't black" << std::endl;
            return false;
        }
        Node* x = this->pRoot;
        size_t BlackCount = 0;
        while (x) {
            if (x->color == BLACK)
                ++BlackCount;
            x = x->pLeftChild;
        }
        size_t pathCount = 0;
        return isValidRBTree(pRoot, BlackCount, pathCount);//property 3,4
    }



    bool Empty() const{
        if(pRoot->pLeftChild == nullptr && pRoot->pRightChild == nullptr)
            return true;
        else
            return false;
    }

    void init(std::string filename) {

    }

    void Delete(const K& el) {
        const K ele = el;
        iterator iy;
        iy = Find(ele); // Find first use
        Node *x = iy.getNode();
        Node *y = x;
        if(x == nullptr) {
            std::cout<<"key "<<ele<<" missing"<<std::endl;
            return;
        }
        // y is the position need to be deleted
        if (x->pLeftChild && x->pRightChild) { // both sides have children
            Node *ssr = successor(x);
            y = ssr;
            // exchange value only
            V temp = x->value;
            x->value = (ssr->value);
            ssr->value = temp;
            // now we need to delete the position of successor, it is the same case list follows
        }
        std::cout << keyOfValue()(y->value) << "is deleted" << std::endl;
        if (y->pLeftChild == nullptr && y->pRightChild == nullptr) { // ele is the leaf node
            if (y->color == BLACK)
                delete_fixup(y);
            if (y->pFather == nullptr) {
                pRoot = nullptr;
            } else if (y == (y->pFather)->pLeftChild) {
                (y->pFather)->pLeftChild = nullptr;
            } else {
                (y->pFather)->pRightChild = nullptr;
            }
        } else { // only have left child or only have right child, y must be black, and child must be red
            Node *child;
            if (y->pLeftChild == nullptr) {
                child = y->pRightChild;
            } else {
                child = y->pLeftChild;
            }
            child->pFather = y->pFather;
            if (y->pFather == nullptr) { // delete the root
                pRoot = child;
            } else if (y == (y->pFather)->pLeftChild) {
                y->pFather->pLeftChild = child;
            } else {
                y->pFather->pRightChild = child;
            }
            child->color = BLACK;
        }
        size--;
        print_by_order();
    }


    Node*& getRoot() {
        return pRoot;
    }

    Node* treeMin(Node* x) {
        Node* y = x;
        if(y == nullptr)
            return nullptr;
        while(y->pLeftChild != nullptr)
            y = y->pLeftChild;
        return y;
    }
    Node* treeMax(Node* x) {
        Node *y = x;
        if(y == nullptr)
            return nullptr;
        while(y->pRightChild != nullptr)
            y = y->pRightChild;
        return y;
    }

    void Update(const K& key, const V& newVal) {
        int i = 0;
        while(i < 1000000)  //debug problem
            i++;
        iterator it;
        it = Find(key);
        if(it.getNode() == nullptr){
            std::cout<<" not in the database"<<std::endl;
            Insert(newVal);
            return;
        }
        Node * x = it.getNode();
        x->value = newVal;

        std::cout<<"\nupdate: "<<key<<std::endl;
        print_by_order();
    }

    iterator find(const K& key){ // print messaage
        Node* x = pRoot;
        while (x){
            if (key == keyOfValue()(x->value)){
                return iterator(x);
            }
            else if (key < keyOfValue()(x->value)){
                x = x->pLeftChild;
            }
            else{
                x = x->pRightChild;
            }
        }
        std::cout<< "Don't find "<<key<< std::endl;
        return iterator(End());//don't find . return root
    }





private:
    Node* pRoot;
    size_t size;

    Node* successor(Node* x) {
        Node* xx = x;
        if(xx->pRightChild){ // have right subtree
            xx= xx->pRightChild;
            return treeMin(xx);
        }
        else { // find father
            Node* y = x->pFather;
            while(y && xx == y->pRightChild){
                xx = y;
                y = xx->pFather;
            }
            return y;
        }
    }

    Node* predecessor(Node* x) {
        Node* xx = x;
        if(xx->pLeftChild){
            xx = xx->pLeftChild;
            return treeMax(xx);
        } else {
            // find father
            Node* y = x->pFather;
            while(y && xx == y->pLeftChild){
                xx = y;
                y = xx->pFather;
            }
            return y;
        }
    }

    void Destroy(Node*& x){
        if (x != nullptr){
            //按照后序遍历来销毁
            Destroy(x->pLeftChild);
            Destroy(x->pRightChild);
            delete x;
            x = nullptr;
        }
    }

    bool isValidRBTree(Node* xx, size_t BlackCount, size_t pathCount) {
        Node* x = xx;
        if (nullptr == x) {
            return true;
        }
        if (x->color == BLACK) {
            pathCount++;
        }
        Node* ftr = x->pFather;
        if (ftr && ftr != pRoot && ftr->color == RED && x->color == RED){
            std::cout << "Violation of red-black tree property 4: red can't be together" << std::endl;
            return false;
        }
        if (nullptr == x->pRightChild && nullptr == x->pRightChild) { // leaf
            if (pathCount != BlackCount) {
                std::cout << "Violation of red-black tree property 5: leaf have the same number of black nodes" << std::endl;
                return false;
            }
        }
        return isValidRBTree(x->pLeftChild, BlackCount, pathCount) && isValidRBTree(x->pRightChild, BlackCount, pathCount);
    }

    void LeftRotate(Node* x) {
        Node* child = x->pRightChild;// child will be the  father of current this
        // if no right child , end
        if(child == nullptr)
            return;

        Node* ftr = x->pFather;
        if(ftr) {
            child->pFather = ftr;

            if(x == ftr->pLeftChild){
                ftr->pLeftChild = child;
            }
            else {
                ftr->pRightChild = child;
            }
        }
        else { // x is root
            pRoot = child;
            child->pFather = nullptr;
        }
        x->pRightChild = child->pLeftChild;
        if(child->pLeftChild != nullptr)
            (child->pLeftChild)->pFather = x;
        x->pFather = child;
        child->pLeftChild = x;

        std::cout<<keyOfValue()(x->value)<<"is the axis of rotation, after left rotate"<< std::endl;
        print_by_order();
    }

    void RightRotate(Node* x) {
        Node* child = x->pLeftChild;// child will be the new father of current this
        // if no right child , end
        if(child == nullptr)
            return;
        // this has a father
        Node* ftr = x->pFather;
        if(ftr != nullptr) {
            child->pFather = ftr;
            if(x == ftr->pLeftChild){
                ftr->pLeftChild = child;
            }
            else {
                ftr->pRightChild = child;
            }
        }
        else { // x is root
            pRoot = child;
            child->pFather = nullptr;
        }
        x->pLeftChild = child->pRightChild;
        if(child->pRightChild)
            (child->pRightChild)->pFather = x;
        x->pFather = child;
        child->pRightChild = x;
        std::cout<< keyOfValue()(x->value)<<"is the axis of rotation, after right rotate"<< std::endl;
        print_by_order();
    }

    void Insert_fixup(Node* insert)
    {
        // color is red, and not the root
        Node* ftr = insert->pFather; //father
        if(ftr == nullptr)
            return;
        if(ftr->color == BLACK){ // father is black, end
            return;
        }
        else { // so that father is red, too
            Node* grandfather = ftr->pFather;
            Node* uncle;
            bool isLeftChild = (insert == ftr->pLeftChild);
            if(ftr == grandfather->pLeftChild) { // father is the left child
                uncle = grandfather->pRightChild;
                if(uncle == nullptr || uncle->color == BLACK) {
                    // uncle is black, or uncle is virtual(black)
                    if(!isLeftChild){  // insert is right child, but its father is left child
                        LeftRotate(ftr);
                    }
                    // now they are in the same line, then right rotate
                    grandfather->color = RED;
                    grandfather->pLeftChild->color = BLACK;
                    RightRotate(grandfather);
                    pRoot->color = BLACK;
                    return;
                }
            }
            else{  // father is the right child
                uncle=grandfather->pLeftChild;
                if(uncle == nullptr || uncle->color == BLACK) {
                    // uncle is black, or uncle is virtual(black)
                    if(isLeftChild){  // insert is left child, but its father is right child
                        RightRotate(ftr);
                    }
                    // now they are in the same line, and left rotate
                    grandfather->color = RED;
                    grandfather->pRightChild->color = BLACK;
                    LeftRotate(grandfather);
                    pRoot->color = BLACK;
                    return;
                }
            }
            // uncle is also red
            ftr->color = BLACK;
            uncle->color = BLACK;
            grandfather -> color = RED;
            pRoot->color = BLACK;
            Insert_fixup(grandfather);
        }
    }

    void delete_fixup(Node* x) {
        Node * y = x;
        Node * w;
        // no case that has both children

        while(y != pRoot && y->color == BLACK) {
            if(y == (y->pFather)->pLeftChild){ // delete one is the left child
                w = y->pFather->pRightChild; // w is the sibling
                if(!w){
                    break;
                }
                if( w->color == RED) { // left rotate + change color
                    w->color = BLACK;
                    (y->pFather)->color = RED;
                    LeftRotate(y->pFather);
                    w = (y->pFather)->pRightChild; // renew sibling
                }// then enter other cases
                if((w->pLeftChild == nullptr || w->pLeftChild->color == BLACK)
                   && (w->pRightChild == nullptr || w->pRightChild->color == BLACK)) {
                    // both children's color of sibling are BLACK
                    w->color = RED;
                    y = y->pFather;
                    break;
                }
                else if(w->pRightChild == nullptr || (w->pRightChild)->color == BLACK){
                    //only the right child's color is BLACK of sibling, left child is RED
                    (w->pLeftChild)->color = BLACK;
                    w->color = RED;
                    RightRotate(w);
                    w = (y->pFather)->pRightChild; // renew sibling
                }
                // right is RED
                w->color = y->pFather->color;
                (y->pFather)->color = BLACK;
                (w->pRightChild)->color = BLACK;
                LeftRotate(y->pFather);
                y = pRoot;
            }
            else { //delete one is the right child
                w = y->pFather->pLeftChild; // w is the sibling
                if(!w){
                    break;
                }
                if( w->color == RED) { // right rotate + change color
                    w->color = BLACK;
                    (y->pFather)->color = RED;
                    RightRotate(y->pFather);
                    w = (y->pFather)->pLeftChild; // renew sibling
                }// then enter other cases
                if(((w->pLeftChild == nullptr || w->pLeftChild->color == BLACK)
                   && (w->pRightChild == nullptr || w->pRightChild->color == BLACK))) {
                    // both children's color of sibling are BLACK, or null
                    w->color = RED;
                    y = y->pFather;
                    break;
                }
                else if( w->pLeftChild == nullptr || (w->pLeftChild->color == BLACK)){
                    //only the left child's color is BLACK of sibling, right child is RED
                    (w->pRightChild)->color = BLACK;
                    w->color = RED;
                    LeftRotate(w);
                    w = y->pFather->pLeftChild; // renew sibling
                }
                // left is RED
                w->color = (y->pFather)->color;
                (y->pFather)->color = BLACK;
                (w->pLeftChild)->color = BLACK;
                RightRotate(y->pFather);
                y = pRoot;
            }
        }
        y->color = BLACK;
    }

    iterator Find(const K& key){ // print messaage
        Node* x = pRoot;
        while (x){
            if (key == keyOfValue()(x->value)){
                return iterator(x);
            }
            else if (key < keyOfValue()(x->value)){
                x = x->pLeftChild;
            }
            else{
                x = x->pRightChild;
            }
        }
        return iterator(End());//don't find . return root
    }

    void isOrdered( Node* x) {
        if (x){
            isOrdered(x->pLeftChild);
            std::string col = (x->color==BLACK)?"B":"R";
            std::cout << keyOfValue()(x->value) <<"("<<col<<")"<< " ";
            isOrdered(x->pRightChild);
        }
    }
};








//
//typedef struct Attribution{
//    std::string word;
//    int frequency;
//    std::string partOfSpeech;
//} *pAttr, Attr;
//typedef struct SingleInt {
//    int data;
//} SinInt, *pSinInt;
//
//
//int arraySize = 1000;
//int print_width = 4;
//
//template <class T>
//class RBTree;
//
//
//template <typename T>
//bool is_ii_empty(T x){
//    if(std::is_same<T,int>::value && x > -1){
//        return true;
//    }
//    else if(std::is_same<T,char>::value && x > 0){
//        return true;
//    }
//    else if(std::is_same<T,std::string>::value && x != ""){
//        return true;
//    }
//    return false;
//}
//template <typename T>
//T set_empty(T){
//    if(std::is_same<T,int>::value){
//        return -1;
//    }
//    else if(std::is_same<T,char>::value){
//        return ' ';
//    }
//    else if(std::is_same<T,std::string>::value){
//        return "";
//    }
//    return false;
//}
//
//template <typename T>
//class RBNode {
//    friend class RBTree<T>;
//public:
//    RBNode(Attr att, COLOR col){
//        data = att.word;
//        color = col;
//        leftChild = nullptr;
//        rightChild = nullptr;
//        father = nullptr;
//        attr.word = att.word;
//        attr.frequency = att.frequency;
//        attr.partOfSpeech = att.partOfSpeech;
//        lexicographical_order = "";
//    }
//    RBNode(T value, COLOR col) {
//        data = value;
//        color = col;
//        leftChild = nullptr;
//        rightChild = nullptr;
//        father = nullptr;
//        lexicographical_order = "";
//    }
//    void setData(T value){
//        data = value;
//    }
//    T getData(){
//        return data;
//    }
//    void setLeftChild(RBNode* left) {
//        leftChild = left;
//    }
//    RBNode* getLeftChild() {
//        return leftChild;
//    }
//    void setRightChild(RBNode* right) {
//        rightChild = right;
//    }
//    RBNode* getRightChild() {
//        return rightChild;
//    }
//    void setFather(RBNode* fat) {
//        father = fat;
//    }
//    RBNode* getFather() {
//        return father;
//    }
//    void setColor(COLOR col) {
//        color = col;
//    }
//    COLOR getColor() {
//        return color;
//    }
//    void setLeOrder(std::string oo){
//        lexicographical_order = oo;
//    }
//    std::string getLeOrder() {
//        return lexicographical_order;
//    }
//    int getOrder(){
//        return order;
//    }
//    void setOrder(int od){
//        order = od;
//    }
//private:
//    T data;
//    Attr attr;
//    int order;
//    RBNode* leftChild;
//    RBNode* rightChild;
//    RBNode* father;
//    COLOR color;
//    std::string lexicographical_order;
//};
//
//template <typename T>
////ty
////typedef struct element_list {
////    RBNode<T> node;
////    int order;
////    element_list *next;
////};
//struct element_list{
//    RBNode<T> node;
//    int order;
//    element_list *next;
//};
//
//
//template <typename T>
//class RBTree {
//public:
//    RBTree() {
//        root = nullptr;
//        size = 0;
//        RBArray = std::vector<std::vector<T> >(arraySize,std::vector<int>(2)); //定义二维数组 1000行 2列
//        for(int i = 0; i < arraySize; i++)
//            RBArray[i][0] = NIL; // initialize
//    }
//    RBTree(Attr aa){
//        root = nullptr;
//        size = 0;
//    }
//
//    void insert(T value);
//    void insert_by_struct(Attr att);
//
//    RBNode<T> * getRoot(){
//        return root;
//    }
//    void init(std::string filename);
//    int treeDelete(T ele);
//    RBNode<T> * treeMin(RBNode<T> *x);
//    RBNode<T> * treeMax(RBNode<T> *x);
//    RBNode<T> * search(RBNode<T>* x, int key);
//    void print_tree();
//    element_list<T>* sort();
//    void print_lexi();
//private:
//    RBNode<T> * root;
//    int size;
//    std::vector<std::vector<T> > RBArray;
//
//    void leftRotate(RBNode<T>* x);
//    void rightRotate(RBNode<T>* x);
//    void insert_fix(RBNode<T>* insert);
//    RBNode<T> * successor(RBNode<T> *x);
//    void delete_fix(RBNode<T>* x);
//
//};
//
//template <typename T>
//element_list<T> * RBTree<T>::sort()
// {
//    element_list<T> *head, *temp1, *temp2;
//    head->order = 0;
//    head->node = *root;
//    int depth = 1;
//    RBNode<T>* x = root;
//    RBNode<T>* child;
//    int order;
//    head->next = temp1;
//    temp2 = head;
//    while(1) {
//        if (x->getLeftChild() != nullptr) {
//            depth++;
//            child = x->getLeftChild();
//            child->setLeOrder(child->getLeOrder()+"0");
//            temp1->node = *x;
//            temp2->next = temp1;
//            temp1 = temp1->next;
//            temp2= temp2->next;
//        }
//        if(x->getRightChild() != nullptr) {
//            depth++;
//            child = x->getLeftChild();
//            child->setLeOrder(child->getLeOrder()+"1");
//            temp1->node = *x;
//            temp2->next = temp1;
//            temp1 = temp1->next;
//            temp2= temp2->next;
//        }
//
//        if(x->getLeftChild() != nullptr){
//            x = x->getLeftChild();
//            continue;
//        }
//        else if(x->getRightChild() != nullptr){
//            x = x->getRightChild();
//            continue;
//        }
//        else{
//            if(x->getFather() == nullptr)
//                break;
//
//            while(x == (x->getFather()->getLeftChild()) && (x->getFather())->getRightChild()== nullptr) { // find successor
//                x = x->getFather();
//                if(x->getFather() == nullptr)
//                    break;
//            }
//
//            while(x->getFather() != nullptr && x == (x->getFather()->getRightChild())){
//                x = x->getFather();
//                if(x->getFather() == nullptr)
//                    break;
//            }
//            if(x->getFather() == nullptr || (x->getFather())->getRightChild() == nullptr)
//                break;
//            x = x->getFather();
//            x = x->getRightChild();
//        }
//
//    }
//
//    return head;
//}
//
//
//
//template <typename T>
//void RBTree<T>::leftRotate(RBNode<T>* x) {
//    RBNode<T>* child = x->getRightChild();// child will be the  father of current this
//    // if no right child , end
//    if(child == nullptr)
//        return;
//    //
//    RBNode<T>* ftr = x->getFather();
//    if(ftr != nullptr) {
//        child->setFather(ftr);
//
//        if(x == ftr->getLeftChild()){
//            ftr->setLeftChild(child);
//        }
//        else {
//            ftr->setRightChild(child);
//        }
//    }
//    else { // x is root
//        root = child;
//        child->setFather(nullptr);
//    }
//    x->setRightChild(child->getLeftChild());
//    if(child->getLeftChild() != nullptr)
//        (child->getLeftChild())->setFather(x);
//    x->setFather(child);
//    child->setLeftChild(x);
//
//    std::cout<< "after left rotate"<< std::endl;
//    print_tree();
//}
//
//template <typename T>
//void RBTree<T>::rightRotate(RBNode<T>* x) {
//    RBNode<T>* child = x->getLeftChild();// child will be the new father of current this
//    // if no right child , end
//    if(child == nullptr)
//        return;
//    // this has a father
//    RBNode<T>* ftr = x->getFather();
//    if(ftr != nullptr) {
//        child->setFather(ftr);
//        if(x == ftr->getLeftChild()){
//            ftr->setLeftChild(child);
//        }
//        else {
//            ftr->setRightChild(child);
//        }
//    }
//    else { // x is root
//        root = child;
//        child->setFather(nullptr);
//    }
//    x->setLeftChild(child->getRightChild());
//    if(child->getRightChild() != nullptr)
//        (child->getRightChild())->setFather(x);
//    x->setFather(child);
//    child->setRightChild(x);
//    std::cout<< "after right rotate"<< std::endl;
//    print_tree();
//}
//
//template <typename T>
//void RBTree<T>::insert(T inst) {
//    size++;
//    // first, find its position by its value
//    RBNode<T> *ist = new RBNode<T>(inst, RED);
//    RBNode<T> *x = this->getRoot(); // x: root node
//    RBNode<T> *y;
//    if (x == nullptr) {
//        // tree is empty
//        this->root = ist;
//        ist->setOrder(1);
//    } else {
//        while (x != nullptr) {
//            y = x; // y track the father
//            if (inst < x->getData()) {
//                // new node is bigger, then search from right side
//                x = x->getLeftChild();
//            } else {
//                x = x->getRightChild();
//            }
//        }
//        ist->setFather(y);
//        if (inst < y->getData()) {
//            y->setLeftChild(ist);
//            ist->setOrder((y->getOrder())*2);
//        }
//        else {
//            y->setRightChild(ist);
//            ist->setOrder((y->getOrder())*2+1);
//        }
//    }
//    insert_fix(ist);
//}
//
//template <typename T>
//void RBTree<T>::insert_by_struct(Attr aa) {
//    size++;
//    // first, find its position by its value
//    RBNode<T> *ist = new RBNode<T>(aa, RED);
//    RBNode<T> *x = this->getRoot(); // x: root node
//    RBNode<T> *y;
//    if (x == nullptr) {
//        // tree is empty
//        this->root = ist;
//        ist->setOrder(1);
//    } else {
//        while (x != nullptr) {
//            y = x; // y track the father
//            if (ist->getData() < x->getData()) {
//                // new node is bigger, then search from right side
//                x = x->getLeftChild();
//            } else {
//                x = x->getRightChild();
//            }
//        }
//        ist->setFather(y);
//        if (ist->getData() < y->getData()) {
//            y->setLeftChild(ist);
//            ist->setOrder((y->getOrder())*2);
//        }
//        else {
//            y->setRightChild(ist);
//            ist->setOrder((y->getOrder())*2+1);
//        }
//    }
//    insert_fix(ist);
//}
//
//template <typename T>
//void RBTree<T>::insert_fix(RBNode<T> *insert) {
//    // color is red
//    RBNode<T>* ftr = insert->getFather();
//    if(this->getRoot() == insert){ // insert is root node, change color, end
//        insert->setColor(BLACK);
//        return;
//    }
//    else if(ftr->getColor() == BLACK){ // father is black, end
//        return;
//    }
//    else { // so that father is red
//        RBNode<T>* grandfather = ftr->getFather();
//        RBNode<T>* uncle;
//        bool isLeftChild = (insert == ftr->getLeftChild());
//        if(ftr == grandfather->getLeftChild()) { // father is the left child
//             uncle = grandfather->getRightChild();
//             if(uncle == nullptr || uncle->getColor() == BLACK) {
//                 // uncle is black, or uncle is virtual(black)
//                 if(!isLeftChild){  // insert is right child, but its father is left child
//                     leftRotate(ftr);
//                 }
//                 // now they are in the same line, and right rotate
//                 grandfather->setColor(RED);
//                 grandfather->getLeftChild()->setColor(BLACK);
//                 rightRotate(grandfather);
//                 return;
//             }
//        }
//        else{  // father is the right child
//            uncle=grandfather->getLeftChild();
//
//            if(uncle == nullptr || uncle->getColor() == BLACK) {
//                // uncle is black, or uncle is virtual(black)
//                if(isLeftChild){  // insert is left child, but its father is right child
//                    rightRotate(ftr);
//                }
//                // now they are in the same line, and right rotate
//                grandfather->setColor(RED);
//                grandfather->getRightChild()->setColor(BLACK);
//                leftRotate(grandfather);
//                return;
//            }
//        }
//        // uncle is also red
//        ftr->setColor(BLACK);
//        uncle->setColor(BLACK);
//        grandfather->setColor(RED);
//        insert_fix(grandfather);
//    }
//}
//
//template <typename T>
//RBNode<T> * RBTree<T>::successor(RBNode<T> *x) {
//    // by default, x is not leaf node
//    RBNode<T> * xx = x;
//    if(x->getRightChild() != nullptr) {
//        return treeMin(x->getRightChild());
//    }
//    // if x don't have right child
//    RBNode<T>* y = x->getFather();
//    while(y != nullptr && xx == y->getRightChild()) {
//        xx = y;
//        y = xx->getFather();
//    }
//    return y;
//}
//
//template <typename T>
//RBNode<T> * RBTree<T>::treeMin(RBNode<T> *x) {
//    RBNode<T> *y = x;
//    while(y->getLeftChild() != nullptr)
//        y = y->getLeftChild();
//    return y;
//}
//
//template <typename T>
//RBNode<T> * RBTree<T>::treeMax(RBNode<T> *x) {
//    RBNode<T> *y = x;
//    while(y->getRightChild() != nullptr)
//        y = y->getRightChild();
//    return y;
//}
//
//template <typename T>
//RBNode<T> * RBTree<T>::search(RBNode<T> *x, int key) {
//    if(x->getData() == key) {
//        std::cout<< " find value "<< key << std::endl;
//        return x;
//    }
//    if(x == nullptr) {
//        std::cout<< "Don't find value "<< key << std::endl;
//        return nullptr;
//    }
//    if(key < x->getData()) {
//        return search(x->getLeftChild(), key);
//    }
//    else {
//        return search(x->getRightChild(), key);
//    }
//}
//
//template <typename T>
//int RBTree<T>::treeDelete(T ele) {
//    RBNode<T> * x = search(root, ele);
//    RBNode<T> * y = x;
//    // y is the position need to be deleted
//    if(x->getLeftChild() != nullptr && x->getRightChild() != nullptr){ // both sides hace children
//        RBNode<T>* ssr = successor(x);
//        y = ssr;
//        // exchange value only
//        int temp = x->getData();
//        x->setData(ssr->getData());
//        ssr->setData(temp);
//        // now we need to delete the position of successor, it is the same case list follows
//    }
//    if(y->getLeftChild() == nullptr && y->getRightChild() == nullptr) { // ele is the leaf node
//        if(y->getColor() == BLACK)
//            delete_fix(y);
//        if(y->getFather() == nullptr){
//            this->root = nullptr;
//        }
//        else if(y == y->getFather()->getLeftChild()){
//            y->getFather()->setLeftChild(nullptr);
//        }
//        else {
//            y->getFather()->setRightChild(nullptr);
//        }
//    }
//    else  { // only have left child or only have right child, y must be black, and child must be red
//        RBNode<T> * child;
//        if(y->getLeftChild() == nullptr) {
//            child = y->getRightChild();
//        } else {
//            child = y->getLeftChild();
//        }
//        child->setFather(y->getFather());
//        if(y->getFather() == nullptr){ // delete the root
//            root = child;
//        } else if(y == (y->getFather())->getLeftChild()) {
//            y->getFather()->setLeftChild(child);
//        } else {
//            y->getFather()->setRightChild(child);
//        }
//        child->setColor(BLACK);
//    }
//
//    return y->getData();
//    /**
//     * 1. 待删除结点两个子结点非空：寻找右子树最小或者左子树最大，交换data后，删除对应位置
//     * 2. 待删除结点仅有一个非空子结点：则非空子结点肯定是红色,待删结点肯定为黑色，则直接删除待删除结点，再将子结点提上来，并变为黑色即可
//     * 3. 待删除结点两个子结点均为空：
//     *      (1) 若待删除结点为红色，则直接删除即可（不影响根的阶——根到任一外部节点路径上黑色结点数目）
//     *      (2) 若待删除结点为黑色，则删除之后该路径上黑色结点-1，红黑树性质不满足，须进行调整(记为双黑)：
//     *          双黑为左（右）结点：
//     *              1.双黑结点的兄弟结点为黑色，且其子结点有红色：
//     *              (a)红色结点为右（左）子节点：先左（右）旋双黑的父节点y;然后y和红色结点变黑;y的新父节点继承y的颜色
//     *              (b)红色结点为左（右）子结点：先右（左）旋双黑的兄弟;再左（右）旋双黑的父节点;此时红色结点继承原双黑parent颜色，双黑父节点变黑
//     *              2.双黑结点的兄弟结点为黑色，且其两个子结点为黑（事实上是两个子结点为空）:
//     *              (a)若双黑父节点为红：双黑父节点和双黑的兄弟换色
//     *              (b)若双黑父节点为黑：双黑的兄弟变红；对双黑结点的父节点成为双黑结点；对双黑结点继续处理
//     *              3.双黑结点的兄弟结点为红：
//     *              则双黑父节点必黑，双黑兄弟的两个子结点必黑，此时左（右）旋双黑的父节点；
//     *              双黑父变红，双黑父的父变黑，转化为前面两种情况之一
//     */
//}
//
//template <typename T>
//void RBTree<T>::delete_fix(RBNode<T> *x) {
//    RBNode<T> * y = x;
//    RBNode<T> * w;
//    // no case that has both children
//    while(y != root && y->getColor() == BLACK) {
//        if(y == (y->getFather())->getLeftChild()){ // delete one is the left child
//            w = y->getFather()->getRightChild(); // w is the sibling
//            if(w->getColor() == RED) { // left rotate + change color
//                w->setColor(BLACK);
//                (y->getFather())->setColor(RED);
//                leftRotate(y->getFather());
//                w = (y->getFather())->getRightChild(); // renew sibling
//            }// then enter other cases
//            if((w->getLeftChild() == nullptr || w->getLeftChild()->getColor() == BLACK)
//                && (w->getRightChild() == nullptr || w->getRightChild()->getColor() == BLACK)) {
//                // both children's color of sibling are BLACK
//                w->setColor(RED);
//                y = y->getFather();
//                break;
//            }
//            else if(w->getRightChild() == nullptr || (w->getRightChild())->getColor() == BLACK){
//                //only the right child's color is BLACK of sibling, left child is RED
//                (w->getLeftChild())->setColor(BLACK);
//                w->setColor(RED);
//                rightRotate(w);
//                w = (y->getFather())->getRightChild(); // renew sibling
//            }
//            // right is RED
//            w->setColor((y->getFather())->getColor());
//            (y->getFather())->setColor(BLACK);
//            (w->getRightChild())->setColor(BLACK);
//            leftRotate(y->getFather());
//            y = root;
//
//        }
//        else { //delete one is the right child
//            w = y->getFather()->getLeftChild(); // w is the sibling
//            if(w->getColor() == RED) { // right rotate + change color
//                w->setColor(BLACK);
//                (y->getFather())->setColor(RED);
//                rightRotate(y->getFather());
//                w = (y->getFather())->getLeftChild(); // renew sibling
//            }// then enter other cases
//            if((w->getLeftChild() == nullptr || w->getLeftChild()->getColor() == BLACK)
//               && (w->getRightChild() == nullptr || w->getRightChild()->getColor() == BLACK)) {
//                // both children's color of sibling are BLACK, or null
//                w->setColor(RED);
//                y = y->getFather();
//                break;
//            }
//            else if( w->getLeftChild() == nullptr || (w->getLeftChild()->getColor() == BLACK)){
//                //only the left child's color is BLACK of sibling, right child is RED
//                (w->getRightChild())->setColor(BLACK);
//                w->setColor(RED);
//                leftRotate(w);
//                w = y->getFather()->getLeftChild(); // renew sibling
//            }
//            // left is RED
//            w->setColor((y->getFather())->getColor());
//            (y->getFather())->setColor(BLACK);
//            (w->getLeftChild())->setColor(BLACK);
//            rightRotate(y->getFather());
//            y = root;
//
//        }
//    }
//    y->setColor(BLACK);
//}
//
//template <typename T>
//void RBTree<T>::print_tree() {
//    RBNode<T>* x = root;
//    int k = 0;
//    for(int i = 0; i < arraySize; i++){
//        RBArray[i][0] = -1;
//    }
//
//    // root
//    x->setOrder(1);
//    RBArray[0][0] = x->data;
//    RBArray[0][1] = BLACK;
//    // labels all nodes
//    while(1) {
//
//        { //   setOrder
//            RBNode<T>* child;
//            if (x->getLeftChild() != nullptr) {
//                child = x->getLeftChild();
//                child->setOrder(x->getOrder()*2);
//                RBArray[child->getOrder()-1][0] = child->getData();
//                RBArray[child->getOrder()-1][1] = child->getColor();
//            }
//            if(x->getRightChild() != nullptr) {
//                child = x->getRightChild();
//                child->setOrder(x->getOrder()*2+1);
//                RBArray[child->getOrder()-1][0] = child->getData();
//                RBArray[child->getOrder()-1][1] = child->getColor();
//            }
//        }
//        if(x->getLeftChild() != nullptr){
//            x = x->getLeftChild();
//            continue;
//        }
//        else if(x->getRightChild() != nullptr){
//            x = x->getRightChild();
//            continue;
//        }
//        else{
//            if(x->getFather() == nullptr)
//                break;
//
//            while(x == (x->getFather()->getLeftChild()) && (x->getFather())->getRightChild()== nullptr) { // find successor
//                x = x->getFather();
//                if(x->getFather() == nullptr)
//                    break;
//            }
//
//            while(x->getFather() != nullptr && x == (x->getFather()->getRightChild())){
//                x = x->getFather();
//                if(x->getFather() == nullptr)
//                    break;
//            }
//            if(x->getFather() == nullptr || (x->getFather())->getRightChild() == nullptr)
//                break;
//            x = x->getFather();
//            x = x->getRightChild();
//        }
//    }
//    int max_height = (log(size+1 + 1)/log(2)) + 2; // *2  。  实际上无+1, +2， 但*2， 范围太大了,屏幕放不下，
//    int root_width = int(2 *pow(2,max_height));
//    int width=0; //空格数
//    int depth=0, depth2 = -1;
//    int width_indicator, index_indicator, line_spacing;
//    //std::cout << root->getData()<<"B"<< std::endl; // print root
//    for(int i = 0; i < pow(2,max_height)-1; i++){
//
//        depth = int((float)log(i+1)/(float)log(2));
//        line_spacing = (int)print_width * (int)pow(2, max_height - depth);
//        index_indicator = (i == 0)? 0: int(pow(2,depth) + pow(2,(depth-1)) - 2);
//        width_indicator = root_width - line_spacing/2;
//        width = width_indicator + (i - index_indicator) * line_spacing;
//        if(RBArray[i][0] == 0){
//            std::cout<<std::setw(line_spacing)<<" ";
//            continue;
//        }
//        if(i == 0)
//            width = root_width;
//
//
////        if(i == 4){
////            for(int j = 0; j < width; j++)
////                std::cout<<" ";
////            std::cout << std::setw(print_width-1) << RBArray[i][0] <<((RBArray[i][1]==BLACK)?"B":"R");
////            continue;
////        }
//
//        if(depth != depth2){ //new line
//            depth2 = depth;
//            std::cout<<std::endl;
//            for(int j = 0; j < width; j++)
//                std::cout<<" ";
//        }
//        else {
//            std::cout<<std::setw(line_spacing)<<" ";
//        }
//        std::cout << std::setw(print_width-1) << RBArray[i][0] <<((RBArray[i][1]==BLACK)?"B":"R");
//    }
//    std::cout<<std::endl;
//}
//
//template <typename T>
//void RBTree<T>::print_lexi() {
//    element_list<T> *list = sort();
//    while(list){
//        std::cout<< list->node.data<< "   ";
//    }
//    std::cout<<std::endl;
//}
//
//template <typename T>
//void RBTree<T>::init(std::string filename) {
//    std::ifstream in(filename , std::ios::in);
//    if(!in.is_open()){
//        std::cout << "Error: opening file fail" << std::endl;
//        exit (1);
//    }
//    std::string line;
//    getline(in, line);
//    std::cout << line << std::endl;
//    while(!in.eof()){
//        Attr  initAttr;
//        in >> initAttr.word >> initAttr.partOfSpeech >> initAttr.frequency;
//        size++;
//        std::cout << initAttr.word <<","<<initAttr.partOfSpeech <<","<<initAttr.frequency<<std::endl;
//        insert_by_struct(initAttr);
//    }
//    in.close();
//}



#endif //PJ1_RBTREE_H