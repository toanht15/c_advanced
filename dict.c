
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

typedef struct
{
 array list[max_list];
 int count;
} Sound;

char file[] ="Dict.nhom7";
char file1[]="Soundex.nhom7";
char data[] ="dict.txt";  //File get data
BTA *Dic;
BTA *Sou;
int key=1;

int SoundEx(char *SoundEx,
  char *WordString,
  int   LengthOption,
  int   CensusOption)
{
  int  InSz = 31;
      char WordStr[32];     /* one bigger than InSz */
  int  SoundExLen, WSLen, i;
  char FirstLetter, *p, *p2;

  SoundExLen = WSLen = 0;
  SoundEx[0] = 0;

  if(CensusOption) {
    LengthOption = 4;
  }

  if(LengthOption) {
    SoundExLen = LengthOption;
  }
  if(SoundExLen > 10) {
    SoundExLen = 10;
  }
  if(SoundExLen < 4) {
    SoundExLen = 4;
  }

  if(!WordString) {
    return(0);
  }

      /* Copy WordString to WordStr
       * without using funcs from other
       * libraries.
      */
       for(p = WordString,p2 = WordStr,i = 0;(*p);p++,p2++,i++) {
        if(i >= InSz) break;
        (*p2) = (*p);
      }
      (*p2) = 0;



      /* Convert WordStr to
       * upper-case, without using funcs
       * from other libraries
      */
       for(p = WordStr;(*p);p++) {
        if( (*p) >= 'a' && (*p) <= 'z' ) {
          (*p) -= 0x20;
        }
      }


      /* convert all non-alpha
       * chars to spaces
      */
       for(p = WordStr;(*p);p++) {
        if( (*p) < 'A' || (*p) > 'Z' ) {
          (*p) = ' ';
        }
      }

      /* Remove leading spaces
      */
      for(i = 0, p = p2 = WordStr;(*p);p++) {
        if(!i) {
          if( (*p) != ' ' ) {
            (*p2) = (*p);
            p2++;
            i++;
          }
        }
        else {
          (*p2) = (*p);
          p2++;
        }
      }
      (*p2) = 0;

      /* Get length of WordStr
      */
      for(i = 0,p = WordStr;(*p);p++) i++;


      /* Remove trailing spaces
      */
        for(;i;i--) {
          if(WordStr[i] == ' ') {
            WordStr[i] = 0;
          }
          else {
            break;
          }
        }

      /* Get length of WordStr
      */
        for(WSLen = 0,p = WordStr;(*p);p++) WSLen++;

          if(!WSLen) {
            return(0);
          }



      /* Perform our own multi-letter
       * improvements
       *
       * underscore placeholders (_) will be
       * removed below.
      */
       if(!CensusOption) {
        if(WordStr[0] == 'P' && WordStr[1] == 'S') {
          WordStr[0] = '_';
        }
        if(WordStr[0] == 'P' && WordStr[1] == 'F') {
          WordStr[0] = '_';
        }

        for(i = 0;i < WSLen;i++) {
          if(WordStr[i] == 'D' && WordStr[i+1] == 'G') {
            WordStr[i] = '_';
            i++;
            continue;
          }
          if(WordStr[i] == 'G' && WordStr[i+1] == 'H') {
            WordStr[i] = '_';
            i++;
            continue;
          }
          if(WordStr[i] == 'K' && WordStr[i+1] == 'N') {
            WordStr[i] = '_';
            i++;
            continue;
          }
          if(WordStr[i] == 'G' && WordStr[i+1] == 'N') {
            WordStr[i] = '_';
            i++;
            continue;
          }
          if(WordStr[i] == 'M' && WordStr[i+1] == 'B') {
            WordStr[i+1] = '_';
            i++;
            continue;
          }

          if(WordStr[i] == 'P' && WordStr[i+1] == 'H') {
            WordStr[i] = 'F';
            WordStr[i+1] = '_';
            i++;
            continue;
          }
          if(WordStr[i]  ==  'T'  &&
           WordStr[i+1] == 'C' &&
           WordStr[i+2] == 'H'
           ) {

            WordStr[i] = '_';
          i++; i++;
          continue;
        }
        if(WordStr[i] == 'M' && WordStr[i+1] == 'P'
         && (WordStr[i+2] == 'S' ||
           WordStr[i+2] == 'T' ||
           WordStr[i+2] == 'Z')
         ) {
          WordStr[i+1] = '_';
        i++;
      }
    }
      } /* end if(!CensusOption) */



      /* squeeze out underscore characters
       * added as a byproduct of above process
       * (only needed in c styled replace)
      */
       for(p = p2 = WordStr;(*p);p++) {
        (*p2) = (*p);
        if( (*p2) != '_' ) {
          p2++;
        }
      }
      (*p2) = 0;





      /* This must be done AFTER our
       * multi-letter replacements
       * since they could change
       * the first letter
      */
       FirstLetter = WordStr[0];


      /* In case we're in CensusOption
       * 1 and the word starts with
       * an 'H' or 'W'
       *  (v1.0c djr: add test for H or W)
      */
       if(FirstLetter == 'H' || FirstLetter == 'W') {
        WordStr[0] = '-';
      }



      /* In properly done census
       * SoundEx, the H and W will
       * be squezed out before
       * performing the test
       * for adjacent digits
       * (this differs from how
       * 'real' vowels are handled)
      */
if(CensusOption == 1) {
  for(p = &(WordStr[1]);(*p);p++) {
   if((*p) == 'H' || (*p) == 'W') {
     (*p) = '.';
   }
 }
}





      /* Perform classic SoundEx
       * replacements.
      */
       for(p = WordStr;(*p);p++) {
        if( (*p) == 'A'   ||
          (*p) == 'E'   ||
          (*p) == 'I'   ||
          (*p) == 'O'   ||
          (*p) == 'U'   ||
          (*p) == 'Y'   ||
          (*p) == 'H'   ||
          (*p) == 'W'
          ){
              (*p) = '0';   /* zero */
        }
      if( (*p) == 'B'   ||
        (*p) == 'P'   ||
        (*p) == 'F'   ||
        (*p) == 'V'
        ){
        (*p) = '1';
    }
    if( (*p) == 'C'   ||
      (*p) == 'S'   ||
      (*p) == 'G'   ||
      (*p) == 'J'   ||
      (*p) == 'K'   ||
      (*p) == 'Q'   ||
      (*p) == 'X'   ||
      (*p) == 'Z'
      ){
      (*p) = '2';
  }
  if( (*p) == 'D'   ||
    (*p) == 'T'
    ){
    (*p) = '3';
}
if( (*p) == 'L' ) {
  (*p) = '4';
}

if( (*p) == 'M'   ||
  (*p) == 'N'
  ){
  (*p) = '5';
}
if( (*p) == 'R' ) {
  (*p) = '6';
}
}
      /* soundex replacement loop done  */




      /* In properly done census
       * SoundEx, the H and W will
       * be squezed out before
       * performing the test
       * for adjacent digits
       * (this differs from how
       * 'real' vowels are handled)
      */
if(CensusOption == 1) {
          /* squeeze out dots
          */
  for(p = p2 = &WordStr[1];(*p);p++) {
    (*p2) = (*p);
    if( (*p2) != '.' ) {
      p2++;
    }
  }
  (*p2) = 0;
}



      /* squeeze out extra equal adjacent digits
       * (don't include first letter)
       * v1.0c djr (now includes first letter)
      */
       for(p = p2 = &(WordStr[0]);(*p);p++) {
        (*p2) = (*p);
        if( (*p2) != p[1] ) {
          p2++;
        }
      }
      (*p2) = 0;



      /* squeeze out spaces and zeros
       * Leave the first letter code
       * to be replaced below.
       * (In case it made a zero)
      */
       for(p = p2 = &WordStr[1];(*p);p++) {
        (*p2) = (*p);
        if( (*p2) != ' ' && (*p2) != '0' ) {
          p2++;
        }
      }
      (*p2) = 0;



      /* Get length of WordStr
      */
      for(WSLen = 0,p = WordStr;(*p);p++) WSLen++;


      /* Right pad with zero characters
      */
        for(i = WSLen;i < SoundExLen;i++ ) {
          WordStr[i] = '0';
        }

      /* Size to taste
      */
        WordStr[SoundExLen] = 0;


      /* Replace first digit with
       * first letter.
      */
       WordStr[0] = FirstLetter;


      /* Copy WordStr to SoundEx
      */
       for(p2 = SoundEx,p = WordStr;(*p);p++,p2++) {
        (*p2) = (*p);
      }
      (*p2) = 0;
      
      return(SoundExLen);
    }

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

