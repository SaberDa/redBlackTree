//
//  redBlackTree.c
//  redBlackTree
//
//  Created by 李庚达 on 2016/11/15.
//  Copyright © 2016年 李庚达. All rights reserved.
//

#include "redBlackTree.h"
#include "redBalckTreeNode.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX(a,b) (a > b) ? a : b

#ifdef ASSERTS
#include <assert.h>
#else
#define assert(X) {/* Asserts are unused unless defined */}
#endif // ASSERTS

void applyKeyValue(Node *node, Key key, Value val) {
    node -> key = key;
    char *nodeBuffer = (char*)calloc(1, strlen(val));
    node -> value = nodeBuffer;
    sprintf(node -> value, "%s", val);
}

void KL_forEach(redBlackBST *self, keyList *list, void(*func)(redBlackBST*, Node*)) {
    Node *node;
    while (list && list -> node) {
        node = list -> node;
        func(self, node);
        keyList *toRelease = list;
        list = list -> next;
        free(toRelease);
    }
    free(list);
}

Node *createNode(Key _key, Value _value, bool _color, int _size) {
    Node *node = calloc(1, sizeof(Node));
    memset(node, 0, sizeof(Node));
    Node structNode = {
        .key = _key,
        .value = _value,
        .color = _color,
        .size = _size,
        .lChild = NULL,
        .rChild = NULL
    };
    applyKeyValue(&structNode, _key, _value);
    memcpy(node, &structNode, sizeof(Node));
    return (Node *)node;
}

/**
    基本二叉树函数
 */

Value *RBTGet(const redBlackBST *self, Key key) {
    if (key == NULL) {
        printf("argument to get() is NULL\n");
        exit(EXIT_FAILURE);
    }
    return NodeGet(self -> root, key);
}

int RBTSize(const redBlackBST *self) {
    return NodeSize(self -> root);
}

bool RBTisEmpty(const redBlackBST *self) {
    return self -> root == NULL;
}

bool RBTContains(const redBlackBST *self, Key key) {
    return RBTGet(self, key) != NULL;
}

const Node *RBTMinbyKey(const redBlackBST *self) {
    if (RBTisEmpty(self)) {
        printf("called max() with empty symbol table");
        exit(EXIT_FAILURE);
    }
    return NodeMinbyKey(self -> root);
}
const Node *RBTMaxbyKey(const redBlackBST *self) {
    if (RBTisEmpty(self)) {
        printf("called max() with empty symbol table");
        exit(EXIT_FAILURE);
    }
    return NodeMaxbyKey(self -> root);
}

/**
    红黑树删除
 */

//移去最大值
void RBTDeleteMax(redBlackBST *self) {
    if (RBTisEmpty(self)) {
        printf("BST underflow");
        exit(EXIT_FAILURE);
    }
    //如果两个孩子都是黑的，将父结点设为红
    if (!NodeisRed(self -> root -> lChild) && !NodeisRed(self -> root -> rChild)) {
        self -> root -> color = RED;
    }
    self -> root = NodeDeleteMax(self -> root);
    if (!RBTisEmpty(self)) {
        self -> root -> color = BLACK;
    }
    assert(RBTSelfCheck(self));
}

//移去特定值
void RBTRemove(redBlackBST *self, Key key) {
    if (key == NULL) {
        printf("argument to remove() is NULL");
        exit(EXIT_FAILURE);
    }
    //如果两个孩子都是黑的，将父结点设为红
    if (!NodeisRed(self -> root -> lChild) && !NodeisRed(self -> root -> rChild)) {
        self -> root -> color = RED;
    }
    self -> root = NodeRemove(self -> root, key);
    if (!RBTisEmpty(self)) {
        self -> root -> color = BLACK;
    }
    assert(RBTSelfCheck(self));
}

//红黑树插入
void RBTPut(redBlackBST *self, Key key, Value val) {
    if (key == NULL) {
        printf("argument to remove() is NULL");
        exit(EXIT_FAILURE);
    }
    if (val == NULL) {
        RBTRemove(self, key);
        return ;
    }
    self -> root = NodePut(self -> root, key, val);
    self -> root -> color = BLACK;
    assert(RBTSelfCheck(self));
}

//求数的深度
int RBTHeight(const redBlackBST *self) {
    return NodeHeight(self -> root);
}

//下面函数功能看redBalckTreeNode.c

