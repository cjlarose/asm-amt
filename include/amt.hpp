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
    ArrayMappedTrie(): map(NULL) {};
    void insert(const char *str);
    bool contains(const char *str);

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

AMTNode *AMTNode::next(char c) {
  if (!sub_trie) return NULL;

  unsigned int index;
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

bool ArrayMappedTrie::contains(const char *str) {
  if (!nodes.size()) return false;

  AMTNode *node = &nodes[0];

  for (; *str; ++str) {
    if (!node) return false;
    node = node->next(*str);
  }
  return node->next('\0') != NULL;
}

void ArrayMappedTrie::insert(const char *str) {
  char *c = (char *) str;

  if (!nodes.size())
    nodes.push_back(AMTNode('\0'));

  AMTNode *node = &nodes[0];

  for (; *c; ++c) {
    AMTNode *tmp = node->next(*c);
    if (!tmp) break;
    node = tmp;
  }

  int size = strlen(str) + 1;
  int i = 0;

  for (i = 0; i < size; ++i, ++c) {
    ArrayMappedTrie *trie = node->sub_trie;
    if (!trie)
      trie = node->sub_trie = new ArrayMappedTrie();

    std::vector<AMTNode> *node_list = &trie->nodes;

    unsigned int index;
    if (trie->map) {
      trie->map->set(*c, true);
      index = trie->map->get_offset(*c);
    } else {
      index = 0;
      if (!node_list->empty()) {
        AMTNode *tmp_node = &node_list->at(0);
        while (index < node_list->size() && tmp_node->character < *c) {
          ++index;
          ++tmp_node;
        }
      }
    }

    node_list->emplace(node_list->begin() + index, AMTNode(*c));

    if (!trie->map && node_list->size() >= MIN_BITMAPPED_SIZE)
      trie->add_bitmap();

    node = &node_list->at(index);
  }
}

void ArrayMappedTrie::add_bitmap() {
  map = new Bitmap();
  for (auto node : nodes)
    map->set(node.character, true);
}

#endif