void add()        //Add new word
{
 Word *new;
 new=(Word *)malloc(sizeof(Word));
 char mean[max],word[max];
 int error;
 getchar();
 printf("Nhap tu: ");          
 fgets(word,100,stdin);word[strlen(word)-1]='\0';
 toi_uu(word);
 strcpy(new->word,word);
 

 printf("Nhap nghia: ");       
 fgets(mean,100,stdin);word[strlen(mean)-1]='\0';
 strcpy(new->mean,mean);

 error=btins(Dic,new->word,(char *)new,sizeof(Word));
 if (error!=0)
  printf("Error:%d Tu nay da co trong tu dien \n",error);

free(new);
}

void delete()    //Xoa tu trong Dictionary
{
 char key[max];
 int error;

 printf("Nhap tu muon xoa: ");
 getchar();fgets(key,100,stdin);key[strlen(key)-1]='\0';
 toi_uu(key);
 

 error=btdel(Dic,key);
 if (error!=0) printf("Error: %d Khong ton tai tu nay \n",error);
}

void edit()     //Chinh sua tu trong Dictionary
{
 Word *buf;
 char key[max];
 char new_mean[max];
 int rsize;
 buf=(Word *)malloc(sizeof(Word));

 printf("Nhap tu muon chinh sua: ");
 getchar();fgets(key,100,stdin);key[strlen(key)-1]='\0';
 toi_uu(key);

 if (btsel(Dic,key,(char *)buf,sizeof(Word),&rsize)!=0) 
   printf("Khong ton tai tu nay \n");
 else
 {
  strcpy(buf->word,key);
  printf("Nhap nghia cua tu: ");
  fgets(new_mean,100,stdin);new_mean[strlen(new_mean)-1]='\0';
  strcpy(buf->mean, new_mean);

  btupd(Dic,key,(char *)buf,sizeof(Word));
}

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

