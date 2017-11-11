#include <iostream>
#include <string>
#include "BinTree.h"

using namespace std;
int main() {
  Tree testTree(0);
  testTree.insertChildWithParent(0, 1);
  testTree.insertChildWithParent(0, 2);
  testTree.insertChildWithParent(2, 3);
  testTree.printValAtIndex(0);
  testTree.printValAtIndex(1);
  testTree.printValAtIndex(2);
  testTree.printValAtIndex(3);
  testTree.printValAtIndex(4);
  testTree.printValAtIndex(5);
  return 0;
}
