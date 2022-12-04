//
// Created by ohadr on 11/22/2022.
//
#include <memory>
#include <functional>
#include <iostream>
#include <cmath>
#include "Pair.h"
#ifndef WORLDCUP23A1_CPP_AVL_TREES_H
#define WORLDCUP23A1_CPP_AVL_TREES_H
#define ERROR -1

using std::unique_ptr;
using std::string;

enum class AVLStatus{AVL_Success, AVL_Fail, AVL_Not_Found};

template <class T , class K>
class AVLTree{
private:
    class Node;
    Node* m_dummyHead;
    //std::function<int(const T&, const T&)> compareFunc; ///TRUE = RIGHT, FALSE = LEFT

    void inOrderWithFuncAUX(Node* root, void* runFunc){
        if(root == nullptr){
            return;
        }
        inOrderWithFuncAUX(root->m_left);
        runFunc(root);
        inOrderWithFuncAUX(root->m_right);
    }

    int inOrderScanToArrAUX(Node* root, Pair<T,K>* arr, int top) const{ ///should get an input function
        if(!root || top == ERROR){
            return top;
        }
        top = inOrderScanToArrAUX(root->m_left, arr, top);
        try {
            arr[top] = *root->m_nodeData;
        }catch(...){
            return ERROR;
        }
//        root->printOut();
        top++;
        top = inOrderScanToArrAUX(root->m_right, arr, top);
        return top;
    }

    AVLStatus createTreeByListAUX(Node* root,Node* prev, Pair<T,K>* arr, int h,int bottom ,int top, string dir){ ///should get an input function
        if(h == -1 || bottom == top){
            return AVLStatus::AVL_Success;
        }
        int root_index = (top - bottom) >=  pow(2,h) + pow(2,h-1) - 1? (int)pow(2,h)-1 : (top - bottom) - (int)pow(2,h-1);
        root_index += bottom;
        try {
            root = new Node(arr[root_index], prev);
        }catch(std::bad_alloc& a){
            return AVLStatus::AVL_Fail;
        }
        if(dir == "l"){
            prev->m_left = root;
        }else{
            prev->m_right = root;
        }
        createTreeByListAUX(root->m_left, root, arr,h-1,bottom , root_index,"l");
        createTreeByListAUX(root->m_right, root, arr,h-1,root_index+1 , top,"r");
        updateNode(root);
        return AVLStatus::AVL_Success;
    }

    void FullRotation(Node* rotator, const string& dir1,const string& dir2){
        if(dir1 == dir2){
            rotator = rotator->m_prev;
            RotationAUX(rotator, dir1);
        }
        else {
            RotationAUX(rotator, dir1);
            rotator = rotator->m_prev; //root
            RotationAUX(rotator, dir2);
        }
    }

    void RotationAUX(Node* rotator , const string& dir){ ///get a pointer to the unstable node (-2)
        Node* root = rotator->m_prev;
        Node* rotatee;
        Node* weight_shift;
        if(dir == "l"){ //L rotation
            rotatee = rotator->m_left;
            weight_shift = rotatee->m_right;
            rotatee->m_right = rotator;
            rotator->m_left = weight_shift;
        }else if(dir == "r"){ // R rotation
            rotatee = rotator->m_right;
            weight_shift = rotatee->m_left;
            rotatee->m_left = rotator;
            rotator->m_right = weight_shift;
        }
        else{ // wrong use of this function
            return;
        }

        //adjust heirarchy
        if(weight_shift) {
            weight_shift->m_prev = rotator;
        }
        rotatee->m_prev = rotator->m_prev;
        rotator->m_prev = rotatee;
        if (rotatee->m_prev->m_left == rotator)
            rotatee->m_prev->m_left = rotatee;
        else
            rotatee->m_prev->m_right = rotatee;
        //adjust stats
        updateNode(rotator);
        updateNode(rotatee);
        //the new rotator position is now rotatee actually
        rotator = rotatee;
    }

