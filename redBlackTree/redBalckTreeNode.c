//
//  redBalckTreeNode.c
//  redBlackTree
//
//  Created by 李庚达 on 2016/11/15.
//  Copyright © 2016年 李庚达. All rights reserved.
//

#include "redBalckTreeNode.h"
#include <string.h>
#include <stdlib.h>

#define MAX(a,b) (a > b) ? a : b

#ifdef ASSERTS
#include <assert.h>
#else
#define assert(X) {/* Asserts are unused unless defined */}
#endif // ASSERTS

#pragma region Private NODE_* functions

//释放该结点内存
void NodeFree(Node **x) {
    free((*x) -> value);
    (*x) -> value = NULL;
    free((*x));
    (*x) = NULL;
}

bool NodeisRed(const Node *x) {
    if (x == NULL) {
        return false;
    }
    return x -> color == RED;
}

//求最小键值
Node *NodeMinbyKey(Node *x) {
    assert(x != NULL);
    if (x -> lChild == NULL) {
        return x;
    }
    return NodeMinbyKey(x -> lChild);
}

//求最大键值
Node *NodeMaxbyKey(Node *x) {
    assert(x != NULL);
    if (x -> rChild == NULL) {
        return x;
    }
    return NodeMaxbyKey(x -> rChild);
}

//获取该结点键值，没有返回NULL
Value *NodeGet(Node *x, Key key) {
    while (x != NULL) {
        if (key < x -> key) {
            x = x ->lChild;
        } else if (key > x ->key) {
            x = x -> rChild;
        } else
            return &(x -> value);
    }
    return NULL;
}

//求以x为根结点的结点数量；如果x为NULL，则返回0
int NodeSize(const Node *x) {
    if (x == NULL) {
        return 0;
    }
    return x -> size;
}

//右旋转
Node *NodeRotateRight(Node *h) {
    assert((h != NULL) && NodeisRed(h -> lChild));
    Node *x = h -> lChild;
    h -> lChild = x -> rChild;
    x -> rChild = h;
    x -> color = x -> rChild -> color;
    x -> rChild -> color = RED;
    x -> size = h -> size;
    h -> size = NodeSize(h -> lChild) + NodeSize(h -> rChild) + 1;
    return x;
}

//左旋转
Node *NodeRotateLeft(Node *h) {
    assert((h != NULL) && NodeisRed(h -> rChild));
    Node *x = h -> rChild;
    x -> lChild = h;
    x -> color = x -> lChild -> color;
    x -> lChild -> color = RED;
    x -> size = h -> size;
    h -> size = NodeSize(h -> lChild) + NodeSize(h -> rChild) + 1;
    return x;
}

//将根结点与其孩子结点的颜色转换
void NodeFlipColors(Node *h) {
    //根结点h必须与其孩子结点颜色不同
    assert((h != NULL) && (h -> lChild != NULL) && (h -> rChild != NULL));
    assert((NodeisRed(h) && !NodeisRed(h -> lChild) && NodeisRed(h -> rChild)) || (!NodeisRed(h) && NodeisRed(h -> lChild) && NodeisRed(h -> rChild)));
    h -> color = !(h -> color);
    h -> lChild -> color = !(h -> lChild -> color);
    h -> rChild -> color = !(h -> rChild -> color);
}

//假定h为红的，h.lChild 和 h.lChild.lChild 都是黑的，将其中一个变成红的
Node *ModeMoveRedLeft(Node *h) {
    assert(h != NULL);
    assert(NodeisRed(h) && !NodeisRed(h -> lChild) && !NodeisRed(h -> lChild -> lChild));
    NodeFlipColors(h);
    if (NodeisRed(h -> rChild -> lChild)) {
        h -> rChild = NodeRotateRight(h -> rChild);
        h = NodeRotateLeft(h);
        NodeFlipColors(h);
    }
    return h;
}

