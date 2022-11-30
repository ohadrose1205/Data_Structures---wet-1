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
using std::shared_ptr;
using std::unique_ptr;
using std::string;

enum class AVLStatus{AVL_Success, AVL_Fail, AVL_Not_Found};

template <class T , class K>
class AVLTree{
private:
    class Node;
    Node* m_dummyHead;
    //std::function<int(const T&, const T&)> compareFunc; ///TRUE = RIGHT, FALSE = LEFT

    int inOrderScanToArrAUX(Node* root, Pair<T,K>* arr, int top) const{ ///should get an input function
        if(!root){
            return top;
        }
        top = inOrderScanToArrAUX(root->m_left, arr, top);
        root->printOut();
        arr[top] = *(root->m_nodeData);
        top++;
        top = inOrderScanToArrAUX(root->m_right, arr, top);
        return top;
    }
    int inOrderScanAssignment(Node* root, Pair<T,K>* arr, int top){ ///should get an input function
        if(!root){
            return top;
        }
        top = inOrderScanAssignment(root->m_left, arr, top);
        root->m_nodeData.reset(new Pair<T,K>(arr[top]));
        //root->printOut();
        top++;
        top = inOrderScanAssignment(root->m_right, arr, top);
        updateNode(root);
        return top;
    }

    AVLStatus FullRotation(Node* rotator, const string& dir1,const string& dir2){
        //std::cout << "\n------------------FullRotation---------------\n\n" <<std::endl;

        if(dir1 == dir2){
            rotator = rotator->m_prev;
            RotationAUX(rotator, dir1);
        }
        else {
            RotationAUX(rotator, dir1);
            rotator = rotator->m_prev; //root
            RotationAUX(rotator, dir2);
        }
        return AVLStatus::AVL_Success;
    }

    AVLStatus RotationAUX(Node* rotator , const string& dir){ ///get a pointer to the unstable node (-2)
        // std::cout << "________________RotationAUX________________\n" <<std::endl;
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
//            std::cout <<"rotator->height: "<< rotator->m_height <<std::endl;
//            std::cout <<"rotatee->height: "<< rotator->m_height <<std::endl;
            weight_shift = rotatee->m_left;
            rotatee->m_left = rotator;
            rotator->m_right = weight_shift;
        }
        else{ // wrong use of this function
            return AVLStatus::AVL_Fail;
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
        return AVLStatus::AVL_Success;
    }

    AVLTree<T,K>* createEmptyTree(int depth, int offset){
        Node* root = new Node();
        createNodes(root, depth);
        cleanEmptyNodes(root, offset);
        AVLTree<T,K>* tree = new AVLTree<T,K>();
        tree->m_dummyHead->m_left = root;
        return tree;
    }

    void createNodes(Node* father, int depth){
        if(depth == 0){
            return;
        }
        father->m_left = new Node();
        father->m_left->m_prev = father;
        father->m_right = new Node();
        father->m_right->m_prev = father;
        createNodes(father->m_left, depth-1);
        createNodes(father->m_right, depth-1);
    }

    int cleanEmptyNodes(Node* root, int offset){
        if(offset == 0 || !root){
            return offset;
        }
        if(!root->m_left && !root->m_right) {
            delete root;
            offset--;
            return offset;
        }
        offset = cleanEmptyNodes(root->m_right,offset);
        offset = cleanEmptyNodes(root->m_left,offset);
        return offset;
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
        if (m_dummyHead->m_height == 0)
            return nullptr;
        Node* result_ptr = m_dummyHead->m_left;
        while (*result_ptr->key() != item) {
            result_ptr = *result_ptr->key() > item ? result_ptr->m_left : result_ptr->m_right;
            if (!result_ptr)
                return nullptr;
        }
        return result_ptr;
    }
public:

    AVLTree(): m_dummyHead(new Node()){}

