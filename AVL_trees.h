//
// Created by ohadr on 11/22/2022.
//
#include <memory>
#include <functional>

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
    void swapOut(Node* dest, Node* src = nullptr, Node* dest_r = nullptr, Node* dest_l = nullptr){
        if(dest == m_dummyHead)
            return;
        src->m_prev = dest->m_prev;
        if(dest->m_prev->m_left == dest){
            dest->m_prev->m_left = src;
        }
        else{
            dest->m_prev->m_right = src;
        }
        if(src) {
            src->m_left = dest_l;
            src->m_right = dest_r;
            if (dest_l) {
                dest_l->m_prev = src;
            }
            if (dest_r) {
                dest_r->m_prev = src;
            }
        }
    }

    AVLStatus FullRotation(Node* rotator, const string& dir1,const string& dir2){
        RotationAUX(rotator, dir1);
        rotator = rotator->m_prev; //root
        RotationAUX(rotator, dir2);
    }

    AVLStatus RotationAUX(Node* rotator , const char dir){ ///get a pointer to the unstable node (-2)
        Node* root = rotator->m_prev;
        Node* rotatee;
        Node* weight_shift;
        if(dir == 'r'){ //R rotation
            rotatee = rotator->m_left;
            weight_shift = rotatee->m_right;
            rotatee->m_right = rotator;
            rotator->m_left = weight_shift;
        }else if(dir == 'l'){ // L rotation
            rotatee = rotator->m_right;
            weight_shift = rotatee->m_left;
            rotatee->m_left = rotator;
            rotator->m_right = weight_shift;
        }
        else{ // wrong use of this function
            return AVLStatus::AVL_Fail;
        }
        //adjust heirarchy
        rotatee->m_prev = root;
        rotator->m_prev = rotatee;
        if(root != m_dummyHead){
            if (root->m_left == rotator)
                root->m_left = rotatee;
            else
                root->m_right = rotatee;
        }
        //adjust heights
        rotator->m_height = rotator->getHeight();
        rotatee->m_height = rotatee->getHeight();
        //the new rotator position is now rotatee actually
        rotator = rotatee;
    }

    ///What is that run_func???
    AVLStatus inOrderScanAUX(shared_ptr<Node> root, bool (run_func*)(T), AVLStatus result) const{ ///should get an input function
        if(result != AVLStatus::AVL_Success){
            return result;
        }
        if(!root){
            return AVLStatus::AVL_Success;
        }
        result = inOrderScanAUX(root->m_left,run_func,result);
        try{
            if(visit_func(*(root->m_data))){
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

    void updatePath(shared_ptr<Node> start, bool updateRollsToRoot){
        do {
            start->m_height = start->getHeight();
            while (abs(start->m_prev->heightDiff()) < 2) { //climb to top
                start = start->m_prev;
                if (start->m_prev == m_dummyHead) { //top reached
                    return;
                }
                start->m_height = start->getHeight();
            }
            char dir1, dir2;
            if (start->heightDIff() > 0) {
                dir1 = 'l';
            } else {
                dir1 = 'r';
            }
            if (start->m_prev->heightDiff() > 0) {
                dir2 = 'l';
            } else {
                dir2 = 'r';
            }
            FullRotation(start, dir1, dir2);

        }while(updateRollsToRoot && start->m_prev != m_dummyHead);

        while (start != m_dummyHead) { // update heights
            start->m_height = start->getHeight();
            start = start->m_prev;
        }
    }

public:
    AVLTree() : m_dummyHead(new Node()){}

    virtual ~AVLTree() = default; ///CANT BE DEFAULT!
//    {
//        deleter(dummyHead);
//    }
/////////////////////////////////////////////////////
    ///--------------------------------help function for cop constructor-----------------------------------
    bool copyHelp(Node* copyPtr, Node* orignPtr){
        copyPtr = orignPtr;
        copyPtr->m_right = new Node();
        copyPtr->m_left = new Node();
        if(orignPtr->m_left != nullptr){
            copyHelp(copyPtr->m_left, orignPtr->m_left);
        }
        if(orignPtr->m_right != nullptr){
            copyHelp(copyPtr->m_right, orignPtr->m_right);
        }
    }

    AVLTree(const AVLTree& tree){
        m_dummyHead = new Node();
        copyHelp(this->m_dummyHead, tree.m_dummyHead);
    }
/////////////////////////////////////////////////////

    AVLTree& operator= (AVLTree& tree){
        if(this == *tree)
            return *this;
        AVLTree<T,K> tempTree(tree);
        AVLTree<T,K>* swapper = this;
        this = &tempTree;
        tempTree = *swapper;
    }
    AVLStatus inOrderScan(bool (run_func)(const T&)) const{ ///should get an input function
        AVLTree<T,K> tempTree = *this;
        AVLStatus result = inOrderScan(tempTree.m_dummyHead->left, run_func);
        if(result == AVLStatus::AVL_Success){
            *this = tempTree;
        }
        return result;

    }




    /// @brief
    /// @param start
    /// @param updateRollsToRoot

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
    shared_ptr<Node> find(const K& item) const{
        if(m_dummyHead->m_height == 0)
            return nullptr;
        shared_ptr<Node> result_ptr = m_dummyHead->m_left;
        while(*result_ptr->m_key != item){
            result_ptr = *result_ptr->m_key > item ? result_ptr->m_left : result_ptr->m_right;
            if(!result_ptr)
                return nullptr;
        }
        return result_ptr;
    }

    AVLStatus insert(const T& newItem,  const K& key){
        shared_ptr<Node> newNode = nullptr;
        try {
            if (m_dummyHead->m_height == 0) {
                newNode = m_dummyHead->m_left = new Node(newItem, key,m_dummyHead);
            }else{
                shared_ptr<Node> ptr = m_dummyHead->m_left;
                while (true) { //add to leafs
                    if (*(ptr->m_key) > *key) {
                        if (!ptr->m_left) {
                            ptr->m_left = newNode;
                            break;
                        }
                        ptr = ptr->m_left;
                    } else if(*(ptr->m_key) < *key){
                        if (!ptr->m_right) {
                            ptr->m_right = newNode;
                            break;
                        }
                        ptr = ptr->m_right;
                    }
                    else{
                        return AVLStatus::AVL_Fail;
                    }
                }
                newNode = new Node(newItem, key, ptr);
            }
        }catch(...){
            return AVLStatus::AVL_Fail;
        }
        updatePath(newNode, false);
        return AVLStatus::AVL_Success;
    }

    AVLStatus remove(const K& ptrKeyDelete) {
        shared_ptr<Node> ptr = find(ptrKeyDelete);
        shared_ptr<Node> lowestInPath;
        if (!ptr) //ptr to delete not found
            return AVLStatus::AVL_Not_Found;
        shared_ptr<Node> replacement = ptr;
        if(replacement->right) { //go to next node in inorder if has children
            replacement= replacement->right;
            while (replacement->left) {
                replacement = replacement->left;
            }
            if (replacement->right) { // this corrects the tree where we took replacement
                lowestInPath = replacement->right;
                swapOut(replacement, replacement->right);
            }
            swapOut(ptr, replacement, ptr->left, ptr->right); //swap out ptr, swap in replacement
        }
        else {
            lowestInPath = ptr->left;
            swapOut(ptr, ptr->left); //simple case (only left children)
        }
        delete ptr;
        updatePath(lowestInPath,true);
        return AVLStatus::AVL_Success;
    }

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
                 m_size(0){}


        Node(const Node& n) : m_data(new T(n.m_data)),
                              m_key(new K(n.m_key)),
                              m_right(n.m_right),
                              m_left(n.m_left),
                              m_prev(n.m_prev),
                              m_height(n.m_height),
                              m_size(n.m_size){}

        virtual ~Node() = default;

        Node& operator= (const Node& n){
            Node temp = n;
            Node* swapper = this;
            this = &temp;
            temp = *swapper;
            return *this;
        }

        /// @brief getter for height
        /// @return
        void setSize(){
            if(!m_left && !m_right){
                if(this == m_dummyHead){
                    m_size = 0;
                }
                m_size = 1;
            }else if(!m_right){
                m_size = this->m_left->m_size;
            }else if(!m_left){
                m_size = this->m_right->m_size;
            }else{
                m_size = this->m_right->m_size + this->m_left->m_size;
            }
            m_size++;
        }

        void setHeight(){
            if(!m_left && !m_right){
                m_height = 0;
            }else if(!m_right){
                m_height = this->m_left->m_height;
            }else if(!m_left){
                m_height = m_right->m_height;
            }else{
                m_height = m_right->m_height > m_left->m_height ? m_right->m_height : m_left->m_height;
            }
            m_height++;
        }
        /// @brief method to calculate balance factor
        /// @return
        int heightDiff(){
            int delta_h = 0;
            if(!m_left && !m_right){
                return delta_h;
            }else if(!m_right){
                delta_h = m_left->m_height + 1;
            }else if(!m_left){
                delta_h = m_right->m_height + 1;
            }else{
                delta_h = m_left->m_height - m_right->m_height;
            }
            return delta_h;
        }
    };
};

#endif //WORLDCUP23A1_CPP_AVL_TREES_H
//
// Created by ohadr on 11/22/2022.
//