//假定h为红的，h.rChild 和 h.rChild.rChild 都是黑的，将其中一个变成红的
Node *NodeMoveRedRight(Node *h) {
    assert(h != NULL);
    assert(NodeisRed(h) && !NodeisRed(h -> rChild) && !NodeisRed(h -> rChild -> lChild));
    NodeFlipColors(h);
    if (NodeisRed(h -> lChild -> lChild)) {
        h = NodeRotateRight(h);
        NodeFlipColors(h);
    }
    return h;
}

//确保红黑树性质不变
Node *NodeBalance(Node *h) {
    assert(h != NULL);
    if (NodeisRed(h -> rChild)) {
        h = NodeRotateLeft(h);
    }
    if (NodeisRed(h -> lChild) && NodeisRed(h -> lChild -> lChild)) {
        h = NodeRotateRight(h);
    }
    if (NodeisRed(h -> lChild) && NodeisRed(h -> rChild)) {
        NodeFlipColors(h);
    }
    h -> size = NodeSize(h -> lChild) + NodeSize(h -> rChild) + 1;
    return h;
}

//删除以h为根结点的树中最大的键值
Node *NodeDeleteMax(Node *h) {
    if (NodeisRed(h -> lChild)) {
        h = NodeMoveRedRight(h);
    }
    if (h -> rChild == NULL) {
        NodeFree(&h);
        return NULL;
    }
    if (!NodeisRed(h -> rChild) && !NodeisRed(h -> rChild -> lChild)) {
        h = NodeMoveRedRight(h);
    }
    h -> rChild = NodeDeleteMax(h -> rChild);
    return NodeBalance(h);
}

//删除以h为根结点的树中最小的键值
Node *NodeDeleteMin(Node *h) {
    if (h -> lChild == NULL) {
        NodeFree(&h);
        return h;
    }
    if (!NodeisRed(h -> lChild) && !NodeisRed(h -> lChild -> lChild)) {
        h = NodeRotateLeft(h);
    }
    h -> lChild = NodeDeleteMin(h -> lChild);
    return NodeBalance(h);
}

//删除给定键值的结点
Node *NodeRemove(Node *h, Key key) {
    assert(NodeGet(h, key) != NULL);
    if (key < h -> key) {
        if (!NodeisRed(h -> lChild) && !NodeisRed(h -> lChild -> lChild)) {
            h = NodeRotateLeft(h);
        }
        h -> lChild = NodeRemove(h -> lChild, key);
    } else {
        if (NodeisRed(h -> lChild)) {
            h = NodeRotateRight(h);
        }
        if ((key == h -> key) && (h -> rChild == NULL)) {
            NodeFree(&h);
            return NULL;
        }
        if (!NodeisRed(h -> rChild) && !NodeisRed(h -> rChild -> lChild)) {
            h = NodeMoveRedRight(h);
        }
        if (key == h ->key) {
            Node *x = NodeMinbyKey(h -> rChild);
            h -> key = x -> key;
            //x在外部操作要释放：确保其数据域被释放
            Value temp = h -> value;
            h -> value = x -> value;
            x -> value = temp;
            h -> rChild = NodeDeleteMin(h -> rChild);
        } else {
            h -> rChild = NodeRemove(h -> rChild, key);
        }
    }
    return NodeBalance(h);
}

//插入
Node *NodePut(Node *h, Key key, Value val) {
    if (h == NULL) {
        return createNode(key, val, RED, 1);
    }
    if (key < h -> key) {
        h -> lChild = NodePut(h -> lChild, key, val);
    } else if (key > h -> key) {
        h -> rChild = NodePut(h -> rChild, key, val);
    } else {
        //以一个新的数据替代
        memcmp(h -> value, val, sizeof(Value));
    }
    //修正树
    if (NodeisRed(h -> rChild) && !NodeisRed(h -> lChild)) {
        h = NodeRotateLeft(h);
    }
    if (NodeisRed(h -> lChild) && NodeisRed(h -> lChild -> lChild)) {
        h = NodeRotateRight(h);
    }
    if (NodeisRed(h -> lChild) && NodeisRed(h -> rChild)) {
        NodeFlipColors(h);
    }
    h -> size = NodeSize(h -> lChild) + NodeSize(h -> rChild) + 1;
    return h;
}

