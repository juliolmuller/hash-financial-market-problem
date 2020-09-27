#include <stdlib.h>
#include <stdio.h>
#include "list.h"

typedef struct _hash {
  int count;
  int size;
  List **items;
} HashTable;

int hash_gen_division(int key, int tableSize)
{
  return (key & 0x7fffffff) % tableSize;
}

int hash_gen_multiplication(int key, int tableSize)
{
  float value = key * 0.6180339887;
  value -= (int) value;

  return (int) (tableSize * value);
}

int hash_gen_fold(int key, int tableSize)
{
  int partial1 = key >> 10;
  int partial2 = key & (tableSize - 10);

  return partial1 ^ partial2;
}

int hash_gen(int key, int tableSize)
{
  return hash_gen_division(key, tableSize);
}

HashTable *hash_create(const int tableSize)
{
  HashTable *hash = (HashTable *) malloc(sizeof(HashTable));
  hash->items = (List **) malloc(sizeof(List *) * tableSize);
  hash->size = tableSize;
  hash->count = 0;

  int i;
  for (i = 0; i < tableSize; i++) {
    hash->items[i] = NULL;
  }

  return hash;
}

List *hash_insert(HashTable *hash, int value)
{
  if (!hash) {
    return NULL;
  }

  int i, position = hash_gen(value, hash->size);
  hash->items[position] = list_add_unique(hash->items[position], value);
  hash->count++;

  return hash->items[position];
}

List *hash_search(HashTable *hash, int value)
{
  if (!hash) {
    return NULL;
  }

  int i, position = hash_gen(value, hash->size);
  List *aux = hash->items[position];
  while (aux) {
    if (aux->value == value) {
      return aux;
    }

    aux = aux->next;
  }
  return NULL;
}

void hash_print(HashTable *hash)
{
  int i;
  for (i = 0; i < hash->size; i++) {
    List *aux = hash->items[i];
    printf("\nPosicao %d: ", i);

    while (aux) {
      printf("%d ", aux->value);
      aux = aux->next;
    }
  }
  printf("\n");
}

void hash_free(HashTable *hash)
{
  if (hash) {
    int i;
    for (i = 0; i < hash->size; i++) {
      free(hash->items[i]);
    }
    free(hash->items);
    free(hash);
  }
}
