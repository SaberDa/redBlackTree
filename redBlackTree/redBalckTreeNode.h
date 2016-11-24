//
//  redBalckTreeNode.h
//  redBlackTree
//
//  Created by 李庚达 on 2016/11/15.
//  Copyright © 2016年 李庚达. All rights reserved.
//

#ifndef redBalckTreeNode_h
#define redBalckTreeNode_h

#include <stdio.h>
#include "redBlackTree.h"

bool NodeisRed(const Node *x);
Node *NodeMinbyKey(Node *x);
Value *NodeGet(Node *x, Key key);
int NodeSize(const Node *x);
Node *NodeMaxbyKey(Node *x);

Node *NodeRotateRight(Node *h);
Node *NodeRotateLeft(Node *h);

void NodeFlipColors(Node *h);

Node *ModeMoveRedLeft(Node *h);
Node *NodeMoveRedRight(Node *h);

Node *NodeBalance(Node *h);

Node *NodeDeleteMax(Node *h);
Node *NodeDeleteMin(Node *h);

Node *NodeRemove(Node *h, Key key);
Node *NodePut(Node *h, Key key, Value val);

int NodeHeight(Node *x);
Node *NodeFloor(Node *x, Key key);
Node *NodeCeiling(Node *x, Key key);
Node *NodeSelect(Node *x, int k);

int NodeRank(Node *x, Key key);
void NodeKeys(Node *x, keyList **queue, const Key low, const Key high);

bool NodeTestisBST(const Node *x, const Key *min, const Key *max);
bool NodeTestisSizeConsistant(const Node *x);
bool NodeTestis23(const Node *x, const Node *root);
bool NodeTestisBalanced(const Node *x, int black);

#endif /* redBalckTreeNode_h */
