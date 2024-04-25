#include "rbtree.h"

#include <stdlib.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  node_t *nil = (node_t *)calloc(1, sizeof(node_t));
  nil->color = RBTREE_BLACK;
  p->root = p->nil = nil;
  return p;
}

void search_delete(rbtree *t,node_t *node){
  if (node -> left != t -> nil)
    search_delete(t,node -> left);
  if (node -> right != t -> nil)
    search_delete(t,node -> right);
  free(node);
}

void delete_rbtree(rbtree *t) {
  node_t *node = t -> root;
  if (node != t->nil)
    search_delete(t,node);
  free(t->nil);
  free(t);
}

void left_rotate(rbtree *t, node_t *x) {

    node_t *y = x->right;
    
    // `y`의 왼쪽 자식을 `x`의 오른쪽 자식으로 이동
    x->right = y->left;
    if (y->left != t->nil) {
        y->left->parent = x;
    }
    
    // `y`의 부모를 `x`의 부모로 설정
    y->parent = x->parent;

    // `x`의 부모를 확인하여 `y`를 적절한 자식으로 설정
    if (x->parent == t->nil) {
        // `x`가 루트 노드인 경우 트리의 루트 노드를 `y`로 설정
        t->root = y;
    } else if (x == x->parent->left) {
        // `x`가 부모의 왼쪽 자식인 경우 `y`를 부모의 왼쪽 자식으로 설정
        x->parent->left = y;
    } else {
        // `x`가 부모의 오른쪽 자식인 경우 `y`를 부모의 오른쪽 자식으로 설정
        x->parent->right = y;
    }
    
    // `y`의 왼쪽 자식을 `x`로 설정
    y->left = x;

    // `x`의 부모를 `y`로 설정
    x->parent = y;
}


void right_rotate(rbtree *t, node_t *x) {

    node_t *y = x->left;

    // `y`의 오른쪽 자식을 `x`의 왼쪽 자식으로 이동
    x->left = y->right;
    if (y->right != t->nil) {
        // `y`의 오른쪽 자식이 `nil`이 아니라면 `x`를 부모로 설정
        y->right->parent = x;
    }

    // `y`의 부모를 `x`의 부모로 설정
    y->parent = x->parent;
    
    // `x`의 부모를 확인하여 `y`를 적절한 자식으로 설정
    if (x->parent == t->nil) {
        // `x`가 루트 노드인 경우 트리의 루트 노드를 `y`로 설정
        t->root = y;
    } else if (x == x->parent->right) {
        // `x`가 부모의 오른쪽 자식인 경우 `y`를 부모의 오른쪽 자식으로 설정
        x->parent->right = y;
    } else {
        // `x`가 부모의 왼쪽 자식인 경우 `y`를 부모의 왼쪽 자식으로 설정
        x->parent->left = y;
    }

    // `y`의 오른쪽 자식을 `x`로 설정
    y->right = x;
    
    // `x`의 부모를 `y`로 설정
    x->parent = y;
}

void rbtree_insert_fixup(rbtree *t, node_t *z) {
    node_t *y;

    while (z->parent->color == RBTREE_RED) {
        if (z->parent == z->parent->parent->left) {
            // 부모 노드가 조부모 노드의 왼쪽 자식인 경우
            y = z->parent->parent->right;

            if (y->color == RBTREE_RED) {
                // 삼촌 노드 `y`가 레드인 경우
                z->parent->color = RBTREE_BLACK;
                y->color = RBTREE_BLACK;
                z->parent->parent->color = RBTREE_RED;
                z = z->parent->parent;
            } else {
                // 삼촌 노드 `y`가 블랙인 경우
                if (z == z->parent->right) {
                    z = z->parent;
                    left_rotate(t, z);
                }
                z->parent->color = RBTREE_BLACK;
                z->parent->parent->color = RBTREE_RED;
                right_rotate(t, z->parent->parent);
            }
        } else {
            // 부모 노드가 조부모 노드의 오른쪽 자식인 경우
            y = z->parent->parent->left;

            if (y->color == RBTREE_RED) {
                // 삼촌 노드 `y`가 레드인 경우
                z->parent->color = RBTREE_BLACK;
                y->color = RBTREE_BLACK;
                z->parent->parent->color = RBTREE_RED;
                z = z->parent->parent;
            } else {
                // 삼촌 노드 `y`가 블랙인 경우
                if (z == z->parent->left) {
                    z = z->parent;
                    right_rotate(t, z);
                }
                z->parent->color = RBTREE_BLACK;
                z->parent->parent->color = RBTREE_RED;
                left_rotate(t, z->parent->parent);
            }
        }
    }
    t->root->color = RBTREE_BLACK;
}

void rb_transplant(rbtree *t, node_t *u, node_t *v) {
    // `u`의 부모 노드가 트리의 `nil` 노드인 경우,
    // `v`를 트리의 루트 노드로 설정
    if (u->parent == t->nil) {
        t->root = v;
    } else if (u == u->parent->left) {
        // `u`가 부모의 왼쪽 자식인 경우, `v`를 부모의 왼쪽 자식으로 설정
        u->parent->left = v;
    } else {
        // `u`가 부모의 오른쪽 자식인 경우, `v`를 부모의 오른쪽 자식으로 설정
        u->parent->right = v;
    }
    
    // `v`의 부모를 `u`의 부모로 설정
    v->parent = u->parent;
}

