#include <vector>
#include <memory>
#include <functional>
#include <cassert>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <sstream>
#include <iostream>
#include <algorithm>

#define DEBUG_REQUIRE_NE(X, Y) assert((X) != (Y))

template <typename T>
struct RBTreeNode {
  RBTreeNode *parent{ nullptr };
  RBTreeNode *left{ nullptr };
  RBTreeNode *right{ nullptr };
  bool isRed{ false };
  T data;

  T& getData() { return data; }

};

template <typename T>
class RBTree {
public:
  RBTreeNode<T> *root{ nullptr };

  RBTree() = default;
  ~RBTree() = default;

  RBTree(const RBTree&) = delete;
  RBTree(RBTree&&) = delete;

  RBTree operator=(const RBTree&) = delete;
  RBTree operator=(RBTree&&) = delete;

  void leftRotate(RBTreeNode<T> *node) { 
    DEBUG_REQUIRE_NE(node, nullptr);
    DEBUG_REQUIRE_NE(node->right, nullptr);

    RBTreeNode<T> *swap{ node->right };
    node->right = swap->left;
    if (swap->left != nullptr) {
      swap->left->parent = node;
    }

    swap->parent = node->parent;
    if (node->parent == nullptr) {
      root = swap;
    } else if (node == node->parent->left) {
      node->parent->left = swap;
    } else {
      node->parent->right = swap;
    }
      
    swap->left = node;
    node->parent = swap;
  }

  void rightRotate(RBTreeNode<T> *node) {
    DEBUG_REQUIRE_NE(node, nullptr);
    DEBUG_REQUIRE_NE(node->left, nullptr);

    RBTreeNode<T> *swap{ node->left };
    node->left = swap->right;
    if (swap->right != nullptr) {
      swap->right->parent = node;
    }

    swap->parent = node->parent;
    if (node->parent == nullptr) {
      root = swap;
    } else if (node == node->parent->left) {
      node->parent->left = swap;
    } else {
      node->parent->right = swap;
    }
      
    swap->right = node;
    node->parent = swap;
  }

  void insertNode(RBTreeNode<T> *newNode) {
    DEBUG_REQUIRE_NE(newNode, nullptr);

    RBTreeNode<T> *insertNode{ nullptr };

    RBTreeNode<T> *tmp{ root };
    while (tmp != nullptr) {
      insertNode = tmp;
      if (newNode->data < tmp->data) {
        tmp = tmp->left;
      } else {
        tmp = tmp->right;
      }
    }

    newNode->parent = insertNode;
    if (insertNode == nullptr) {
      root = newNode;
    } else if (newNode->data < insertNode->data) {
      insertNode->left = newNode;
    } else {
      insertNode->right = newNode;
    }

    newNode->left = newNode->right = nullptr;
    newNode->isRed = true;
    fixTree(newNode);
  }

  void fixTree(RBTreeNode<T> *fromNode) {
    while (fromNode != nullptr && fromNode->isRed
      && fromNode->parent != nullptr
      && fromNode->parent->parent != nullptr) {

      if (fromNode->parent == fromNode->parent->parent->left) {
        RBTreeNode<T> *pPR{ fromNode->parent->parent->right };
        // Case 1: Uncle is red
        if (pPR != nullptr && pPR->isRed) {
          fromNode->parent->isRed = false;
          pPR->isRed = false;
          fromNode->parent->parent->isRed = true;
        // Case 2/3: Uncle is black and fromNode is r/l child.
        } else if (fromNode == fromNode->parent->right) {
          fromNode = fromNode->parent;
          leftRotate(fromNode);
          fromNode->parent->isRed = false;
          fromNode->parent->parent->isRed = true;
        } else {
          // Nothing is violated
          break;
        }
      } else if (fromNode->parent->parent->left != nullptr) {
        RBTreeNode<T> *pPL{ fromNode->parent->parent->left };
        // Case 1: Uncle is red
        if (pPL != nullptr && pPL->isRed) {
          fromNode->parent->isRed = false;
          pPL->isRed = false;
          fromNode->parent->parent->isRed = true;
        // Case 2/3: Uncle is black and fromNode is r/l child.
        } else if (fromNode == fromNode->parent->left) {
          fromNode = fromNode->parent;
          rightRotate(fromNode);
          fromNode->parent->isRed = false;
          fromNode->parent->parent->isRed = true;
        } else {
          // Nothing is violated
          break;
        }
      } else {
        // Nothing is violated
        break;
      }
    }

    if (root != nullptr && root->isRed)
      root->isRed = false;
  }
};



int main() {
    RBTree<int> myTree;
    srand(time(NULL));

    printf("Inserting...\n");
    for (int i = 0; i < 100; ++i) {
        int newValue{ rand() % 100 };
        printf("%d ", newValue);

        auto* newNode{ new RBTreeNode<int>() };
        newNode->data = newValue;
        
        myTree.insertNode(newNode);
    }
    printf("\n");
    
    // Navigate tree
    std::function<void(const RBTreeNode<int>*)> exploreNode;
    exploreNode = [&exploreNode](const RBTreeNode<int>* node) {
      if (node == nullptr)
        return;

      if (node->left != nullptr)
        exploreNode(node->left);

      printf("%d ", node->data);

      if (node->right!= nullptr)
        exploreNode(node->right);
    };

    printf("Printing...\n");
    exploreNode(myTree.root);
    printf("\n");
}

