#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {
    
    TreeMap * tree = (TreeMap *)malloc(sizeof(TreeMap));
    tree->root = NULL;
    tree->current = NULL;
    tree->lower_than = lower_than;
    return tree;
    
    //new->lower_than = lower_than;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) {

    TreeNode *nuevoNodo = createTreeNode(key, value);
    
    if(tree->root == NULL){
        tree->root = nuevoNodo;
        tree->current = nuevoNodo;
        return;
    }

    TreeNode *current = tree->root;

    while(current != NULL){
        if(tree->lower_than(key,current->pair->key)){
            if(current->left == NULL){
                current->left = nuevoNodo;
                nuevoNodo->parent = current;
                tree->current = nuevoNodo;
                return;
            }
            else current = current->left;
        }
        else if(tree->lower_than(current->pair->key , key)){
            if(current->right == NULL){
                current->right = nuevoNodo;
                nuevoNodo->parent = current;
                tree->current = nuevoNodo;
                return;
            }
            else current = current->right;
        }
        else{
            current->pair->value = value;
            return;
        }
    }
    return;
}

TreeNode * minimum(TreeNode * tree){
    if(tree == NULL) return NULL;

    while(tree->left != NULL) tree = tree->left;
    return tree;
}


void removeNode(TreeMap * tree, TreeNode* node) {

    if(node == NULL)return;
    if(node->left == NULL && node->right == NULL){

        if(node->parent->left == node) node->parent->left = NULL;
        else node->parent->right = NULL;
        free(node);
        return;
    }

    if(node->left != NULL && node->right != NULL){
        TreeNode *min = minimum(node->right);
        node->pair->key = min->pair->key;
        node->pair->value = min->pair->value;
        removeNode(tree, min);
        return;
    }

    if(node->left != NULL){
        TreeNode *min = minimum(node->left);
        if (min != NULL){
            node->pair->key = min->pair->key;
            node->pair->value = min->pair->value;
            removeNode(tree, min);
        }
        return;
    }
    
    if(node->right!= NULL){
        TreeNode *min = minimum(node->right);
        if (min != NULL){
            node->pair->key = min->pair->key;
            node->pair->value = min->pair->value;
            removeNode(tree, min);
        }
        return;
    }

    return;
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}




Pair * searchTreeMap(TreeMap * tree, void* key) {

    if (tree == NULL || tree->root == NULL) return NULL;
    TreeNode * current = tree->root;

    while(current != NULL){
        Pair *pair = current->pair;
        
        if(tree->lower_than(key,pair->key)) current = current->left;  
        else if(tree->lower_than(pair->key,key)) current = current->right;
        else{
            tree->current = current;
            return pair;
        }
    }
        
    return NULL;
}


Pair * upperBound(TreeMap * tree, void* key) {
    return NULL;
}

Pair * firstTreeMap(TreeMap * tree) {
    if(tree == NULL) return NULL;

    TreeNode *min = minimum(tree->root);
    if(min == NULL) return NULL;
    
    tree->current = min;
    return min->pair;
}

Pair * nextTreeMap(TreeMap * tree) {

    if(tree == NULL || tree->current == NULL) return NULL;

    TreeNode *current = tree->current;

    if(current->right != NULL){
        tree->current = minimum(current->right);
        return tree->current->pair;
    }
    
    while(current->right != NULL && current == current->parent->right){
        current = current->parent;
    }
    if (current->parent == NULL) return NULL;
    
    tree->current = current->parent;   
    
    return tree->current->pair;   
}