void rbtree_delete_fixup(rbtree *t, node_t *x) {
    while (x != t->root && x->color == RBTREE_BLACK) {
        if (x == x->parent->left) {
            // `x`가 부모의 왼쪽 자식인 경우
            node_t *w = x->parent->right;

            // `w`가 레드인 경우 색상 조정 및 왼쪽 회전
            if (w->color == RBTREE_RED) {
                w->color = RBTREE_BLACK;
                x->parent->color = RBTREE_RED;
                left_rotate(t, x->parent);
                w = x->parent->right;
            }

            // `w`의 자식이 모두 블랙인 경우
            if (w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK) {
                w->color = RBTREE_RED;
                x = x->parent;
            } else {
                // `w`의 오른쪽 자식이 블랙인 경우 색상 조정 및 오른쪽 회전
                if (w->right->color == RBTREE_BLACK) {
                    w->left->color = RBTREE_BLACK;
                    w->color = RBTREE_RED;
                    right_rotate(t, w);
                    w = x->parent->right;
                }
                // `w`의 색상을 부모와 일치시켜 왼쪽 회전
                w->color = x->parent->color;
                x->parent->color = RBTREE_BLACK;
                w->right->color = RBTREE_BLACK;
                left_rotate(t, x->parent);
                // 기준점 초기화
                x = t->root;
            }
        } else {
            // `x`가 부모의 오른쪽 자식인 경우
            node_t *w = x->parent->left;

            // `w`가 레드인 경우 색상 조정 및 오른쪽 회전
            if (w->color == RBTREE_RED) {
                w->color = RBTREE_BLACK;
                x->parent->color = RBTREE_RED;
                right_rotate(t, x->parent);
                w = x->parent->left;
            }

            // `w`의 자식이 모두 블랙인 경우
            if (w->right->color == RBTREE_BLACK && w->left->color == RBTREE_BLACK) {
                w->color = RBTREE_RED;
                x = x->parent;
            } else {
                // `w`의 왼쪽 자식이 블랙인 경우 색상 조정 및 왼쪽 회전
                if (w->left->color == RBTREE_BLACK) {
                    w->right->color = RBTREE_BLACK;
                    w->color = RBTREE_RED;
                    left_rotate(t, w);
                    w = x->parent->left;
                }
                // `w`의 색상을 부모와 일치시켜 오른쪽 회전
                w->color = x->parent->color;
                x->parent->color = RBTREE_BLACK;
                w->left->color = RBTREE_BLACK;
                right_rotate(t, x->parent);
                // 기준점 초기화
                x = t->root;
            }
        }
    }
    // `x`의 색상을 블랙으로 설정
    x->color = RBTREE_BLACK;
}

void inorder(const rbtree *t, node_t *node, key_t *arr, const size_t n, size_t *cnt) {
    // `cnt`가 `n`에 도달하거나 `node`가 `nil` 노드인 경우 종료
    if (*cnt == n || node == t->nil) {
        return;
    }
    // 왼쪽 하위 트리 순회
    inorder(t, node->left, arr, n, cnt);
    // 현재 노드의 키를 배열에 저장
    arr[*cnt] = node->key;
    // `cnt` 증가
    (*cnt)++;
    // 오른쪽 하위 트리 순회
    inorder(t, node->right, arr, n, cnt);
}
 

node_t *rbtree_insert(rbtree *t, const key_t key) {

    node_t *z = (node_t *)calloc(1, sizeof(node_t));
    z->key = key;
    z->color = RBTREE_RED;  // 새 노드는 레드로 시작
    z->left = t->nil;
    z->right = t->nil;
    
    node_t *y = t->nil;
    node_t *x = t->root;

    // `z`를 삽입할 위치 찾기
    while (x != t->nil) {
        y = x;
        if (key < x->key) {
            x = x->left;
        } else {
            x = x->right;
        }
    }

    // `z`의 부모를 `y`로 설정
    z->parent = y;

    // 트리가 비어 있으면 `z`를 루트 노드로 설정
    if (y == t->nil) {
        t->root = z;
    } else if (key < y->key) {
        // `z`를 `y`의 왼쪽 자식으로 설정
        y->left = z;
    } else {
        // `z`를 `y`의 오른쪽 자식으로 설정
        y->right = z;
    }

    // 삽입 후 트리 균형 조정
    rbtree_insert_fixup(t, z);

    return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
    
    node_t *current = t->root;

    while (current != t->nil) {
        if (key == current->key) {
            return current;
        }
        else if (key < current->key) {
            current = current->left;
        }
        else {
            current = current->right;
        }
    }

    return NULL;
}

node_t *rbtree_min(const rbtree *t) {
  node_t *min = t -> root;
  while (min -> left != t -> nil){
    min = min -> left;
  }
  return min;
}

node_t *rbtree_min_search(const rbtree *t , node_t *node) {
 node_t *cur = node;
 while(cur->left != t->nil){
    cur = cur->left;
  }
 return cur;
}

node_t *rbtree_max(const rbtree *t) {
  node_t *max = t -> root;
  while (max -> right != t -> nil)
  {
    max = max -> right;
  }
  return max;
}

int rbtree_erase(rbtree *t, node_t *z) {

  node_t *y = z;
  node_t *x;
  color_t y_original_color = y ->color;

  if (z -> left == t -> nil){
    x = z -> right;
    rb_transplant(t,z,z->right);
  }
  else if( z-> right == t -> nil){
    x = z -> left;
    rb_transplant(t,z,z->left);
  }else{
    y = rbtree_min_search(t, z -> right);
    y_original_color = y -> color;
    x = y -> right;

    if(y -> parent == z){
      x -> parent = y;
    }else{
      rb_transplant(t,y,y->right);
      y -> right = z -> right;
      y -> right -> parent = y;
    }
    rb_transplant(t,z,y);
    y -> left = z -> left;
    y -> left -> parent = y;
    y -> color = z -> color;
  }

  if (y_original_color == RBTREE_BLACK){
    rbtree_delete_fixup(t,x);
  }

  free(z);

  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
    size_t cnt = 0;
    inorder(t, t->root, arr, n, &cnt);
    return 0;
}

