#include <stdio.h>

int main()
{
  int i, j;
  printf("enter a value1 ");
  scanf("%d", &i);
  printf("enter a value2 ");
  scanf("%d", &j);

  for (int n = ((i < j) ? i : j); n <= ((i > j) ? i : j); n++)
  {
    int sum = 0;
    int tempNumber = n;
    while (tempNumber != 0)
    {
      int rem = 0;
      rem = tempNumber % 10;
      sum = sum + (rem * rem * rem);
      tempNumber /= 10;
    }

    if (sum == n)
    {

      printf("the following armstrong number %d\n", n);
    }
  }
  return 0;
}