    void updateNode(Node* nodeToUpdate){
        nodeToUpdate->setHeight();
        nodeToUpdate->setSize();
    }

    void updatePath(Node* start, bool updateRollsToRoot){
        do {
            updateNode(start);
            if(start == m_dummyHead){
                return;
            }
            while ((start->m_prev == m_dummyHead || abs(start->m_prev->heightDiff()) < 2)) { //climb to top
//                std::cout<<"data :" << start->data() << std::endl;
                start = start->m_prev;
                updateNode(start);
                if (start == m_dummyHead) { //top reached
                    return;
                }
            }
            string dir1, dir2;
            if (start->heightDiff() > 0) {
                dir1 = "l";
            } else {
                dir1 = "r";
            }
            if (start->m_prev->heightDiff() > 0) {
                dir2 = "l";
            } else {
                dir2 = "r";
            }
            FullRotation(start, dir1, dir2);
        }while(updateRollsToRoot && start->m_prev != m_dummyHead);

        while (start->m_prev) { // update heights
            start = start->m_prev;
            updateNode(start);
        }
    }

    ///-----------------------------help function for delete----------------------------------------
//    /// @brief help-function for destroyer of AVL_tree, works on post-order
//    /// @param ptr
    void deleter(Node* ptr){
        if(ptr) {
            deleter(ptr->m_left);
            deleter(ptr->m_right);
            delete ptr;
        }
    }

    ///get pointer to the head and pointer to the item we wanna find, make a recursive calls depend on the comparefunc of the tree, if didnt find / empty return nullptr.
    Node* findAUX(const K& item) const {
        if (empty())
            return nullptr;
        Node* result_ptr = m_dummyHead->m_left;
        while (result_ptr->key() != item) {
            result_ptr = result_ptr->key() > item ? result_ptr->m_left : result_ptr->m_right;
            if (!result_ptr)
                return nullptr;
        }
        return result_ptr;
    }

    Node* createCopy(Node* srcCopy, Node* prevDest){
        Node *destCopy = new Node(*srcCopy->m_nodeData, prevDest, srcCopy->m_height, srcCopy->m_size);
        if(srcCopy->m_left){
            destCopy->m_left = createCopy(srcCopy->m_left, destCopy);
        }
        if(srcCopy->m_right){
            destCopy->m_right = createCopy(srcCopy->m_right, destCopy);
        }
      //  destCopy->printOut();
        return destCopy;
    }

        int inOrderConditionToArrAUX(Node* root, int* size, Pair<T,K>* pairArray, K min = NULL, K max = NULL, int ptrArr = 0) const{
        if(root == nullptr){
            return ptrArr;
        }
        Node* ptr = m_dummyHead->m_left;
        inOrderConditionToArrAUX(ptr->m_left, size, pairArray, ptrArr, min, max);
        if(ptr->key() >= min && ptr->key() <= max){
            pairArray[ptrArr] = *ptr->m_nodeData;
            ptrArr++;
            size++;
        }
        inOrderConditionToArrAUX(ptr->m_right, size, pairArray, ptrArr, min, max);
        return ptrArr;
    }
public:

    AVLTree(): m_dummyHead(new Node()){}

    virtual ~AVLTree(){
        deleter(m_dummyHead);
    }

    AVLTree(const AVLTree& tree): m_dummyHead(new Node()){
        m_dummyHead->m_size = tree.m_dummyHead->m_size;
        m_dummyHead->m_height = tree.m_dummyHead->m_height;
        Node* ptrCopyTree = tree.m_dummyHead->m_left;
        m_dummyHead->m_left = createCopy(ptrCopyTree,m_dummyHead);
    }

    AVLTree& operator= (const AVLTree& tree){
        if(this == &tree)
            return *this;
        AVLTree<T,K> tempTree(tree);
        Node* swapper = tempTree.m_dummyHead;
        tempTree.m_dummyHead = this->m_dummyHead;
        this->m_dummyHead = swapper;
        return *this;
    }

