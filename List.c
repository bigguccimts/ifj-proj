#include "list.h"

#include <stdio.h>  
#include <stdlib.h> 

int error_flag;
int solved;

void List_Error()
{
  printf("*ERROR* The program has performed an illegal operation.\n");
  error_flag = TRUE;
}

void List_Init(List *list)
{
  // solved = FALSE; /* V případě řešení, smažte tento řádek! */
  list->firstElement = NULL;
  list->activeElement = NULL;
}

void List_Dispose(List *list)
{
  struct ListElement *tmpElement;
  while (list->firstElement != NULL)
  {
    tmpElement = list->firstElement;
    if (list->firstElement->nextElement != NULL)
    {
      list->firstElement = tmpElement->nextElement;
    }
    else
    {
      list->firstElement = NULL;
    }
    free(tmpElement);
  }

  list->activeElement = NULL;
  list->firstElement = NULL;
}

void List_First(List *list) { list->activeElement = list->firstElement; }

void List_InsertFirst(List *list, Data_type data)
{

  struct ListElement *tmpElement = malloc(sizeof(struct ListElement));

  // Case when list is empty
  if (tmpElement != NULL)
  {
    tmpElement->type = data;
    if (list->firstElement != NULL)
    {
      tmpElement->nextElement = list->firstElement;
    }
    else
    {
      tmpElement->nextElement = NULL;
    }
    list->firstElement = tmpElement;
  }
  else
  {
    List_Error();
  }
}

void List_DeleteFirst(List *list)
{
  struct ListElement *tmpElement ;
  if (list->firstElement != NULL)
  {
    tmpElement = list->firstElement;

    if (list->firstElement == list->activeElement)
    {
      list->activeElement = NULL;
    }
    
    if(tmpElement->nextElement != NULL){
    list->firstElement = tmpElement->nextElement;
    }else{
      list->firstElement = NULL;
    }
    
    free(tmpElement);
  }
}
//To Do 
void List_SetValue(List *list, Data_type data)
{
  if (list->activeElement != NULL)
  {
    list->activeElement->type = data;
  }
}

void List_Next(List *list)
{
  if (list->activeElement != NULL)
  {
    if (list->activeElement->nextElement == NULL)
    {
      list->activeElement = NULL;
    }
    else
    {
      list->activeElement = list->activeElement->nextElement;
    }
  }
}

int List_IsActive(List *list)
{
  return list->activeElement != NULL;
}

