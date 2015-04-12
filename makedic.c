#include <stdio.h>
#include <stdlib.h>

char *make_word(int lenght)
{
  int i;
  char *word;


  word = malloc(lenght+1);
  for (i = 0; i <= lenght; i++) 
    word[i] = rand()%('z'-'a'+1) + 'a';
  word[i] = '\0';

  return word;
}

int main()
{
  FILE *f;
  int i;
  char *word;
  char *mean;

  f = fopen("dict.txt", "w");
  for (i = 0; i < 10000; i++) {
    word = make_word(3);
    mean = make_word(6);
    fprintf(f, "%s   %s\n", word,mean);
    free(word);
  }
  fclose(f);
  return 0;
}