    bool empty() const{
        return !m_dummyHead->m_left;
    }

    int size() const{
        return m_dummyHead->m_left->m_size;
    }

    AVLStatus insert(const T& newItem,  const K& key){
        Node** newNodePlace;
        Node* ptr = m_dummyHead;
        if (empty()) {
            newNodePlace = &m_dummyHead->m_left;
        }else{
            ptr = m_dummyHead->m_left;
            while (true) { //add to leafs
                if (ptr->key() > key) {
                    if (!ptr->m_right) {
                        newNodePlace = &ptr->m_right;
                        break;
                    }
                    ptr = ptr->m_right;
                } else if(key > ptr->key()){
                    if (!ptr->m_left) {
                        newNodePlace = &ptr->m_left;
                        break;
                    }
                    ptr = ptr->m_left;
                }
                else{
                    return AVLStatus::AVL_Fail;
                }
            }
        }try {
            *newNodePlace = new Node(newItem, key, ptr);
        }catch(...){
            return AVLStatus::AVL_Fail;
        }
        updatePath(*newNodePlace, false);
        return AVLStatus::AVL_Success;
    }

    AVLStatus remove(const K& keyDelete) {
        Node* ptr = findAUX(keyDelete);
        Node* lowestInPath;
        Node* toDelete;
        if (!ptr) //ptr to delete not found
            return AVLStatus::AVL_Not_Found;
        Node* replacement = ptr;
        if(replacement->m_right){
            replacement= replacement->m_right;
            while (replacement->m_left) {
                replacement = replacement->m_left;
            }
            ptr->m_nodeData = std::move(replacement->m_nodeData);
        }
        Node** fatherLeg = replacement->m_prev->m_left == replacement ? &replacement->m_prev->m_left : &replacement->m_prev->m_right;
        if(!replacement->m_right && !replacement->m_left){
            lowestInPath = replacement->m_prev;
            *fatherLeg = nullptr;
            toDelete = replacement;
        }
        else if(replacement->m_right) { //go to next node in inorder if has children
            lowestInPath = replacement->m_right;
            *fatherLeg = replacement->m_right;
            replacement->m_right->m_prev = replacement->m_prev;
            toDelete = replacement;
        }else{
            lowestInPath = replacement->m_left;
            *fatherLeg = replacement->m_left;
            replacement->m_left->m_prev = replacement->m_prev;
            toDelete = replacement;
        }
        delete toDelete;
        updatePath(lowestInPath,true);
        return AVLStatus::AVL_Success;
    }

        ///----------------------help functions for knockout------------------------
    Pair<T, K>* conditionArr(int* size, K min = NULL, K max = NULL) const{
        if(this->empty()){
            return nullptr;
        }
        Pair<T, K>* arr = new Pair<T, K>[this->size()];
        inOrderConditionToArrAUX(m_dummyHead->m_left, size, arr, min, max);
        return (arr);
    }

    int rank(const K& dataKey) const{
        if(empty()){
            return ERROR;
        }
        Node* ptr = m_dummyHead->m_left;
        int sideRank;
        int rankIndex = ptr->m_left ? ptr->m_left->m_size+1 : 1;
        while(ptr->key() != dataKey){
            if( dataKey > ptr->key() && ptr->m_right){
                ptr = ptr->m_right;
                sideRank = ptr->m_left ? ptr->m_left->m_size : 0;
                rankIndex +=  sideRank + 1;
            }else if(ptr->key() > dataKey && ptr->m_left){
                ptr = ptr->m_left;
                sideRank = ptr->m_right ? ptr->m_right->m_size : 0;
                rankIndex -= sideRank +1;
            }else{
                return ERROR;
            }
        }
        return rankIndex;
    }

