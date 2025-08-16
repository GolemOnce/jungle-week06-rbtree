#include "rbtree.h"

#include <stdlib.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  if (!p) {return NULL;}

  node_t *nilNode = (node_t*)malloc(sizeof(node_t));
  if (!nilNode) {free(p); return NULL;}
  nilNode->key = 0;
  nilNode->left = nilNode;
  nilNode->right = nilNode;
  nilNode->parent = nilNode;
  nilNode->color = RBTREE_BLACK;
  
  p->root = nilNode;
  p->nil = nilNode;
  
  return p;
}

//후위순회 돌면서 free?
void delete_rbtree(rbtree *t) {
  node_t *cur = t->root;

  if (!t) return;
  
  node_t *parent;

  while(cur != t->nil){
    if (cur->left != t->nil){
      cur = cur->left;
    }
    else if (cur->right != t->nil){
      cur = cur->right;
    }
    else if (cur->parent == t->nil){
      free(cur);
      t->root = t->nil;
      break;
    }
    else{
      parent = cur->parent;
      if (parent->left == cur) parent->left = t->nil;
      else parent->right = t->nil;
      free(cur);
      cur = parent;
    }
  }

  free(t->nil);
  free(t);
  return;
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  node_t *newNode = (node_t*)malloc(sizeof(node_t));
  newNode->color = RBTREE_RED;
  newNode->key = key;
  newNode->parent = t->nil;
  newNode->left = t->nil;
  newNode->right = t->nil;

  if (t->root = t->nil) t->root = newNode;

  return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  return t->root;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  return 0;
}