    virtual ~AVLTree(){
        deleter(m_dummyHead);
    }

////////////////////     TO-DO     /////////////////////////////////
    AVLTree(const AVLTree& tree){}

    AVLTree& operator= (AVLTree& tree){
        if(this == *tree)
            return *this;
        AVLTree<T,K> tempTree(tree);
        AVLTree<T,K>* swapper = this;
        this = &tempTree;
        tempTree = *swapper;
    }

////////////////////////////////////////////////////////////////////
    int size() const{
        return m_dummyHead->m_left->m_size;
    }
    AVLStatus insert(const T& newItem,  const K& key){
        Node* newNode;
        try {
            if (m_dummyHead->m_height == 0) {
                newNode = m_dummyHead->m_left = new Node(newItem, key,m_dummyHead);
            }else{
                Node* ptr = m_dummyHead->m_left;
                while (true) { //add to leafs
                    if (key > *(ptr->key())) {
                        if (!ptr->m_right) {
                            ptr->m_right = newNode =  new Node(newItem, key,ptr);
                            break;
                        }
                        ptr = ptr->m_right;
                    } else if(*(ptr->key()) > key){
                        if (!ptr->m_left) {
                            ptr->m_left = newNode = new Node(newItem, key,ptr);
                            break;
                        }
                        ptr = ptr->m_left;
                    }
                    else{
                        return AVLStatus::AVL_Fail;
                    }
                }
            }
        }catch(...){
            return AVLStatus::AVL_Fail;
        }
        updatePath(newNode, false);
        return AVLStatus::AVL_Success;
    }

    AVLStatus remove(const K& ptrKeyDelete) {
        Node* ptr = findAUX(ptrKeyDelete);
        Node* lowestInPath;
        Node* toDelete;
        if (!ptr) //ptr to delete not found
            return AVLStatus::AVL_Not_Found;
        Node* replacement = ptr;
        if(!replacement->m_right && !replacement->m_left){
            if(ptr->m_prev->m_left == ptr){
                ptr->m_prev->m_left = nullptr;
            }else{
                ptr->m_prev->m_right = nullptr;
            }
            toDelete = ptr;
        }
        else if(replacement->m_right) { //go to next node in inorder if has children
            replacement= replacement->m_right;
            while (replacement->m_left) {
                replacement = replacement->m_left;
            }
            ptr->m_nodeData = std::move(replacement->m_nodeData);
            if (replacement->m_right) { // this corrects the tree where we took replacement
                lowestInPath = replacement->m_right;
                replacement->m_prev->m_left = replacement->m_right;
                replacement->m_right->m_prev = replacement->m_prev;
            }else{
                lowestInPath = replacement->m_prev;
                if(replacement->m_prev->m_left == replacement){
                    replacement->m_prev->m_left = nullptr;
                }else{
                    replacement->m_prev->m_right = nullptr;
                }

            }
            toDelete = replacement;
        }
        else {
            lowestInPath = ptr->m_left;
            if(ptr->m_prev->m_left == ptr){
                ptr->m_prev->m_left = ptr->m_left;
            }else{
                ptr->m_prev->m_right = ptr->m_left;
            }
            ptr->m_left->m_prev = ptr->m_prev;
            toDelete = ptr;
        }
        delete toDelete;
        updatePath(lowestInPath,true);
        return AVLStatus::AVL_Success;
    }

    Pair<T,K>* inOrderScanToArray() const{ ///should get an input function
        Pair<T,K>* pairArray = new Pair<T,K>[size()]();
        inOrderScanToArrAUX(m_dummyHead->m_left, pairArray, 0);
        for(int i = 0; i < size(); i++){
            std::cout<< *(pairArray[i].Data()) << std::endl;
        }
        return pairArray;
    }

