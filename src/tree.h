/* tree.h : intrusive general tree as binary trees */
/* provides also basic intrusive lists             */

# ifndef DOTPSTREE_TREE_H_
# define DOTPSTREE_TREE_H_

# include <stddef.h>

# define CONTAINER_OF(_type_, _field_, _ptr_) \
	  ((_type_*)((char*)(_ptr_) - offsetof(_type_, _field_)))

/* classic intrusive lists */
struct list
{
  struct list *next;
};

/* list_init_sentinel(head) initializes a new list with head as a sentinel */
/* head must be allocated separately, of course                            */
static inline
void list_init_sentinel(struct list *head)
{
  head->next = NULL;
}

/* list_push_front(head, elm) pushes elm in front of the sentinel head */
/* head and elm must be allocated list cells                           */
static inline
void list_push_front(struct list *head, struct list *elm)
{
  elm->next = head->next;
  head->next = elm;
}

/* list_pop_front(head) pops the first element of the list and returns it */
/* returns NULL if the list is empty                                      */
static inline
struct list* list_pop_front(struct list *head)
{
  struct list *elm = head->next;
  if (elm) {
    head->next = elm->next;
    elm->next = NULL;
  }
  return elm;
}

/* foreach_list(_head_, _list_iter_) iterating macro on list */
/* _list_iter_ must be the name of an existing variable of   */
/* type struct list*                                         */

# define foreach_list(_head_, _list_iter_)	\
	 for (_list_iter_ = _head_;		\
	      _list_iter_->next;		\
	      _list_iter_ = _list_iter_->next )

/* General trees implemented using binary representation */

struct tree
{
  struct tree *child, *sibling;
};

/* tree_attach_child(parent, child) attaches the node child to parent      */
/* both parent and child must be valid nodes, child must not have siblings */
static inline
void tree_attach_child(struct tree *parent, struct tree *child)
{
  child->sibling = parent->child;
  parent->child = child;
}

/* foreach_child(_node_, _chld_iter_) macro iterating on children of _node_ */
/* _chld_iter_ must the name of a variable of type struct tree*             */

# define foreach_child(_node_, _chld_iter_)	  \
	 for (_chld_iter_ = _node_->child;	  \
	      _chld_iter_;			  \
	      _chld_iter_ = _chld_iter_->sibling)

/* for example only */
static inline
size_t tree_number_of_children(struct tree *node)
{
  size_t count = 0;
  struct tree *child;
  foreach_child(node, child) {
    count += 1;
  }
  return count;
}

# endif /* DOTPSTREE_TREE_H_ */
