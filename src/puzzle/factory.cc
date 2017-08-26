#include "puzzle/factory.h"

/* createSubPuzzle */
SHA256SubPuzzle *createSubPuzzle ()
{
	SHA256SubPuzzle *subpuzzle = 
		(SHA256SubPuzzle *) malloc ( sizeof (SHA256SubPuzzle) );

	return subpuzzle;
}

/* createChallange */
SHA256Challenge	*createChallenge ()
{
	SHA256Challenge *challenge = 
		(SHA256Challenge *) malloc ( sizeof (SHA256Challenge) );
    
    return challenge;
}

/* createSubSolution */
SHA256SubSolution *createSubSolution ()
{
	SHA256SubSolution *subsolution =
		(SHA256SubSolution *) malloc ( sizeof (SHA256SubSolution) );

    return subsolution;
}

/* createSolution */
SHA256Solution	*createSolution ()
{
	SHA256Solution *solution = 
		(SHA256Solution *) malloc ( sizeof (SHA256Solution) );

    return solution;
}

/* create_optchallenge */
SHA256OptChallenge *create_optchallenge ()
{
	SHA256OptChallenge *challenge = 
		(SHA256OptChallenge *) malloc ( sizeof (SHA256OptChallenge) );

	return challenge;
}

/* create_optsolution */
SHA256OptSolution *create_optsolution ()
{
	SHA256OptSolution *solution = 
		(SHA256OptSolution *) malloc ( sizeof (SHA256OptSolution) );

	return solution;
}

SHA256OptSubSolution *create_optsubsolution ()
{
	SHA256OptSubSolution *subsol = 
		(SHA256OptSubSolution *) malloc ( sizeof (SHA256OptSubSolution) );
}
