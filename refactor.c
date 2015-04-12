
#include "btree.h"
#include "help.h"

#define max 50
#define max_list 1000

typedef char array[50];

typedef struct
{
 char word[max];
 char mean[max];
} Word;

// typedef struct
// {
//  array list[max_list];
//  int count;
// } Sound;

typedef struct 
{
  array *list;
  int size;
  int max_size;
};
  char file[] ="Dict.nhom7";
  char file1[]="Soundex.nhom7";
  char data[] ="dict.txt";  //File get data
  BTA *Dic;
  BTA *Sou;
  int key=1;


  void String_(char s[max])
  {
   Word *new;
   Sound *buf;
   int error,i,rsize;
   char convert[max];

   new=(Word *)malloc(sizeof(Word));
   buf=(Sound *)malloc(sizeof(Sound));
   char word[max]="",mean[max]="";

   for (i=0;i<strlen(s);i++)
    if (s[i]=='\t') break;      
  strncpy(word,s+0,i);     
     // toi_uu(word);

  strcpy(mean,s+i+1);     
  strcpy(new->word,word);
  strcpy(new->mean,mean);

     //Add vao B Tree Dict
  error=btins(Dic,new->word,(char *)new,sizeof(Word));
  if (error!=0)
    printf("Error:%d Tu \"%s\" da co trong tu dien \n",error,new->word);
     //Add vao B Tree Soundex
  SoundEx(convert,new->word,50,1);
  if (btsel(Sou,convert,(char *)buf,sizeof(Sound),&rsize)!=0)
  {
    buf->count=0;
    btins(Sou,convert,(char *)buf,sizeof(Sound));
  }
  {
    buf->count++;
    strcpy(buf->list[buf->count],new->word); 
    btupd(Sou,convert,(char *)buf,sizeof(Sound));
  }
  free(new);
  free(buf);
}

void get_data() //Lay du lieu tu file da co
{
 FILE *fi;
 if ((fi=fopen(data,"r"))==NULL)
 {
   printf("File nay khong ton tai !!!\n");
   exit(0);
 }

 while (!feof(fi))
 {
   char s[max];
   fgets(s,max,fi);
   if (!feof(fi))
   {
      // printf("%s",s); 
    if(s[0] != '\0')String_(s);
  } 
}
fclose(fi);
}

void create()       //Create file B Tree neu chua ton tai
{
 //Create Dic
 Dic=btopn(file,0,0);
 if (Dic==NULL) Dic=btcrt(file,0,0);
 //Create Soundex
 Sou=btopn(file1,0,0);
 if (Sou==NULL) Sou=btcrt(file1,0,0);
}

void find()        //Tim tu trong Dictionary
{
 int error,rsize,i;
 char word[max],convert[max];
 Word *buf;
 Sound *buf1;
 buf=(Word *)malloc(sizeof(Word));
 buf1=(Sound *)malloc(sizeof(Sound));

 printf("Nhap tu muon tra: "); 
 getchar();fgets(word,100,stdin);word[strlen(word)-1] = '\0';
 toi_uu(word);

 error=btsel(Dic,word,(char *)buf,sizeof(Word),&rsize);
 if (error!=0) {
  printf("%s: Khong tim thay tu nay \n",word);
  SoundEx(convert,word,50,1);
  error=btsel(Sou,convert,(char *)buf1,sizeof(Sound),&rsize);
  if(buf1->count>0) {printf("Co phai ban muon tim tu: ");
  //for (i=1;i<=buf1->count;i++)
  for (i=1;i<=10;i++)
    printf("%s,",buf1->list[i]);
  printf("\n");
}
}
else 
 printf("%s ---> %s \n",buf->word,buf->mean);

 //In Soundex
 // SoundEx(convert,word,50,1);
 // error=btsel(Sou,convert,(char *)buf1,sizeof(Sound),&rsize);
 // for (i=1;i<=buf1->count;i++)
 //  printf("%s,",buf1->list[i]);
 // printf("\n");
}

void output()      //Print Dictionary
{
  int rsize,error=0,count=0;
  char key[max];
  Word *buf;
  buf=(Word *)malloc(sizeof(Word));

  printf(" \n");
  btsel(Dic,"$$default",(char *)buf,sizeof(Word),&rsize);
 //while (error==0)
  while (1)
  {
   error=btseln(Dic,key,(char *)buf,sizeof(Word),&rsize);
   
   if (error==0) {
    printf("%s %s \n",buf->word,buf->mean);
    count++;
  }
   //else printf("%d\n", error);
  else if(error==47) break;
  else printf("%d\n", error);
   // else printf("Error \n");
}
printf("%d\n", count);
free(buf);
}







void generateDict(){
  int size;
  printf("Enter size of dictionary: ");
  scanf("%d",&size);
  makeDict(size);
}

main()
{
 //btinit();   //Khoi tao B Tree
 create();   //Khoi tao or open file B Tree
 //get_data(); //Get data tu file co san vao B Tree

 banner();getchar();
 clear();

 while (key!=0)
 {
  srand (time(NULL));
  menu();
  printf("Chon: "); scanf("%d",&key);
  switch (key)
  {
    case 0:   clear();  goodbye();      break;
    case 1: find();getchar();clear();   break;
    case 2: add();getchar();clear();    break;
    case 3: delete();getchar();clear(); break;
    case 4: edit();getchar();clear();   break;
    case 5: output();getchar(); break;
    case 6: get_data();getchar();clear();break;
    case 7: generateDict(); getchar();break;
  }
}
 btcls(Dic);        //Close B Tree file
 btcls(Sou);
}

