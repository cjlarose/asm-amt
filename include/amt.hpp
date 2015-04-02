#ifndef AMT_H
#define AMT_H

#include <vector>
#include "bit_map.hpp"

const int MIN_BITMAPPED_SIZE = 5;

class Trie {
  private:

    class SubTrie;

    class AMTNode {
      public:
        uint16_t character;
        std::unique_ptr<Trie::SubTrie> sub_trie;
        AMTNode(char c): character(c), sub_trie(nullptr) {}
        AMTNode *next(char c);
        size_t sub_trie_size();
    };

    class SubTrie {
      public:
        virtual Trie::AMTNode *next(char c) = 0;
        virtual Trie::AMTNode *insert_node(char c) = 0;
        std::unique_ptr<std::vector<Trie::AMTNode>> nodes;
        SubTrie() : nodes(new std::vector<Trie::AMTNode>) {};
    };

    class CompactTrie : public SubTrie {
      public:
        Trie::AMTNode *next(char c);
        Trie::AMTNode *insert_node(char c);
    };

    class ArrayMappedTrie : public SubTrie {
      public:
        Bitmap map;
        Trie::AMTNode *next(char c);
        Trie::AMTNode *insert_node(char c);
    };

    static ArrayMappedTrie *add_bitmap(SubTrie *old);
    CompactTrie root;

  public:
    void insert(const char *str);
    bool contains(const char *str);
    size_t size();
};

Trie::AMTNode *Trie::CompactTrie::next(char c) {
  for (unsigned int index = 0; index < nodes->size(); ++index) {
    AMTNode *node = &nodes->at(index);
    if (node->character == c)
      return node;
    if (node->character > c)
      return NULL;
  }
  return NULL;
}

Trie::AMTNode *Trie::ArrayMappedTrie::next(char c) {
  if (!map.get(c)) return NULL;
  int index = map.get_offset(c);
  return &nodes->at(index);
}

Trie::AMTNode *Trie::AMTNode::next(char c) {
  return sub_trie ? sub_trie->next(c) : NULL;
}

bool Trie::contains(const char *str) {
  if (!root.nodes->size()) return false;

  AMTNode *node = &root.nodes->at(0);

  for (; *str; ++str) {
    if (!node) return false;
    node = node->next(*str);
  }
  return node->next('\0') != NULL;
}

Trie::AMTNode *Trie::CompactTrie::insert_node(char c) {
  unsigned int index = 0;
  size_t sz = nodes->size();

  while (index < sz && nodes->at(index).character < c)
    index++;

  nodes->emplace(nodes->begin() + index, AMTNode(c));
  return &nodes->at(index);
}

Trie::AMTNode *Trie::ArrayMappedTrie::insert_node(char c) {
  map.set(c, true);
  int index = map.get_offset(c);
  nodes->emplace(nodes->begin() + index, AMTNode(c));
  return &nodes->at(index);
}

void Trie::insert(const char *str) {
  char *c = (char *) str;

  if (!root.nodes->size())
    root.nodes->push_back(AMTNode('\0'));

  AMTNode *node = &root.nodes->at(0);

  for (; *c; ++c) {
    AMTNode *tmp = node->next(*c);
    if (!tmp) break;
    node = tmp;
  }

  int size = strlen(str) + 1;

  for (int i = 0; i < size; ++i, ++c) {
    if (!node->sub_trie)
      node->sub_trie.reset(new CompactTrie());

    std::vector<AMTNode> *node_list = node->sub_trie->nodes.get();
    auto tmp_node = node->sub_trie->insert_node(*c);

    if (node_list->size() == MIN_BITMAPPED_SIZE)
      node->sub_trie.reset(add_bitmap(node->sub_trie.get()));

    node = tmp_node;
  }
}

size_t Trie::size() {
  if (root.nodes->empty()) {
    return 0;
  } else {
    assert(root.nodes->at(0).sub_trie);
    return root.nodes->at(0).sub_trie_size();
  }
}

size_t Trie::AMTNode::sub_trie_size() {
  assert(sub_trie);
  size_t total = 0, sz = sub_trie->nodes->size();
  for (unsigned int i = 0; i < sz; ++i) {
    auto node = &sub_trie->nodes->at(i);
    if (node->character == '\0')
      total++;
    else if (node->sub_trie)
      total += node->sub_trie_size();
  }
  return total;
}

Trie::ArrayMappedTrie *Trie::add_bitmap(SubTrie *old) {
  auto new_trie = new ArrayMappedTrie();
  new_trie->nodes = std::move(old->nodes);

  size_t sz = new_trie->nodes->size();
  for (unsigned int i = 0; i < sz; ++i)
    new_trie->map.set(new_trie->nodes->at(i).character, true);
  return new_trie;
}
#endif
