
//**********************************************************************************************************************************
//
//   Purpose: Add Project Utility source code file
//
//   Project: Development Utilities
//
//  Filename: Name.cpp
//
//   Authors: Caroline.M.Mathieson (CMM)
//
//**********************************************************************************************************************************
//
//  Description
//  -----------
//
//! \file Name.cpp
//! \brief Contains the implementation of the NAME object.
//!
//! This file contains the implementation of the NAME Object.
//!
//**********************************************************************************************************************************

#include "AddProject.h"

NUMBER NumberOfNames = 0;

//*********************************************************************************************************************************
/**
\fn NAME::NAME ( CHARACTER *NewName )

\brief The constructor which initialises the variables.

**/
//**********************************************************************************************************************************

//**********************************************************************************************************************************
//
//  NAME
//
//**********************************************************************************************************************************

NAME::NAME ( CHARACTER *NewName )
{
    NUMBER     NewNameLength;
    CHARACTER *Word;
    CHARACTER *Line;
    CHARACTER  InputLineCopy [ 2000 ];

    ASSERTION ( NewName != NULL );

    NewNameLength = strlen ( NewName );

    // allocate the storage for the names based on the input string length.

	if ( ( Long                     = (CHARACTER *) calloc ( NewNameLength + 10, sizeof ( CHARACTER ) ) ) == NULL ) return;
	if ( ( LongUppercase            = (CHARACTER *) calloc ( NewNameLength + 10, sizeof ( CHARACTER ) ) ) == NULL ) return;
	if ( ( LongLowercase            = (CHARACTER *) calloc ( NewNameLength + 10, sizeof ( CHARACTER ) ) ) == NULL ) return;
	if ( ( LongCapitalised          = (CHARACTER *) calloc ( NewNameLength + 10, sizeof ( CHARACTER ) ) ) == NULL ) return;
	if ( ( LongUppercaseUnderscored = (CHARACTER *) calloc ( NewNameLength + 10, sizeof ( CHARACTER ) ) ) == NULL ) return;
	if ( ( LongLowercaseUnderscored = (CHARACTER *) calloc ( NewNameLength + 10, sizeof ( CHARACTER ) ) ) == NULL ) return;
	if ( ( ShortLowercase           = (CHARACTER *) calloc ( NewNameLength + 10, sizeof ( CHARACTER ) ) ) == NULL ) return;
	if ( ( ShortUppercase           = (CHARACTER *) calloc ( NewNameLength + 10, sizeof ( CHARACTER ) ) ) == NULL ) return;
	if ( ( ShortCapitalised         = (CHARACTER *) calloc ( NewNameLength + 10, sizeof ( CHARACTER ) ) ) == NULL ) return;

    strcpy ( InputLineCopy, NewName );

    // remove extra spaces and tabs from end of line

    Word = &InputLineCopy [ strlen ( InputLineCopy ) - 1 ];

    while ( ( *Word == 0x09 ) || ( *Word == 0x0A ) || ( *Word == 0x0D ) || ( *Word == 0x20 ) ) { *Word = '\0'; Word--; }

    // remove extra spaces and tabs at start of line

    Line = &InputLineCopy [ 0 ];

    while ( ( *Line == 0x09 ) || ( *Line == 0x20 ) ) Line++;

    // get first word of name and capitalise as appropriate

    Word = strtok ( Line, " " );

    if ( Word != NULL )
    {
        strcpy ( Long,                     Word );
		strcpy ( LongUppercase,            Word );
        strcpy ( LongLowercase,            Word );
        strcpy ( LongCapitalised,          Word );
        strcpy ( LongUppercaseUnderscored, Word );
        strcpy ( LongLowercaseUnderscored, Word );
        strcpy ( ShortLowercase,           Word );
        strcpy ( ShortUppercase,           Word );
        strcpy ( ShortCapitalised,         Word );
		
		LongCapitalised  [ 0 ] = (CHARACTER) toupper ( LongCapitalised  [ 0 ] );
        ShortCapitalised [ 0 ] = (CHARACTER) toupper ( ShortCapitalised [ 0 ] );
    }

    // then other words in name and seperate them as appropriate

    while ( Word != NULL )
    {
        Word = strtok ( NULL, " " );

        if ( Word != NULL )
        {
            // seperate long versions with spaces

            strcat ( Long, " "  );
            strcat ( Long, Word );

            strcat ( LongLowercase,  " "  );
            strcat ( LongLowercase,  Word );
            strcat ( ShortLowercase, Word );

            strcat ( LongUppercase,  " "  );
            strcat ( LongUppercase,  Word );
            strcat ( ShortUppercase, Word );

            // seperate underscored versions with underscores

            strcat ( LongLowercaseUnderscored, "_"  );
            strcat ( LongLowercaseUnderscored, Word );

            strcat ( LongUppercaseUnderscored, "_"  );
            strcat ( LongUppercaseUnderscored, Word );

            // make capitalised versions capitalised

            Word [ 0 ] = (CHARACTER) toupper ( Word [ 0 ] );

            strcat ( LongCapitalised,  " "  );
            strcat ( LongCapitalised,  Word );
            strcat ( ShortCapitalised, Word );
        }
    }

    // make the upper case ones upper case

    StringUpper ( LongUppercase            );
    StringUpper ( ShortUppercase           );
    StringUpper ( LongUppercaseUnderscored );

    // make the lower case ones lower case

    StringLower ( LongLowercase            );
    StringLower ( ShortLowercase           );
    StringLower ( LongLowercaseUnderscored );

    NameNumber = NumberOfNames++;
}

