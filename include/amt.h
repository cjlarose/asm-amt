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
  printf("oldlen addr: %p\n", len);
  printf("oldlen: %d\n", *len);
  (*len)++;
  printf("newlen: %d\n", *len);
  printf("final size: %zu\n", *len * sizeof(AMTNode));
  *node = (AMTNode *) realloc(*node, *len * sizeof(AMTNode));

  // move nodes [index..*len) down one slot
  size_t sz = sizeof(AMTNode) * (*len - index - 1);

  printf("size: %zu\n", sz);
  printf("dest: %p\n", &(*node[index]) + 1);
  printf("src (new node addr): %p\n", &(*node[index]));
  memmove(&(*node[index]) + 1, &(*node[index]), sz);

  // update new node with data
  amtnode_init(&(*node[index]), 0, c);

  /*
  assert((*node[index]).node_count == 0);
  assert((*node[index]).character == c);
  assert((*node[index]).sub_trie == NULL);
  */
}

void amt_init(BitMappedNode *trie) {
  bitmap_init(&trie->map);
  //trie->node = NULL;
  trie->node = (AMTNode *) malloc(sizeof(AMTNode));
  amtnode_init(trie->node, 0, '\0'); // initialize dummy root node
}

void amt_insert(BitMappedNode *root, const void *value, size_t len) {
  int i = 0;
  BitMappedNode **trie = &root->node->sub_trie;
  uint16_t *node_count = &root->node->node_count;
  unsigned char *c = (unsigned char *) value;

  for (i = 0; i < len; ++i, ++c) {
    if (!*trie) {
      *trie = (BitMappedNode *) malloc(sizeof(BitMappedNode));
      bitmap_init(&(*trie)->map);
      (*trie)->node = NULL;
      //(*trie)->node = (AMTNode *) malloc(sizeof(AMTNode));
      //amtnode_init((*trie)->node, 0, '\0');
    }
    assert(*trie != NULL);
    ///assert((*trie)->node != NULL);

    printf("\n\ntrie: %p\n", trie);
    printf("trie node: %p\n", (*trie)->node);
    printf("char: %d %c\n", *c, *c);

    int index = bitmap_get_offset(&(*trie)->map, *c);

    // see if the character is already there
    if (!bitmap_get(&(*trie)->map, *c)) {
      // set the character in the bitmap
      bitmap_set(&(*trie)->map, *c, true);
      bitmap_print(&(*trie)->map);
      //assert(bitmap_get(&(*trie)->map, *c));

      // actually add the node
      printf("insert into index %d\n", index);
      printf("count before insert %d\n", *node_count);
      amtnodelist_insert(&(*trie)->node, node_count, index, *c);
      printf("count after insert %d\n", *node_count);
    }

    AMTNode *new_node = &(*trie)->node[index];
    printf("NEW NODE addr: %p\n", new_node);


    /*
    assert(new_node->node_count == 0);
    assert(new_node->character == *c);
    assert(new_node->sub_trie == NULL);
    */

    trie = &new_node->sub_trie;
    printf("next trie: %p\n", trie);
    printf("next *trie: %p\n", *trie);
    node_count = &new_node->node_count;
  }
}

#endif
