///////////////////////////////////////////////////////////////////////////////
//
//  Original System: Neural Tissue
//  Subsystem:       XML
//  Workfile:        abb2.h
//  Revision:        1.0 - 24 March, 2010
//
//  Author:          J. Marvel
//
//  Description
//  ===========
//  Parser for XML strings for the ABB GSI protocol.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef ABB2_H
#define ABB2_H

#include <string.h>
#include <iostream>
#include "portable.h"
#include "types.h"
#include "reporter.h" 
using namespace Reporter;

namespace Xml
{
	class ABB2Parse
	{

		public:

			//! @brief Constructor
			//!
			//! @param robot  TODO
			//! @param logPtr TODO
			//! @param target TODO
			//!
			ABB2Parse (robotStruct *robot, Logger *logPtr, robotStruct *target);

			//! @brief Default destructor
			//!
			~ABB2Parse ();


			//! NOTE: On changing the XML schema, these are the only functions that
			//!       need to be modified.  Add new flags (*Active_) in the "private"
			//!       section to reflect additions.

			//! @brief Parse the first tag of a tag pair
			//!
			//! @param tagName The tag label
			//! @param attr    String of additional attributes located within the tag
			//!
			//! @return True if parsing is successful
			//!
			bool startElement (const std::string& tagName, 
					const xmlAttributes& attr);


			//! @brief Parse the second tag of a tag pair
			//!
			//! @param tagName The tag label
			//!
			//! @return True if parsing is successful
			//!
			bool endElement (const std::string& tagName);


			//! @brief Encode an XML string from an input schema
			//!
			//! @param line The output XML string
			//!
			//! @return True if encoding was successful, false otherwise
			//!
			bool encode (char *line);

			int grabMutex(int *Count);
			int releaseMutex();

		private:

			//! @brief TODO
			//!
			robotStruct *robot_;

			//! @briefoTODO
			//!
			Logger *logger_;

			//! @brief TODO
			//!
			robotStruct *target_;

			//! @brief Display an error message
			//!
			//! @parm where The location where the error occurred
			//! @param what The error type
			//!
			void exception (const char *where, const char *what);
	}; // ABB2Parse
} // XML

#endif
