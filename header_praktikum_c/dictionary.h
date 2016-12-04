/* *********************************************************************
 * IMPORTANT: This header file declares the publicly visible functions
 *            for the task. Do not modify this header, but use it as
 *            it is. Modifications of the header result in a loss of
 *            points.
 *            Please implement the declared types and funtions in a
 *            source file with the same base name.
 * ********************************************************************/

#ifndef DICTIONARY_H
#define DICTIONARY_H

typedef struct Dictionary Dictionary;

/**
 * Creates a new Dictionary object.
 */
Dictionary*
Dictionary_create();

/**
 * Deletes a Dictionary object.
 */
void
Dictionary_delete( Dictionary* dict );

/**
 * Inserts a word into the dictionary.
 */
void
Dictionary_insert( Dictionary* dict, const char* word );

/**
 * Checks whether a word is in the Dictionary.
 * Returns non-zero, if word is in the dictionary and
 * zero otherwise.
 */ 
int
Dictionary_isIn( const Dictionary* dict, const char* word );

/**
 * Prints all words in the dictionary.
 */
void
Dictionary_print( const Dictionary* dict );

/**
 * Adds all words from source to destination
 */
void
Dictionary_merge( Dictionary* destination, const Dictionary* source );

#endif
