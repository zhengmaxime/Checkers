#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "board.h"

/* Linked lists */

/*
 * Standard operations
 * We're working with sentinels and external allocation
 * This means that our lists always have a fake head (the sentinel)
 * and all allocations (and free) are performed by the caller, not the operation
 */

/*
 * initialise the sentinel node list
 */

void seq_init(struct move_seq *list)
{
  list->next = NULL;
  for (int i = 0; i < 20; ++i)
  {
    struct coords c;
    c.x = -1;
    c.y = -1;
    list->captures[i] = c;
  }
  list->nb_captures = 0;
}

void moves_init(struct moves *list, struct move_seq *seq)
{
  list->next = NULL;
  list->seq = seq;
}

/*
 * list_is_empty(list)
 * returns true if the list is empty
 * remember, we have a sentinel thus a list always has at least one node,-
 * the list is empty if this node doesn't have a next.
 */
int list_is_empty(struct moves *list)
{
  return (list->next == NULL) ? 1 : 0;
}

/*
 * list_len(list)
 * returns the length of the list (don't count the sentinel)
 */

size_t list_len(struct moves *list)
{
  size_t i = 0;
  if (list_is_empty(list))
  return i;
  list = list->next;
  for (; list; list = list->next)
  ++i;
  return i;
}

void list_print(struct moves *list)
{
  int i;
  list = list->next;
  for (i = 1; list; list = list->next, ++i)
  {
    printf("(%d)", i);
    struct move_seq *seq = list->seq->next;
    for (; seq; seq = seq->next)
      printf(" -> (%d, %d) to (%d, %d)",
             seq->orig.x, seq->orig.y,
             seq->dest.x, seq->dest.y);
    puts("");
  }
}
/*
 * list_push_front(list, elm)
 * attach elm in front of list, that is after the sentinel.
 * Note that elm is already a node, you just have to attach it.
 */
void list_push_front(struct list *list, struct list *elm)
{
	elm->next = list->next;
	list->next = elm;
}

void seq_push_front(struct move_seq *list, struct move_seq *elm)
{
	elm->next = list->next;
	list->next = elm;
}

void moves_push_front(struct moves *list, struct moves *elm)
{
	elm->next = list->next;
	list->next = elm;
}

/*
 * insert move_seq in moves list
 * moves list is sorted in descending order of captures number
 * return 1 if success, else 0
 */
int moves_insert(struct moves *moves, struct move_seq *move_seq)
{
  if (moves->next == NULL) // empty list
  {
    list_rev(move_seq); // because push_front was used
    struct moves *elm = malloc(sizeof (struct moves));
    moves_init(elm, move_seq);
    moves_push_front(moves, elm);
    return 1;
  }

  for (; moves->next; moves = moves->next)
  {
    if (move_seq->nb_captures >= (moves->next)->seq->nb_captures)
    {
      list_rev(move_seq); // because push_front was used
      struct moves *elm1 = malloc(sizeof (struct moves));
      moves_init(elm1, move_seq);

      elm1->next = moves->next;
      moves->next = elm1;

      // keep the best sequences and destroy the rest
      moves = moves->next;
      for (; moves->next; moves = moves->next)
      {
        if (moves->next->seq->nb_captures < elm1->seq->nb_captures)
        {
          moves->next = NULL;
          break;
        }
      }
      return 1;
    }
  }

  return 0;
}

/*
 * deep copy a list
 */
struct move_seq *copy(struct move_seq *move_seq)
{
  if (move_seq == NULL)
    return NULL;

  struct move_seq *res = malloc(sizeof (struct move_seq));
  seq_init(res);

  for (int i = 0; i < 20; ++i)
  {
    res->captures[i] = move_seq->captures[i];
  }
  res->nb_captures = move_seq->nb_captures;
  move_seq = move_seq->next;
  struct move_seq *seq2 = res;
  while (move_seq)
  {
    struct move_seq *node2 = malloc(sizeof (struct move_seq));

    node2->orig = move_seq->orig;
    node2->capt = move_seq->capt;
    node2->dest = move_seq->dest;
    node2->nb_captures = move_seq->nb_captures;
    for (int i = 0; i < 20; ++i)
    {
      node2->captures[i] = move_seq->captures[i];
    }

    seq2->next = node2;
    seq2 = seq2->next;
    move_seq = move_seq->next;
  }

  seq2->next = NULL;
  return res;
}

/*
 * list_rev(list)
 * reverse the list using the same nodes (just move them) and the same sentinel.
*/
 void list_rev(struct move_seq *list)
{
  struct move_seq *cur = list->next; // sentinel
  struct move_seq *prev = NULL; // reverse

  while (cur)
  {
    struct move_seq *tmp;

    tmp = cur->next;
    cur->next = prev;
    prev = cur;
    cur = tmp;
  }

  list->next = prev;
}

void free_seq(struct move_seq *l)
{
	struct move_seq *tmp = NULL;
	while (l != NULL)
	{
		tmp = l->next;
		free(l);
		l = tmp;
	}
}

void free_moves(struct moves *l)
{
	struct moves *tmp = NULL;
	while (l != NULL)
	{
    free_seq(l->seq);
		tmp = l->next;
		free(l);
		l = tmp;
	}
}
