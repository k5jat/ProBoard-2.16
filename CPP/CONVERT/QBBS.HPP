struct QBBS_MsgArea
  {
   char Name[41];
   char Type;
   char Kind;
   bool Combined;
   bool Aliases;
   byte Aka;
   char OriginLine[59];
   bool AllowDelete;
   unsigned KeepCnt,
            KillRcvd,
            KillOld;
   unsigned ReadLevel;
   long     ReadFlags;
   unsigned WriteLevel;
   long     WriteFlags;
   unsigned TemplateLevel;
   long     TemplateFlags;
   unsigned SysopLevel;
   long     SysopFlags;
   byte Spare[12];
  };

struct QBBS_FileArea
  {
   char Name[67];
   char FilePath[67];
   char ListPath[67];
   bool AllowUploads;
   bool IgnoreKlimit;
   bool IgnoreRatio;
   unsigned Level;
   long Flags;
   unsigned SearchLevel;
   long     SearchFlags;
   unsigned TemplateLevel;
   long     TemplateFlags;
   byte     Spare[12];
  };