    const Pair<T,K>* find(const K& item) const {
        if (m_dummyHead->m_height == 0)
            return nullptr;
        Node* result_ptr = m_dummyHead->m_left;
        while (*result_ptr->m_nodeData->Key() != item) {
            result_ptr = *result_ptr->m_nodeData->Key() > item ? result_ptr->m_left : result_ptr->m_right;
            if (!result_ptr)
                return nullptr;
        }
        return *result_ptr->m_nodeData;
    }
    /// @brief
    /// @param start
    /// @param updateRollsToRoot

    void createTreeByList(int size ,Pair<T,K>* arr){
        int depth = (int)log2(size);
        int offset = pow(2,depth) - size;
        AVLTree<T,K>* result = createEmptyTree(depth,offset);
        result->inOrderScanAssignment(result->m_dummyHead->m_left, arr, 0);
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
        Node() : m_nodeData(new Pair<T,K>()),
                 m_right(nullptr),
                 m_left(nullptr),
                 m_prev(nullptr),
                 m_height(0),
                 m_size(1)
        {}

        Node(const Node& n) = delete;
        Node& operator= (const Node& n) = delete;

        /// @brief getter for height
        /// @return
        T* data(){
            return m_nodeData->Data();
        }

        const K* key(){
            return m_nodeData->Key();
        }

        void printOut(){
            std::cout << "\n \\ \\ \\ \\ \\ \\ \\ \\ \\ \\ \\ \\ \\  printOut / / / / / / / / / / / \n" <<std::endl;
            string str1 = m_prev->data() ? *m_prev->data() : "m_dummyNode";
            if(data()){
                std::cout << "this is printing: " << *data() <<" (prev: "<< str1+")" << std::endl;
            }else{
                std::cout << "this is is printing: m_dummyNode" << std::endl;
            }
            if(!m_left && !m_right) {
                std::cout << "leaf:  " << *data() << std::endl;
            }else if(m_left && m_right){
                if(data())
                    std::cout << "is " << * m_left->data()<<
                              " (h,s:" <<m_left->m_height<<", "<< m_left->m_size <<") father and "<<
                              *m_right->data() <<" (h,s:" <<m_right->m_height<<", "<< m_right->m_size <<
                              ") father, height, size: " <<m_height << ", " << m_size<< std::endl;
            }else if(m_right){
                if(data())
                    std::cout << "is " << * m_right->data() <<" father. height, size: " <<m_height << ", " << m_size<< std::endl;
            }else{
                if(data())
                    std::cout << "is " << *m_left->data() <<" father. height, size: " <<m_height << ", " << m_size<< std::endl;
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
            }else if(m_left && m_right){
                m_height = m_right->m_height > m_left->m_height ? m_right->m_height : m_left->m_height;
                m_height++;
            }else if(m_right){
                m_height = m_right->m_height + 1;
            }else{
                m_height = m_left->m_height + 1;
            }
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
    public:
        virtual ~Node() = default;
    };
};

template<class T, class K>
AVLTree<T,K>* mergeTreesByKeyGenFunc(AVLTree<T,K>& avl1 , AVLTree<T,K>& avl2){
    Pair<T,K> *avl1Arr = avl1.inOrderScanToArray();
    Pair<T,K> *avl2Arr = avl2.inOrderScanToArray();
    int l_len = avl1.size();
    int r_len = avl2.size();
    Pair<T,K>* destArr = new Pair<T,K>[l_len+r_len]();

    int k= 0,l = 0, r = 0;
    while(l < l_len && r < r_len){
        if(*(avl1Arr[l].Key()) < *(avl2Arr[r].Key()))
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
    for(int i = 0; i < r_len+l_len; i++){
        std::cout<< "Key: "<< *(destArr[i].Key()) << ", Value:" <<  *(destArr[i].Data()) << std::endl;
    }
    AVLTree<T,K>* result = new AVLTree<T,K>();
    result->createTreeByList(l_len+r_len, destArr);
    return result;
}


#endif //WORLDCUP23A1_CPP_AVL_TREES_H
//
// Created by ohadr on 11/22/2022.
//
