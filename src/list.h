/* Linked lists */

# ifndef EPITA_IP_LIST_H_
# define EPITA_IP_LIST_H_

/* Simply linked list of integers */

struct list {
  struct list *next;
  int          data;
};

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
void list_init(struct list *list);

/*
 * list_is_empty(list)
 * returns true if the list is empty
 * remember, we have a sentinel thus a list always has at least one node,
 * the list is empty if this node doesn't have a next.
 */
int list_is_empty(struct list *list);

/*
 * list_len(list)
 * returns the length of the list (don't count the sentinel)
 */
size_t list_len(struct list *list);

/*
 * list_push_front(list, elm)
 * attach elm in front of list, that is after the sentinel.
 * Note that elm is already a node, you just have to attach it.
 */
void list_push_front(struct list *list, struct list *elm);

/*
 * list_pop_front(list)
 * Extract the first element (not the sentinel) of list.
 * This operation detaches the element from the list and returns it (caller is
 * responsible for freeing it.)
 * If the list is empty, the function returns NULL.
 */
struct list* list_pop_front(struct list *list);

/*
 * list_find(list, value)
 * search for the first node containing value and returns (without detaching it)
 * the corresponding list node. The function returns NULL if the value is not
 * present in the list.
 */
struct list* list_find(struct list *list, int value);

/*
 * list_is_sorted(list)
 * check whether the list is sorted in increasing order
 */
int list_is_sorted(struct list *list);

/*
 * list_insert(list, elm)
 * insert elm in the sorted list (keeping the list sorted)
 * Like list_push_front, elm is already a list node.
 */
void list_insert(struct list *list, struct list *elm);

/*
 * More algorithms
 */

/*
 * list_rev(list)
 * reverse the list using the same nodes (just move them) and the same sentinel.
 */
void list_rev(struct list *list);

/*
 * list_half_split(list, second)
 * split the list in half and put the second half in second
 * second is an empty list (just a sentinel)
 */
void list_half_split(struct list *list, struct list *second);

# endif /* EPITA_IP_LIST_H_ */
