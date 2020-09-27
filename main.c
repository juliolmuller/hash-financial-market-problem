#include <stdlib.h>
#include <stdio.h>
#include "finmkt.h"

void main()
{
  char operation, lineBreak;
  int partner, portfolio;
  List *partnersList = NULL;
  List *portfoliosList = NULL;

  do {
    operation = 0; partner = 0; portfolio = 0;
    scanf("%c%d%d%c", &operation, &partner, &portfolio, &lineBreak);
    partnersList = list_add(partnersList, partner);
    portfoliosList = list_add(portfoliosList, portfolio);
  } while (partner);

  if (portfoliosList) {
    partnersList = partnersList->next;
    portfoliosList = portfoliosList->next;
    while (partnersList) {
      operation = portfoliosList->value ? '+' : '-';
      printf("%c %5d %7d \n", operation, partnersList->value, portfoliosList->value);
      partnersList = partnersList->next;
      portfoliosList = portfoliosList->next;
    }
  }
}
