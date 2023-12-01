/* doubly_linked_list.c
 *
 * Implement a sorted linked list of strings with operations Insert
 * in alphabetical order, Print, Member, Delete, Free_list.
 * The list nodes are doubly linked.
 *
 * Input:    Single character lower case letters to indicate operations,
 *           possibly followed by value needed by operation -- e.g. 'i'
 *           followed by "hello" to insert the string "hello" -- no
 *           double or single quotes.
 *
 * Output:   Results of operations.
 *
 * Compile:  gcc -g -Wall -o doubly_linked_list doubly_linked_list.c
 *
 * Run:      ./doubly_linked_list
 *
 */

/* You may not add any new header file */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Max 99 chars + '\0' */
#define STRING_MAX 100

typedef struct list_node_s node;
typedef struct list_s list;

struct list_node_s
{
    char *data;
    struct list_node_s *prev_p;
    struct list_node_s *next_p;
};

/* Pointers to the head and tail of the list */
struct list_s
{
    struct list_node_s *h_p;
    struct list_node_s *t_p;
};

void Insert(struct list_s *list_p, char string[]);
void Print(struct list_s *list_p);
int Member(struct list_s *list_p, char string[]);
void Delete(struct list_s *list_p, char string[]);
void Free_list(struct list_s *list_p);
char Get_command(void);
void Get_string(char string[]);
void Free_node(struct list_node_s *node_p);
struct list_node_s *Allocate_node(int size);
void Print_node(char title[], struct list_node_s *node_p);

/*-----------------------------------------------------------------*/
int main(void)
{
    char command;
    char string[STRING_MAX];
    struct list_s list;

    list.h_p = list.t_p = NULL;
    /* start with empty list */

    command = Get_command();
    while (command != 'q' && command != 'Q')
    {
        switch (command)
        {
        case 'i':
        case 'I':
            Get_string(string);
            Insert(&list, string);
            break;
        case 'p':
        case 'P':
            Print(&list);
            break;
        case 'm':
        case 'M':
            Get_string(string);
            if (Member(&list, string))
                printf("%s is in the list\n", string);
            else
                printf("%s is not in the list\n", string);
            break;
        case 'd':
        case 'D':
            Get_string(string);
            Delete(&list, string);
            break;
        case 'f':
        case 'F':
            Free_list(&list);
            break;
        default:
            printf("There is no %c command\n", command);
            printf("Please try again\n");
        }
        command = Get_command();
    }
    Free_list(&list);

    return 0;
}
/* main */

/*-----------------------------------------------------------------*/
/* Function:   Allocate_node
 * Purpose:    Allocate storage for a list node
 * Input arg:  size = number of chars needed in data member (including
 *                storage for the terminating null)
 * Return val: Pointer to the new node
 */
struct list_node_s *Allocate_node(int size)
{

    // Allocate new memory only if each step is successful
    node *newNode = (node *)calloc(1, sizeof(node));
    if (newNode != NULL)
    {
        newNode->data = (char *)calloc(size + 1, sizeof(char));
        if (newNode->data != NULL)
        {
            return newNode;
        }
    }
    perror("Error");
    return NULL;

} /* Allocate_node */

/*-----------------------------------------------------------------*/
/* Function:   Insert
 * Purpose:    Insert new node in correct alphabetical location in list
 * Input arg:  string = new string to be added to list
 * In/out arg: list_p = pointer to struct storing head and tail ptrs
 * Note:       If the string is already in the list, print a message
 *                and return, leaving list unchanged
 */
void Insert(struct list_s *list_p, char string[])
{

    if (Member(list_p, string))
    {
        printf("Duplicate node. %s is already present in the list. It was not added.", string);
        return;
    }
    // If not found create new node with string as data
    node *newNode = Allocate_node(strlen(string));

    if (newNode == NULL)
    {
        perror("Error");
        return;
    }
    strcpy(newNode->data, string);

    node *curr = list_p->t_p;

    // Empty list
    if (list_p->h_p == NULL)
    {
        list_p->h_p = newNode;
        list_p->t_p = newNode;

        // Testing purposes only
        // printf("%s inserted into empty list.\n", newNode->data);
        return;
    }

    // Only one element in list
    if (list_p->h_p == list_p->t_p)
    {
        // Insert at head
        if (strcmp(curr->data, newNode->data) > 0)
        {
            newNode->next_p = curr;
            list_p->h_p = newNode;
            curr->prev_p = newNode;
            // Testing only
            // printf("%s inserted at head of 1 element list.\n", newNode->data);
            return;
        }
        // Insert at tail
        else
        {
            newNode->prev_p = list_p->t_p;
            curr->next_p = newNode;
            list_p->t_p = newNode;
            //
            // printf("%s inserted at tail of 1 element list.", newNode->data);
            return;
        }
    }

    while (curr != NULL)
    {
        int test = strcmp(curr->data, newNode->data);
        if (test < 0)
        {
            // Inserting to last element
            if (curr == list_p->t_p)
            {
                curr->next_p = newNode;
                newNode->prev_p = curr;
                list_p->t_p = newNode;
            }
            else
            {
                curr->next_p->prev_p = newNode;
                newNode->next_p = curr->next_p;
                newNode->prev_p = curr;
                curr->next_p = newNode;
            }
            // Testing purposes only
            // printf("%s inserted into list.", newNode->data);

            return;
        }
        else if (curr == list_p->h_p)
        {
            curr->prev_p = newNode;
            newNode->next_p = curr;
            list_p->h_p = newNode;

            return;
        }

        curr = curr->prev_p;
    }
}; /* Insert */

