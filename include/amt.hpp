#ifndef AMT_H
#define AMT_H
 
#include <vector>
#include "bit_map.hpp"

class AMTNode;

class ArrayMappedTrie {
  friend class AMTNode;

  Bitmap *map;
  std::vector<AMTNode> nodes;

  public:
    ArrayMappedTrie();
    void insert(const void *value, size_t len);
    bool contains(const void *value, size_t len);
};

class AMTNode {
  public:
    uint16_t character;
    ArrayMappedTrie *sub_trie;
    AMTNode(char c): character(c), sub_trie(NULL) {}
    AMTNode *next(char c);
};

ArrayMappedTrie::ArrayMappedTrie(): map(new Bitmap()) {
  nodes.push_back(AMTNode('\0'));
}

AMTNode *AMTNode::next(char c) {
  if (!sub_trie || !sub_trie->map->get(c)) return NULL;
  int index = sub_trie->map->get_offset(c);
  AMTNode *node = &sub_trie->nodes[index];
  return node;
}

bool ArrayMappedTrie::contains(const void *value, size_t len) {
  int i;
  AMTNode *node = &nodes[0];
  unsigned char *c = (unsigned char *) value;

  for (i = 0; i < len; ++i, ++c) {
    if (!node) return false;
    node = node->next(*c);
  }
  return true;
}

void ArrayMappedTrie::insert(const void *value, size_t len) {
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
      node->sub_trie = new ArrayMappedTrie();

    node->sub_trie->map->set(*c, true);

    int index = node->sub_trie->map->get_offset(*c);
    std::vector<AMTNode> *node_list = &node->sub_trie->nodes;
    node_list->emplace(node_list->begin() + index, AMTNode(*c));

    node = &node_list->at(index);
  }
}

#endif