    const Pair<T,K>& select(int rank) const{
        if(rank < 1 || rank > size()){
            return *m_dummyHead->m_nodeData;
        }
        Node* ptr = m_dummyHead->m_left;
        int leftSideSize;
        Node* result;
        while(rank > 0){
            result = ptr;
            leftSideSize = ptr->m_left ? ptr->m_left->m_size : 0;
            if(rank > leftSideSize) {
                rank -= leftSideSize+1;
                ptr = ptr->m_right;
            }
            else{
                ptr = ptr->m_left;
            }
        }
        return *result->m_nodeData;
    }

    const Pair<T,K>* find(const K& dataKey) const{ ///PROBLEM BECAUSE OF UNIQUE...CANT BE ASSIGN DO IT CANT RETURN AS A VALUE
        if (empty())
            return m_dummyHead->m_nodeData;
        Node* result_ptr = m_dummyHead->m_left;
        while (result_ptr->key() != dataKey) {
            result_ptr = result_ptr->key() > dataKey ? result_ptr->m_left : result_ptr->m_right;
            if (!result_ptr)
                return nullptr;
        }
        return m_dummyHead->m_nodeData;
    }
    /// @brief
    /// @param start
    /// @param updateRollsToRoot

    Pair<T,K>* inOrderScanToArray() const{ ///should get an input function
        Pair<T,K>* pairArray;
        try{
            pairArray = new Pair<T,K>[size()]();
        }catch (std::bad_alloc& a){
            return nullptr;
        }
        if(inOrderScanToArrAUX(m_dummyHead->m_left, pairArray, 0) == ERROR){
            delete[]pairArray;
            return nullptr;
        }
        return pairArray;
    }

    AVLStatus createTreeByList(int size ,Pair<T,K>* arr){
        int h = (int)log2(size + 1) ;
        Node* newTree = nullptr;
        Node* oldTree = m_dummyHead->m_left;
        if(createTreeByListAUX(newTree,m_dummyHead, arr,h, 0, size,"l") == AVLStatus::AVL_Success){
            deleter(oldTree);
            return  AVLStatus::AVL_Success;
        }
        else{
            deleter(newTree);
            return AVLStatus::AVL_Fail;
        }
    }


    void inOrderWithFunc(void* runFunc(T*)){
        inOrderScanToArrAUX(this->m_dummyHead->m_left);
    }


///----------------------implementation of Node class---------------------------
private:
    class Node{
        friend AVLTree;

        std::unique_ptr<Pair<T,K>> m_nodeData;
        Node* m_right;
        Node* m_left;
        Node* m_prev;
        int m_height;
        int m_size;

        Node(const T& dataPtr, const K& key, Node* previous) : m_nodeData(new Pair<T,K>(dataPtr,key)),
                                                                    m_right(nullptr),
                                                                    m_left(nullptr), m_prev(previous),
                                                                    m_height(0),m_size(1){}
        Node(const Pair<T,K>& pairData, Node* previous) : m_nodeData(new Pair<T,K>(pairData)),
                                                               m_right(nullptr),
                                                               m_left(nullptr), m_prev(previous),
                                                               m_height(0),m_size(1){}
        Node(const Pair<T,K>& pairData, Node* previous, int height, int size) : m_nodeData(new Pair<T,K>(pairData)),
                                                          m_right(nullptr),
                                                          m_left(nullptr), m_prev(previous),
                                                          m_height(height),m_size(size){}
        Node() : m_nodeData(nullptr),
                 m_right(nullptr),
                 m_left(nullptr),
                 m_prev(nullptr),
                 m_height(0),
                 m_size(1)
                 {}
    public:
        Node(const Node& n) = delete;
        Node& operator= (const Node& n) = delete;

        /// @brief getter for height
        /// @return
        bool empty(){
            return m_nodeData == nullptr;
        }
        T& data(){
            return *m_nodeData->data();
        }

        const K& key(){
            return *m_nodeData->key();
        }

