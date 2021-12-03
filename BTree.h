
#ifndef PJ1_BTREE_H
#define PJ1_BTREE_H

static const int M = 2; // minimum degree
static const int KEY_MAX = M * 2 - 1;
static const int KEY_MIN = M -1;
static const int C_MAX = M * 2; // children number
static const int C_MIN = M;


template <typename ValueType>
struct BNode {
    BNode(const ValueType& value)
        :isLeaf(true),
        keyNum(1){
        keys[0] = value;
        for (int i=0 ; i<C_MAX ; i++)
            child[i] = nullptr;
    }
    BNode()
    :   keyNum(0), isLeaf(true){
        for (int i=0 ; i<C_MAX ; i++)
            child[i] = nullptr;
    }

    BNode<ValueType>* child[C_MAX];
    ValueType keys[KEY_MAX];
    bool isLeaf;
    int keyNum;
};

template <class C>
class ITerator{
public:
    typedef BNode<C> Node;
    typedef ITerator<C> self;

    ITerator(Node* node= nullptr)
            :pNode(node)
    {}

    C& operator * (){
        return pNode->keys[0];
    }

    C* operator -> () {
        return &(operator*());
    }
    //compare
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
    void Successor(){  // error
        while(!pNode->isLeaf)
        {
            pNode = pNode->child[pNode->keyNum];
        }
    }
    void Predecessor() {  // error
        while(!pNode->isLeaf)
        {
            pNode = pNode->child[0];
        }
    }
    Node* getNode() {
        return pNode;
    }
private:
    Node* pNode;
};

template <class V, class K, class keyOfValue>
class BTree{
public:
    typedef ITerator<V> iterator;
    typedef BNode<V> Node;
public:
    BTree():t(M), pRoot(nullptr), size(0){
    }

    ~BTree(){
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
        return treeMax(pRoot)->child[treeMax(pRoot)->keyNum-1]; // if treemax, max not belonged
    }

    std::pair<iterator , bool> Insert(const V& data){
        size++;
        Node* pNewN = nullptr;
        if(getRoot() == nullptr){
            pRoot = pNewN = new Node(data);
        }
        else {
            if (search(pRoot, keyOfValue()(data)) != nullptr) {
                std::cout << keyOfValue()(data) << " already existed" << std::endl;
                size--;
                return make_pair(iterator(End()), false); // same key
            }
            auto *new_node = new Node();

            if (pRoot->keyNum == KEY_MAX) { // root can't be bigger
                new_node->keyNum = 0;// --------------0->1
                new_node->isLeaf = false;
                new_node->child[0] = pRoot;
                splitChild(new_node, 0);
                Insert_fixup(new_node, data);
                pRoot = new_node;
            } else {
                Insert_fixup(pRoot, data);
            }
        }
        return std::pair<iterator,bool>(iterator(pNewN), true);
    }

    void Delete(const K& el){
        const K ele = el;
        if(pRoot == nullptr)
            return ;
        // can't find,
        if(search(pRoot,ele) == nullptr){
            std::cout<<"delete: key "<<ele<<" missing"<<std::endl;
            return;
        }
        if(pRoot->keyNum == 1){
            if(pRoot->isLeaf){ // only one node in the tree
                Destroy(pRoot);
                pRoot = NULL;
                size--;
                return;
            }
            else {// have more than one nodes, but root only one
                // have at most two children
                Node* c1 = (pRoot) ->child[0];
                Node* c2 = (pRoot)->child[1];
                if ((c1->keyNum==KEY_MIN && c1->isLeaf) && (c2->keyNum==KEY_MIN &&c2->isLeaf)) { // both children have minimum key_numbers, and x is the deleted one
                    // merge two children , and then delete
                    mergeChild(pRoot,0);
                    //deleteNode(pRoot);
                    pRoot = c1;
                }
            }
        }
        size--;
        Delete_fixup(pRoot, ele);
    }

