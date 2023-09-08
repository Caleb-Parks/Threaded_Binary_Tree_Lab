// From the software distribution accompanying the textbook
// "A Practical Introduction to Data Structures and Algorithm Analysis,
// Third Edition (C++)" by Clifford A. Shaffer.
// Source code Copyright (C) 2007-2011 by Clifford A. Shaffer.
// 1/26/22 pragma once modification by Prof Sipantzi
// This file includes all of the pieces of the BST implementation

// Include the node implementation
#include "BSTNode.h"

// Include the dictionary ADT
#include "dictionary.h"

#pragma once

using namespace std;

// Binary Search Tree implementation for the Dictionary ADT
template <typename Key, typename E>
class BST : public Dictionary<Key, E> {
private:
    BSTNode<Key, E>* root;   // Root of the BST

    int nodecount;         // Number of nodes in the BST

    // Private "helper" functions
    void clearhelp(BSTNode<Key, E>*);
    BSTNode<Key, E>* inserthelp(BSTNode<Key, E>*, const Key&, const E&, BSTNode<Key, E>* =NULL, BSTNode<Key, E>* =NULL);
    BSTNode<Key, E>* deletemin(BSTNode<Key, E>*);
    BSTNode<Key, E>* getmin(BSTNode<Key, E>*);
    BSTNode<Key, E>* removehelp(BSTNode<Key, E>*, const Key&);
    E* findhelp(BSTNode<Key, E>*, const Key&) const;
    void printhelp(BSTNode<Key, E>*, int) const;
    void vist(BSTNode<Key, E>*) const;

public:
    BST() { root = NULL; nodecount = 0; }  // Constructor

    //Note from Prof Sipantzi -- I've commented out the destructor
    //since you would have to change clearhelp() to make it work with
    //doubly-threaded trees and that is not part of the assignment.
    //~BST() { clearhelp(root); }            // Destructor

    void clear()   // Reinitialize tree
    {
        clearhelp(root); root = NULL; nodecount = 0;
    }

    // Insert a record into the tree.
    // k Key value of the record.
    // e The record to insert.
    void insert(const Key& k, const E& e) {
        root = inserthelp(root, k, e);
        nodecount++;
    }

    // Remove a record from the tree.
    // k Key value of record to remove.
    // Return: The record removed, or NULL if there is none.
    E* remove(const Key& k) {
        E* temp = findhelp(root, k);   // First find it
        if (temp != NULL) {
            root = removehelp(root, k);
            nodecount--;
        }
        return temp;
    }
    // Remove and return the root node from the dictionary.
    // Return: The record removed, null if tree is empty.
    E* removeAny() {  // Delete min value
        if (root != NULL) {
            E* temp = new E;
            *temp = root->element();
            root = removehelp(root, root->key());
            nodecount--;
            return temp;
        }
        else return NULL;
    }

    // Return Record with key value k, NULL if none exist.
    // k: The key value to find. */
    // Return some record matching "k".
    // Return true if such exists, false otherwise. If
    // multiple records match "k", return an arbitrary one.
    E* find(const Key& k) const { return findhelp(root, k); }

    // Return the number of records in the dictionary.
    int size() { return nodecount; }

    void print() const { // Print the contents of the BST
        if (root == NULL) cout << "The BST is empty.\n";
        else printhelp(root, 0);
    }

    /* Does an inorder printing of the tree without recursion.
    Time Complexity = Linear, O(n) */
    void printinorder() const {
        BSTNode<Key, E>* temp = root;// Start at the root
        while (temp->left() != NULL) { temp = temp->left(); }// Go as far left as you can

        while (temp != NULL) {// If not at the last (in traversal) node
            cout << temp->element() << "\n";// Visit the node
            if (temp->getRThr())
                temp = temp->right();// If right child of processed node was thread: follow it and then we're ready to process          
            else {
                temp = temp->right();
                while (!temp->getLThr())// If right child of processed node was NOT thread: follow it then go left as far as you can till you hit a left thread
                    temp = temp->left();                
            }
        }
    }// End of printinorder

    /* Does a reverse order printing of the tree without recursion.
    Time Complexity = Linear, O(n) */
    void printReverse() const {
        BSTNode<Key, E>* temp = root;// Start at the root
        while (temp->right() != NULL) { temp = temp->right(); }// Go as far left as you can

        while (temp != NULL) {// If not at the first (in traversal) node
            cout << temp->element() << "\n";// Visit the node
            if (temp->getLThr())
                temp = temp->left();// If left child of processed node was thread: follow it and then we're ready to process          
            else {
                temp = temp->left();
                while (!temp->getRThr())// If right child of processed node was NOT thread: follow it then go right as far as you can till you hit a right thread
                    temp = temp->right();
            }
        }
    }// End of printReverse

};// End of BST class

