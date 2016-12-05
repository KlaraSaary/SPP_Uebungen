/* *********************************************************************
 * IMPORTANT: This header file declares the publicly visible functions
 *            for the task. Do not modify this header, but use it as
 *            it is. Modifications of the header result in a loss of
 *            points.
 *            Please implement the declared types and funtions in a
 *            source file with the same base name.
 * ********************************************************************/

#ifndef PARSER_H
#define PARSER_H

/**
 * Parser is an opaque type that contains the status and data 
 * associated with the parsing of a text. The parsed text must not
 * be modified as long as the Parser object exists.
 * You basically need to store a pointer to the text in the Parser
 * and infromation about the current position.
 */
typedef struct Parser Parser;

/**
 * Creates a new Parser object. You need to pass a pointer to 
 * the text that you want to parse. The text need to be 
 * NULL-terminated.
 */
Parser*
Parser_create( const char* text );

/**
 * This functions returns the next word in the text. 
 * With nextWord, you need to provide a buffer of bufferLength bytes
 * into which the parser can copy the next word. The word written in
 * nextWord is terminated with a '\0'-character.
 *
 * If the buffer is too short to store the whole next word including
 * a terminating '\0'-character, it will split it and return the 
 * bufferLength-1 long parts of the word.
 *
 * If the end of the text is reached, the return value of the function
 * is zero and nextWord is empty (only a '\0'-character). Otherwise 
 * the return value is non-zero. 
 *
 * The parser assumes that a word consists only of the letters A-Z. 
 * All other characters (e.g., full stops, comma, brackets, numbers, 
 * whitespaces) including special letters like Ä, Ö, Ü, ß, é, are 
 * considered to be not part of a word and can separate words. 
 * 
 * Furthermore, the parser converts all letters to upper case.
 */
int
Parser_getNextWord( Parser* parser, 
		    char** nextWord,
		    unsigned int bufferLength );

/**
 * deletes a Parser object.
 */
void
Parser_delete( Parser* parser );

#endif