    int Find_key_of_x(Node* x, const K& key){
        if(x == nullptr) {
            return -1;
        }
        int k = 0;
        while( k < x->keyNum && key > keyOfValue()(x->keys[k])) // find until ele = key[k] or the first bigger, or the end
            k++;
        if(k < x->keyNum && key == keyOfValue()(x->keys[k])){ // find it
            return k;
        }
        else if(x->isLeaf){
            return -1;
        }
        else { // child
            return -1;
        }
    }

    iterator search(Node*x, const K& el) {
        const K ele = el;
        if(x == nullptr) {
            return iterator(nullptr);//---
        }
        int k = 0;
        while( k < x->keyNum && ele > keyOfValue()(x->keys[k])) // find until ele = key[k] or the first bigger, or the end
            k++;
        if(k < x->keyNum && ele == keyOfValue()(x->keys[k])){ // find it
            return iterator(x);
        }
        else if(x->isLeaf){
            return iterator(nullptr);//---
        }
        else {
            return search(x->child[k], ele);
        }
    }

    Node* getRoot(){
        return pRoot;
    }

    bool Empty(){
        if(pRoot->keyNum == 0 || pRoot == nullptr){
            return true;
        }
        return false;
    }

    Node* treeMin(Node* x){
        Node* y = x;
        if(y == nullptr)
            return nullptr;
        while(y->child[0] != nullptr)
            y = y->child[0];
        return y;
    }

    Node* treeMax(Node* x){
        Node *y = x;
        if(y == nullptr)
            return nullptr;
        while(y->child[y->keyNum] != nullptr)
            y = y->child[y->keyNum];
        return y;
    }

    void Update(const K& key, const V& newVal){
        iterator it = search(pRoot,key);
        Node* x = it.getNode();
        if(x == nullptr){
            std::cout << key<<" is missing"<<std::endl;
            Insert(newVal);
        }
        else { // find if
            int knum = Find_key_of_x(x, key);
            x->keys[knum] = newVal;
        }
        std::cout<<"\nupdate: "<<key<<std::endl;
        print_by_order();
    }

    void init(std::string filename){}

    void print_by_order(){
        isOrdered(pRoot);
    }

    void Clear(){
        Destroy(this->pRoot);//从根开始销毁
        size = 0;
    }

    size_t Size()const
    {
        return size;
    }

    iterator Find(const K& key) {
        if(search(pRoot, key) == nullptr){
            std::cout<< "Don't find "<<key<< std::endl;
            return iterator(nullptr);
        }
        else {
            return search(pRoot, key);
        }
    }

    void Print(Node* xx , int level) {
        Node *x = xx;
        int i = 0;
        if (x) {
            while (i < level - 2) {
                std::cout << " ";
                i++;
            }
            std::cout << "[ ";
            for (int j = 0; j < x->keyNum; j++) {
                std::cout << keyOfValue()(x->keys[j]) << " ";
            }
            std::cout << "]";

            if (!x->isLeaf) {
                int j = 0;
                for (; j <= x->keyNum; j++) {
                    if (j == 0)
                        std::cout << std::endl;
                    Print(x->child[j], level + 3);
                }
                j--;
                if (j == (x->keyNum) && x->child[j]->isLeaf)
                    std::cout << std::endl;
            } else if (x == pRoot) {
                std::cout << std::endl;
            }
        }
    }

private:
    void Insert_fixup(Node* x, V data){
        int i = x->keyNum;
        Node* child;
        // enough capacity
        if(x->isLeaf){
            while ( i>0 && (keyOfValue()(data) < keyOfValue()(x->keys[i-1]))) // 右移一位
            {
                x->keys[i] = x->keys[i-1];
                i--;
            }
            x->keys[i] = data;
            x->keyNum++;
        }
        else { // x isn't leaf
            while(i>0 && keyOfValue()(data) < keyOfValue()(x->keys[i-1]))
            {
                i--; // find position
            }
            child = x->child[i];
            if (child->keyNum == KEY_MAX){ // child[i] is full
                // split child
                splitChild(x, i);
                // get a new key of x
                if (keyOfValue()(data) > keyOfValue()(x->keys[i])){
                    i++;
                    child = x->child[i];
                }
            }
            Insert_fixup(child, data);
        }
    }

