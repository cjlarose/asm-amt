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
  unsigned char *c = (unsigned char *) value;
  AMTNode *node = &nodes[0];

  for (i = 0; i < len; ++i, ++c) {
    AMTNode *tmp = node->next(*c);
    if (!tmp) break;
    node = tmp;
  }

  for (; i < len; ++i, ++c) {
    if (!node->sub_trie)
      node->sub_trie = new BitMappedNode();

    node->sub_trie->map.set(*c, true);

    int index = node->sub_trie->map.get_offset(*c);
    AMTNode new_node = AMTNode();
    new_node.character = *c;

    std::vector<AMTNode> *node_list = &node->sub_trie->nodes;
    node_list->emplace(node_list->begin() + index, new_node);

    node = &node_list->at(index);
  }
}

#endif