//**********************************************************************************************************************************

NAME::~NAME ( void )
{
	if ( Long                     != NULL ) free ( Long );
	if ( LongUppercase            != NULL ) free ( LongUppercase );
	if ( LongLowercase            != NULL ) free ( LongLowercase );
	if ( LongCapitalised          != NULL ) free ( LongCapitalised );
	if ( LongUppercaseUnderscored != NULL ) free ( LongUppercaseUnderscored );
	if ( LongLowercaseUnderscored != NULL ) free ( LongLowercaseUnderscored );
	if ( ShortLowercase           != NULL ) free ( ShortLowercase );
	if ( ShortUppercase           != NULL ) free ( ShortUppercase );
	if ( ShortCapitalised         != NULL ) free ( ShortCapitalised );
}

//**********************************************************************************************************************************

void NAME::Debug ( void )
{
    TOOL   *Tool;
    FILE   *DebugFile;
    NUMBER  IndentLevel;

    Tool = GetTool ();

    DebugFile = Tool->GetDebugFile();

    IndentLevel= Tool->GetIndentLevel ();

    fprintf ( DebugFile, "\n");

    fprintf ( DebugFile, "%*s NAME (%07d)\n", IndentLevel, "", NameNumber );

    fprintf ( DebugFile, "%*s --------------\n", IndentLevel, "" );

    fprintf ( DebugFile, "%+*s: %s\n", IndentLevel, "Long",                     Long                     );
    fprintf ( DebugFile, "%+*s: %s\n", IndentLevel, "LongUppercase",            LongUppercase            );
    fprintf ( DebugFile, "%+*s: %s\n", IndentLevel, "LongLowercase",            LongLowercase            );
    fprintf ( DebugFile, "%+*s: %s\n", IndentLevel, "LongCapitalised",          LongCapitalised          );
    fprintf ( DebugFile, "%+*s: %s\n", IndentLevel, "LongUppercaseUnderscored", LongUppercaseUnderscored );
    fprintf ( DebugFile, "%+*s: %s\n", IndentLevel, "LongLowercaseUnderscored", LongLowercaseUnderscored );
    fprintf ( DebugFile, "%+*s: %s\n", IndentLevel, "ShortLowercase",           ShortLowercase           );
    fprintf ( DebugFile, "%+*s: %s\n", IndentLevel, "ShortUppercase",           ShortUppercase           );
    fprintf ( DebugFile, "%+*s: %s\n", IndentLevel, "ShortCapitalised",         ShortCapitalised         );
}

//**********************************************************************************************************************************

CHARACTER *NAME::GetLong ( void )
{
    return ( Long );
}

CHARACTER *NAME::GetLongUppercase ( void )
{
    return ( LongUppercase );
}

CHARACTER *NAME::GetLongLowercase ( void )
{
    return ( LongLowercase );
}

CHARACTER *NAME::GetLongCapitalised ( void )
{
    return ( LongCapitalised );
}

CHARACTER *NAME::GetLongUppercaseUnderscored ( void )
{
    return ( LongUppercaseUnderscored );
}

CHARACTER *NAME::GetLongLowercaseUnderscored ( void )
{
    return ( LongLowercaseUnderscored );
}

CHARACTER *NAME::GetShortLowercase ( void )
{
    return ( ShortLowercase );
}

CHARACTER *NAME::GetShortUppercase ( void )
{
    return ( ShortUppercase );
}

CHARACTER *NAME::GetShortCapitalised ( void )
{
    return ( ShortCapitalised );
}

//**********************************************************************************************************************************

NUMBER GetNumberOfNames ( void )
{
    return ( NumberOfNames );
}

//**********************************************************************************************************************************
