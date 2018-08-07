#ifndef _EXCEPTION_HANDLER_H_
#define _EXCEPTION_HANDLER_H_

#include <iostream>
#include <string>
#include <exception>
using namespace std;

namespace exceptionh {
	class ExceptionHandler : public exception
	{
	public:
		/* Constructor (C strings).
		*		@param message C-style string error message.
		*       The string contents are copied upon construction.
		*       Hence, responsibility for deleting the char* lies
		*       with the caller.
		*/
		explicit ExceptionHandler(const char* message);

		/* Constructor (C++ STL strings).
		*		@param message The error message.
		*/
		explicit ExceptionHandler(const string& message);

		/* Destructor.
		*		Virtual to allow for subclassing.
		*/
		virtual ~ExceptionHandler() throw ();

		/* Returns a pointer to the (constant) error description.
		*		@return A pointer to a const char*. The underlying memory
		*       is in posession of the Exception object. Callers must
		*       not attempt to free the memory.
		*/
		virtual const char* what() const throw ();

	protected:
		string msg_;
	};
}
#endif
