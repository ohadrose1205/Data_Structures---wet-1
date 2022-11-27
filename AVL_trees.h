//
// Created by ohadr on 11/22/2022.
//
#include <memory>
#ifndef WORLDCUP23A1_CPP_AVL_TREES_H
#define WORLDCUP23A1_CPP_AVL_TREES_H
using std::shared_ptr;
using std::string;

template <class T, class CF>
class AVLTree{
public:
    enum class AVLStatus{AVL_Success, AVL_Fail};
private:
    class Node;

    shared_ptr<Node> dummyHead;
    CF compareFunc; ///TRUE = RIGHT, FALSE = LEFT

    void swapOut(Node* dest, Node* src = nullptr, Node* dest_r = nullptr, Node* dest_l = nullptr){
        if(dest == dummyHead)
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
        if(root != dummyHead){
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

    void updatePath(Node* start){
        start->height = start->getHeight();
        while(abs(start->prev->heightDiff) < 2){ //climb to top
            start = start->prev;
            if(start->prev == dummyHead){ //top reached
                return;
            }
            start->height = start->getHeight();
        }
        string dir1,dir2;
        if (start->heightDIff > 0) {
            dir1 = "r";
        }else{
            dir1 = "l";
        }
        if(start->prev->heightDiff > 0) {
            dir2 = "r";
        }else{
            dir2 = "l";
        }
        FullRotation(start,dir1,dir2);

        if(start->prev->height == start->prev->getHeight()) //check if a new  layer formed (if we need to update heights)
            return;
        while(start->prev != dummyHead){ // update heights
            start = start->prev;
            start->height = start->getHeight();
        }
    }

    void deleter(shared_ptr<Node> ptr){
        if(ptr) {
            deleter(ptr->left);
            deleter(ptr->right);
            delete ptr;
        }
    }

public:

    AVLTree() : dummyHead(){}

    virtual ~AVLTree(){
        deleter(dummyHead);
    }
/////////////////////////////////////////////////////workkkkkkkkkkkkkkk
    AVLTree(const AVLTree& tree){

    }

    AVLTree& operator= (AVLTree& tree){
        if(this == *tree)
            return *this;
        AVLTree<T,CF> tempTree(tree);
        AVLTree<T,CF>* swapper = this;
        this = &tempTree;
        &tempTree = swapper;
    }
//////////////////////////////////////////////////////workkkkkkkkkkkkkkkkk
    AVLStatus inOrderScan(Node* root, CF* visit_func){ ///should get an input function
        if(root == dummyHead) return AVLStatus::AVL_Fail;
        if(root == nullptr) return AVLStatus::AVL_Success;
        inOrderScan(root->left);
        visit_func(root);
        inOrderScan(root->right);
        return(AVLStatus::AVL_Success);
    }

    ///get pointer to the head and pointer to the item we wanna find, make a recursive calls depend on the comparefunc of the tree, if didnt find / empty return nullptr.
    Node* findInTree(const T* item){
        if(dummyHead->height == 0)
            return nullptr;
        Node* result_ptr = dummyHead->left;
        int comp_result = compareFunc(result_ptr->data, item);
        while(comp_result != 0){
            if(comp_result > 0){
                result_ptr = result_ptr->right;
            }
            else{
                result_ptr = result_ptr->left;
            }
            if(!result_ptr)
                return nullptr;
            comp_result = compareFunc(result_ptr->data, item);
        }
        return result_ptr;
    }

    AVLStatus insertNode(const T* newItem){
        Node* newNode;
        try {
            if (dummyHead->height != 0) {
                Node *ptr = dummyHead->left;
                while (true) { //add to leafs
                    if (compareFunc(newNode, ptr)) {
                        if (!ptr->left) {
                            newNode = ptr->left = new Node(*newItem, ptr);
                            break;
                        }
                        ptr = ptr->left;
                    } else {
                        if (!ptr->right) {
                            newNode = ptr->right = new Node(*newItem, ptr);
                            break;
                        }
                        ptr = ptr->right;
                    }
                }
            } else {
                newNode = dummyHead->left = new Node(*newItem, dummyHead);
            }
        }catch(...){
            return AVLStatus::AVL_Fail;
        }
        updatePath(newNode);
        return AVLStatus::AVL_Success;
    }

    AVLStatus deleteNode(T* ptrToDelete){
        Node* ptr = find(ptrToDelete);
        if(!ptr)
            return AVLStatus::AVL_Fail;
        Node* replacement = ptr;
        while(replacement->height >= 2) {
            if (replacement->heightDiff() <= 0)
                replacement = replacement->right->left;
            else {
                replacement = replacement->left->right;
            }
            ptr->data = replacement->data;
        }
        if(replacement->left && replacement->right) {
            if (replacement->prev->left == replacement) {
                swapOut(replacement, replacement->right, replacement->left);
            } else {
                swapOut(replacement, replacement->left, replacement->right);
            }
            updatePath(replacement->left);
        }else {
            if (replacement->left) {
                swapOut(replacement, replacement->left);
            } else if (replacement->right) {
                swapOut(replacement, replacement->right);
            } else {
                swapOut(replacement);
            }
            updatePath(replacement->prev);
        }
        delete replacement;
        return AVLStatus::AVL_Success;
    }

private:
    class Node{
        Node(const T* dataPtr,const Node* previous) : data(new T(*dataPtr)), right(nullptr), left(nullptr), prev(previous), height(0){}
        explicit Node(const T* dataPtr) : data(new T(*dataPtr)), right(nullptr), left(nullptr), prev(nullptr), height(0){}
        Node() : data(nullptr), right(nullptr), left(nullptr), prev(nullptr), height(0){}
        virtual ~Node() = default;
        Node(const Node& n){
            data = new T(n.data);
            right = n.right;
            left = n.left;
            prev = n.prev;
            height = n.height;
        }

        Node& operator= (const Node& n){
            data = new T(n.data);
            right = n.right;
            left = n.left;
            prev = n.prev;
            height = n.height;
        }
        friend AVLTree;

        shared_ptr<T> data;
        shared_ptr<Node> right;
        shared_ptr<Node> left;
        shared_ptr<Node> prev;
        int height; ///MAKE IT UPDATED AUTO

        int getFullHeight(){
            if(this->left == nullptr && this->right == nullptr){
                return 0;
            }
            int heightR = this->right->getHeight();
            int heightL =this->left->getHeight();
            int h = heightL > heightR ? heightL : heightR;
            return (h + 1);
        }
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
