
//
// Created by ohadr on 11/22/2022.
//
#include <memory>
#include <functional>
#include <iostream>
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
    shared_ptr<Node> m_dummyHead;
    //std::function<int(const T&, const T&)> compareFunc; ///TRUE = RIGHT, FALSE = LEFT

    AVLStatus inOrderScanAUX(shared_ptr<Node> root, bool (run_func)(const T&), AVLStatus result) const{ ///should get an input function
        if(result != AVLStatus::AVL_Success){
            return result;
        }
        if(!root){
            return AVLStatus::AVL_Success;
        }
        result = inOrderScanAUX(root->m_left,run_func,result);
        try{
            root->printOut();
            if(run_func(*(root->m_data))){
                result = AVLStatus::AVL_Success;
            }else{
                result = AVLStatus::AVL_Fail;
            }
        }catch(...){
            result = AVLStatus::AVL_Fail;
        }
        result = inOrderScanAUX(root->m_right,run_func,result);
        return result;
    }

    AVLStatus FullRotation(shared_ptr<Node>& rotator, const string& dir1,const string& dir2){
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

    AVLStatus RotationAUX(shared_ptr<Node>& rotator , const string& dir){ ///get a pointer to the unstable node (-2)
        // std::cout << "________________RotationAUX________________\n" <<std::endl;
        shared_ptr<Node> root = rotator->m_prev;
        shared_ptr<Node> rotatee;
        shared_ptr<Node> weight_shift;
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

    void updateNode(shared_ptr<Node> nodeToUpdate){
        nodeToUpdate->setHeight();
        nodeToUpdate->setSize();
    }

    void updatePath(shared_ptr<Node> start, bool updateRollsToRoot){
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
//    void deleter(Node* ptr){
//        if(ptr) {
//            deleter(ptr->left);
//            deleter(ptr->right);
//            delete ptr;
//        }
//    }

    ///get pointer to the head and pointer to the item we wanna find, make a recursive calls depend on the comparefunc of the tree, if didnt find / empty return nullptr.
    shared_ptr<Node> findAUX(const K& item) const {
        if (m_dummyHead->m_height == 0)
            return nullptr;
        shared_ptr<Node> result_ptr = m_dummyHead->m_left;
        while (*result_ptr->m_key != item) {
            result_ptr = *result_ptr->m_key > item ? result_ptr->m_left : result_ptr->m_right;
            if (!result_ptr)
                return nullptr;
        }
        return result_ptr;
    }
public:

    AVLTree(): m_dummyHead(new Node()){}

////////////////////     TO-DO     /////////////////////////////////

    virtual ~AVLTree() = default;
//    {
//        deleter(dummyHead);
//    }

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

    AVLStatus insert(const T& newItem,  const K& key){
        shared_ptr<Node> newNode;
        try {
            if (m_dummyHead->m_height == 0) {
                newNode = m_dummyHead->m_left = std::shared_ptr<Node>(new Node(newItem, key,m_dummyHead));
            }else{
                shared_ptr<Node> ptr = m_dummyHead->m_left;
                while (true) { //add to leafs
                    if (*(ptr->m_key) < key) {
                        if (!ptr->m_right) {
                            ptr->m_right = newNode =  std::shared_ptr<Node>(new Node(newItem, key,ptr));
                            break;
                        }
                        ptr = ptr->m_right;
                    } else if(*(ptr->m_key) > key){
                        if (!ptr->m_left) {
                            ptr->m_left = newNode =  std::shared_ptr<Node>(new Node(newItem, key,ptr));
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
        shared_ptr<Node> ptr = findAUX(ptrKeyDelete);
        shared_ptr<Node> lowestInPath;
        shared_ptr<Node> toDelete;
        if (!ptr) //ptr to delete not found
            return AVLStatus::AVL_Not_Found;
        shared_ptr<Node> replacement = ptr;
        if(replacement->m_right) { //go to next node in inorder if has children
            replacement= replacement->m_right;
            while (replacement->m_left) {
                replacement = replacement->m_left;
            }
            ptr->m_data = replacement->m_data;
            ptr->m_key = replacement->m_key;
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
        toDelete.reset();
        updatePath(lowestInPath,true);
        return AVLStatus::AVL_Success;
    }

    AVLStatus inOrderScan(bool (run_func)(const T&)) const{ ///should get an input function
        AVLStatus result = inOrderScanAUX(m_dummyHead->m_left, run_func, AVLStatus::AVL_Success);
        return result;
    }

    const T& find(const K& item) const {
        if (m_dummyHead->m_height == 0)
            return nullptr;
        shared_ptr<Node> result_ptr = m_dummyHead->m_left;
        while (*result_ptr->m_key != item) {
            result_ptr = *result_ptr->m_key > item ? result_ptr->m_left : result_ptr->m_right;
            if (!result_ptr)
                return nullptr;
        }
        return *result_ptr->m_data;
    }
    /// @brief
    /// @param start
    /// @param updateRollsToRoot




///----------------------implementation of Node class---------------------------
private:
    class Node{
        friend AVLTree;

        shared_ptr<T> m_data;
        shared_ptr<K> m_key;
        shared_ptr<Node> m_right;
        shared_ptr<Node> m_left;
        shared_ptr<Node> m_prev;
        int m_height;
        int m_size;

        Node(const T& dataPtr, const K& key,const shared_ptr<Node> previous) : m_data(new T(dataPtr)),
                                                                               m_key(new K(key)),m_right(nullptr),
                                                                               m_left(nullptr), m_prev(previous),
                                                                               m_height(0),m_size(1){}
        Node() : m_data(nullptr),
                 m_key(nullptr),
                 m_right(nullptr),
                 m_left(nullptr),
                 m_prev(nullptr),
                 m_height(0),
                 m_size(1)
        {}

        Node(const Node& n) : m_data(new T(n.m_data)),
                              m_key(new K(n.m_key)),
                              m_right(n.m_right),
                              m_left(n.m_left),
                              m_prev(n.m_prev),
                              m_height(n.m_height),
                              m_size(n.m_size){}



        Node& operator= (const Node& n){
            Node temp = n;
            Node* swapper = this;
            this = &temp;
            temp = *swapper;
            return *this;
        }

        /// @brief getter for height
        /// @return

        void printOut(){
            std::cout << "\n \\ \\ \\ \\ \\ \\ \\ \\ \\ \\ \\ \\ \\  printOut / / / / / / / / / / / \n" <<std::endl;
            string str1 = m_prev->m_data ? *m_prev->m_data : "m_dummyNode";
            if(m_data){
                std::cout << "this is printing: " << *m_data <<" (prev: "<< str1+")" << std::endl;
            }else{
                std::cout << "this is is printing: m_dummyNode" << std::endl;
            }
            if(!m_left && !m_right) {
                std::cout << "leaf:  " << *m_data << std::endl;
            }else if(m_left && m_right){
                if(m_data)
                    std::cout << "is " << * m_left->m_data<<
                              " (h,s:" <<m_left->m_height<<", "<< m_left->m_size <<") father and "<<
                              *m_right->m_data <<" (h,s:" <<m_right->m_height<<", "<< m_right->m_size <<
                              ") father, height, size: " <<m_height << ", " << m_size<< std::endl;
            }else if(m_right){
                if(m_data)
                    std::cout << "is " << * m_right->m_data <<" father. height, size: " <<m_height << ", " << m_size<< std::endl;
            }else{
                if(m_data)
                    std::cout << "is " << *m_left->m_data <<" father. height, size: " <<m_height << ", " << m_size<< std::endl;
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


#endif //WORLDCUP23A1_CPP_AVL_TREES_H
//
// Created by ohadr on 11/22/2022.
//
