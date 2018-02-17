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
  puts("  End of sequence, call moves_insert");
  int ll = list_len(moves);
  printf("  length of moves list before insert = %d\n", ll);

  if (moves->next == NULL) // empty list
  {
    puts("  Moves list is currently empty, INSERT (push front)");
    printf("  len move just found = %d\n", move_seq->nb_captures);
    list_rev(move_seq); // because push_front was used
    struct moves *elm = malloc(sizeof (struct moves));
    moves_init(elm, move_seq);
    moves_push_front(moves, elm);
    return 1;
  }

  for (; moves->next; moves = moves->next)
  {
    printf("  len move just found = %d\n"
           "  len move already in list = %d\n"
           "  if %d >= %d it should INSERT (bug there?)\n",
           move_seq->nb_captures, (moves->next)->seq->nb_captures,
           move_seq->nb_captures, (moves->next)->seq->nb_captures);

    if (move_seq->nb_captures >= (moves->next)->seq->nb_captures)
    {
      list_rev(move_seq); // because push_front was used
      struct moves *elm1 = malloc(sizeof (struct moves));
      moves_init(elm1, move_seq);

      elm1->next = moves->next;
      moves->next = elm1;
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

/*
 * list_pop_front(list)
 * Extract the first element (not the sentinel) of list.
 * This operation detaches the element from the list and returns it (caller is
 * responsible for freeing it.)
 * If the list is empty, the function returns NULL.
 */
/*
struct list* list_pop_front(struct list *list)
{
	if (list_is_empty(list) == 1)
		return NULL;

	struct list *n = list->next;
	list->next = (list->next)->next;
	return n;
}
*/
/*
 * list_find(list, value)
 * search for the first node containing value and returns (without detaching it)
 * the corresponding list node. The function returns NULL if the value is not
 * present in the list.
 */
/*struct list* list_find(struct list *list, int value)
{
	if (list_is_empty(list))
		return NULL;

	list = list->next;
	for (; list && list->data != value; list = list->next);
	return list;
}*/

/*
 * list_is_sorted(list)
 * check whether the list is sorted in increasing order
 */
/*int list_is_sorted(struct list *list)
{
	list = list->next;
	if (list != NULL)
	{
		int x = list->data;
		list = list->next;
		for (; list; list = list->next)
		{
			if (x > list->data)
				return 0;
		}
	}
	return 1;
}*/

/*
 * list_insert(list, elm)
 * insert elm in the sorted list (keeping the list sorted)
 * Like list_push_front, elm is already a list node.
 */
/*void list_insert(struct list *list, struct list *elm){

	if (elm != NULL)
	{
		if (list_is_empty(list))
		{
			list_push_front(list, elm);
			return;
		}
		for (; list->next; list = list->next)
		{
			if (elm->data < (list->next)->data)
			{
				list_push_front(list, elm);
				return;
			}
		}
		list->next = elm;
		elm->next = NULL;
	}
}*/

static void freeMemoryList(struct list *l)
{
	struct list *tmp = NULL;
	while (l != NULL)
	{
		tmp = l->next;
		free(l);
		l = tmp;
	}
}
/*
 * More algorithms


 * list_rev(list)
 * reverse the list using the same nodes (just move them) and the same sentinel.
void list_rev(struct list *list)
{
	struct list *rev = malloc(sizeof (struct list));
	list_init(rev);
	struct list *l = list;
	list = list->next;
	for (; list; list = list->next)
	{
			struct list *tmp = malloc(sizeof (struct list));
			tmp->data = list->data;
			tmp->next = NULL;
			list_push_front(rev, tmp);
	}
	struct list *sv_rev = rev;
	for (; l ; l = l->next, rev = rev->next)
		l->data = rev->data;
	freeMemoryList(sv_rev);
}

 * list_half_split(list, second)
 * split the list in half and put the second half in second
 * second is an empty list (just a sentinel)

void list_half_split(struct list *list, struct list *second)
{
	size_t n = list_len(list) / 2;
	for (; list && n > 0; list = list->next, n--);
	struct list *k = list->next;
	list->next = NULL;
	struct list *sentinel = malloc(sizeof (struct list));
	struct list *sv_sentinel = sentinel;
	sentinel->next = k;
	sentinel->data = 0;
	list_rev(sentinel);
	sentinel = sentinel->next;
	for (; sentinel; sentinel = sentinel->next)
	{
			struct list *tmp = malloc(sizeof (struct list));
			tmp->data = sentinel->data;
			tmp->next = NULL;
			list_push_front(second, tmp);
	}
	freeMemoryList(sv_sentinel);
}*/
