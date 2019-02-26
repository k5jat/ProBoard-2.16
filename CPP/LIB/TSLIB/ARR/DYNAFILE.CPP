#include <stdio.h>
#include <tslib.hpp>

#include "test.hpp"

int fEmpty(long l)
{
   return (int)l+1;
}

void
fMove(long r1,long r2,int& obj)
{
   if(obj == (int)r1+1)
      obj = (int)r2+1;
}

main()
{
   File f;

   f.open("TEST.DAT",fmode_copen|fmode_rw);

   DynamicArrayFile<int> df(f);

   printf("# recs = %ld\n",df.numRecords());

   df.setEmptyFunc(fEmpty);
   df.setMoveFunc(fMove);

   if(df.numRecords() < 1)
      df.write(100,1);

   printf("# recs = %ld\n",df.numRecords());

   df.remove(50);
   df.remove(50);
   df.remove(50);
   df.remove(50);
   df.remove(50);
   df.remove(50);
   df.remove(50);
   df.remove(50);
   df.remove(50);
   df.remove(50);

   printf("# recs = %ld\n",df.numRecords());
}