/*-----------------------------------------------------------------*/
/* Function:   Print
 * Purpose:    Print the contents of the nodes in the list
 * Input arg:  list_p = pointers to first and last nodes in list
 *
 * Hint: The implementation of this function (below) shows how
 * to traverse a linked list.
 */
void Print(struct list_s *list_p)
{
    struct list_node_s *curr_p = list_p->h_p;

    printf("list = ");

    while (curr_p != NULL)
    {
        printf("%s ", curr_p->data);
        curr_p = curr_p->next_p;
    }
    printf("\n");
} /* Print */

/*-----------------------------------------------------------------*/
/* Function:   Member
 * Purpose:    Search list for string
 * Input args: string = string to search for
 *             list_p = pointers to first and last nodes in list
 * Return val: 1, if string is in the list, 0 otherwise
 */
int Member(struct list_s *list_p, char string[])
{
    // List not instantiated or empty return 0 (false)
    if (list_p == NULL || list_p->h_p == NULL)
    {
        return 0;
    }

    struct list_node_s *curr = list_p->h_p;

    while (curr != NULL)
    {
        // Node matches "string" return 1 (true)
        if (strcmp(curr->data, string) == 0)
        {
            return 1;
        }
        curr = curr->next_p;
    }

    // Not found return 0 (false)
    return 0;
} /* Member */

/*-----------------------------------------------------------------*/
/* Function:   Free_node
 * Purpose:    Free storage used by a node of the list
 * In/out arg: node_p = pointer to node to be freed
 */
void Free_node(struct list_node_s *node_p)
{
    free(node_p->data);
    free(node_p);
} /* Free_node */

/*-----------------------------------------------------------------*/
/* Function:   Delete
 * Purpose:    Delete node containing string.
 * Input arg:  string = string to be deleted
 * In/out arg  list_p = pointers to head and tail of list
 * Note:       If the string is in the list, it will be unique.  So
 *             at most one node will be deleted.  If the string isn't
 *             in the list, the function just prints a message and
 *             returns, leaving the list unchanged.
 */
void Delete(struct list_s *list_p, char string[])
{

    if (!Member(list_p, string))
    {
        printf("%s not found in list. Cannot delete it.\n", string);
        return;
    }
    node *curr = list_p->h_p;

    while (curr != NULL)
    {
        // Node found
        if (strcmp(curr->data, string) == 0)
        {
            // Only item in list
            if (curr == list_p->t_p && curr == list_p->h_p)
            {
                Free_node(curr);

                // Nullify head and tail
                list_p->h_p = list_p->t_p = NULL;

                // Testing only
                // printf("Node deleted from 1 element list. List is empty.\n");
                return;
            }
            // Remove first node in list
            if (curr == list_p->h_p)
            {
                list_p->h_p = curr->next_p;

                Free_node(curr);

                // Testing only
                // printf("Node deleted from head of list.\n");
                return;
            }
            // Remove last node in list
            if (curr == list_p->t_p)
            {
                list_p->t_p = curr->prev_p;

                Free_node(curr);

                // Testing only
                // printf("Node deleted from tail of list.\n");
                return;
            }

            curr->prev_p->next_p = curr->next_p;
            curr->next_p->prev_p = curr->prev_p;

            Free_node(curr);

            // Testing only
            // printf("Node deleted from within the list.\n");
            return;
        }

        // Advance curr
        curr = curr->next_p;
    }

} /* Delete */

/*-----------------------------------------------------------------*/
/* Function:   Free_list
 * Purpose:    Free storage used by list
 * In/out arg: list_p = pointers to head and tail of list
 */
void Free_list(struct list_s *list_p)
{

    node *curr = list_p->h_p;

    // Iterate and free all nodes individually
    while (curr != NULL)
    {
        node *prev = curr;
        curr = curr->next_p;

        Free_node(prev);
    }

    // Nullify list
    list_p->h_p = NULL;
    list_p->t_p = NULL;

    // Testing only
    // printf("Whole list is freed.\n");
} /* Free_list */

/*-----------------------------------------------------------------*/
/* Function:   Get_command
 * Purpose:    Find and return the next non-whitespace character in
 *             the input stream
 * Return val: The next non-whitespace character in stdin
 */
char Get_command(void)
{
    char c;

    printf("Please enter a command (i, d, m, p, f, q):  ");
    /* Put the space before the %c so scanf will skip white space */
    scanf(" %c", &c);
    return c;
} /* Get_command */

/*-----------------------------------------------------------------*/
/* Function:   Get_string
 * Purpose:    Read the next string in stdin (delimited by whitespace)
 * Out arg:    string = next string in stdin
 */
void Get_string(char string[])
{

    printf("Please enter a string:  ");
    scanf("%s", string);
} /* Get_string */

/*-----------------------------------------------------------------*/
/* Function:  Print_node
 * Purpose:   Print the data member in a node or NULL if the
 *            pointer is NULL
 * In args:   title:  name of the node
 *            node_p:  pointer to node
 */
void Print_node(char title[], struct list_node_s *node_p)
{
    printf("%s = ", title);
    if (node_p != NULL)
        printf("%s\n", node_p->data);
    else
        printf("NULL\n");
} /* Print_node */
