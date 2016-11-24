//
//  main.c
//  redBlackTree
//
//  Created by 李庚达 on 2016/11/15.
//  Copyright © 2016年 李庚达. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "redBlackTree.h"

void printNode(redBlackBST *self, Node *node) {
    printf("%d %s at %p => (%p)\n", node -> key, *RBTGet(self, node -> key), node, node -> value);
}

void testBST(redBlackBST* self, int test_size) {
    keyList *q;
    int i;
    
    // Add test_size nodes to the tree
    for (i = 1; i <= test_size; i++)
    {
        char val[2];
        sprintf(val, "%c", i % ((int)'Z' - 'A' - 1) + 'A' - 1);
        
        RBTPut(self, i, val);
    }
    
    // Print the in order traversal
    q = RBTKeys(self);
    KL_forEach(self, q, printNode);
    
    // Remove in order
    for (i = 1; i <= test_size; i++)
    {
        RBTRemove(self, i);
        keyList *list = RBTKeys(self);
        printf("---\n");
        KL_forEach(self, list, printNode);
    }
    
    // Fill the tree back up for another test
    for (i = 1; i <= test_size; i++)
    {
        char val[2];
        sprintf(val, "%c", i % ((int)'Z' - 'A' - 1) + 'A' - 1);
        
        RBTPut(self, i, val);
    }
    
    // Print the in order traversal
    q = RBTKeys(self);
    KL_forEach(self, q, printNode);
    
    // Remove the nodes by removing the root one at a time
    while (!RBTisEmpty(self))
    {
        RBTRemove(self, self->root->key);
        keyList *list = RBTKeys(self);
        printf("---\n");
        KL_forEach(self, list, printNode);
    }
}

int main(int argc, const char * argv[]) {
    redBlackBST st = {
        .root = NULL
    };
    //输入
    testBST(&st, 20);
    
    //...
    RBTFree(&st);
    return 0;
}

