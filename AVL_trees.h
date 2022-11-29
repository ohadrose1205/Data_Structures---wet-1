//
// Created by ohadr on 11/22/2022.
//
#include <memory>
#include <functional>

#ifndef WORLDCUP23A1_CPP_AVL_TREES_H
#define WORLDCUP23A1_CPP_AVL_TREES_H
using std::shared_ptr;
using std;

template <class T , class K>
class AVLTree{
public:
    enum class AVLStatus{AVL_Success, AVL_Fail, AVL_Not_Found};
private:
    class Node;
    Node* m_dummyHead;
    //std::function<int(const T&, const T&)> compareFunc; ///TRUE = RIGHT, FALSE = LEFT
    void swapOut(Node* dest, Node* src = nullptr, Node* dest_r = nullptr, Node* dest_l = nullptr){
        if(dest == m_dummyHead)
            return;
        src->prev = dest->prev;
        if(dest->prev->left == dest){
            dest->prev->left = src;
        }
        else{
            dest->prev->right = src;
        }
        if(src) {
            src->left = dest_l;
            src->right = dest_r;
            if (dest_l) {
                dest_l->prev = src;
            }
            if (dest_r) {
                dest_r->prev = src;
            }
        }
    }

    AVLStatus FullRotation(Node* rotator, const string& dir1,const string& dir2){
        RotationAUX(rotator, dir1);
        rotator = rotator->prev; //root
        RotationAUX(rotator, dir2);
    }

    AVLStatus RotationAUX(Node* rotator , const string& dir){ ///get a pointer to the unstable node (-2)
        Node* root = rotator->prev;
        Node* rotatee;
        Node* weight_shift;
        if(dir == "r"){ //R rotation
            rotatee = rotator->left;
            weight_shift = rotatee->right;
            rotatee->right = rotator;
            rotator->left = weight_shift;
        }else if(dir == "l"){ // L rotation
            rotatee = rotator->right;
            weight_shift = rotatee->left;
            rotatee->left = rotator;
            rotator->right = weight_shift;
        }
        else{ // wrong use of this function
            return;
        }
        //adjust heirarchy
        rotatee->prev = root;
        rotator->prev = rotatee;
        if(root != m_dummyHead){
            if (root->left == rotator)
                root->left = rotatee;
            else
                root->right = rotatee;
        }
        //adjust heights
        rotator->height = rotator->getHeight();
        rotatee->height = rotatee->getHeight();
        //the new rotator position is now rotatee actually
        rotator = rotatee;
    }

    void updatePath(Node* start, bool updateRollsToRoot){
        do {
            start->height = start->getHeight();
            while (abs(start->prev->heightDiff) < 2) { //climb to top
                start = start->prev;
                if (start->prev == m_dummyHead) { //top reached
                    return;
                }
                start->height = start->getHeight();
            }
            string dir1, dir2;
            if (start->heightDIff > 0) {
                dir1 = "r";
            } else {
                dir1 = "l";
            }
            if (start->prev->heightDiff > 0) {
                dir2 = "r";
            } else {
                dir2 = "l";
            }
            FullRotation(start, dir1, dir2);

        }while(updateRollsToRoot && start->prev != m_dummyHead);

        while (start != dummyHead) { // update heights
            start->height = start->getHeight();
            start = start->prev;
        }
    }

    void copyNode(Node* mainRoot, Node* rootToCopy){

        p->left = new Node(ptrToCopy->left);
        ptr = ptr->left;
        ptrToCopy = ptrToCopy->left;
    }
public:
    AVLTree(int (*compareFunction)(const T&, const T&)) : compareFunction(compareFunction){
        dummyHead = new Node(nullptr);
    }

    virtual ~AVLTree(){
        deleter(dummyHead);
    }
/////////////////////////////////////////////////////

    AVLTree(const AVLTree& tree){
        dummyHead = new Node(nullptr);
        Node* ptr = m_dummyHead;
        Node* ptrToCopy = tree.dummyHead;

    }
                                                   //
    AVLTree& operator= (AVLTree& tree){            //
        if(this == *tree)                          //
            return *this;                          //
        AVLTree<T,CF> tempTree(tree);              //
        AVLTree<T,CF>* swapper = this;             //
        this = &tempTree;                          //
        &tempTree = swapper;                       //
    }        //
/////////////////////////////////////////////////////
    AVLStatus inOrderScan(Node* root){ ///should get an input function
        if(root == dummyHead) return AVLStatus::AVL_Success;
        if(root == nullptr) return AVLStatus::AVL_Fail;
        inOrderScan(root->left);
        visit_func(root);
        inOrderScan(root->right);
        return(AVLStatus::AVL_Success);
    }