Node *RBTFloor(const redBlackBST *self, Key key) {
    if (key == NULL) {
        printf("argument to remove() is NULL");
        exit(EXIT_FAILURE);
    }
    if (RBTisEmpty(self)) {
        printf("called RBTFloor() with empty symbol table");
        exit(EXIT_FAILURE);
    }
    Node *x = NodeFloor(self -> root, key);
    if (x == NULL) {
        return NULL;
    } else {
        return x;
    }
}

Node *RBTCeiling(const redBlackBST *self, Key key) {
    if (key == NULL) {
        printf("argument to remove() is NULL");
        exit(EXIT_FAILURE);
    }
    if (RBTisEmpty(self)) {
        printf("called RBTCeiling() with empty symbol table");
        exit(EXIT_FAILURE);
    }
    Node *x = NodeCeiling(self -> root, key);
    if (x == NULL) {
        return NULL;
    } else {
        return x;
    }
}

Key RBTSelect(const redBlackBST *self, int k) {
    if (k < 0 || k > RBTSize(self)) {
        printf("Illegal arguement");
        exit(EXIT_FAILURE);
    }
    Node *x = NodeSelect(self -> root, k);
    return x -> key;
}

int RBTRank(const redBlackBST *self, Key key) {
    if (key == NULL) {
        printf("argument to rank() is NULL");
        exit(EXIT_FAILURE);
    }
    return NodeRank(self -> root, key);
}

keyList *RBTKeys(const redBlackBST *self) {
    if (RBTisEmpty(self)) {
        return NULL;
    }
    return RBTKeyRange(self, RBTMinbyKey(self) -> key, RBTMaxbyKey(self) -> key);
}
keyList *RBTKeyRange(const redBlackBST *self, const Key low, const Key high) {
    if (low == NULL) {
        printf("first argument to keys() is NULL");
        exit(EXIT_FAILURE);
    }
    if (high == NULL) {
        printf("second argument to keys() is NULL");
        exit(EXIT_FAILURE);
    }
    keyList *list = (keyList *)calloc(1, sizeof(keyList));
    keyList *end = list;
    NodeKeys(self -> root, &end, low, high);
    return list;
}

int RBTRangeSize(const redBlackBST *self, Key low, Key high) {
    if (low == NULL) {
        printf("first argument to keys() is NULL");
        exit(EXIT_FAILURE);
    }
    if (high == NULL) {
        printf("second argument to keys() is NULL");
        exit(EXIT_FAILURE);
    }
    if (low < high) {
        return 0;
    }
    if (RBTContains(self, high)) {
        return RBTRank(self, high) - RBTRank(self, low) + 1;
    }
    return RBTRank(self, high) - RBTRank(self, low);
}

bool RBTFree(redBlackBST *self) {
    keyList *list = RBTKeys(self);
    while (list && list->node) {
        keyList* release = list;
        list = list -> next;
        free(release -> node -> value);
        release -> node -> value = NULL;
        free(release -> node);
        release -> node -> lChild = NULL;
        release -> node -> rChild = NULL;
        release -> node = NULL;
        free(release);
        release = NULL;
    }
    self -> root = NULL;
    free(list);
    return true;
}

bool RBTTestisBST(const redBlackBST *self) {
    return NodeTestisBST(self -> root, NULL, NULL);
}

bool RBTTestisSizeConsistent(const redBlackBST *self) {
    return NodeTestisSizeConsistant(self -> root);
}

bool RBTTestisRankConsistent(const redBlackBST *self) {
    int i = 0;
    for (; i < RBTSize(self); i++) {
        if (i != RBTRank(self, RBTSelect(self, i))) {
            return false;
        }
    }
    keyList *list = RBTKeys(self);
    Node *n;
    while (list && list -> node) {
        n = list -> node;
        if (n -> key != RBTSelect(self, RBTRank(self, n -> key))) {
            while (list -> next) {
                keyList *releasse = list;
                list = list -> next;
                free(releasse);
            }
            free(list);
            return false;
        }
        keyList *release = list;
        list = list -> next;
        free(release);
    }
    free(list);
    return true;
}

bool RBTTestis23(const redBlackBST *self) {
    return NodeTestis23(self -> root, self -> root);
}

bool RBTTestisBalanced(const redBlackBST *self) {
    int black = 0;
    Node *x = self -> root;
    while (x != NULL) {
        if (!NodeisRed(x)) {
            black++;
        }
    }
    return NodeTestisBalanced(self -> root, black);
}




