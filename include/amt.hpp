#ifndef AMT_H
#define AMT_H
 
#include <vector>
#include "bit_map.hpp"

class AMTNode;

class BitMappedNode {
  Bitmap map;
  std::vector<AMTNode> nodes;

  public:
    BitMappedNode();
    void insert(const void *value, size_t len);
    bool contains(const void *value, size_t len);

  private:
    BitMappedNode *next(char c);
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

BitMappedNode *BitMappedNode::next(char c) {
  if (!map.get(c)) return NULL;
  int index = map.get_offset(c);
  AMTNode *node = &nodes[index];
  return node->sub_trie;
}

bool BitMappedNode::contains(const void *value, size_t len) {
  int i;
  BitMappedNode *trie = nodes[0].sub_trie;
  unsigned char *c = (unsigned char *) value;

  for (i = 0; i < len; ++i, ++c) {
    if (!trie) return false;
    trie = trie->next(*c);
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
