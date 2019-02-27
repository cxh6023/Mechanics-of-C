///
// File: $Id: tree.h,v 1.1 2018/10/11 13:10:14 csci243 Exp $
//
// Concordance generator search tree definitions
//
// @author  wrc
//
// Revision history:
//
//	$Log: tree.h,v $
//	Revision 1.1  2018/10/11 13:10:14  csci243
//	Initial revision
//
///

#ifndef _TREE_H_
#define _TREE_H_

// The definition of a node in the tree

typedef struct TreeNode_st {
    char *word;                   // the word held in this node
    unsigned int frequency;       // how many times it has been seen
    unsigned int refLength;       // capacity of the reference list
    unsigned int *references;     // list of references
    struct TreeNode_st *left;     // node's left child
    struct TreeNode_st *right;    // node's right child
} TreeNode;

// FUNCTIONS STUDENTS ARE REQUIRED TO IMPLEMENT

///
// insert_word() 
//     Dynamically build BST by allocating nodes from the heap
//
// args -
//        root - a pointer to the pointer to the root of the tree
//               to build this tree on to.
//        word - string containing the word to be inserted
///

void insert_word( TreeNode** root, const char *word );

///
// traverse_tree()
//    Recursively traverses the tree and prints the value of each
//    node.
//
// args -
//        root - a pointer to the root of the tree to traverse
///

void traverse_tree( const TreeNode* root );

///
// cleanup_tree()
//    Cleanup all memory management associated with the nodes on the heap
//
// args
//      root - the current root of the tree
///

void cleanup_tree( TreeNode* root );

#endif // BST_H