    /// @brief
    /// @param start
    /// @param updateRollsToRoot

///-----------------------------help function for delete----------------------------------------
    /// @brief help-function for destroyer of AVL_tree, works on post-order
    /// @param ptr
    void deleter(Node* ptr){
        if(ptr) {
            deleter(ptr->left);
            deleter(ptr->right);
            delete ptr;
        }
    }

    ///get pointer to the head and pointer to the item we wanna find, make a recursive calls depend on the comparefunc of the tree, if didnt find / empty return nullptr.
    Node* findInTree(const K& item){
        if(dummyHead->height == 0)
            return nullptr;
        Node* result_ptr = dummyHead->left;
        while(result_ptr->m_key != item){
            result_ptr = result_ptr->m_key > item ? result_ptr->m_left : result_ptr->m_right;
            if(!result_ptr)
                return nullptr;
        }
        return result_ptr;
    }

    AVLStatus insert(const T& newItem,  const K& key){
        Node* newNode = nullptr;
        try {
            if (dummyHead->height == 0) {
                newNode = dummyHead->left = new Node(newItem, key,dummyHead);
            }else{
                Node *ptr = dummyHead->left;
                while (true) { //add to leafs
                    if (ptr->key > key) {
                        if (!ptr->left) {
                            ptr->left = newNode;
                            break;
                        }
                        ptr = ptr->left;
                    } else if{
                        if (!ptr->right) {
                            ptr->right = newNode;
                            break;
                        }
                        ptr = ptr->right;
                    }
                    else{

                    }
                }
                newNode = new(newItem, key, ptr);
            }
        }catch(...){
            return AVLStatus::AVL_Fail;
        }
        updatePath(newNode, false);
        return AVLStatus::AVL_Success;
    }

    AVLStatus remove(const K& ptrKeyDelete) {
        Node *ptr = find(ptrKeyDelete);
        Node *lowestInPath;
        if (!ptr) //ptr to delete not found
            return AVLStatus::AVL_Fail;
        Node *replacement = ptr;
        if (replacement->right) { //go to next node in inorder if has children
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

        T* m_data;
        K m_key;
        Node* m_right;
        Node* m_left;
        Node* m_prev;
        int m_height;

        Node(const T& dataPtr, const K& key,const Node* previous) : m_data(new T(dataPtr)),
                                                                    m_key(key),m_right(nullptr),
                                                                    m_left(nullptr), m_prev(previous),
                                                                    m_height(0){}
        Node() : m_data(nullptr),m_key(),
                 m_right(nullptr),
                 m_left(nullptr),
                 m_prev(nullptr),
                 m_height(0){}

        Node(const Node& n) : m_data(new T(n.m_data)),
                              m_key(n.m_key),
                              m_right(n.m_right),
                              m_left(n.m_left),
                              m_prev(n.m_prev),
                              m_height(n.m_height){}

        virtual ~Node(){
            delete m_data;
        }

        Node& operator= (const Node& n){
            Node temp = n;
            Node* swapper = this;
            this = &temp;
            temp = *swaper;
            return *this;
        }

        /// @brief getter for height
        /// @return
        int getHeight(){
            int h;
            if(this->left == nullptr && this->right == nullptr){
                return 0;
            }else if(this->right == nullptr){
                h = this->left->height;
            }else if(this->left == nullptr){
                h = this->right->height;
            }else{
                h = this->right->height > this->left->height ? this->right->height : this->left->height;
            }
            return ++h;
        }
        /// @brief method to calculate balance factor
        /// @return
        int heightDiff(){
            int delta_h = 0;
            if(this->left == nullptr && this->right == nullptr){
                return delta_h;
            }else if(this->right == nullptr){
                delta_h = left->height + 1;
            }else if(this->left == nullptr){
                delta_h = right->height + 1;
            }else{
                delta_h = left->height - right->height;
            }
            return delta_h;
        }
    };
};

#endif //WORLDCUP23A1_CPP_AVL_TREES_H
//
// Created by ohadr on 11/22/2022.
//
