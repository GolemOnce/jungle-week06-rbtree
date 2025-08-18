#include "rbtree.h"
#include <stdio.h>
#include <stdlib.h>

rbtree *new_rbtree(void)
{
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  if (!p)
  {
    return NULL;
  }

  node_t *nilNode = (node_t *)malloc(sizeof(node_t));
  if (!nilNode)
  {
    free(p);
    return NULL;
  }
  nilNode->key = 0;
  nilNode->left = nilNode->right = nilNode->parent = nilNode;
  nilNode->color = RBTREE_BLACK;

  p->root = nilNode;
  p->nil = nilNode;

  return p;
}

// 후위순회 돌면서 free?
void delete_rbtree(rbtree *t)
{
  node_t *cur = t->root;

  if (!t)
    return;

  node_t *parent;

  while (cur != t->nil)
  {
    if (cur->left != t->nil)
    {
      cur = cur->left;
    }
    else if (cur->right != t->nil)
    {
      cur = cur->right;
    }
    else if (cur->parent == t->nil)
    {
      free(cur);
      t->root = t->nil;
      break;
    }
    else
    {
      parent = cur->parent;
      if (parent->left == cur)
        parent->left = t->nil;
      else
        parent->right = t->nil;
      free(cur);
      cur = parent;
    }
  }

  free(t->nil);
  free(t);
  return;
}

node_t *rbtree_insert(rbtree *t, const key_t key)
{
  node_t *newNode = (node_t *)malloc(sizeof(node_t));
  if (!newNode)
    return NULL;
  newNode->color = RBTREE_RED;
  newNode->key = key;
  newNode->parent = newNode->left = newNode->right = t->nil;
  // newNode 동적생성
  if (t->root == t->nil)
  {
    t->root = newNode;
    newNode->color = RBTREE_BLACK;
    return t->root;
  }
  // 빈 트리인 경우 루트노드 만들고 리턴

  node_t *cur = t->root;

  while (cur != t->nil)
  {
    if (cur->key == key)
    {
      free(newNode);
      return t->root;
    } // 이미 존재하는 값이면 free하고 리턴

    if (cur->key > key)
    {
      if (cur->left != t->nil)
      {
        cur = cur->left;
      }
      else
      {
        cur->left = newNode;
        break;
      }
    }
    else if (cur->key < key)
    {
      if (cur->right != t->nil)
      {
        cur = cur->right;
      }
      else
      {
        cur->right = newNode;
        break;
      }
    }
  }
  newNode->parent = cur;
  node_t *parent = cur;
  node_t *grand; // parent->parent;
  node_t *uncle; //((grand->left==parent)?grand->right:grand->left);

  if (parent->color == RBTREE_BLACK)
    return t->root;

  while (parent->color == RBTREE_RED)
  {
    parent = newNode->parent;
    if (parent->color != RBTREE_RED)
      break;
    grand = parent->parent;
    uncle = (grand->left == parent ? grand->right : grand->left);
    if (uncle->color == RBTREE_RED)
    {
      parent->color = RBTREE_BLACK;
      uncle->color = RBTREE_BLACK;
      grand->color = RBTREE_RED;
      newNode = grand;
      continue;
    } // case 1 부모, 삼촌 빨강 - recoloring
    else
    {
      if (grand->left == parent && parent->right == newNode)
      {
        left_rotation(t, parent);
        parent = newNode;
        newNode = newNode->left;
      } // case 2-1 부모 빨강, 삼촌 검정 + N\P/G
      else if (grand->right == parent && parent->left == newNode)
      {
        right_rotation(t, parent);
        parent = newNode;
        newNode = newNode->right;
      } // case 2-2 부모 빨강, 삼촌 검정 - G\P/N

      if (grand->left == parent && parent->left == newNode)
      {
        right_rotation(t, grand);
      } // case3-1 부모 빨강, 삼촌 검정 + N/P/G 직선

      else if (grand->right == parent && parent->right == newNode)
      {
        left_rotation(t, grand);
      } // case3-2 부모 빨강, 삼촌 검정 + G\P\N 직선
      grand->color = RBTREE_RED;
      parent->color = RBTREE_BLACK;
      break;
    }
  }
  t->root->color = RBTREE_BLACK;
  return t->root;
}

static void left_rotation(rbtree *t, node_t *parent)
{
  node_t *cur = parent->right;
  if (parent->right == t->nil)
    return;
  if (parent == t->root)
  {
    t->root = cur;
    cur->parent = t->nil;
  }
  else
  {
    node_t *grand = parent->parent;
    cur->parent = grand;
    if (grand->left == parent)
      grand->left = cur;
    else
      grand->right = cur;
  }
  parent->right = cur->left;
  if (cur->left != t->nil)
  {
    cur->left->parent = parent;
  }
  parent->parent = cur;
  cur->left = parent;
}

static void right_rotation(rbtree *t, node_t *parent)
{
  node_t *cur = parent->left;
  if (parent->left == t->nil)
    return;
  if (parent == t->root)
  {
    t->root = cur;
    cur->parent = t->nil;
  }
  else
  {
    node_t *grand = parent->parent;
    cur->parent = grand;
    if (grand->left == parent)
      grand->left = cur;
    else
      grand->right = cur;
  }
  parent->left = cur->right;
  if (cur->right != t->nil)
  {
    cur->right->parent = parent;
  }
  parent->parent = cur;
  cur->right = parent;
}

node_t *rbtree_find(const rbtree *t, const key_t key)
{
  // node_t *cur = t->root;
  // if (cur == t->nil)
  //   return t->nil;

  // while (cur->key != key) {
  //   if (cur->key > key && cur->left != t->nil){ cur = cur->left; }
  //   else if (cur->key < key && cur->right != t->nil) { cur = cur->right; }
  //   else { break; }
  // }
  // if (cur->key == key)
  //   return cur;
  // return t->nil;
  node_t *cur = t->root;
  while (cur != t->nil) {
    if (key < cur->key) cur = cur->left;
    else if (key > cur->key) cur = cur->right;
    else return cur;
  }
  return t->nil;
}

node_t *rbtree_min(const rbtree *t)
{
  // TODO: implement find
  if (t->root == t->nil)
    return t->nil;

  node_t *cur = t->root;
  while (cur->left != t->nil)
  {
    cur = cur->left;
  }

  return cur;
}

node_t *rbtree_max(const rbtree *t)
{
  // TODO: implement find
  if (t->root == t->nil)
    return t->nil;

  node_t *cur = t->root;
  while (cur->right != t->nil)
  {
    cur = cur->right;
  }

  return cur;
}

int rbtree_erase(rbtree *t, node_t *p)
{
  if (t->root = t->nil)
  {
    return 1;
  }
  node_t *cur = t->root;
  if (cur->key > p->key)
  {
  
  }

  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n)
{
  // TODO: implement to_array
  return 0;
}
