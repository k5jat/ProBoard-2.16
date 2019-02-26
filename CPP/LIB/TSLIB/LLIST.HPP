
template <class T>
class LinkedListObject
   {
      friend class LinkedList<T>;

      T data;

      LinkedListObject *next,
                       *prev;
   public:
      LinkedListObject(T& d) : data(d)
         {
            next = prev = NULL;
         }
   };

template <class T>
class LinkedList
   {
      LinkedListObject<T> *last    ,
                          *current ;

   public:
      LinkedList();
     ~LinkedList()
         {
            kill();
         }

      void kill();

      void  add(T& , int append = 1);

      void  append(T& data) { add(data,1); }
      void  insert(T& data) { add(data,0); }
      void  remove();

      int   empty()
         {
            return (last == NULL);
         }

      T&    get();
      T&    get(int recno);

      void  rewind();
      int   past_end();
      int   eol()
         {
            return past_end();
         }

      int   count();

      void  operator++();
      void  operator++(int);
      void  operator--();
      void  operator--(int);

      T&    operator[](int i)
         {
            return get(i);
         }

      void  sort(int (*compare)(T&,T&));
   };

template <class T>
LinkedList<T>::LinkedList()
{
   current = last = NULL;
}


template <class T>
void
LinkedList<T>::kill()
{
   if(last == NULL) return;

   LinkedListObject<T> *p = last;

   do
   {
      LinkedListObject<T> *next = p->next;

      delete p;

      p = next;
   }
   while(p != last);

   last = NULL;
}

template <class T>
int
LinkedList<T>::count()
{
   int i = 0;

   if(last == NULL) return 0;

   LinkedListObject<T> *p = last;

   do
   {
      i++;

      p = p->next;
   }
   while(p != last);

   return i;
}


template <class T>
void
LinkedList<T>::add(T& data , int append )
{
   if(last == NULL)
   {
      last = current = new LinkedListObject<T>(data);

      last->next = last->prev = last;
   }
   else
   {
      LinkedListObject<T> *p = new LinkedListObject<T>(data);

      p->next    = last->next;
      p->prev    = last;

      last->next->prev = p;
      last->next       = p;

      if(append) last = p;
   }
}

template <class T>
void
LinkedList<T>::remove()
{
   if(current == NULL) return;

   if(last == last->next)
   {
      delete last;

      last = current = NULL;

      return;
   }

   LinkedListObject<T> *new_current = NULL;

   if(current == last)
   {
      last        = current->prev;
   }
   else
   {
      new_current = current->next;
   }

   current->next->prev = current->prev;
   current->prev->next = current->next;

   delete current;

   current = new_current;
}


template <class T>
void
LinkedList<T>::rewind()
{
   current = (last != NULL) ? last->next : NULL;
}

template <class T>
T&
LinkedList<T>::get()
{
   static T dummy;

   return (current != NULL) ? current->data : dummy;
}

template <class T>
T&
LinkedList<T>::get(int recno)
{
   static T dummy;

   if(last == NULL)
      return dummy;

   LinkedListObject<T> *p = last->next;

   for(int i=0 ; i<recno ; i++)
   {
      p = p->next;

      if(p == last->next)
         break;
   }

   current = p;

   if(i < recno)
      return dummy;
   else
      return p->data;
}


template <class T>
void
LinkedList<T>::operator++()
{
    current = current->next;

    if(current == last->next) current = NULL;
}

template <class T>
void
LinkedList<T>::operator++(int)
{
    current = current->next;

    if(current == last->next) current = NULL;
}

template <class T>
void
LinkedList<T>::operator--()
{
    current = current->prev;

    if(current == last) current = NULL;
}

template <class T>
void
LinkedList<T>::operator--(int)
{
    current = current->prev;

    if(current == last) current = NULL;
}

template <class T>
int
LinkedList<T>::past_end()
{
   return (current == NULL);
}

template <class T>
void
LinkedList<T>::sort(int (*compare)(T&,T&))
{
   if(count() < 2)
      return;

   LinkedListObject<T> *lle1,
                       *lle2;

   for(lle1 = last->next; lle1 != last ; lle1 = lle1->next)
   {
      for(lle2 = lle1->next; lle2 != last->next ; lle2 = lle2->next)
      {
         if(compare(lle1->data,lle2->data) > 0)
         {
            T tmp(lle1->data);

            lle1->data = lle2->data;
            lle2->data = tmp;
         }
      }
   }
}
