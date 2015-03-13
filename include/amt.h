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
  printf("index: %d\n", index);
  printf("oldlen: %u\n", *len);
  (*len)++;
  printf("newlen: %u\n", *len);

  printf("*node before: %p\n", *node);
  *node = (AMTNode *) realloc(*node, *len * sizeof(AMTNode));
  printf("*node after: %p\n", *node);

  // move nodes [index..*len) down one slot
  printf("sizeof(AMTNode): %zu\n", sizeof(AMTNode));
  size_t sz = sizeof(AMTNode) * (*len - index - 1);
  printf("size: %zu\n", sz);
  printf("dest: %p\n", &(*node[index]) + sizeof(AMTNode));
  printf("src: %p\n", &(*node[index]));
  memmove(&(*node[index]) + sizeof(AMTNode), &(*node[index]), sz);

  // update new node with data
  amtnode_init(&(*node[index]), 0, c);
}

void amt_init(BitMappedNode *trie) {
  bitmap_init(&trie->map);
  trie->node = (AMTNode *) malloc(sizeof(AMTNode));
  amtnode_init(trie->node, 0, '\0'); // initialize dummy root node
}

void amt_insert(BitMappedNode *root, const void *value, size_t len) {
  printf("\n\nINSERT\n");
  int i = 0;

  BitMappedNode *trie = root->node->sub_trie;
  if (!trie) {
    printf("trie: %p\n", trie);
    trie = root->node->sub_trie = (BitMappedNode *) malloc(sizeof(BitMappedNode));
    bitmap_init(&trie->map);
    trie->node = NULL;
    printf("trie: %p\n", trie);
  }
  assert(trie != NULL);

  bitmap_print(&trie->map);

  unsigned char *c = (unsigned char *) value;

  for (i = 0; i < len; ++i, ++c) {
    printf("char: %u %c\n", *c, *c);

    // set the character in the bitmap
    bitmap_set(&trie->map, *c, true);
    bitmap_print(&trie->map);
    assert(bitmap_get(&trie->map, *c));

    // actually add the node
    int index = bitmap_get_offset(&trie->map, *c);
    printf("index: %d\n", index);
    amtnodelist_insert(&trie->node, &root->node->node_count, index, *c);
  }
}

#endif
