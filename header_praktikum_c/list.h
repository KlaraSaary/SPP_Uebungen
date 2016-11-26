/* *********************************************************************
 * IMPORTANT: This header file declares the publicly visible functions
 *            for the task. Do not modify this header, but use it as
 *            it is. Modifications of the header result in a loss of
 *            points.
 *            Please implement the declared types and funtions in a
 *            source file with the same base name.
 * ********************************************************************/

#ifndef LIST_H
#define LIST_H

/**
 * This header file defines a linked list object and its accessor
 * functions.
 *
 * It provides opaque type definitions. Files that include this interface
 * will be able to use pointers to a LinkedList object or LinkedListNode
 * but will not be able to access its elements. The interaction with 
 * the LinkedList object can only happen through the functions declared 
 * in this header. This is a common method in a clean programming style.
 *
 * In the source file which contains the implementation of this interface
 * you need to define struct LinkedList and struct LinkedListNode. 
 */

/**
 * The LinkedList defines a structure that represents a double linked
 * list. A double linked list means that each data element points to
 * the next and the previous element in the list. Each list element
 * should be able to store a char* as data.
 */
typedef struct LinkedList LinkedList;

/**
 * The LinkedListNode defines one list element of the list of type 
 * LinkedList.
 */
typedef struct LinkedListNode LinkedListNode;

/**
 * This function creates a new double linked list, without any content.
 * It allocates the memory for the new list and initializes the fields
 * of the new LinkedList.
 */
LinkedList* 
LinkedList_create();

/**
 * This function creates a new LinkedListNode that stores the provided
 * pointer. The new LinkedListNode should be appended to the LinkedList.
 */
void
LinkedList_append( LinkedList* list, char* data );

/**
 * This function deletes the linked list. It should also delete
 * all data stored in the linked list and all the LinkedListNodes
 * of the list.
 */
void
LinkedList_delete( LinkedList* list );

/**
 * This function returns a pointer to the first LinkedListNode
 * in the list. If the list is empty, it returns NULL;
 */
LinkedListNode*
LinkedList_getFirst( LinkedList* list );

/**
 * This function returns a pointer to the last LinkedListNode
 * in the list. If the list is empty, it return NULL;
 */ 
LinkedListNode*
LinkedList_getLast( LinkedList* list );

/**
 * This functions returns a pointer to the next LinkedListNode
 * after node. If this node was the last, it returns NULL.
 */
LinkedListNode*
LinkedList_getNext( LinkedListNode* node );

/**
 * This function returns a pointer to the previous LinkedListNode
 * before node. If this node was the first, it returns NULL.
 */
LinkedListNode*
LinkedList_getPrevious( LinkedListNode* node );

/**
 * This function return the pointer to the data that is stored
 * in the specified node.
 */ 
char*
LinkedList_getData( LinkedListNode* node );

/**
 * Counts the number of nodes in the list.
 */
unsigned int
LinkedList_getSize( LinkedList* list );

/**
 * Returns the pointer to the data that is stored at the node
 * with position index. The first node has index 0. If no node
 * with the given index exist, the function returns NULL.
 */
char*
LinkedList_getDataAt( LinkedList* list, unsigned int index );

#endif
