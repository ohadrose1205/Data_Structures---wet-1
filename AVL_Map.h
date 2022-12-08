//
// Created by ohadr on 11/22/2022.
//
#include <memory>
#include <functional>
#include <iostream>
#include <cmath>
#include "Pair.h"
#include "wet1util.h"
#ifndef WORLDCUP23A1_CPP_AVL_TREES_H
#define WORLDCUP23A1_CPP_AVL_TREES_H
#define ERROR -1

using std::shared_ptr;
using std::unique_ptr;
using std::string;

template <class T , class K>
class AVLTree{
private:
    class Node;
    Node* m_dummyHead;

    int inOrderScanToArrAUX(Node* root, Pair<T,K>** arr, int top, const K& minKey, const K& maxKey) const{ ///should get an input function
        if(!root || top == ERROR){
            return top;
        }
        if(root->key() < minKey){
            return inOrderScanToArrAUX(root->m_right, arr, top , minKey, maxKey);
        }else{
            top = inOrderScanToArrAUX(root->m_left, arr, top , minKey, maxKey);
        }
        if(root->key() <= maxKey){
            arr[top] = &*root->m_nodeData;
            top++;
            top = inOrderScanToArrAUX(root->m_right, arr, top , minKey, maxKey);
        }
        return top;
    }

    void createTreeByListAUX(Node* root,Node* prev, Pair<T,K>* arr, int h,int bottom ,int top, string dir){ ///should get an input function
        if(h == -1 || bottom == top) return;
        int root_index = (top - bottom) >=  pow(2,h) + pow(2,h-1) - 1? (int)pow(2,h)-1 : (top - bottom) - (int)pow(2,h-1);
        root_index += bottom;
        root = new Node(arr[root_index], prev);
        if(dir == "l"){
            prev->m_left = root;
        }else{
            prev->m_right = root;
        }
        createTreeByListAUX(root->m_left, root, arr,h-1,bottom , root_index,"l");
        createTreeByListAUX(root->m_right, root, arr,h-1,root_index+1 , top,"r");
        updateNode(root);
    }

    void FullRotation(Node* rotator, const string& dir1,const string& dir2){
        if(dir1 == dir2){
            rotator = rotator->m_prev;
            RotationAUX(rotator, dir1);
        }
        else {
            RotationAUX(rotator, dir1);
            rotator = rotator->m_prev->m_prev; //root
            RotationAUX(rotator, dir2);
        }
    }

    void RotationAUX(Node* rotator , const string& dir){ ///get a pointer to the unstable node (-2)
        Node* rotatee;
        Node* weight_shift;
        if(dir == "l"){ //L rotation from left to right
            rotatee = rotator->m_left;
            weight_shift = rotatee->m_right;
            rotatee->m_right = rotator;
            rotator->m_left = weight_shift;
        }else if(dir == "r"){ // R rotation from right to left
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
                start = start->m_prev;
                updateNode(start);
                if (start == m_dummyHead) { //top reached
                    return;
                }
            }
            string dir1, dir2;
            if (start->m_prev->heightDiff() > 0) {
                if (start->heightDiff() > -1) {
                    dir1 = "l";
                }else {
                    dir1 = "r";
                }
                dir2 = "l";
            }else {
                if (start->heightDiff()< 1) {
                    dir1 = "r";
                } else {
                    dir1 = "l";
                }
                dir2 = "r";
            }
            //start->printOut();
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
        if(!ptr) return;

        deleter(ptr->m_left);
        deleter(ptr->m_right);

        delete ptr;
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
        if(!srcCopy)
            return nullptr;
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
        return (m_dummyHead->m_size-1);
    }

    StatusType insert(const T& newItem,  const K& key){
        Node** newNodePlace;
        Node* ptr = m_dummyHead;
        if (empty()) {
            newNodePlace = &m_dummyHead->m_left;
        }else{
            ptr = m_dummyHead->m_left;
            while (true) { //add to leafs
                if (ptr->key() < key) {
                    if (!ptr->m_right) {
                        newNodePlace = &ptr->m_right;
                        break;
                    }
                    ptr = ptr->m_right;
                } else if(ptr->key() > key){
                    if (!ptr->m_left) {
                        newNodePlace = &ptr->m_left;
                        break;
                    }
                    ptr = ptr->m_left;
                }
                else{
                    return StatusType::FAILURE;
                }
            }
        }try {
            *newNodePlace = new Node(newItem, key, ptr);
        }catch(...){
            return StatusType::ALLOCATION_ERROR;
        }
        updatePath(*newNodePlace, false);
        return StatusType::SUCCESS;
    }