int NodeHeight(Node *x) {
    if (x == NULL) {
        return  -1;
    }
    return (1 + (MAX(NodeSize(x -> lChild), NodeSize(x -> rChild))));
}

//根的子树中x小于或等于给定键的最大关键值
Node *NodeFloor(Node *x, Key key) {
    if (x == NULL) {
        return NULL;
    }
    if (key == x -> key) {
        return x;
    }
    if (key < x -> key) {
        return NodeFloor(x -> lChild, key);
    }
    Node *t = NodeFloor(x -> rChild, key);
    if (t != NULL) {
        return t;
    } else {
        return x;
    }
}

//根的子树中x小于或等于给定键的最小关键值
Node *NodeCeiling(Node *x, Key key) {
    if (x == NULL) {
        return NULL;
    }
    if (key == x -> key) {
        return x;
    }
    if (key > x -> key) {
        return NodeCeiling(x -> rChild, key);
    }
    Node *t = NodeCeiling(x -> lChild, key);
    if (t != NULL) {
        return t;
    } else {
        return x;
    }
}

//the key of rank k in the subtree rooted at x
Node *NodeSelect(Node *x, int k) {
    assert(x != NULL);
    assert(k >= 0 && k < NodeSize(x));
    int t = NodeSize(x -> lChild);
    if (t > k) {
        return NodeSelect(x -> lChild, k);
    }
    if (t < k) {
        return NodeSelect(x -> rChild, k);
    }
    return x;
}

//求秩
int NodeRank(Node *x, Key key) {
    if (x == NULL) {
        return 0;
    }
    if (key < x -> key) {
        return NodeRank(x -> lChild, key);
    } else if (key > x -> key) {
        return (1 + NodeSize(x -> rChild) + NodeRank(x -> lChild, key));
    } else {
        return NodeSize(x -> lChild);
    }
}

//将low与high之间的结点入队
void NodeKeys(Node *x, keyList **queue, const Key low, const Key high) {
    if (x == NULL || queue == NULL) {
        return;
    }
    if (low < x -> key) {
        NodeKeys(x -> lChild, queue, low, high);
    }
    if (low <= x -> key && high >= x -> key) {
        keyList *next = (keyList *)calloc(1, sizeof(keyList));
        (*queue) -> node = x;
        (*queue) -> next = next;
        (*queue) = next;
    }
    if (high > x -> key) {
        NodeKeys(x -> rChild, queue, low, high);
    }
}

#pragma region Node Tests

bool NodeTestisBST(const Node *x, const Key *min, const Key *max) {
    if (x == NULL) {
        return true;
    }
    if ((min != NULL) && (x -> key <= *min)) {
        return false;
    }
    if ((max != NULL) && (x -> key >= *max)) {
        return false;
    }
    return NodeTestisBST(x -> lChild, min, &(x -> key)) && NodeTestisBST(x -> rChild, &(x -> key), max);
}

bool NodeTestisSizeConsistant(const Node *x) {
    if (x == NULL) {
        return true;
    }
    if (x -> size != (NodeSize(x -> lChild) + NodeSize(x -> rChild) + 1)) {
        return false;
    }
    return NodeTestisSizeConsistant(x -> lChild) && NodeTestisSizeConsistant(x -> rChild);
}

bool NodeTestis23(const Node *x, const Node *root) {
    if (x == NULL) {
        return true;
    }
    if (NodeisRed(x -> rChild)) {
        return false;
    }
    if (x != root && NodeisRed(x) && NodeisRed(x -> rChild)) {
        return false;
    }
    return NodeTestis23(x -> lChild, root) && NodeTestis23(x -> rChild, root);
}

bool NodeTestisBalanced(const Node *x, int black) {
    if (x == NULL) {
        return black = 0;
    }
    if (!NodeisRed(x)) {
        black--;
    }
    return NodeTestisBalanced(x -> lChild, black) && NodeTestisBalanced(x -> rChild, black);
}