    void Delete_fixup(Node* x, const K& el){ //const K& el ???
        // ele被修改
        const K  ele = el;
        int i = x->keyNum;
        if (x->isLeaf){ // delete one is leaf
            while (i>0 && (ele < keyOfValue()(x->keys[i-1])))
                i--; // find position
            if(ele == keyOfValue()(x->keys[i-1])){ // find it
                if(i == x->keyNum){
                    ;
                }
                else {
                    for (int j = i; j < x->keyNum; j++) {// 左移
                        x->keys[j - 1] = x->keys[j];
                    }
                }
                x->keys[x->keyNum-1] = V();
                x->keyNum--;
            }
        }
        else { // not leaf
            while (i>0 && (ele < keyOfValue()(x->keys[i-1]))) {
                i--;
            }
            if (i>0 && (ele == keyOfValue()(x->keys[i-1]))) { // find it
                Node* child1 = x->child[i-1];
                Node* child2 = x->child[i];
                // 左Child 多，从左Child移一个
                if (child1->keyNum > KEY_MIN){
                    Node* preKey = rightMost(child1);
                    x->keys[i-1] = preKey->keys[preKey->keyNum-1];
                    Delete_fixup(child1, keyOfValue()(preKey->keys[preKey->keyNum-1]));
                }
                //右Child 多，从右Child移一个
                else if (child2->keyNum > KEY_MIN) {
                    Node* sucKey = leftMost(child2); // must be leaf
                    x->keys[i-1] = sucKey->keys[0];
                    Delete_fixup(child2, keyOfValue()(sucKey->keys[0]));
                }
                //左右都是M-1，合并
                else{
                    mergeChild(x, i-1);
                    Delete_fixup(child1, ele);
                }
            }
            else { // ele 在 child[i]里
                Node* subTree = x->child[i];
                Node* right = nullptr; //------------
                if(i < x->keyNum) {
                    right = x->child[i+1];
                }
                Node * left = nullptr;
                if(i > 0) {
                    left = x->child[i-1];
                }

                if(subTree->keyNum == KEY_MIN){ // need merge or substitute, then delete
                    if (left && left->keyNum > KEY_MIN) { // Replace with ele in the left brothers
                        getFromLeftBrother(x, i-1, subTree, left); //  ????? ele被改变了？这个bug找了好久，终于找出来了
                       // 为什么ele会被改变
                    }
                    else if(right && right->keyNum > KEY_MIN) { // Replace with ele in the right brothers
                        getFromRightBrother(x, i, subTree, right);
                    }
                    else if (left && left->keyNum == KEY_MIN){ // left brother also very small, then merge them
                        mergeChild(x, i-1);  //---------------------------------
                        subTree = left;
                    }
                    else if (right && right->keyNum == KEY_MIN) { // right brother also very small, then merge them
                        mergeChild(x, i);  // ------------------------
                        // subtree is still itself
                    }
                    Delete_fixup(subTree, ele);
                }
                Delete_fixup(subTree, ele);

            }
        }
    }

