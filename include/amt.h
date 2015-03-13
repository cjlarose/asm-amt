#ifndef AMT_H
#define AMT_H
 
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "bit_map.h"

typedef struct BitMappedNode {
  Bitmap map;
  struct AMTNode *node;
} BitMappedNode;

typedef struct AMTNode {
  uint16_t node_count;
  uint16_t character;
  BitMappedNode *sub_trie;
} AMTNode;

void amtnode_init(AMTNode *node, uint16_t count, uint16_t character) {
  node->node_count = count;
  node->character = character;
  node->sub_trie = NULL;
}

void amtnodelist_insert(AMTNode **node, uint16_t *len, int index, char c) {
  // resize node list to accommodate one more entry
  (*len)++;
  *node = (AMTNode *) realloc(*node, *len * sizeof(AMTNode));

  // move nodes [index..*len) down one slot
  size_t sz = sizeof(AMTNode) * (*len - index - 1);

  AMTNode *src = &(*node)[index];
  AMTNode *dst = &(*node)[index] + 1;
  memmove(dst, src, sz);

  // update new node with data
  amtnode_init(&(*node)[index], 0, c);
}

void amt_init(BitMappedNode *trie) {
  bitmap_init(&trie->map);
  trie->node = (AMTNode *) malloc(sizeof(AMTNode));
  amtnode_init(trie->node, 0, '\0'); // initialize dummy root node
}

void amt_insert(BitMappedNode *root, const void *value, size_t len) {
  int i;
  BitMappedNode **trie = &root->node->sub_trie;
  uint16_t *node_count = &root->node->node_count;
  unsigned char *c = (unsigned char *) value;

  for (i = 0; i < len; ++i, ++c) {
    if (!*trie) {
      *trie = (BitMappedNode *) malloc(sizeof(BitMappedNode));
      bitmap_init(&(*trie)->map);
      (*trie)->node = NULL;
    }

    int index = bitmap_get_offset(&(*trie)->map, *c);

    // see if the character is already there
    if (!bitmap_get(&(*trie)->map, *c)) {
      // set the character in the bitmap
      bitmap_set(&(*trie)->map, *c, true);
      bitmap_print(&(*trie)->map);

      // actually add the node
      amtnodelist_insert(&(*trie)->node, node_count, index, *c);
    }

    AMTNode *new_node = &(*trie)->node[index];
    trie = &new_node->sub_trie;
    node_count = &new_node->node_count;
  }
}

bool amt_contains(BitMappedNode *root, const void *value, size_t len) {
  int i;
  BitMappedNode **trie = &root->node->sub_trie;
  unsigned char *c = (unsigned char *) value;

  for (i = 0; i < len; ++i, ++c) {
    if (!*trie || !bitmap_get(&(*trie)->map, *c))
      return false;

    int index = bitmap_get_offset(&(*trie)->map, *c);
    AMTNode *node = &(*trie)->node[index];
    trie = &node->sub_trie;
  }
  return true;
}

#endif
