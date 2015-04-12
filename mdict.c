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

int main(int argc, char *argv[])
{
  FILE *f;
  int i;
  char *word;
  char *mean;

  if(argc<2) printf("Ban nhap sai doi so\n");

  f = fopen(argv[1], "w");
  for (i = 0; i < 10000; i++) {
    word = make_word(rand()%10+4);
    mean = make_word(7);
    fprintf(f, "%s\t%s\n", word,mean);
    free(word);
  } 
  fclose(f);
  return 0;
}

// void makeDict(int size){
//   FILE *fData;
//   int i;
//   char *word,*mean;

//   f=fopen(fData,"w");
//   for ( i = 0; i < size; ++i)
//   {
//         word = make_word(rand()%10+4);
//     mean = make_word(7);
//     fprintf(f, "%s\t%s\n", word,mean);
//     free(word);
//   }
//   printf("Successfull\n");
//   fclose(f);
// }