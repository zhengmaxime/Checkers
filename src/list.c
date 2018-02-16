#include <stdio.h>
#include <stdlib.h>
#include "list.h"
# include "board.h"

/* Linked lists */

/* Simply linked list of integers */

/* struct list {
  struct list *next;
  int          data;
};*/

/*
 * Standard operations
 * We're working with sentinels and external allocation
 * This means that our lists always have a fake head (the sentinel)
 * and all allocations (and free) are performed by the caller, not the operation
 */

/*
 * list_init(list)
 * initialise the sentinel node list
 */
void list_init(struct list *list)
{
	list->next = NULL;
	list->x = 0;
    list->y = 0;
}

void seq_init(struct move_seq *list)
{
	list->next = NULL;
    list->captures = {0};
    list->nb_captures = 0;
}

void move_init(struct move_list *list)
{
	list->next = NULL;
}
/*
 * list_is_empty(list)
 * returns true if the list is empty
 * remember, we have a sentinel thus a list always has at least one node,-
 * the list is empty if this node doesn't have a next.
 */
int list_is_empty(struct list *list)
{
	return (list->next == NULL) ? 1 : 0;
}

/*
 * list_len(list)
 * returns the length of the list (don't count the sentinel)
 */


//fixmeeeee

/*size_t list_len(struct list *list)
{
	size_t i = 0;
	if (list_is_empty(list))
		return i;
	list = list->next;
	for (; list; list = list->next)
		++i;
	return i;
}*/

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
	elm->next = move_seq->next;
	move_seq->next = elm;
}

void move_push_front(struct move_list *list, struct move_list *elm)
{
	elm->next = move_list->next;
	move_list->next = elm;
}
/*
 * list_pop_front(list)
 * Extract the first element (not the sentinel) of list.
 * This operation detaches the element from the list and returns it (caller is
 * responsible for freeing it.)
 * If the list is empty, the function returns NULL.
 */
struct list* list_pop_front(struct list *list)
{
	if (list_is_empty(list) == 1)
		return NULL;

	struct list *n = list->next;
	list->next = (list->next)->next;
	return n;
}

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