// Visit -- prints out root
template <typename Key, typename E>
void BST<Key, E>::vist(BSTNode<Key, E>* r) const {
    cout << "Node - " << r->element() << endl;
}

// Clean up BST by releasing space back free store
template <typename Key, typename E>
void BST<Key, E>::
clearhelp(BSTNode<Key, E>* root) {
    if (root == NULL) return;
    clearhelp(root->safeLeft());
    clearhelp(root->safeRight());
    delete root;
}

// Insert a node into the BST, returning the updated tree
template <typename Key, typename E>
BSTNode<Key, E>* BST<Key, E>::inserthelp(BSTNode<Key, E>* root, const Key& k, const E& it, BSTNode<Key, E>* pred, BSTNode<Key, E>* succ){
    if (root == NULL) {  // Empty tree: create node
        BSTNode<Key, E>* tempNode = new BSTNode<Key, E>(k, it, pred, succ);// Creating new node
        tempNode->setLThr(true);// Recordiong in the node that both it's child pointers are threads
        tempNode->setRThr(true);// Recordiong in the node that both it's child pointers are threads
        return tempNode;
    }

    if (k < root->key()) {
        root->setLeft(inserthelp(root->safeLeft(), k, it, pred, root));// Moving left
        root->setLThr(false);// Recording that this root's left child is no longer a thread
    }
    else {
        root->setRight(inserthelp(root->safeRight(), k, it, root, succ));// Moving right
        root->setRThr(false);// Recording that this root's right child is no longer a thread
    }

    return root;// Return tree with node inserted
}

// Delete the minimum value from the BST, returning the revised BST
template <typename Key, typename E>
BSTNode<Key, E>* BST<Key, E>::getmin(BSTNode<Key, E>* rt) {
    if (rt->safeLeft() == NULL)
        return rt;
    else return getmin(rt->safeLeft());
}
template <typename Key, typename E>
BSTNode<Key, E>* BST<Key, E>::deletemin(BSTNode<Key, E>* rt) {
    if (rt->safeLeft() == NULL) // Found min
        return rt->safeRight();
    else {                      // Continue left
        rt->setLeft(deletemin(rt->safeLeft()));
        return rt;
    }
}

// Remove a node with key value k
// Return: The tree with the node removed
template <typename Key, typename E>
BSTNode<Key, E>* BST<Key, E>::removehelp(BSTNode<Key, E>* rt, const Key& k) {
    if (rt == NULL) return NULL;    // k is not in tree
    else if (k < rt->key())
        rt->setLeft(removehelp(rt->safeLeft(), k));
    else if (k > rt->key())
        rt->setRight(removehelp(rt->safeRight(), k));
    else {                            // Found: remove it
        BSTNode<Key, E>* temp = rt;
        if (rt->safeLeft() == NULL) {     // Only a right child
            rt = rt->safeRight();         //  so point to right
            delete temp;
        }
        else if (rt->safeRight() == NULL) { // Only a left child
            rt = rt->safeLeft();          //  so point to left
            delete temp;
        }
        else {                    // Both children are non-empty
            BSTNode<Key, E>* temp = getmin(rt->safeRight());
            rt->setElement(temp->element());
            rt->setKey(temp->key());
            rt->setRight(deletemin(rt->safeRight()));
            delete temp;
        }
    }
    return rt;
}

// Find a node with the given key value
template <typename Key, typename E>
E* BST<Key, E>::findhelp(BSTNode<Key, E>* root, const Key& k) const {
    if (root == NULL) return NULL;          // Empty tree
    if (k < root->key())
        return findhelp(root->safeLeft(), k);   // Check left
    else if (k > root->key())
        return findhelp(root->safeRight(), k);  // Check right
    else {
        E* temp = new E;
        *temp = root->element();
        return temp;  // Found it
    }
}

// Print out a BST
template <typename Key, typename E>
void BST<Key, E>::printhelp(BSTNode<Key, E>* root, int level) const {
    if (root == NULL) 
        return;           // Empty tree
    printhelp(root->safeLeft(), level + 1);   // Do left subtree, if it's not a thread
    for (int i = 0; i < level; i++)         // Indent to level
        cout << "  ";
    cout << root->key() << "\n";        // Print node value
    printhelp(root->safeRight(), level + 1);  // Do right subtree, if it's not a thread
}