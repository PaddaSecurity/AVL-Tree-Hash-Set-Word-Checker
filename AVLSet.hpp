// AVLSet.hpp
//
// ICS 46 Winter 2022
// Project #4: Set the Controls for the Heart of the Sun
//
// An AVLSet is an implementation of a Set that is an AVL tree, which uses
// the algorithms we discussed in lecture to maintain balance every time a
// new element is added to the set.  The balancing is actually optional,
// with a bool parameter able to be passed to the constructor to explicitly
// turn the balancing on or off (on is default).  If the balancing is off,
// the AVL tree acts like a binary search tree (e.g., it will become
// degenerate if elements are added in ascending order).
//
// You are not permitted to use the containers in the C++ Standard Library
// (such as std::set, std::map, or std::vector) to store the information
// in your data structure.  Instead, you'll need to implement your AVL tree
// using your own dynamically-allocated nodes, with pointers connecting them,
// and with your own balancing algorithms used.

#ifndef AVLSET_HPP
#define AVLSET_HPP

#include <functional>
#include "Set.hpp"



template <typename ElementType>
class AVLSet : public Set<ElementType>
{
public:
    // A VisitFunction is a function that takes a reference to a const
    // ElementType and returns no value.
    using VisitFunction = std::function<void(const ElementType&)>;

public:
    // Initializes an AVLSet to be empty, with or without balancing.
    explicit AVLSet(bool shouldBalance = true);

    // Cleans up the AVLSet so that it leaks no memory.
    ~AVLSet() noexcept override;

    // Initializes a new AVLSet to be a copy of an existing one.
    AVLSet(const AVLSet& s);

    // Initializes a new AVLSet whose contents are moved from an
    // expiring one.
    AVLSet(AVLSet&& s) noexcept;

    // Assigns an existing AVLSet into another.
    AVLSet& operator=(const AVLSet& s);

    // Assigns an expiring AVLSet into another.
    AVLSet& operator=(AVLSet&& s) noexcept;


    // isImplemented() should be modified to return true if you've
    // decided to implement an AVLSet, false otherwise.
    bool isImplemented() const noexcept override;


    // add() adds an element to the set.  If the element is already in the set,
    // this function has no effect.  This function always runs in O(log n) time
    // when there are n elements in the AVL tree.
    void add(const ElementType& element) override;


    // contains() returns true if the given element is already in the set,
    // false otherwise.  This function always runs in O(log n) time when
    // there are n elements in the AVL tree.
    bool contains(const ElementType& element) const override;


    // size() returns the number of elements in the set.
    unsigned int size() const noexcept override;


    // height() returns the height of the AVL tree.  Note that, by definition,
    // the height of an empty tree is -1.
    int height() const noexcept;


    // preorder() calls the given "visit" function for each of the elements
    // in the set, in the order determined by a preorder traversal of the AVL
    // tree.
    void preorder(VisitFunction visit) const;


    // inorder() calls the given "visit" function for each of the elements
    // in the set, in the order determined by an inorder traversal of the AVL
    // tree.
    void inorder(VisitFunction visit) const;


    // postorder() calls the given "visit" function for each of the elements
    // in the set, in the order determined by a postorder traversal of the AVL
    // tree.
    void postorder(VisitFunction visit) const;

    struct Node
    {
        ElementType data;
        Node* left;
        Node* right;
        int depth;
    }; 

    void addElement(const ElementType& element, Node*& root);
    void leftRotate(Node*& root);
    void rightRotate(Node*& root);
    int getMax(int rootLeft, int rootRight);
    int getDepth(Node*& root);

private:

    int treeSize, treeHeight;
    bool balance = true;
    Node* root;
    
};


template <typename ElementType>
AVLSet<ElementType>::AVLSet(bool shouldBalance)
    :treeSize{0}, treeHeight{0}, balance{shouldBalance}, root{nullptr}
{
    
}


template <typename ElementType>
AVLSet<ElementType>::~AVLSet() noexcept
{
}


template <typename ElementType>
AVLSet<ElementType>::AVLSet(const AVLSet& s)
{ 
    this->treeSize = s.treeSize;
    this->treeHeight = s.treeHeight;
    this->balance = s.balance;
    this->root = s.root;
}


template <typename ElementType>
AVLSet<ElementType>::AVLSet(AVLSet&& s) noexcept
{
}


template <typename ElementType>
AVLSet<ElementType>& AVLSet<ElementType>::operator=(const AVLSet& s)
{
    return *this;
}


template <typename ElementType>
AVLSet<ElementType>& AVLSet<ElementType>::operator=(AVLSet&& s) noexcept
{
    return *this;
}


