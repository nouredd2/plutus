#ifndef __FACTORY_H
#define __FACTORY_H

#include <puzzle/puzzle.h>
#include <puzzle/optpuzzle.h>

/* Factory create functions for ease of use */

/* creating challanges and subpuzzles */
SHA256SubPuzzle 	*createSubPuzzle ();
SHA256Challenge 	*createChallenge ();

/* create solutions and subsolutions */
SHA256SubSolution 	*createSubSolution ();
SHA256Solution 		*createSolution ();

/* create optimized challenges */
SHA256OptChallenge * 	create_optchallenge ();

/* create optimzed solutions and subsolutions */
SHA256OptSolution 		*create_optsolution ();
SHA256OptSubSolution 	*create_optsubsolution ();

#endif /* factory.h */
