///////////////////////////////////////////////////////////////////////////////
//
//  Original System: Neural Tissue
//  Subsystem:       XML
//  Workfile:        xml.h
//  Revision:        1.0 - 21 January, 2008
//
//  Author:          J. Marvel
//
//  Description
//  ===========
//  Handler for pure XML strings.  Calls specialized XML parsers.
//
///////////////////////////////////////////////////////////////////////////////
//
// (UNIX Version / E. Young) 8/3/2011 merged this file with xml.cpp to follow the
// inclusion-compilation model, provides faster builds from fresh and 
// easier makefiles.  This file should be in a different directory than xml.cpp
// and the original xml.h, to avoid conflicts
//  
///////////////////////////////////////////////////////////////////////////////

#ifndef XML_H
#define XML_H

#include "portable.h"
#include "types.h"
#include "reporter.h"
#include "abb2.h"
// #include "appOptions.h"
// #include "asm.h"
#include "surgery.h"
#include <iostream>

using namespace Reporter;
using namespace std;

namespace Xml
{
	//! @ingroup Xml
	//!
	//! @brief XML parsing class based on the SAX structure
	//!
	template <class T> class XmlParse
	{
		public:

			//! @brief Constructor
			//!
			//! @param strctr TODO
			//! @param logPtr TODO
			//!
			XmlParse (T *strctr, Logger *logPtr);

			//! @brief Destructor
			//!
			~XmlParse ();

			//! @brief Parse a line of XML tagging, storing the tag value in tag and
			//!        the attribute value in attribute.  
			//!
			//! @param line The input string to be parsed
			//!
			//! @return True if parsing was successful, false otherwise
			//!
			//! The order of attributes coincides with the order of attribute tags
			//! @note Currently only has minimal error handling
			//!
			bool parse (const std::string& line);

			//! @brief Encode an XML string from an input schema
			//!
			//! @param line The output XML string
			//!
			//! @return True if encoding was successful, false otherwise
			//!
			bool encode (char* line);

		private:

			//! @brief Parse the text between the tag pair
			//!
			//! @param ch The string of character located between the tag pair
			//!
			//! @return True if parsing is successful
			//!
			bool characters (const std::vector<std::string>& ch);

			//! @brief Display an error message
			//!
			//! @param where The location where the error occurred
			//! @param what  The error type
			//!
			void exception (const char *where, const char *what);

			//! @brief The schema structure being populated by the XML parser
			//!
			T *xmlData_;

			//! @brief TODO
			//!
			Logger *logger_;

	}; // XmlParse
} // Xml namespace

///////////////////////////////////////////////////////////////////////////////
//
//  Original System: Neural Tissue
//  Subsystem:       XML
//  Workfile:        xml.cpp
//  Revision:        1.0 - 21 January, 2008
//                   1.1 - 4 February, 2008 : Modified to be an exported
//                                            template library for generic XML
//                                            parsing and encoding
//  Author:          J. Marvel
//
//  Description
//  ===========
//  XML parser class definition file.
//
///////////////////////////////////////////////////////////////////////////////


namespace Xml
{
	template <class T> XmlParse<T>::XmlParse (T *strctr, Logger *logPtr) :
		xmlData_(strctr),
		logger_(logPtr)
	{
	}


	template <class T> XmlParse<T>::~XmlParse ()
	{
		xmlData_ = NULL;
		logger_ = NULL;
	}


	template <class T> bool XmlParse<T>::characters(const vector<string>& ch)
	{
		vector<string>::const_iterator chIter = ch.begin();

		for (; chIter != ch.end(); ++chIter)
		{
		}

		return true;
	}


	template <class T> bool XmlParse<T>::encode (char *line)
	{
		return xmlData_->encode (line);
	}

	//***************************************************************************
	//                        END CONTEXT-SPECIFIC CODE
	//***************************************************************************

