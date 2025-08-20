#include "rbtree.h"
#include <stdio.h>
#include <stdlib.h>

rbtree *new_rbtree(void)
{
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  if (!p)
    return NULL;

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
void delete_rbtree(rbtree *t) {
  if (!t || t->root == t->nil) return;
  node_t *cur = t->root;
  node_t *parent;

  while (cur != t->nil){
    if (cur->left != t->nil) cur = cur->left;
    else if (cur->right != t->nil) cur = cur->right;
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
    cur->left->parent = parent;

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
    cur->right->parent = parent;

  parent->parent = cur;
  cur->right = parent;
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
    if (cur->key >= key)
    {
      if (cur->left != t->nil)
        cur = cur->left;
      else
      {
        cur->left = newNode;
        break;
      }
    }
    else if (cur->key < key)
    {
      if (cur->right != t->nil)
        cur = cur->right;
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

      if (grand != t->nil)
      { // ★ nil은 절대 RED로 칠하면 안 됨
        grand->color = RBTREE_RED;
        newNode = grand;
        continue;
      }
      else
      {
        break;
      }
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
  // return NULL;
  node_t *cur = t->root;
  while (cur != t->nil)
  {
    if (key < cur->key)
      cur = cur->left;
    else if (key > cur->key)
      cur = cur->right;
    else
      return cur;
  }
  return NULL;
}

node_t *rbtree_min(const rbtree *t)
{
  // TODO: implement find
  if (t->root == t->nil)
    return t->nil;

  node_t *cur = t->root;
  while (cur->left != t->nil)
    cur = cur->left;

  return cur;
}

node_t *rbtree_max(const rbtree *t)
{
  // TODO: implement find
  if (t->root == t->nil)
    return t->nil;

  node_t *cur = t->root;
  while (cur->right != t->nil)
    cur = cur->right;

  return cur;
}

static void delete_fixup(rbtree *t, node_t *x)
{
  node_t *parent;
  node_t *bro;
  while (x != t->root && x->color == RBTREE_BLACK)
  {
    parent = x->parent;
    bro = (x == parent->left ? parent->right : parent->left);
    // CASE 1.
    if (bro->color == RBTREE_RED)
    {
      bro->color = RBTREE_BLACK;
      parent->color = RBTREE_RED;
      if (x == parent->left)
        left_rotation(t, parent);
      else
        right_rotation(t, parent);
      continue;
    }
    // CASE 2.
    if (bro->color == RBTREE_BLACK && bro->left->color == RBTREE_BLACK && bro->right->color == RBTREE_BLACK && bro != t->nil)
    {
      bro->color = RBTREE_RED;
      x = x->parent;
      continue;
    }

    else
    {
      // CASE 3.
      color_t tempcolor;
      node_t *cousin = (parent->right == bro) ? bro->left : bro->right;
      if (bro->color == RBTREE_BLACK && cousin->color == RBTREE_RED)
      {
        tempcolor = bro->color;
        bro->color = cousin->color;
        cousin->color = tempcolor;
        if (parent->right == bro)
          right_rotation(t, bro);
        else
          left_rotation(t, bro);
        bro = (x == parent->left) ? parent->right : parent->left;
      }

      // CASE 4. 형제 == BLACK, (**형제의 펴진 자식**) == RED
      bro->color = parent->color;
      parent->color = RBTREE_BLACK;
      cousin = (parent->right == bro) ? bro->right : bro->left;
      cousin->color = RBTREE_BLACK;
      if (parent->left == x)
      {
        left_rotation(t, parent);
        x = parent->right;
      }
      else
      {
        right_rotation(t, parent);
        x = parent->left;
      }
      break;
    }
  }
}

// static void delete_fixup(rbtree *t, node_t *x)
// {
//   while (x != t->root && x->color == RBTREE_BLACK) {
//     if (x == x->parent->left) {
//       node_t *w = x->parent->right;

//       // Case 1
//       if (w->color == RBTREE_RED) {
//         w->color = RBTREE_BLACK;
//         x->parent->color = RBTREE_RED;
//         left_rotation(t, x->parent);
//         w = x->parent->right;
//       }

//       // Case 2 (w 및 w의 자식 모두 BLACK; nil이면 자식도 BLACK 취급)
//       if (w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK) {
//         if (w != t->nil) w->color = RBTREE_RED;
//         x = x->parent;
//       } else {
//         // Case 3 (가까운 쪽이 RED)
//         if (w->right->color == RBTREE_BLACK) {
//           w->left->color = RBTREE_BLACK;
//           w->color = RBTREE_RED;
//           right_rotation(t, w);
//           w = x->parent->right;
//         }
//         // Case 4 (먼 쪽이 RED)
//         w->color = x->parent->color;
//         x->parent->color = RBTREE_BLACK;
//         w->right->color = RBTREE_BLACK;
//         left_rotation(t, x->parent);
//         x = t->root;              // ★ 여기!
//       }
//     } else {
//       // 대칭
//       node_t *w = x->parent->left;

//       if (w->color == RBTREE_RED) {
//         w->color = RBTREE_BLACK;
//         x->parent->color = RBTREE_RED;
//         right_rotation(t, x->parent);
//         w = x->parent->left;
//       }

//       if (w->right->color == RBTREE_BLACK && w->left->color == RBTREE_BLACK) {
//         if (w != t->nil) w->color = RBTREE_RED;
//         x = x->parent;
//       } else {
//         if (w->left->color == RBTREE_BLACK) {
//           w->right->color = RBTREE_BLACK;
//           w->color = RBTREE_RED;
//           left_rotation(t, w);
//           w = x->parent->left;
//         }
//         w->color = x->parent->color;
//         x->parent->color = RBTREE_BLACK;
//         w->left->color = RBTREE_BLACK;
//         right_rotation(t, x->parent);
//         x = t->root;              // ★ 대칭 쪽도 동일
//       }
//     }
//   }
//   x->color = RBTREE_BLACK;        // ★ 최종 보정
// }

int rbtree_erase(rbtree *t, node_t *p)
{
  if (t->root == t->nil || p == t->nil)
    return 0;

  node_t *x;
  node_t *y = p->parent;
  color_t originalcolor = p->color; // ← 실제 제거 노드의 원래 색

  if (p->left != t->nil && p->right != t->nil)
  {
    // 자식이 둘
    node_t *s = p->right;
    while (s->left != t->nil)
      s = s->left;

    originalcolor = s->color; // ← 후계자의 원래 색이 fixup 트리거 기준
    x = s->right;             // ← 후계자 자리를 대체할 노드(= fixup 인자)

    if (s->parent == p)
    {
      // 후계자가 바로 p의 오른쪽 자식
      x->parent = s; // x가 NIL이어도 parent 설정
    }
    else
    {
      if (s == s->parent->left)
        s->parent->left = x;
      else
        s->parent->right = x;
      x->parent = s->parent;

      // p의 오른쪽을 s로 연결
      s->right = p->right;
      s->right->parent = s;
    }

    // p자리에 s
    if (p->parent == t->nil)
      t->root = s;
    else if (p == y->left)
      y->left = s;
    else
      y->right = s;

    s->parent = y;
    s->left = p->left; // p의 왼쪽을 s로 연결
    s->left->parent = s;
    s->color = p->color; // s는 p의 색을 승계

    free(p);
  }
  else
  {
    // 자식이 0~1
    node_t *child = (p->left != t->nil) ? p->left : p->right;
    x = child; // fixup 인자

    // p 자리를 child로 치환 (루트 우선)
    if (p->parent == t->nil)
      t->root = child;
    else if (p == y->left)
      y->left = child;
    else
      y->right = child;

    if (child != t->nil)
      child->parent = y;
    else
      t->nil->parent = y;
    free(p);
  }

  // 구조 수정
  if (originalcolor == RBTREE_BLACK)
  {
    if (x->color == RBTREE_RED)
    {
      x->color = RBTREE_BLACK;
    }
    else
    {
      delete_fixup(t, x);
    }
  }

  t->root->color = RBTREE_BLACK; // 루트 보정
  return 0;
}

static void preorder(const rbtree *t, const node_t *node, key_t *arr, size_t n, size_t *idx)
{
  if (node == t->nil || *idx >= n)
    return;
  
  preorder(t, node->left, arr, n, idx);
  if (*idx < n)
  {
    arr[(*idx)++] = node->key;
  }
  preorder(t, node->right, arr, n, idx);
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n)
{
  if (!t || !arr || n == 0)
    return 0;

  size_t idx = 0;
  preorder(t, t->root, arr, n, &idx);
  return 0;
}