    StatusType remove(const K& keyDelete) {
        Node* ptr = findAUX(keyDelete);
        Node* lowestInPath;
        Node* toDelete;
        if (!ptr) //ptr to delete not found
            return StatusType::FAILURE;
        Node* replacement = ptr;
        if(replacement->m_right){
            replacement= replacement->m_right;
            while (replacement->m_left) {
                replacement = replacement->m_left;
            }
            ptr->m_nodeData = std::move(replacement->m_nodeData);
            replacement->m_nodeData = nullptr;
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
        return StatusType::SUCCESS;
    }

    int rank(const K& dataKey) const{
        if(empty()){
            return ERROR;
        }
        Node* ptr = m_dummyHead->m_left;
        int sideRank;
        int rankIndex = ptr->m_left ? ptr->m_left->m_size+1 : 1;
        while(ptr->key() != dataKey){
            if(ptr->key() < dataKey && ptr->m_right){
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

    Pair<T,K>& select(int rank){
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

    Pair<T,K>& find(const K& dataKey) const{
        if (empty())
            return *m_dummyHead->m_nodeData;
        Node* result_ptr = m_dummyHead->m_left;
        while (result_ptr->key() != dataKey) {
            result_ptr = result_ptr->key() > dataKey ? result_ptr->m_left : result_ptr->m_right;
            if (!result_ptr)
                return *m_dummyHead->m_nodeData;
        }
        return *result_ptr->m_nodeData;
    }
    /// @brief
    /// @param start
    /// @param updateRollsToRoot

    Pair<T,K>** inOrderScanToArray() const{ ///should get an input function
        Pair<T,K>** pairArray;
        try{
            pairArray = new Pair<T,K>*[size()+1]();
        }catch (std::bad_alloc& a){
            return nullptr;
        }
        pairArray[size()] = nullptr;
        if(inOrderScanToArrAUX(m_dummyHead->m_left, pairArray, 0, select(1).key(),select(size()).key()) == ERROR){
            delete[] pairArray;
            pairArray = nullptr;
        }
        return pairArray;
    }
    Pair<T,K>** inOrderScanToArrayFromTo(int& arrSize, const K& from, const K& to) const{ ///should get an input function
        Pair<T,K>** pairArray;
        try{
            pairArray = new Pair<T,K>*[size()+1]();
        }catch (std::bad_alloc& a){
            return nullptr;
        }
        arrSize = inOrderScanToArrAUX(m_dummyHead->m_left, pairArray, 0, from,to);
        if(arrSize <= 0){
            delete[] pairArray;
            return nullptr;
        }
        pairArray[arrSize] = nullptr;
        return pairArray;
    }

    StatusType createTreeByList(int size ,Pair<T,K>* arr){
        int h = (int)log2(size + 1) ;
        Node* newTree = new Node();
        try{
            createTreeByListAUX(newTree->m_left,newTree, arr,h, 0, size,"l");
        }catch(std::bad_alloc& ba) {
            deleter(newTree);
            newTree = nullptr;
            return StatusType::ALLOCATION_ERROR;
        }
        m_dummyHead->m_left = newTree->m_left;
        updateNode(m_dummyHead);
        delete newTree;
        newTree = nullptr;
        return StatusType::SUCCESS;

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
        Node() : m_nodeData(new Pair<T,K>()),
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
            return m_nodeData->empty();
        }
        T& data(){
            return m_nodeData->data();
        }

        const K& key(){
            return m_nodeData->key();
        }

        void printOut(){
            std::cout << " \\ \\ \\ \\ \\ \\ \\ \\ \\ \\ \\ \\ \\  printOut / / / / / / / / / / / \n" <<std::endl;
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
        ~Node(){
            m_nodeData.reset();
        }
    };
};

template<class T, class K>
AVLTree<T,K>* mergeTrees(const AVLTree<T,K>& avl1 ,const AVLTree<T,K>& avl2){
    Pair<T,K> **avl1Arr = avl1.inOrderScanToArray();
    if(!avl1Arr){
        return nullptr;
    }
    Pair<T,K> **avl2Arr = avl2.inOrderScanToArray();
    if(!avl2Arr){
        delete[] avl1Arr;
        avl1Arr = nullptr;
        avl2Arr= nullptr;
        return nullptr;
    }
    int l_len = avl1.size();
    int r_len = avl2.size();
    Pair<T, K> *destArr;
    try {
        destArr = new Pair<T, K>[l_len + r_len]();
    }catch(...){
        delete[] avl1Arr;
        delete[] avl2Arr;
        avl1Arr = nullptr;
        avl2Arr = nullptr;
        return nullptr;
    }

    int k= 0,l = 0, r = 0;
    while(l < l_len && r < r_len){
        if(avl1Arr[l]->key() < avl2Arr[r]->key())
            destArr[k++] = *avl1Arr[l++];
        else
            destArr[k++] = *avl2Arr[r++];
    }
    //add leftovers already sorted
    while (l < l_len) {
        destArr[k++] = *avl1Arr[l++];
    }
    while (r < r_len) {
        destArr[k++] = *avl2Arr[r++];
    }
    delete[] avl1Arr;
    delete[] avl2Arr;
    avl1Arr = nullptr;
    avl2Arr = nullptr;
    AVLTree<T,K>* result = new AVLTree<T,K>();
    if(result->createTreeByList(l_len+r_len, destArr) != StatusType::SUCCESS){
        delete result;
        result = nullptr;
    }
    delete[] destArr;
    destArr = nullptr;
    return result;
}


#endif //WORLDCUP23A1_CPP_AVL_TREES_H
//
// Created by ohadr on 11/22/2022.
//
