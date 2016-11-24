//
//  redBlackTree.h
//  redBlackTree
//
//  Created by 李庚达 on 2016/11/15.
//  Copyright © 2016年 李庚达. All rights reserved.
//

#ifndef redBlackTree_h
#define redBlackTree_h

#include <stdio.h>
#include <stdbool.h>

typedef char* Value;
typedef int Key;

const static bool RED = 1;
const static bool BLACK = 0;

typedef struct _Node {
    
    Value value;
    struct _Node *lChild, *rChild;
    bool color;
    Key key;
    int size;
    
} Node;

typedef struct _keyList {
    
    Node *node;
    struct _keyList *next;
    
} keyList;

#define INIT_KeyList(X) KeyList X = { .node = NULL,	.next = NULL }

typedef struct _redBlackBST {
    
    Node *root;
    
} redBlackBST;

void applyKeyValue(Node *node, Key key, Value val);
void KL_forEach(redBlackBST *self, keyList *list, void(* func)(redBlackBST *, Node *));
Node *createNode(Key _key, Value _value, bool _color, int _size);

void RBTDeleteMax(redBlackBST *self);
void RBTRemove(redBlackBST *self, Key key);
void RBTPut(redBlackBST *self, Key key, Value val);

Value *RBTGet(const redBlackBST *self, Key key);
int RBTSize(const redBlackBST *self);
bool RBTisEmpty(const redBlackBST *self);
bool RBTContains(const redBlackBST *self, Key key);

const Node *RBTMinbyKey(const redBlackBST *self);
const Node *RBTMaxbyKey(const redBlackBST *self);

int RBTHeight(const redBlackBST *self);
Node *RBTFloor(const redBlackBST *self, Key key);
Node *RBTCeiling(const redBlackBST *self, Key key);
Key RBTSelect(const redBlackBST *self, int k);

int RBTRank(const redBlackBST *self, Key key);

keyList *RBTKeys(const redBlackBST *self);
keyList *RBTKeyRange(const redBlackBST *self, const Key low, const Key high);
int RBTRangeSize(const redBlackBST *self, Key low, Key high);

//bool RBTSelfCheck(const redBlackBST *self);
bool RBTFree(redBlackBST *self);

#endif /* redBlackTree_h */
