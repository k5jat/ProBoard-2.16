
/* ------------------------------------------------------------
 * Filename ............... Th_LList.Cpp
 *
 * General Purpose ........ Linked List class
 * ------------------------------------------------------------
 * First date ............. 14 sep 1993
 *
 * First in version ....... 2.00
 *
 * Written by ............. Alain Schellinck
 * ------------------------------------------------------------
 * Revisions:
 * ----------
 *
 *   Date   | By |                  Purpose                   |
 * ---------+----+--------------------------------------------+
 *          |    |                                            |
 *          |    |                                            |
 *          |    |                                            |
 */

/*-------------------------------------------------------------------------*/

#ifndef PEX
#endif

#define Uses_LinkedList
#include <TheHouse.Hpp>

/*-------------------------------------------------------------------------*/

/*
 * Routine   : LListEntry()
 * Purpose   : constructor
 * ------------------------------------------------------------------------
 * Parameters: pointer to a data object
 * Return    : None
 *
 */

LListEntry::LListEntry(ObjectP r)
{
   prev = next = NULL;
   rec = r;
}

/*
 * Routine   : ~LListEntry()
 * Purpose   : destructor
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : None
 *
 */

LListEntry::~LListEntry()
{
   if(rec != NULL)
      delete(rec);
}

/*-------------------------------------------------------------------------*/

/*
 * Routine   : LinkedList()
 * Purpose   : constructor
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : None
 *
 */

LinkedList::LinkedList()
{
   _first = _cur = _last = NULL;
   _listLen = _curNum = 0;
   _pastEnd = FALSE;
}

/*
 * Routine   : ~LinkedList()
 * Purpose   : destructor
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : None
 *
 */

LinkedList::~LinkedList()
{
   release();
}

/*
 * Routine   : release()
 * Purpose   : release all memory used by the linked list
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : None
 *
 */

void LinkedList::release()
{
   top();

   while(_cur != NULL)
   {
      LListEntryP entry = _cur->next;

      delete(_cur);
      _cur = entry;
   }

   _first = _last = _cur = NULL;
}

/*
 * Routine   : add()
 * Purpose   : add an entry to the linked list
 * ------------------------------------------------------------------------
 * Parameters: pointer to the object
 * Return    : TRUE if OK, FALSE if NOT
 *
 */

FlgT LinkedList::add(ObjectP r)
{
   FlgT ret_val = FALSE;

   LListEntryP entry = new LListEntryT(r);
   if(entry != NULL)
   {
      if(_last != NULL)
         _last->next = entry;

      entry->prev = _last;

      if(_first == NULL)
         _first = entry;

      _last = _cur = entry;

      _curNum = _listLen++;

      ret_val = TRUE;
   }

   return(ret_val);
}

/*
 * Routine   : remove()
 * Purpose   : remove the current entry
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : None
 *
 */

void LinkedList::remove()
{
   LListEntryP tmp = _cur;

   if(tmp->prev != NULL)               // restore the NEXT pointer
      tmp->prev->next = tmp->next;
   else
      _first = tmp->next;

   if(tmp->next != NULL)               // restore the PREV pointer
      tmp->next->prev = tmp->prev;
   else
      _last = tmp->prev;

   if(tmp->next != NULL)
      _cur = tmp->next;
   else
      if(tmp->prev != NULL)
      {
         _cur = tmp->prev;
         _curNum--;
      }
      else
      {
         _cur = NULL;
         _curNum = -1;
      }

   _listLen--;
   delete(tmp);
}

/*
 * Routine   : count()
 * Purpose   : count the number of entries in the linked list
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : the number of entries
 *
 */

ShrtT LinkedList::count()
{
   return(_listLen);
}

/*
 * Routine   : get()
 * Purpose   : get a pointer to a specified entry
 * ------------------------------------------------------------------------
 * Parameters: required entry (-1 = current entry)
 * Return    : pointer to the object or NULL if it doesn't exist
 *
 */