    bool splitChild(Node* x, int childIndex) {
        Node* child1 = x->child[childIndex];
        Node* child2 = new Node();
        //copy
        child2->isLeaf = child1->isLeaf;
        //设置分裂节点key数
        child2->keyNum = KEY_MIN;
        for (int i=0;i<KEY_MIN;i++){
            child2->keys[i] = child1->keys[i+t];
        }
        //如果不是叶节点，复制指针
        if (!child1->isLeaf){
            for (int i=0;i<C_MIN;i++)
            {
                child2->child[i] = child1->child[i+t];
            }
        }
        child1->keyNum = KEY_MIN;


        //将中间数作为索引插入到双亲节点中
        //插入点后面的关键字和指针都往后移动一个位置
        for (int i=x->keyNum; i>childIndex; i--)
        {
            x->child[i+1] = x->child[i];
        }
//        for (int j=x->keyNum; j>=childIndex; j--)
//        {
//            x->keys[j] = x->keys[j-1];
//        }
        for (int j=x->keyNum; j>childIndex; j--)
        {
            x->keys[j] = x->keys[j-1];
        }
        x->keys[childIndex] = child1->keys[M-1];

        x->child[childIndex+1] = child2;
        x->keyNum++;

        // clear child others
        for(int i = child1->keyNum; i < KEY_MAX; i++)
            child1->keys[i] = V();
        for(int i = child1->keyNum+1; i <= KEY_MAX; i++)
            child1->child[i] = nullptr;

        return true;
    }

    void mergeChild(Node *x, int index) {
        Node *child1 = x->child[index];
        Node *child2 = x->child[index + 1];
        //将child2数据 合并到child1, 最终结果为child1
        child1->keyNum = KEY_MAX;
        //
        for (int i = t; i < KEY_MAX; i++) {
            child1->keys[i] = child2->keys[i - t];
        }
        child1->keys[t - 1] = x->keys[index];
        if (!child2->isLeaf) {
            for (int i = t; i < C_MAX; i++) {
                child1->child[i] = child2->child[i - t];
            }
        }

        //父节点删除index的key，index后的往前移动一位
        for (int i = index +1; i < (x->keyNum); i++) {
            x->keys[i-1] = x->keys[i];
            x->child[i ] = x->child[i + 1];
        }

        //删除child2

        x->keyNum--;
        deleteNode(child2);
    }

    Node* successor(Node* x){
        Node* xx = x;
        while(!xx->isLeaf){
            xx = xx->child[xx->keyNum];
        }
        return xx;
    } //un debug

    Node* predecessor(Node* x) {
        Node* xx = x;
        Node* y = pRoot;
        if(xx->isLeaf) { // from root get
            int i = 0;
            while (1) {
                i = 0;
                while (keyOfValue()(y->keys[i]) < keyOfValue()(x->keys[0]))
                    i++;
                for(int k = 0; k < y->child[i]->keyNum; k++){
                    if(y->child[i]->child[k] == x)
                        if(k == 0){

                        }
                        xx = y->child[i]->child[k-1]; // k>0
                }
                if ((Find_key_of_x(y->child[i], x->keys[0])) != -1) {
                    // x = y->child[i]
                    y = y->child[i - 1];
                    break;
                } else {
                    y = y->child[i];
                }
            }
            xx = y;
        }
        while(!xx->isLeaf) {
            xx = xx->child[0];
        }
        return xx;
    } //no debug

    Node* leftMost(Node * x){
        Node * y = x;
        while(!y->isLeaf) {
            y = y->child[0];
        }
        return y;
    }

    Node* rightMost(Node* x){
        Node * y = x;
        while(!y->isLeaf) {
            y = y->child[y->keyNum];
        }
        return y;
    }

    void getFromLeftBrother(Node *father, int index, Node *x, Node *leftBrother) {
        for(int i=x->keyNum ; i>0 ; i--){
            x->keys[i] = x->keys[i-1]; // 右移，腾出空间
        }
        x->keys[0] = father->keys[index]; //赋值 key, father ele replace x[0]
        if (!x->isLeaf){
            for(int i=x->keyNum ; i>=0 ; i--){
                x->child[i+1] = x->child[i]; //指针右移
            }
            x->child[0] = leftBrother->child[leftBrother->keyNum]; // 赋值 child pointer
        }
        x->keyNum++;

        //father
        father->keys[index] = leftBrother->keys[leftBrother->keyNum-1]; // left brother replace father

        //leftBrother
        leftBrother->keyNum--;
    }

