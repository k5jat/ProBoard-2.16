#include <stdio.h>
#include <string.h>
#include <tslib.hpp>

#define PAGE_SIZE 60

class Page
  {
  String lines[PAGE_SIZE];
  int breakat;
  int numlines;
 public:
  Page()     { newpage(); }
  Page(Page&);
  void operator=(Page&);
  void newpage() { breakat=0; numlines=0; }  // Create an empty page
  String operator[](int);               // Get nth line from page
  int operator<<(char *);                // Add a line to the page (0:ok,1:print,-1:error)
  int output(FILE *);                    // Output page to file
  };

Page::Page(Page& p)
{
numlines=p.numlines;
breakat=p.breakat;
for(int i=0;i<PAGE_SIZE;i++) lines[i]=p.lines[i];
}

void
Page::operator=(Page& p)
{
numlines=p.numlines;
breakat=p.breakat;
for(int i=0;i<PAGE_SIZE;i++) lines[i]=p.lines[i];
}

String
Page::operator[](int i)
{
return lines[i-1];
}

int
Page::operator<<(char *str)
{
int l=strlen(str);
if(l && str[l-1]=='\n') str[l-1]='\0';

String s(str);

if(s[0]=='{')
  {
  if(s[1]=='}') s=" ";
          else { breakat=numlines; return 1; }
  }

if(!s.len()) breakat=numlines;

lines[numlines++]=s;

if(numlines>PAGE_SIZE-10)
  {
  if(breakat) return 1;
         else return -1;
  }

if(numlines==1 && !s.len()) numlines=0;
return 0;
}

int
Page::output(FILE *fp)
{
for(int i=0;i<breakat;i++)
  {
  fprintf(fp,"%s\n",(char *)lines[i]);
  }

Page tmp;

for(i=breakat;i<numlines;i++) tmp << lines[i];

i=breakat;

*this=tmp;

return i;
}

main(int argc,char *argv[])
{
 FILE *fpi,*fpo;

 if(argc != 3)
   {
    printf("\nUsage: PAG <infile> <outfile>\n");

    return 0;
   }

 fpi=fopen(argv[1],"rt");
 if(fpi==NULL)
   {
    printf("\nUnable to open file %s for input\n" , argv[1] );
    return 1;
   }
 fpo=fopen(argv[2],"wt");
 if(fpo==NULL)
   {
    printf("\nUnable to create output file %s\n" , argv[2] );
    return 1;
   }

 Page p;

 for(int nr=0;;)
   {
   char s[100];
   if(fgets(s,99,fpi)==NULL) break;
   switch(p << s)
     {
     case 1: {
             if(nr) fprintf(fpo,"\n\n\n");
               else fprintf(fpo,"\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
             int n=p.output(fpo);
             for(int i=0;i<PAGE_SIZE-n-8;i++) fprintf(fpo,"\n");
             if(nr) fprintf(fpo,"                                      - %d -\f\n",nr);
               else fprintf(fpo,"\f");
             nr++;
             } break;
     case -1: {
             printf("Error on page %d\n",nr);
             return 1;
             }
     }
   }

 fprintf(fpo,"\n\n\n\n");
 int n=p.output(fpo);
 for(int i=0;i<PAGE_SIZE-n-8;i++) fprintf(fpo,"\n");
 fprintf(fpo,"                                      - %d -\n",nr);

 fclose(fpi);
 fclose(fpo);

 return 0;
}


