#ifndef AMT_H
#define AMT_H
 
#include <vector>
#include "bit_map.hpp"

const int MIN_BITMAPPED_SIZE = 5;

class AMTNode;

class ArrayMappedTrie {
  friend class AMTNode;

  Bitmap *map;
  std::vector<AMTNode> nodes;

  public:
    ArrayMappedTrie();
    void insert(const void *value, size_t len);
    bool contains(const void *value, size_t len);

  private:
    void add_bitmap();
};

class AMTNode {
  public:
    uint16_t character;
    ArrayMappedTrie *sub_trie;
    AMTNode(char c): character(c), sub_trie(NULL) {}
    AMTNode *next(char c);
};

ArrayMappedTrie::ArrayMappedTrie(): map(NULL) {
  nodes.push_back(AMTNode('\0'));
}

AMTNode *AMTNode::next(char c) {
  if (!sub_trie) return NULL;

  int index;
  if (sub_trie->map) {
    if (!sub_trie->map->get(c)) return NULL;
    index = sub_trie->map->get_offset(c);
    return &sub_trie->nodes[index];
  } else {
    for (index = 0; index < sub_trie->nodes.size(); ++index) {
      AMTNode *node = &sub_trie->nodes[index];
      if (node->character == c)
        return node;
    }
    return NULL;
  }
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
    ArrayMappedTrie *trie = node->sub_trie;
    if (!trie)
      trie = node->sub_trie = new ArrayMappedTrie();

    std::vector<AMTNode> *node_list = &trie->nodes;

    int index;
    if (trie->map) {
      trie->map->set(*c, true);
      index = trie->map->get_offset(*c);
    } else {
      AMTNode *tmp_node = &node_list->at(0);
      for (index = 0; index < node_list->size() && tmp_node->character < *c; ++index, ++tmp_node);
    }

    node_list->emplace(node_list->begin() + index, AMTNode(*c));

    if (!trie->map && node_list->size() >= MIN_BITMAPPED_SIZE)
      trie->add_bitmap();

    node = &node_list->at(index);
  }
}

void ArrayMappedTrie::add_bitmap() {
  assert(false);
  map = new Bitmap();
  for (auto node : nodes)
    map->set(node.character, true);
}

#endif
