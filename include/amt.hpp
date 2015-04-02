#ifndef AMT_H
#define AMT_H
 
#include <vector>
#include "bit_map.hpp"

const int MIN_BITMAPPED_SIZE = 5;

class ArrayMappedTrie {

  private:
    class AMTNode {
      public:
        uint16_t character;
        std::unique_ptr<ArrayMappedTrie> sub_trie;
        AMTNode(char c): character(c), sub_trie(nullptr) {}
        AMTNode *next(char c);
        size_t sub_trie_size();
    };

    Bitmap *map;
    std::vector<ArrayMappedTrie::AMTNode> nodes;
    void add_bitmap();

  public:
    ArrayMappedTrie(): map(NULL) {};
    void insert(const char *str);
    bool contains(const char *str);
    size_t size();

};

ArrayMappedTrie::AMTNode *ArrayMappedTrie::AMTNode::next(char c) {
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
      if (node->character > c)
        return NULL;
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
    if (!node->sub_trie) {
      node->sub_trie.reset(new ArrayMappedTrie());
    }

    std::vector<AMTNode> *node_list = &node->sub_trie->nodes;

    unsigned int index;
    if (node->sub_trie->map) {
      node->sub_trie->map->set(*c, true);
      index = node->sub_trie->map->get_offset(*c);
    } else {
      size_t sz = node_list->size();
      for (index = 0; index < sz; ++index) {
        if (node_list->at(index).character > *c)
          break;
      }
    }

    node_list->emplace(node_list->begin() + index, AMTNode(*c));

    if (!node->sub_trie->map && node_list->size() >= MIN_BITMAPPED_SIZE)
      node->sub_trie->add_bitmap();

    node = &node_list->at(index);
  }
}

size_t ArrayMappedTrie::size() {
  if (nodes.empty()) {
    return 0;
  } else {
    AMTNode *root = &nodes[0];
    assert(root->sub_trie);
    return root->sub_trie_size();
  }
}

size_t ArrayMappedTrie::AMTNode::sub_trie_size() {
  assert(sub_trie);
  size_t total = 0, sz = sub_trie->nodes.size();
  for (unsigned int i = 0; i < sz; ++i) {
    auto node = &sub_trie->nodes.at(i);
    if (node->character == '\0')
      total++;
    else if (node->sub_trie)
      total += node->sub_trie_size();
  }
  return total;
}

void ArrayMappedTrie::add_bitmap() {
  map = new Bitmap();
  size_t sz = nodes.size();
  for (unsigned int i = 0; i < sz; ++i)
    map->set(nodes.at(i).character, true);
}

#endif
