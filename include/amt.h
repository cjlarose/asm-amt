#ifndef AMT_H
#define AMT_H
 
#include <vector>
#include "bit_map.h"

class AMTNode;

class BitMappedNode {
  Bitmap map;
  std::vector<AMTNode> nodes;

  public:
    BitMappedNode();
    void insert(const void *value, size_t len);
    bool contains(const void *value, size_t len);
};

class AMTNode {
  public:
    uint16_t character;
    BitMappedNode *sub_trie;
    AMTNode(): character('\0'), sub_trie(NULL) {}
};

BitMappedNode::BitMappedNode() {
  nodes.push_back(AMTNode());
}

void BitMappedNode::insert(const void *value, size_t len) {
  int i;
  BitMappedNode **trie = &nodes[0].sub_trie;
  unsigned char *c = (unsigned char *) value;

  for (i = 0; i < len; ++i, ++c) {
    if (!*trie)
      *trie = new BitMappedNode();

    int index = (*trie)->map.get_offset(*c);

    // see if the character is already there
    if (!(*trie)->map.get(*c)) {
      // set the character in the bitmap
      (*trie)->map.set(*c, true);

      // actually add the node
      AMTNode new_node = AMTNode();
      new_node.character = *c;
      (*trie)->nodes.emplace((*trie)->nodes.begin()+index, new_node);
    }

    AMTNode *new_node = &(*trie)->nodes[index];
    trie = &new_node->sub_trie;
  }
}

bool BitMappedNode::contains(const void *value, size_t len) {
  int i;
  BitMappedNode **trie = &nodes[0].sub_trie;
  unsigned char *c = (unsigned char *) value;

  for (i = 0; i < len; ++i, ++c) {
    if (!*trie || !(*trie)->map.get(*c))
      return false;

    int index = (*trie)->map.get_offset(*c);
    AMTNode *node = &(*trie)->nodes[index];
    trie = &node->sub_trie;
  }
  return true;
}

#endif
