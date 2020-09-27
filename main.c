#include <stdlib.h>
#include <stdio.h>
#include "hash.h"

void problema_mercado_financeiro(List* partners, List *portfolios)
{
  HashTable *cancelledPartners = hash_create(5);
  HashTable *activePortfolios = hash_create(5);

  while (partners) {
    if (portfolios->value) {
      if (!hash_search(cancelledPartners, partners->value)) {
        hash_insert(activePortfolios, portfolios->value);
      }
    } else {
      hash_insert(cancelledPartners, partners->value);
    }

    partners = partners->next;
    portfolios = portfolios->next;
  }

  printf("\nParceiros com ordens canceladas:");
  hash_print(cancelledPartners);
  printf("\nPortfolios ativos:");
  hash_print(activePortfolios);

  hash_free(cancelledPartners);
  hash_free(activePortfolios);
}

void main()
{
  char operation, lineBreak;
  int partner, portfolio;
  long int count = 0;
  List *partnersList = NULL;
  List *portfoliosList = NULL;

  do {
    operation = 0; partner = 0; portfolio = 0;
    scanf("%c%d%d%c", &operation, &partner, &portfolio, &lineBreak);
    partnersList = list_add(partnersList, partner);
    portfoliosList = list_add(portfoliosList, portfolio);
    count++;
  } while (partner);

  partnersList = partnersList->next;
  portfoliosList = portfoliosList->next;
  problema_mercado_financeiro(partnersList, portfoliosList);

  list_clear(partnersList);
  list_clear(portfoliosList);
}