        void printOut(){
            std::cout << "\n \\ \\ \\ \\ \\ \\ \\ \\ \\ \\ \\ \\ \\  printOut / / / / / / / / / / / \n" <<std::endl;
            string str1 = !m_prev->empty() ? m_prev->data() : "m_dummyNode";
            if(!empty()){
                std::cout << "this is printing: " << data() <<", key: "<< key() <<" (prev: "<< str1+")" << std::endl;
            }else{
                std::cout << "this is is printing: m_dummyNode" << std::endl;
            }
            if(!m_left && !m_right) {
                std::cout << "leaf" << std::endl;
            }else if(m_left && m_right){
                std::cout << "left: " <<  m_left->data()<<
                " (l_h: " <<m_left->m_height<<", l_s: "<< m_left->m_size <<") , right: "<<
                m_right->data() <<" (r_h: " <<m_right->m_height<<", r_s: "<< m_right->m_size <<
                ") "<<std::endl <<"h: " <<m_height << ", s:" << m_size<< std::endl;
            }else if(m_right){
                std::cout << "right: " << m_right->data() << std::endl << "height: " <<m_height << ", size: " << m_size<< std::endl;
            }else{
                std::cout << "left: " << m_left->data() << std::endl << "height: " <<m_height << ", size: " << m_size<< std::endl;
            }
        }

        void setSize(){
            if(!m_left && !m_right){
                m_size = 0;
            }else if(m_left && m_right){
                m_size = m_right->m_size + m_left->m_size;
            }else if(m_right){
                m_size = m_right->m_size;
            }else{
                m_size = m_left->m_size;
            }
            m_size++;
        }

        void setHeight(){
            if(!m_left && !m_right){
                m_height = 0;
                return;
            }else if(m_left && m_right){
                m_height = m_right->m_height > m_left->m_height ? m_right->m_height : m_left->m_height;
            }else if(m_right){
                m_height = m_right->m_height;
            }else{
                m_height = m_left->m_height;
            }
            m_height++;

        }
        /// @brief method to calculate balance factor
        /// @return
        int heightDiff(){
            int delta_h = 0;
            if(!m_left && !m_right){
                return delta_h;
            }else if(m_right && m_left){
                delta_h = m_left->m_height - m_right->m_height;
            }else if(m_left){
                delta_h = m_left->m_height + 1;
            }else{
                delta_h = -(m_right->m_height + 1);
            }
            return delta_h;
        }
        virtual ~Node() = default;
    };
};

template<class T, class K>
AVLTree<T,K>* mergeTrees(AVLTree<T,K>& avl1 , AVLTree<T,K>& avl2){
    Pair<T,K> *avl1Arr = avl1.inOrderScanToArray();
    Pair<T,K> *avl2Arr = avl2.inOrderScanToArray();
    if(!avl1Arr || !avl2Arr){
        return nullptr;
    }
    int l_len = avl1.size();
    int r_len = avl2.size();
    Pair<T, K> *destArr;
    try {
        destArr = new Pair<T, K>[l_len + r_len]();
    }catch(...){
        return nullptr;
    }

    int k= 0,l = 0, r = 0;
    while(l < l_len && r < r_len){
        if(avl1Arr[l].key() < avl2Arr[r].key())
            destArr[k++] = avl1Arr[l++];
        else
            destArr[k++] = avl2Arr[r++];
    }
    //add leftovers already sorted
    while (l < l_len) {
        destArr[k++] = avl1Arr[l++];
    }
    while (r < r_len) {
        destArr[k++] = avl2Arr[r++];
    }
    delete[] avl1Arr;
    delete[] avl2Arr;
    AVLTree<T,K>* result = new AVLTree<T,K>();
    if(result->createTreeByList(l_len+r_len, destArr) != AVLTree<T,K>::AVLStatus::AVL_Success){
        delete result;
        result = nullptr;
    }
    delete[] destArr;
    return result;
}


#endif //WORLDCUP23A1_CPP_AVL_TREES_H
//
// Created by ohadr on 11/22/2022.
//