ObjectP LinkedList::get(ShrtT recno)
{
   ObjectP ret_val = NULL;
   FlgT status = TRUE;

   if(recno != -1)
      status = pos(recno);

   if(status == TRUE)
      ret_val = _cur->rec;

   return(ret_val);
}

/*
 * Routine   : pos()
 * Purpose   : position current pointer to a specified entry
 * ------------------------------------------------------------------------
 * Parameters: required entry
 * Return    : TRUE if OK, FALSE if NOT
 *
 */

FlgT LinkedList::pos(ShrtT recno)
{
   _pastEnd = FALSE;

   if(recno >= _listLen)
   {
      _pastEnd = TRUE;
      return(FALSE);
   }

   if(recno < 0)
      return(FALSE);

   FlgT backwards = FALSE;

   if(recno < _curNum)
   {
      if (recno > _curNum/2)
      {
         backwards = TRUE;
      }
      else
      {
         backwards = FALSE;
         _cur    = _first;
         _curNum = 0;
      }
   }
   else
   {
      if (recno < ( (_listLen - _curNum) / 2 ) + _curNum)
      {
         backwards = FALSE;
      }
      else
      {
         backwards = TRUE;
         _cur    = _last;
         _curNum = _listLen - 1;
      }
   }

   if(backwards == TRUE)
   {
      while(_curNum != recno)
      {
         _cur = _cur->prev;
         _curNum--;
      }
   }
   else
   {
      while(_curNum != recno)
      {
         _cur = _cur->next;
         _curNum++;
      }
   }

   return(TRUE);
}

/*
 * Routine   : top()
 * Purpose   : go to the first entry
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : None
 *
 */

void LinkedList::top()
{
   pos(0);
}

/*
 * Routine   : bottom()
 * Purpose   : go to the last entry
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : None
 *
 */

void LinkedList::bottom()
{
   pos(_listLen - 1);
}

/*
 * Routine   : next()
 * Purpose   : go to the next entry
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : TRUE if OK, FALSE if NOT
 *
 */

FlgT LinkedList::next()
{
   return(pos(_curNum + 1));
}

/*
 * Routine   : prev()
 * Purpose   : go to the previous record
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : TRUE if OK, FALSE if NOT
 *
 */

FlgT LinkedList::prev()
{
   return(pos(_curNum - 1));
}

/*
 * Routine   : onTop()
 * Purpose   : check if we are on top of the linked list
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : TRUE if we are on TOP, FALSE if NOT
 *
 */

FlgT LinkedList::onTop()
{
   FlgT ret_val = FALSE;

   if(_cur == _first)
      ret_val = TRUE;

   return(ret_val);
}

/*
 * Routine   : onBottom()
 * Purpose   : check if we are on the bottom of the linked list
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : TRUE if we are on the BOTTOM, FALSE if NOT
 *
 */

FlgT LinkedList::onBottom()
{
   FlgT ret_val = FALSE;

   if(_cur == _last)
      ret_val = TRUE;

   return(ret_val);
}

/*
 * Routine   : pastEnd()
 * Purpose   : check if we are past the end of the linked list
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : TRUE if we are past the END, FALSE if NOT
 *
 */

FlgT LinkedList::pastEnd()
{
   return(_pastEnd);
}

/*
 * Routine   : sort()
 * Purpose   : sort the linked list
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : None
 *
 */

void LinkedList::sort()
{
   if(count() > 1)
   {
      LListEntryP lle1;
      LListEntryP lle2;

      for(lle1 = _first; lle1->next != NULL; lle1 = lle1->next)
      {
         for(lle2 = lle1->next; lle2 != NULL; lle2 = lle2->next)
         {
            ObjectP e1 = lle1->rec;
            ObjectP e2 = lle2->rec;
            if(compare(e1, e2) > 0)
            {
               lle1->rec = e2;
               lle2->rec = e1;
            }
         }
      }
   }
}

/*---------------------------] END OF THE CODE [---------------------------*/