    void getFromRightBrother(Node *father, int index, Node *x, Node *rightBrother) {
        // x
        x->keys[x->keyNum] = father->keys[index]; // 赋值 key, father ele replace x[keynum]
        x->child[x->keyNum+1] = rightBrother->child[0]; // 赋值 child pointer
        x->keyNum++;

        //father
        father->keys[index] = rightBrother->keys[0]; // left brother replace father

        //rightBrother
        for(int i=0 ; i < rightBrother->keyNum-1 ; i++){
            rightBrother->keys[i] = rightBrother->keys[i+1]; // 左移，收回空间
        }
        if(!rightBrother->isLeaf) {
            for(int i=0 ; i<rightBrother->keyNum ; i++){
                rightBrother->child[i] = rightBrother->child[i+1]; //指针左移
            }
        }
        rightBrother->keyNum--;
    }

    void Destroy(Node *x) {
        if (x) {
//            if(x == pRoot){
//                pRoot = nullptr;
//                return;
//            }------------1
            for(int i=0 ; i<=x->keyNum ; i++){
                if (x->isLeaf)
                    break;
                if (x->child[i])
                    Destroy(x->child[i]);
            }
            deleteNode(x);
        }
    }

    void deleteNode(Node* &x) {
        if (x) {
            delete x;
            x = nullptr;
        }
    }

    void isOrdered(Node * x) {
        Node* y = x;
        int k = 0;
        if(x!= nullptr){
            k = 0;
            isOrdered(y->child[k]);
            for(; k < y->keyNum; k++){
                print_value(y, k);
                isOrdered(y->child[k + 1]);

            }
           // std::cout<<std::endl;
        }
    }

    //   change by different struct
    void print_value(Node* x, int knum){
        V v = x->keys[knum];
        //std::cout<<v[keyOfValue()(x->keys[knum])]<<std::endl;
        //std::cout<<(x->keys[knum])[keyOfValue()(x->keys[knum])];//<<" "<<(x->keys[knum]).second.partOfSpeech<<" "<<x->keys[knum].second.frequency<<std::endl;
    }


    Node* pRoot;
    int t; // minimum degree
    size_t size;
};


