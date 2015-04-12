#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "inc/btree.h"

#define BASE 26
#define MAXLENG 20
#define MAXSIZE 1000000
#define NRES 10			/* The number of results displayed
				   at a time*/

void mem_cpy(void *item1, const void *item2, size_t size)
{
  int k;
  char *i1, *i2;

  i1 = (char *)item1;
  i2 = (char *)item2;
  for (k = 0; k < size; k++) {
    *i1 = *i2;
    i1++;
    i2++;
  }
}

void delbuf()
{
  while (getchar() != '\n');
}

/* Gets index from a word */
long long get_index(char *word)
{
  int i;
  long long index=0;

  for (i = 0; i < strlen(word); i++) 
    index += (word[i] - 'a' + 1) * pow(BASE, strlen(word)-1-i);
  return index;
}

/* Gets word from index */
void get_word(long long index, char *word)
{
  int i=0;
  int j;
  char tmp;
  
  do {
    word[i++] = index%BASE + 'a' - 1;
    index = (long long)(index/BASE);     
  } while (index != 0);
  word[i] = '\0';
  
  /* Reverse string */
  for (i = 0, j = strlen(word)-1; i < j; i++, j--) {
    tmp = word[i];
    word[i] = word[j];
    word[j] = tmp;
  }
}

/* Add a word and its subword into tree with */
void add_word(BTA *b, char *word)
{
  int i, j;
  char tmp[MAXLENG+1];
  long long index;

  index = get_index(word);
  for (i = 1; i <= strlen(word); i++) {
    for (j = 0; j < i; j++) tmp[j] = word[j];
    tmp[j] = '\0';
    binsky(b, tmp, index);
  }
}


void menu_search(BTA *b)
{
  char c;
  int i=0;
  char word[MAXLENG+1], next_word[MAXLENG];
  char result[MAXLENG+1];
  BTint index;
  BTint index_arr[MAXSIZE];
  long size, j;

  printf("Find word: ");
  i = 0;
  while (1) {
    c = getchar();

    if (c != '\t' && c != '\n') word[i++] = c;

    else if (i == '\t') {
      word[i] = '\0';
      if (bfndky(b, word, &index) == 0) {
	size = 1;
	index_arr[0] = index;
	while (bnxtky(b, next_word, &index) == 0 || 
	       strcmp(next_word, word) == 0) {
	  size++;
	  index_arr[size-1] = index;
	}
	
	if (size == 1) {
	  get_word(index_arr[0], result);
	  for (j = i; j < strlen(result); j++) printf("%c", result[j]);
	  printf("\n");
	  break;
	}
	else if (size > 1) {
	  for (j = 0; j < NRES && j < size; j++) {
	    get_word(index_arr[j], result);
	    printf("%s\n", result);
	  }
	  while (j < size) {
	    c = getchar();
	    if (c == '\n') {
	      get_word(index_arr[j], result);
	      printf("%s", result);
	      j++;
	    }
	    else if (c == 'q') break;
	  }
	  printf("Find word: %s", word);
	}
      }
    }

    else if (i == '\n') {
      word[i] = '\0';
      if (bfndky(b, word, &index) == 0) 
	printf("This word is found\n");
      else printf("Not found!\n");
      break;
    }
  }
}

void menu_delete(BTA *b)
{
  char word[MAXLENG+1];

  printf("Input word to delete: ");
}


 int main()
{
  BTA *b;
  FILE *f;
  char word[MAXLENG+1];
  int i, choice, value;
  long long index;

  btinit();
  b = btcrt("dictionary", 0, 0);

  /* Enable support for duplicate keys */
  btdups(b, 1);

  f = fopen("sub.txt", "r");
  if (f == NULL) {
    printf("Can't open this file");
    exit(1);
  }
  
  i = 0;
  while (1) {
    fscanf(f, "%s", word);
    if (feof(f)) break;

    i++;
    add_word(b, word);
  }
  fclose(f);

  while (1) {
    printf("\n\nMenu\n");
    printf("1. Add\n");
    printf("2. Search\n");
    printf("3. Delete\n");
    printf("4. Exit\n");
    printf("Your choice: ");
    scanf("%d", &choice);
    delbuf();
    if (choice == 4) break;

    switch (choice) {
    case 1:
      printf("Input a new word to add: ");
      scanf("%s", word);
      delbuf();
      i++;
      binsky(b, word, index);
      break;

    case 2:
	printf("Input word to find: ");
	scanf("%s", word);
	delbuf();
	if (bfndky(b, word, &index) == 0) 
		printf("Word found\n");
	else printf("Not found!!!\n");
      break;

    case 3:
	printf("Input word to delete: ");
	scanf("%s", word);
	delbuf();
	if (bdelky(b, word) == 0) 
		printf("Deleting done\n");
	else printf("This word not exist!\n");
	break;
      
    default: 
      printf("Wrong choice!\n");
      printf("Please input again");
    }
  }

  btcls(b);
  return 0;
}
