#include <stdlib.h>
#include <stdio.h>

typedef struct _list {
  int value;
  struct _list *next;
} List;

List *list_create_node(int value)
{
  List *list = (List *) malloc(sizeof(List));
  list->value = value;
  list->next = NULL;
  return list;
}

int list_count(List *list)
{
  int count = 0;
  List *aux = list;
  while (aux) {
    aux = aux->next;
    count++;
  }
  return count;
}

List *list_add(List *list, int value)
{
  List *newNode = list_create_node(value);

  if (list) {
    newNode->next = list;
  }

  return newNode;
}

List *list_add_unique(List *list, int value)
{
  List *aux = list;

  while (aux) {
    if (aux->value == value)
      return list;
    aux = aux->next;
  }

  return list_add(list, value);
}

int list_get(List *list, int index)
{
  List *aux = list;
  int count = list_count(list);

  if (index >= count || index < 0) {
    exit(1);
  }

  while (index--) {
    aux = aux->next;
  }

  return aux->value;
}

void list_print(List *list)
{
  List *aux = list;
  int i, count = list_count(list);
  printf("[");
  for (i = 0; i < count; i++) {
    printf("%d", aux->value);
    aux = aux->next;
    if (i < count - 1) {
      printf(", ");
    }
  }
  printf("]");
}

void list_clear(List *list)
{
  List *aux = list;
  while (list) {
    aux = list->next;
    free(list);
    list = aux;
  }
}
