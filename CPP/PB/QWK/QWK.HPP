struct QWK_Header
  {
    char status;
    char num  [7];
    char date [8];
    char time [5];
    char to   [25];
    char from [25];
    char subj [25];
    char pwd  [12];
    char rnum [8];
    char blks [6];
    byte active;
    word area;
    word logicalnum;
    char network;
  };

struct QWK_Index
  {
    long record;
    char dummy;
  };