//
//class BTree{
//public:
//    typedef Iterator<V> iterator;
//    typedef BNode<V> Node;
//
//public:
//    BTree(){
//        t = M;
//        root = nullptr;
//    }
//    bool insert(T ele);
//    bool Delete(T ele);
//    bool search(BNode<T>* x, T ele);
//    void print();
//    void print(BNode<T>* x, int level);
//
//private:
//    void getFromLeftBrother(BNode<T>* father, int index, BNode<T>* x, BNode<T>* leftBrother);
//    void getFromRightBrother(BNode<T>* father, int index, BNode<T>* x, BNode<T>* rightBrother);
//    T predecessor(BNode<T>* x);
//    T successor(BNode<T>* x);
//    void DestroyTree(BNode<T>* x);
//    void InsertNoneFull(BNode<T>* x, T ele);
//    void DeleteNoneEmpty(BNode<T>* x, T ele);
//    bool splitChild(BNode<T>* x, int childIndex);
//    void mergeChild(BNode<T>* x, int index);
//    void deleteNode(BNode<T>* &x);
//    int t; // minimum degree
//    BNode<T>* root;
//};
//
//
//template <typename T>
//bool BTree<T>::insert(T ele) {
//    if(search(root,ele)) {
//        std::cout<<ele<<" already existed"<< std::endl;
//        return true;
//    }
//    auto* new_node = new BNode<T>();
//
//    if (root == nullptr) {// empty
//        root = new_node;
//    }
//
//    if(root->keyNum == KEY_MAX) { // root can't be bigger
//        new_node->keyNum = 0;
//        new_node->isLeaf = false;
//        new_node->child[0] = root;
//        splitChild(new_node, 0);
//        InsertNoneFull(new_node, ele);
//        root = new_node;
//    }
//    else {
//        InsertNoneFull(root, ele);
//    }
//    return true;
//}
//
//template <typename T>
//bool BTree<T>::Delete(T ele) {
//    if(root == nullptr)
//        return true;
//    // can't find,
//    if(!search(root,ele)){
//        std::cout<<"key "<<ele<<" missing"<<std::endl;
//        return true;
//    }
//    if(root->keyNum == 1){
//        if(root->isLeaf){ // only one node in the tree
//            DestroyTree(root);
//            root = NULL;
//            return true;
//        }
//        else {// have more than one nodes, but root only one
//            // have at most two children
//            BNode<T>* c1 = root->child[0];
//            BNode<T>* c2 = root->child[1];
//            if (c1->keyNum==KEY_MIN && c2->keyNum==KEY_MIN) { // both children have minimum key_numbers
//                // merge two children , and then delete
//                mergeChild(root,0);
//                deleteNode(root);
//                root = c1;
//            }
//        }
//    }
//    DeleteNoneEmpty(root, ele);
//    return true;
//}
//
//template <typename T>
//bool BTree<T>::search(BNode<T>* x, T ele) {
//    if(x == nullptr) {
//        return false;
//    }
//    int k = 0;
//    while( k < x->keyNum && ele > x->keys[k]) // find until ele = key[k] or the first bigger, or the end
//        k++;
//    if(k < x->keyNum && ele == x->keys[k]){ // find it
//        std::cout<<"find key "<<ele<< std::endl;
//        return true;
//    }
//    else if(x->isLeaf){
//        return false;
//    }
//    else {
//        return search(x->child[k], ele);
//    }
//}
//
//template <typename T>
//bool BTree<T>::splitChild(BNode<T>* x, int childIndex) {
//    BNode<T>* child = x->child[childIndex];
//    BNode<T>* child2 = new BNode<T>();
//    //copy
//    child2->isLeaf = child->isLeaf;
//    //设置分裂节点key数
//    child2->keyNum = KEY_MIN;
//    for (int i=0;i<KEY_MIN;i++){
//        child2->keys[i] = child->keys[i+M];
//    }
//    //如果不是叶节点，复制指针
//    if (!child->isLeaf){
//        for (int i=0;i<C_MIN;i++)
//        {
//            child2->child[i] = child->child[i+M];
//        }
//    }
//    child->keyNum = KEY_MIN;
//
//
//    //将中间数作为索引插入到双亲节点中
//    //插入点后面的关键字和指针都往后移动一个位置
//    for (int i=x->keyNum; i>childIndex; i--)
//    {
//        x->keys[i] = x->keys[i-1];
//        x->child[i+1] = x->child[i];
//    }
//    x->keys[childIndex] = child->keys[M-1];
//    x->keyNum++;
//    x->child[childIndex+1] = child2;
//
//    // clear child others
//    for(int i = child->keyNum; i < KEY_MAX; i++)
//        child->keys[i] = NULL;
//
//    return true;
//}
//
//template <typename T>
//void BTree<T>::InsertNoneFull(BNode<T> *x, T ele) {
//    int i = x->keyNum;
//    BNode<T>* child;
//    bool iii = x->isLeaf;
//    if(iii){
//        while (i>0 && (ele < x->keys[i-1])) // 右移一位
//        {
//            x->keys[i] = x->keys[i-1];
//            i--;
//        }
//        x->keys[i] = ele;
//        x->keyNum++;
//    }
//    else { // x isn't leaf
//        while(i>0 && ele < x->keys[i-1])
//        {
//            i--; // find position
//        }
//        child = x->child[i];
//        if (child->keyNum == KEY_MAX){ // child[i] is full
//            // split child
//            splitChild(x, i);
//            // get a new key of x
//            if (ele > x->keys[i]){
//                i++;
//                child = x->child[i];
//            }
//        }
//        InsertNoneFull(child, ele);
//    }
//}
//
//template <typename T>
//void BTree<T>::DeleteNoneEmpty(BNode<T> *x, T ele) {
//    int i = x->keyNum;
//    if (x->isLeaf){ // delete one is leaf
//        while (i>0 && (ele < x->keys[i-1]))
//            i--; // find position
//        if(ele == x->keys[i-1]){ // find it
//            for(int j=i ; j<x->keyNum ; j++) {// 左移
//                x->keys[j-1] = x->keys[j];
//            }
//            x->keyNum--;
//        }
//    }
//    else { // not leaf
//        while (i>0 && (ele < x->keys[i-1])) {
//            i--;
//        }
//        if (i>0 && ele==x->keys[i-1]) { // find it
//            BNode<T>* child1 = x->child[i-1];
//            BNode<T>* child2 = x->child[i];
//            // 左Child 多，从左Child移一个
//            if (child1->keyNum > KEY_MIN){
//                T preKey = predecessor(child1);
//                DeleteNoneEmpty(child1, preKey);
//                x->keys[i-1] = preKey;
//            }
//            //右Child 多，从右Child移一个
//            else if (child2->keyNum > KEY_MIN) {
//                T sucKey = successor(child2);
//                DeleteNoneEmpty(child2, sucKey);
//                x->keys[i-1] = sucKey;
//            }
//            //左右都是M-1，合并
//            else{
//                mergeChild(x, i-1);
//                DeleteNoneEmpty(child1, ele);
//            }
//        }
//        else { // ele 在 child[i]里
//            BNode<T>* subTree = x->child[i];
//            if(subTree->keyNum == KEY_MIN){ // need merge or substitute, then delete
//                BNode<T>* left = (i>0 ? x->child[i-1] : NULL);
//                BNode<T>* right = (i < x->keyNum ? x->child[i+1] : NULL);
//                if (left && left->keyNum > KEY_MIN) { // Replace with ele in the left brothers
//                    getFromLeftBrother(x, i-1, subTree, left);
//                }
//                else if(right && right->keyNum > KEY_MIN) { // Replace with ele in the right brothers
//                    getFromRightBrother(x, i-1, subTree, right);
//                }
//                else if (left && left->keyNum == KEY_MIN){ // left brother also very small, then merge them
//                    mergeChild(x, i-1);
//                    subTree = left;
//                }
//                else if (right && right->keyNum == KEY_MIN) { // right brother also very small, then merge them
//                    mergeChild(x, i);
//                    // subtree is still itself
//                }
//            }
//            DeleteNoneEmpty(subTree, ele);
//        }
//    }
//}
//
//template <typename T>
//void BTree<T>::mergeChild(BNode<T> *x, int index) {
//    BNode<T>* child1 = x->child[index];
//    BNode<T>* child2 = x->child[index+1];
//    //将child2数据 合并到child1, 最终结果为child1
//    child1->keyNum = KEY_MAX;
//    child1->keys[t-1] = x->keys[index];
//    for (int i=t ; i<KEY_MAX ; i++){
//        child1->keys[i] = child2->keys[i-t];
//    }
//    if (!child1->isLeaf){
//        for (int i=t ; i<C_MAX ; i++){
//            child1->child[i] = child2->child[i-t];
//        }
//    }
//
//    //父节点删除index的key，index后的往前移动一位
//    x->keyNum--;
//    for (int i=index ; i<(x->keyNum) ; i++){
//        x->keys[i] = x->keys[i+1];
//        x->child[i+1] = x->child[i+2];
//    }
//
//    //删除child2
//    deleteNode(child2);
//}
//
//template <typename T>
//T BTree<T>::predecessor(BNode<T>* x){
//    while(!x->isLeaf)
//    {
//        x = x->child[x->keyNum];
//    }
//    return x->keys[x->keyNum-1];
//
//}
//template <typename T>
//T BTree<T>::successor(BNode<T>* x) {
//    while(!x->isLeaf)
//    {
//        x = x->child[0];
//    }
//    return x->keys[0];
//}
//
//template <typename T>
//void BTree<T>::getFromLeftBrother(BNode<T> *father, int index, BNode<T> *x, BNode<T> *leftBrother) {
//    for(int i=x->keyNum ; i>0 ; i--){
//        x->keys[i] = x->keys[i-1]; // 右移，腾出空间
//    }
//    x->keys[0] = father->keys[index]; //赋值 key, father ele replace x[0]
//    if (!x->isLeaf){
//        for(int i=x->keyNum ; i>=0 ; i--){
//            x->child[i+1] = x->child[i]; //指针右移
//        }
//        x->child[0] = leftBrother->child[leftBrother->keyNum]; // 赋值 child pointer
//    }
//    x->keyNum++;
//
//    //father
//    father->keys[index] = leftBrother->keys[leftBrother->keyNum-1]; // left brother replace father
//
//    //leftBrother
//    leftBrother->keyNum--;
//}
//
//template <typename T>
//void BTree<T>::getFromRightBrother(BNode<T> *father, int index, BNode<T> *x, BNode<T> *rightBrother) {
//    // x
//    x->keys[x->keyNum] = father->keys[index]; // 赋值 key, father ele replace x[keynum]
//    x->child[x->keyNum+1] = rightBrother->child[0]; // 赋值 child pointer
//    x->keyNum++;
//
//    //father
//    father->keys[index] = rightBrother->keys[0]; // left brother replace father
//
//    //rightBrother
//    for(int i=0 ; i < rightBrother->keyNum-1 ; i++){
//        rightBrother->keys[i] = rightBrother->keys[i+1]; // 左移，收回空间
//    }
//    x->keys[0] = father->keys[index]; //赋值 key, father ele replace x[0]
//    if(!rightBrother->isLeaf) {
//        for(int i=0 ; i<rightBrother->keyNum ; i++){
//            rightBrother->child[i] = rightBrother->child[i+1]; //指针左移
//        }
//    }
//    rightBrother->keyNum--;
//}
//
//template <typename T>
//void BTree<T>::DestroyTree(BNode<T> *x) {
//    if (x) {
//        for(int i=0 ; i<=x->keyNum ; i++){
//            if (x->isLeaf)
//                break;
//            if (x->child[i])
//                DestroyTree(x->child[i]);
//        }
//        deleteNode(x);
//    }
//}
//
//template <typename T>
//void BTree<T>::deleteNode(BNode<T>* &x) {
//    if (x){
//        delete x;
//        x = NULL;
//    }
//
//}
//
//template <typename T>
//void BTree<T>::print(){
//    print(root,2);
//}
//
//template <typename T>
//void BTree<T>::print(BNode<T>* x , int level) {
//    int i=0;
//    if (x){
//        while(i<level-2){
//            std::cout<<" ";
//            i++;
//        }
//        std::cout<< "[ " ;
//        for (int j=0; j<x->keyNum; j++){
//            std::cout<<x->keys[j]<<" ";
//        }
//        std::cout<< "]";
//
//        if(!x->isLeaf){
//            int j = 0;
//            for (; j<=x->keyNum; j++)
//            {
//                if(j == 0)
//                    std::cout<< std::endl;
//                print(x->child[j], level+3);
//            }
//            j--;
//            if( j == (x->keyNum) && x->child[j]->isLeaf)
//                std::cout<< std::endl;
//        } else if(x == root){
//            std::cout<< std::endl;
//        }
//    }
//}

#endif //PJ1_BTREE_H
