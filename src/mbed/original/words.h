/* ==================================================================================================
ePower Socket Program: words.h                                           NOV/DEC 2009  Stuart Hadfield 
=====================================================================================================
File containing global keywords

Notes/todo:
    - put ALL constants/keywords here
    - need to add proper handling in all files for invalid/unassigned keyword...
=================================================================================================== */
#ifndef WORDS_H
#define WORDS_H

// keywords
#define FALSE 0
#define TRUE 1
#define CLEAR 0
#define SET 1
#define OFF 0
#define ON 1
#define UNDEFINED -1

// command words                // chars are 8bits
#define TURN_ON '2'        //   *** be very careful distinguishing btwn chars and ints
#define TURN_OFF '3'
#define IDENTIFY '4'
#define ACK '9'
 
#define INVALID 8                //-1
#define ALL_OUTLETS 9    

#endif /* WORDS_H */