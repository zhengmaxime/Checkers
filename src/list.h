/* Linked lists */

// Data structures for sequences and list of sequences

# ifndef LIST_H_
# define LIST_H_

# include <unistd.h>

struct list {
  struct list *next;
  int data;
  int x,y;
};

struct coords
{
  int x;
  int y;
  int data;
};

/*
 * One sequence
 */
struct move_seq
{
  struct move_seq *next;
  struct coords orig;
  struct coords capt;
  struct coords dest;
  struct coords captures[20]; // only in sentinel
  int nb_captures; // only in sentinel
  int can_be_played; // only in sentinel,
                     // used for playing a sequence one jump at the time
};

/*
 * List of sequences
 */
struct moves
{
  struct moves *next;
  struct move_seq *seq;
  struct coords crowned; // for undo, destination of the new king
};

/*
 * Standard operations
 * We're working with sentinels and external allocation
 * This means that our lists always have a fake head (the sentinel)
 * and all allocations (and free) are performed by the caller, not the operation
 */

/*
 * initialise the sentinel node list
 */
void seq_init(struct move_seq *list);
void moves_init(struct moves *list, struct move_seq *seq);

/*
 * list_is_empty(list)
 * returns true if the list is empty
 * remember, we have a sentinel thus a list always has at least one node,
 * the list is empty if this node doesn't have a next.
 */
int list_is_empty(struct moves *list);

/*
 * list_len(list)
 * returns the length of the list (don't count the sentinel)
 */
size_t list_len(struct moves *list);

/*
 * print all the mandatory moves
 */
void list_print(struct moves *list);

// get seq[i]
struct move_seq *seq_get_elm(struct move_seq *seq, int i);

/*
 * list_push_front(list, elm)
 * attach elm in front of list, that is after the sentinel.
 * Note that elm is already a node, you just have to attach it.
 */
void list_push_front(struct list *list, struct list *elm);
void seq_push_front(struct move_seq *list, struct move_seq *elm);
void moves_push_front(struct moves *list, struct moves *elm);

/*
 * list_pop_front(list)
 * Extract the first element (not the sentinel) of list.
 * This operation detaches the element from the list and returns it (caller is
 * responsible for freeing it.)
 * If the list is empty, the function returns NULL.
 */
struct moves *moves_pop_front(struct moves *list);

/*
 * deep copy a list
 */
struct move_seq *copy(struct move_seq *move_seq);

/*
 * insert move_seq in moves list
 * moves list is sorted in descending order of captures number
 * return 1 if success, else 0
 */
int moves_insert(struct moves *moves, struct move_seq *move_seq);

/*
 * list_rev(list)
 * reverse the list using the same nodes (just move them) and the same sentinel.
 */
void list_rev(struct move_seq *list);

void free_seq(struct move_seq *l);
void free_moves(struct moves *l);

# endif /* LIST_H_ */