	template <class T> bool XmlParse<T>::parse (const string &line)
	{
		static char buff[1024];
		bool readTags = false;
		bool state = true;
		bool error = false;

		xmlAttributes attr;
		vector<string> chars;

		int marker = 0;
		int openTags = 0;
		bool tagActive = false;
		bool start = false;
		bool single = false;

		int strLength = line.length();
		int count;
		string temp;
		string name;
		string attrname;
		string attrval;


		try
		{
			while (marker < strLength && !error && line[marker] != '\0')
			{
				if (line[marker] == '<')
				{
					//! ***** Handle tags *****
					tagActive = true;
					for (int i = 0; i < 20; i++)
					{
						buff[i] = ' ';
					}

					name = "";
					++marker;

					//! Skip white space
					while (line[marker] == ' ' || line[marker] == '\t')
					{
						++marker;
						if (marker >= (strLength - 1))
						{
							break;
						}
					}

					if (marker >= (strLength - 1))
					{
						//! Ill-formed XML string (end encountered before closing tags
						exception ("XML:parse", "End of string buffer reached before closing tags 1");
						exception ("XML:parse", line.c_str());
						error = true;
						break;
					}

					if (line[marker] == '/')
					{
						start = false;
						marker++;
					}
					else
					{
						start = true;
					}

					int count = 0;
					while (line[marker] != ' ' && line[marker] != '\t' &&
							line[marker] != '/' && line[marker] != '>' &&
							marker <= (strLength - 1))
					{
						buff[count] = line[marker];
						count++;
						marker++;
						if (marker >= (strLength - 1))
						{
							break;
						}
					}
					if (marker > (strLength - 1))
					{
						//! Ill-formed XML string (end encountered before closing tags
						exception ("XML:parse", "End of string buffer reached before closing tags 2");
						exception ("XML:parse", line.c_str());
						error = true;
						break;
					}

					buff[count] = '\0';
					name = buff;
					name = name.substr(0, count);

					if (start)
					{
						//! Look for attributes of opening tags
						while (line[marker] != '/' && line[marker] != '>' &&
								marker <= (strLength -1))
						{
							if (line[marker] != ' ' && line[marker] != '\t' &&
									line[marker] != '\n')
							{
								//! Non-white-space character & non-terminating element
								//! Ergo, an attribute
								count = 0;

								//! grab the attribute name
								while (line[marker] != '=' && line[marker] != ' ')
								{
									buff[count++] = line[marker++];
									if (marker >= strLength - 1)
									{
										break;
									}
								}
								if (marker >= strLength - 1)
								{
									break;
								}
								buff[count] = '\0';
								attrname = buff;
								attrname = attrname.substr(0, count);
								attr.name.push_back(attrname);

								//! grab the attribute value
								while (line[marker] != '\"')
								{
									marker++;
									if (marker >= strLength - 1)
									{
										break;
									}
								}
								marker++;
								if (marker >= strLength - 1)
								{
									break;
								}

								count = 0;
								while (line[marker] != '\"')
								{
									buff[count++] = line[marker++];
									if (marker >= strLength - 1)
									{
										break;
									}
								}
								buff[count] = '\0';
								attrval = buff;
								attrval = attrval.substr(0, count);
								attr.val.push_back(attrval);
							} // if (line[marker] != ' ' && line[marker] != '\t' && line[marker] != '\n')
							marker++;
							if (marker > strLength - 1)
							{
								break;
							}
						} // while (line[marker] != '/' && line[marker] != '>')

						if (marker > (strLength - 1))
						{
							//! Ill-formed XML string (end encountered before closing tags
							exception ("XML:parse", "End of string buffer reached before closing tags 3");
							exception ("XML:parse", line.c_str());
							error = true;
							break;
						} // if (marker > (strLength - 1))

						//! Test for single-tag XML lines (ex. <tag/>
						if (line[marker] == '/')
						{
							single = true;
						}
						while (line[marker] != '>')
						{
							marker++;
							if (marker >= strLength - 1)
							{
								break;
							}
						} // while (line[marker] != '>')
					} // if (start)

					if (error)
					{
						break;
					}

					//! Pass tag information for data structure assignment
					if (start)
					{
						if (state && xmlData_->startElement (name, attr))
						{
							state = true;
							attr.name.clear();
							attr.val.clear();
						}
						else
						{
							state = false;
						}

						 // validXML = false;
						openTags++;
						readTags = true;
						if (single)
						{
							tagActive = false;
						// 	validXML = true;
							if (state && xmlData_->endElement (name))
							{
								state = true;
							}
							else
							{
								state = false;
							}
							openTags--;
							start = false;
						}
					} // if (start)
					else
					{
						tagActive = false;
						// validXML = true;
						if (state && xmlData_->endElement (name))
						{
							state = true;
						}
						else
						{
							state = false;
						}
						openTags--;
					} // if (start) ... else
				} // if (line[marker] == '<')
				else if (tagActive)
				{
					//! ***** Handle text between tags *****
					while(start)
					{
						if (marker > strLength || line[marker] == '<')
						{
							marker--;
							break;
						}

						//! Skip white space
						while (line[marker] == ' ' || line[marker] == '\t' ||
								line[marker] == '\n' || line[marker] == ',')
						{
							marker++;
							if (marker >= strLength)
							{
								break;
							}
						}

						if (marker > strLength || line[marker] == '<')
						{
							marker--;
							break;
						}

						count = 0;
						while (line[marker] != ',' && line[marker] != '<')
						{
							buff[count++] = line[marker++];
							if (marker >= strLength - 1)
							{
								break;
							}
						}
						if (marker >= (strLength - 1))
						{
							//! Ill-formed XML string (end encountered before closing tags
							exception ("XML:parse", "End of string buffer reached before closing tags 4");
							exception ("XML:parse", line.c_str());
							error = true;
							break;
						}
						if (count > 0)
						{
							temp = buff;
							temp = temp.substr (0, count);
							chars.push_back (temp);
						}
					} // while(start)
					if (start)
					{
						if (state && characters (chars))
						{
							state = true;
						}
						else
						{
							state = false;
						}
						chars.clear ();
					}
					start = false;

				} // else if (tagActive)

				++marker;
				if (openTags < 1 && readTags)
				{
					//          break;
				}
			} // while (marker < strLength && marker < 1024)
			/*
			   if (state && validXML && (openTags == 0) && !error)
			   {
			   state = true;
			   }
			   else
			   {
			   state = false;
			   }
			   */
		} // try
		catch (...)
		{
			exception ("XML:parse", "unknown exception");
			exception ("XML:parse", line.c_str());
			return false;
		}

		if (error)
		{
			exception ("XML:parse", "exiting with error");
		}
		return state;
	}


	template <class T> void XmlParse<T>::exception (const char *where, const char *what)
	{
		static char message[1024];
		if (logger_ != NULL)
		{
			sprintf (message, "Exception in \" %s \" : %s", where, what);
			logger_->error (message);
		}
	}
} // XML

#endif
