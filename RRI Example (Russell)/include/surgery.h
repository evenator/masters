///////////////////////////////////////////////////////////////////////////////
//
//  Original System: Neural Tissue
//  Subsystem:       XML
//  Workfile:        surgery.h
//  Revision:        1.0 - 24 May, 2010
//
//  Author:          J. Marvel
//
//  Description
//  ===========
//  Parser for XML strings for communicating robot state with other computers
//  for the robot surgery project
//
///////////////////////////////////////////////////////////////////////////////

#ifndef SURGERY_H
#define SURGERY_H

#include "portable.h"
#include "types.h"
#include "reporter.h"
#include "Object1.h"

using namespace Reporter;

namespace Xml
{
	class SurgeryParse
	{

		public:

			//! @brief Constructor
			//!
			//! @param robot  TODO
			//! @param logPtr TODO
			//!
			SurgeryParse (objectClass *object, Logger *logPtr);

			//! @brief Default destructor
			//!
			~SurgeryParse ();

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


			objectTypes get_type_enum(const char *type);

			int grabMutex(int *count);
			int releaseMutex();
			

		private:

			//! @brief TODO
			//!
			objectClass *object_;
			
			//! @brief TODO
			//!
			objectClass *tempObject;

			//! @brief TODO
			//!
			Logger *logger_;

			//! @brief TODO
			//!
			int addMutex();

			//! @brief TODO
			//!
			

			int altMutex;
			//! @brief Display an error message
			//!
			//! @parm where The location where the error occurred
			//! @param what The error type
			//!
			void exception (const char *where, const char *what);
	}; // SurgeryParse
} // XML

#endif