template <typename ElementType>
bool AVLSet<ElementType>::isImplemented() const noexcept
{
    return true;
}


template <typename ElementType>
void AVLSet<ElementType>::add(const ElementType& element)
{
    if (!contains(element))
    {
        addElement(element, root);
        treeSize += 1;
    }
}

template <typename ElementType>
void AVLSet<ElementType>::addElement(const ElementType& element, Node*& root)
{
    if (root == nullptr)
    {
        root = new Node{element};
        root->depth = 1;
    }
    else if (element < root->data)
    {
        if (balance)
        {
            addElement(element, root->left);
            root->depth = getMax(getDepth(root->right), getDepth(root->left)) + 1;
            //Right Right Case
            if ((getDepth(root->left) - getDepth(root->right)) < -1)
            {
                if (root->right->data < root->data)
                {
                    leftRotate(root);
                }
            }

            // //Right Left Case
            if ((getDepth(root->left) - getDepth(root->right)) < -1)
            {
                if (root->right->data > root->data)
                {
                    rightRotate(root->right);
                    leftRotate(root);
                }
            }

            // //Left Left Case
            if ((getDepth(root->left) - getDepth(root->right)) > 1)
            {
                if (root->left->data > root->data)
                {
                    rightRotate(root);
                }
            }

            // //Left Right Case
            if ((getDepth(root->left) - getDepth(root->right)) > 1)
            {
                if (root->left->data < root->data)
                {
                    leftRotate(root->left);
                    rightRotate(root);
                }
            }
        }
        else
        {
            addElement(element, root->left);
        }
    }
    else if (element > root->data)
    {
        if (balance)
        {
            addElement(element, root->right);
            
            //Right Right Case
            if ((getDepth(root->left) - getDepth(root->right)) < -1)
            {
                if (root->right->data < root->data)
                {
                    leftRotate(root);
                }
            }

            // //Right Left Case
            if ((getDepth(root->left) - getDepth(root->right)) < -1)
            {
                if (root->right->data > root->data)
                {
                    rightRotate(root->right);
                    leftRotate(root);
                }
            }

            // //Left Left Case
            if ((getDepth(root->left) - getDepth(root->right)) > 1)
            {
                if (root->left->data > root->data)
                {
                    rightRotate(root);
                }
            }

            // //Left Right Case
            if ((getDepth(root->left) - getDepth(root->right)) > 1)
            {
                if (root->left->data < root->data)
                {
                    leftRotate(root->left);
                    rightRotate(root);
                }
            }
        }
        else
        {
            addElement(element, root->right);

        }
    }
}


template <typename ElementType>
bool AVLSet<ElementType>::contains(const ElementType& element) const
{
    Node* temp = root;

    while (temp != nullptr)
    {
        if (temp->data == element) { return true; }
        else if (element > temp->data) { temp = temp->right; }
        else if (element < temp->data) { temp = temp->left; }
    }
    
    return false;
}


template <typename ElementType>
unsigned int AVLSet<ElementType>::size() const noexcept
{
    return treeSize;
}


template <typename ElementType>
int AVLSet<ElementType>::height() const noexcept
{
    if (root == nullptr){ return -1; }
    return root->depth;
}


template <typename ElementType>
void AVLSet<ElementType>::preorder(VisitFunction visit) const
{
}


template <typename ElementType>
void AVLSet<ElementType>::inorder(VisitFunction visit) const
{
}


template <typename ElementType>
void AVLSet<ElementType>::postorder(VisitFunction visit) const
{
}


template <typename ElementType>
void AVLSet<ElementType>::leftRotate(Node*& root)
{
    Node* a = root->right;
    Node* b = a->left;


    a->left = root;
    root->right = b;

    root->depth = getMax(getDepth(root->left), getDepth(root->right)) + 1;
    a->depth = getMax(getDepth(a->left), getDepth(a->right)) + 1;
}

template <typename ElementType>
void AVLSet<ElementType>::rightRotate(Node*& root)
{

    Node* a = root->left;
    Node* b = a->right;


    a->right = root;
    root->left = b;

    root->depth = getMax(getDepth(root->left), getDepth(root->right)) + 1;
    root->depth = getMax(getDepth(root->left), getDepth(root->right)) + 1;


}

template <typename ElementType>
int AVLSet<ElementType>::getMax(int rootLeft, int rootRight)
{
    return rootLeft > rootRight ? rootLeft : rootRight;
}

template <typename ElementType>
int AVLSet<ElementType>::getDepth(Node*& root)
{
    if (root == nullptr){ return 0; }
    else { return root->depth; }
}




#endif

