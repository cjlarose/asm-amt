#ifndef AMT_H
#define AMT_H
 
#include <vector>
#include "bit_map.hpp"

class AMTNode;

class BitMappedNode {
  friend class AMTNode;

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
    AMTNode *next(char c);
};

BitMappedNode::BitMappedNode() {
  nodes.push_back(AMTNode());
}

AMTNode *AMTNode::next(char c) {
  if (!sub_trie || !sub_trie->map.get(c)) return NULL;
  int index = sub_trie->map.get_offset(c);
  AMTNode *node = &sub_trie->nodes[index];
  return node;
}

bool BitMappedNode::contains(const void *value, size_t len) {
  int i;
  AMTNode *node = &nodes[0];
  unsigned char *c = (unsigned char *) value;

  for (i = 0; i < len; ++i, ++c) {
    if (!node) return false;
    node = node->next(*c);
  }
  return true;
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

#endif
