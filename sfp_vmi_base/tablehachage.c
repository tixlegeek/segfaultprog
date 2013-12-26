#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "tablehachage.h"

typedef struct
{
   char *key;
   void *data;
} cell_s;

struct hach
{
   cell_s *p_cell;
   int size;
};

hach_s *hach_new (void)
{
   hach_s *p_table = NULL;

   p_table = malloc (sizeof (*p_table));
   if (p_table != NULL)
   {
      p_table->p_cell = NULL;
      p_table->size = 0;
   }
   else
   {
      fprintf (stderr, "Memoire insufisante\n");
      exit (EXIT_FAILURE);
   }
   return (p_table);
}

void hach_add (hach_s * p_table, const char *key, void *data)
{
   if (p_table != NULL && key != NULL)
   {
      int i = 0;
      cell_s *p_cell = NULL;
      void *tmp;

      while (i < p_table->size)
      {
         if (strcmp (p_table->p_cell[i].key, key) == 0)
         {
            p_cell = &p_table->p_cell[i];
            break;
         }
         i++;
      }
      if (p_cell == NULL)
      {
         p_table->size++;
         tmp =
            realloc (p_table->p_cell,
                     sizeof (*p_table->p_cell) * p_table->size);
         if (tmp != NULL)
         {
            p_table->p_cell = tmp;
            p_cell = &p_table->p_cell[p_table->size - 1];
            p_cell->key = NULL;
         }
         else
         {
            fprintf (stderr, "Memoire insufisante\n");
            exit (EXIT_FAILURE);
         }
      }
      tmp =
         realloc (p_cell->key,
                  sizeof (*p_cell->key) * (strlen (key) + 1));
      if (tmp != NULL)
      {
         p_cell->key = tmp;
         strcpy (p_cell->key, key);
         p_cell->data = data;
      }
      else
      {
         fprintf (stderr, "Memoire insufisante\n");
         exit (EXIT_FAILURE);
      }
   }
   else
   {
      fprintf (stderr, "Memoire insufisante\n");
      exit (EXIT_FAILURE);
   }
   return;
}

void *hach_search (hach_s * p_table, const char *key)
{
   void *data = NULL;

   if (p_table != NULL && key != NULL)
   {
      int i = 0;

      while (i < p_table->size)
      {
         if (strcmp (p_table->p_cell[i].key, key) == 0)
         {
            data = p_table->p_cell[i].data;
            break;
         }
         i++;
      }
   }
   return (data);
}

void hach_delete (hach_s ** p_table)
{
   int i;

   for (i = 0; i < (*p_table)->size; i++)
      free ((*p_table)->p_cell[i].key);
   free ((*p_table)->p_cell);
   free (*p_table);
   *p_table = NULL;
   return;
}
