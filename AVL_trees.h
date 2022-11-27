//
// Created by ohadr on 11/22/2022.
//

#ifndef WORLDCUP23A1_CPP_AVL_TREES_H
#define WORLDCUP23A1_CPP_AVL_TREES_H

const int LEAF = 1;
using namespace std;

enum AVLStatus  {AVL_Success, AVL_Fail};

template <class T, class CF>
class AVLTree{
    class Node{
    private:
        int height; ///MAKE IT UPDATED AUTO
        int heightDiff;
        T* data;
        Node* right;
        Node* left;
        Node* prev;
    public:
        Node(T* mainDataPtr) : data(mainDataPtr){}
        virtual ~Node();
        Node(const Node& n);
        AVLTree::Node& operator= (const Node& n);

        int getHeight(){
            if(this->left == nullptr && this->right == nullptr){
                return 0;
            }
            int heightR = this->right->getHeight();
            int heightL =this->left->getHeight();
            int h = heightL > heightR ? heightL : heightR;
            return (h + 1);
        }
    };
    explicit AVLTree(CF* compFunc) : compareFunc(compFunc){
        head = nullptr;
    }

    ~AVLTree(){
        AVLTree::Node ptr = head;
        while(ptr->left){
            AVLTree::Node temp = head;
        }
    }

    AVLTree(const AVLTree& tree);
    AVLTree& operator= (AVLTree& tree);

    ///get a pionter to the head of the tree, and ptr to a function that gonna run on each node
    AVLStatus inOrderScan(Node* root, CF* visit_func){ ///should get an input function
        if(root == nullptr) return AVL_Fail;
        inOrderScan(root->left);
        visit_func(root);
        inOrderScan(root->right);
        return(AVL_Success);
    }

    ///get pointer to the head and pointer to the item we wanna find, make a recursive calls depend on the comparefunc of the tree, if didnt find / empty return nullptr.
    Node* findInTree(AVLTree* tree, const T* item){
        if(tree->head == nullptr){
            return nullptr;
        }
        Node* root = tree->head;
        Node* result_ptr = nullptr;
        if(root->data == item){
            return head;
        }
        else{
            if(compareFunc(root->data, item)){
                result_ptr = findInTree(root->right, item);
            }
            else{
                result_ptr = findInTree(root->left, item);
            }
        }
        return result_ptr;
    }

    ///inserting new node in tree, get new information as an input []
    AVLStatus insertNode(AVLTree* tree, const T* newItem){
        if(findInTree(tree, newItem) != nullptr){
            return AVL_Fail;
        }
        Node* newNode = new Node(newItem);
        Node* ptr = tree->head;
        while(ptr->getHeight() != 0){
            if(compareFunc(newNode, ptr)){
                ptr->heightDiff++;
                ptr = ptr->right;
            }
            else{
                ptr->heightDiff--;
                ptr = ptr->left;
            }
        }
        ptr -> left = newNode;
        while(ptr->prev->abs(heightDiff())<=1) {
            ptr = ptr->prev;
        }
            if (ptr->prev > 1 && ptr->heightDIff > 1)
                LL();
            else if (ptr->prev > 1 && ptr->heightDIff < 1)
                LR();
            else if (ptr->prev < 1 && ptr->heightDIff < 1)
                RR();
            else
                RL();
        ///ROTATIONS
    }
    AVLStatus RightRotation(const Node* rotatee){ ///get a pointer to the unstable node
        if(rotatee->prev -> left == rotatee){
            rotatee->prev->left = rotatee ->left;
            rotatee->left->right = rotatee;
            rotatee ->left -> prev = rotatee -> prev;
            rotatee -> prev = rotatee -> left;
            rotatee ->left;
        } ///what should be the else way?
    }
    AVLStatus LeftRotation(const Node* rotatee){

    }

    AVLStatus deleteNode(AVLTree tree, T* ptrToDelete){}
    private:
    Node* head;
    CF compareFunc; ///TRUE = RIGHT, FALSE = LEFT, what about equal???
};

#endif //WORLDCUP23A1_CPP_AVL_TREES_H
