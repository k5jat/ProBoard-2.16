template <class dataT,class keyT>
class CachedObject
   {
      friend class Cache<dataT,keyT>;

      dataT data;
      keyT  key;

      CachedObject *next , *prev;
   public:

      CachedObject(dataT& d,keyT& k) : data(d) , key(k)
         {
            next = prev = NULL;
         }
   };

template <class dataT,class keyT>
class Cache
   {
      long max,num;

      CachedObject<dataT,keyT> *last;

      void promote(CachedObject<dataT,keyT> *);
   public:
      Cache(long n = 2)
         {
            max  = (n<2) ? 2 : n;
            num  = 0;
            last = NULL;
         }
      ~Cache() 
         { 
            purge(); 
         }
      
      bool find(dataT& , keyT);
      bool update(dataT& , keyT);

      void add(dataT , keyT);
      void purge();
   };

template <class dataT,class keyT>
void
Cache<dataT,keyT>::add( dataT data , keyT key )
{
   if(!num)
   {
      last = new CachedObject<dataT,keyT>(data,key);
      last->next = last->prev = last;
      num++;
      return;
   }

   CachedObject<dataT,keyT> *next , *prev;

   if(num == max)
   {
      next = last->next;
      prev = last->prev;

      delete last;

      prev->next = next;
      next->prev = prev;

      last = prev;

      num--;
   }

   next = last->next;

   last->next       = new CachedObject<dataT,keyT>(data,key);
   last->next->prev = last;
   last->next->next = next;
   next->prev       = last->next;

   num++;
}

template <class dataT,class keyT>
void
Cache<dataT,keyT>::promote(CachedObject<dataT,keyT> *obj)
{
   if(obj == last)
   {
      last = obj->prev;
      return;
   }

   CachedObject<dataT,keyT> *next = obj->next,
                            *prev = obj->prev;

   prev->next = next;        // Object removed from
   next->prev = prev;        // list

   next       = last->next;

   obj->prev  = last;
   obj->next  = next;

   last->next = obj;

   next->prev = obj;
}

template <class dataT,class keyT>
bool
Cache<dataT,keyT>::find(dataT& data , keyT key)
{
   if(!num) return FALSE;

   CachedObject<dataT,keyT> *p = last->next;

   do
   {
      if(p->key == key)
      {
         data = p->data;
         promote(p);

         return TRUE;
      }

      p = p->next;
   }
   
   while(p != last->next);

   return FALSE;
}

template <class dataT,class keyT>
bool
Cache<dataT,keyT>::update(dataT data , keyT key)
{
   if(!num) return FALSE;

   CachedObject<dataT,keyT> *p = last->next;

   do
   {
      if(p->key == key)
      {
         p->data = data;

         promote(p);

         return TRUE;
      }

      p = p->next;
   }
   while(p != last->next);

   return FALSE;
}


template <class dataT,class keyT>
void
Cache<dataT,keyT>::purge()
{
   if(!num) return;

   CachedObject<dataT,keyT> *p = last;

   do
   {
      CachedObject<dataT,keyT> *next = p->next;

      delete p;

      p = next;
   }
   while(p != last);

   num  = 0;
   last = NULL;
}
