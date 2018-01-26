
//**********************************************************************************************************************************
//
//   Purpose: Add Project Utility source code file
//
//   Project: Development Utilities
//
//  Filename: TemplateFile.cpp
//
//   Authors: Caroline.M.Mathieson (CMM)
//
//**********************************************************************************************************************************
//
//  Description
//  -----------
//
//! \file TemplateFile.cpp
//! \brief Contains the implementation of the TEMPLATEFILE Object.
//!
//! This file contains the implementation of the TEMPLATEFILE Object.
//!
//**********************************************************************************************************************************

#include "AddProject.h"

//**********************************************************************************************************************************

#define MAX_MACRO_NAMES_PER_TABLE (12)

//**********************************************************************************************************************************

typedef struct macro_name_entry
{
   char *MacroName;
   char *MethodName;

} MacroNameEntry;

//**********************************************************************************************************************************

typedef enum master_and_source_macro_indices
{
    GET_SOURCE_DIRECTORY_NAME = 0,  // GetSourceDirectoryName()
    GET_SOURCE_FILE_NAME,           // GetSourceFileName()
    GET_DESTINATION_DIRECTORY_NAME, // GetDestinationDirectoryName()
    GET_DESTINATION_FILE_NAME,      // GetDestinationFileName()
    GET_SOURCE_NAME,                // GetSourceName()
    GET_SOURCE_EXTENSION,           // GetSourceExtension()
    GET_DESTINATION_NAME,           // GetDestinationName()
    GET_DESTINATION_EXTENSION,      // GetDestinationExtension()

} MasterAndSourceMacroIndices;

typedef enum repetition_macro_indices
{
    SEPERATOR = 0, // TemplateFile::InsertSeperator()

    REPEAT,     // TemplateFile::RepeatLevelOne()
    END_REPEAT, // TemplateFile::EndRepeatLevelOne()

    REPEAT_MASTER,     // MasterFile::RepeatRootMasterFiles()
    END_REPEAT_MASTER, // MasterFile::EndRepeatRootMasterFiles()

    REPEAT_SOURCE,     // SourceFile::RepeatRootSourceFiles()
    END_REPEAT_SOURCE, // SourceFile::EndRepeatRootSourceFiles()

} RepetitionMacroIndices;

typedef enum functio_nmacro_indices
{
    PROCEDURE_HEADER = 0, // InsertProcedureHeader()
    FUNCTION_HEADER,      // InsertFunctionHeader()
    PROCEDURE_ENDING,     // InsertProcedureEnding()
    FUNCTION_ENDING,      // InsertFunctionEnding()

} FunctionMacroIndices;

typedef enum name_macro_indices
{
    LONG_ORIGONAL = 0,          // Name::Long()
    LONG_LOWERCASE_UNDERSCORED, // Name::LongLowercaseUnderscored()
    LONG_UPPERCASE_UNDERSCORED, // Name::GetLongUppercaseUnderscored()
    LONG_LOWERCASE,             // Name::GetLongLowercase()
    LONG_UPPERCASE,             // Name::GetLongUppercase()
    LONG_CAPITALISED,           // Name::GetLongCapitalised()
    SHORT_LOWERCASE,            // Name::GetShortLowercase()
    SHORT_UPPERCASE,            // Name::GetShortUppercase()
    SHORT_CAPITALISED,          // Name::GetShortCapitalised()

} NameMacroIndices;

typedef enum standard_macro_indices
{
    AUTHOR_NAME = 0, // GetAuthorName()
    AUTHOR_INITIALS, // GetAuthorInitials()
    FILE_DATE,       // GetFileDate()
    FILE_LONGDATE,   // GetFileTimeLong()
    FILE_TIME,       // GetFileTime()
    FILE_YEAR,       // GetFileYear()
    COMPANY_URL,     // GetCompanyWebAddress()
    DEST_NAME,       // GetDestinationName()
    DEST_EXT,        // GetDestinationExtension()
    DIRECTORY_NAME,  // GetMnemonic()
    MNEMONIC,        // GetMnemonic()

} StandardMacroIndices;

//**********************************************************************************************************************************

MacroNameEntry MasterFileMacroNames [ MAX_MACRO_NAMES_PER_TABLE ] =
{
    { "MFSDN", "MasterFile::GetSourceDirectoryName"      },
    { "MFSFN", "MasterFile::GetSourceFileName"           },
    { "MFDDN", "MasterFile::GetDestinationDirectoryName" },
    { "MFDFN", "MasterFile::GetDestinationFileName"      },
    { "MFSN",  "MasterFile::GetSourceName"               },
    { "MFSE",  "MasterFile::GetSourceExtension"          },
    { "MFDN",  "MasterFile::GetDestinationName"          },
    { "MFDE",  "MasterFile::GetDestinationExtension"     },

    { "ZZZZ", "ZZZZ" }
};

MacroNameEntry MasterFileMacroNamesBackslash [ MAX_MACRO_NAMES_PER_TABLE ] =
{
    { "MFBSDN", "MasterFile::GetSourceDirectoryName      (Append Backslash)" },
    { "MFBSFN", "MasterFile::GetSourceFileName           (Append Backslash)" },
    { "MFBDDN", "MasterFile::GetDestinationDirectoryName (Append Backslash)" },
    { "MFBDFN", "MasterFile::GetDestinationFileName      (Append Backslash)" },
    { "MFBSN",  "MasterFile::GetSourceName               (Append Backslash)" },
    { "MFBSE",  "MasterFile::GetSourceExtension          (Append Backslash)" },
    { "MFBDN",  "MasterFile::GetDestinationName          (Append Backslash)" },
    { "MFBDE",  "MasterFile::GetDestinationExtension     (Append Backslash)" },

    { "ZZZZ", "ZZZZ" }
};

MacroNameEntry SourceFileMacroNames [ MAX_MACRO_NAMES_PER_TABLE ] =
{
    { "SFSDN", "SourceFile::GetSourceDirectoryName"      },
    { "SFSFN", "SourceFile::GetSourceFileName"           },
    { "SFDDN", "SourceFile::GetDestinationDirectoryName" },
    { "SFDFN", "SourceFile::GetDestinationFileName"      },
    { "SFSN",  "SourceFile::GetSourceName"               },
    { "SFSE",  "SourceFile::GetSourceExtension"          },
    { "SFDN",  "SourceFile::GetDestinationName"          },
    { "SFDE",  "SourceFile::GetDestinationExtension"     },

    { "ZZZZ", "ZZZZ" }
};

MacroNameEntry SourceFileMacroNamesBackslash [ MAX_MACRO_NAMES_PER_TABLE ] =
{
    { "SFBSDN", "SourceFile::GetSourceDirectoryName      (Append Backslash)" },
    { "SFBSFN", "SourceFile::GetSourceFileName           (Append Backslash)" },
    { "SFBDDN", "SourceFile::GetDestinationDirectoryName (Append Backslash)" },
    { "SFBDFN", "SourceFile::GetDestinationFileName      (Append Backslash)" },
    { "SFBSN",  "SourceFile::GetSourceName               (Append Backslash)" },
    { "SFBSE",  "SourceFile::GetSourceExtension          (Append Backslash)" },
    { "SFBDN",  "SourceFile::GetDestinationName          (Append Backslash)" },
    { "SFBDE",  "SourceFile::GetDestinationExtension     (Append Backslash)" },

    { "ZZZZ", "ZZZZ" }
};

MacroNameEntry RepetitionMacroNames [ MAX_MACRO_NAMES_PER_TABLE ] =
{
    { "SEPERATOR", "TemplateFile::InsertSeperator" },

    { "REPEAT",    "TemplateFile::RepeatLevelOne"    },
    { "ENDREPEAT", "TemplateFile::EndRepeatLevelOne" },

    { "REPEATMASTER", "MasterFile::RepeatRootMasterFiles"    },
    { "ENDMASTER",    "MasterFile::EndRepeatRootMasterFiles" },

    { "REPEATSOURCE", "SourceFile::RepeatRootSourceFiles"    },
    { "ENDSOURCE",    "SourceFile::EndRepeatRootSourceFiles" },

    { "ZZZZ", "ZZZZ" }
};

MacroNameEntry FunctionMacroNames [ MAX_MACRO_NAMES_PER_TABLE ] =
{
    { "PROCEDUREHEADER", "TemplateFile::InsertProcedureHeader" },
    { "FUNCTIONHEADER",  "TemplateFile::InsertFunctionHeader"  },
    { "PROCEDUREENDING", "TemplateFile::InsertProcedureEnding" },
    { "FUNCTIONENDING",  "TemplateFile::InsertFunctionEnding"  },

    { "ZZZZ", "ZZZZ" }
};

MacroNameEntry CompanyMacroNames [ MAX_MACRO_NAMES_PER_TABLE ] =
{
    { "COMNAME", "CompanyName::Long"                        },
    { "COMNLLU", "CompanyName::LongLowercaseUnderscored"    },
    { "COMNLUU", "CompanyName::GetLongUppercaseUnderscored" },
    { "COMNLL",  "CompanyName::GetLongLowercase"            },
    { "COMNLU",  "CompanyName::GetLongUppercase"            },
    { "COMNLC",  "CompanyName::GetLongCapitalised"          },
    { "COMNSL",  "CompanyName::GetShortLowercase"           },
    { "COMNSU",  "CompanyName::GetShortUppercase"           },
    { "COMNSC",  "CompanyName::GetShortCapitalised"         },

    { "ZZZZ", "ZZZZ" }
};

MacroNameEntry NameMacroNames [ MAX_MACRO_NAMES_PER_TABLE ] =
{
    { "NAME", "Name::Long"                        },
    { "NLLU", "Name::LongLowercaseUnderscored"    },
    { "NLUU", "Name::GetLongUppercaseUnderscored" },
    { "NLL",  "Name::GetLongLowercase"            },
    { "NLU",  "Name::GetLongUppercase"            },
    { "NLC",  "Name::GetLongCapitalised"          },
    { "NSL",  "Name::GetShortLowercase"           },
    { "NSU",  "Name::GetShortUppercase"           },
    { "NSC",  "Name::GetShortCapitalised"         },

    { "ZZZZ", "ZZZZ" }
};

MacroNameEntry StandardMacroNames [ MAX_MACRO_NAMES_PER_TABLE ] =
{
    { "AUTHOR",   "TemplateFile::GetAuthorName"           },
    { "INITIALS", "TemplateFile::GetAuthorInitials"       },
    { "DATE",     "TemplateFile::GetFileDate"             },
    { "LONGDATE", "TemplateFile::GetFileTimeLong"         },
    { "TIME",     "TemplateFile::GetFileTime"             },
    { "YEAR",     "TemplateFile::GetFileYear"             },
    { "CURL",     "TemplateFile::GetCompanyWebAddress"    },
    { "NAME",     "TemplateFile::GetDestinationName"      },
    { "EXT",      "TemplateFile::GetDestinationExtension" },
    { "DIRNAME",  "TemplateFile::GetMnemonic"             },
    { "MNEMONIC", "TemplateFile::GetMnemonic"             },

    { "ZZZZ", "ZZZZ" }
};

MacroNameEntry FileDescriptionMacroNames [ MAX_MACRO_NAMES_PER_TABLE ] =
{
    { "FDNAME", "FileDescriptionName::Long"                        },
    { "FDNLLU", "FileDescriptionName::LongLowercaseUnderscored"    },
    { "FDNLUU", "FileDescriptionName::GetLongUppercaseUnderscored" },
    { "FDNLL",  "FileDescriptionName::GetLongLowercase"            },
    { "FDNLU",  "FileDescriptionName::GetLongUppercase"            },
    { "FDNLC",  "FileDescriptionName::GetLongCapitalised"          },
    { "FDNSL",  "FileDescriptionName::GetShortLowercase"           },
    { "FDNSU",  "FileDescriptionName::GetShortUppercase"           },
    { "FDNSC",  "FileDescriptionName::GetShortCapitalised"         },

    { "ZZZZ", "ZZZZ" }
};

//**********************************************************************************************************************************
//
//  TEMPLATEFILE
//
//**********************************************************************************************************************************

/**
\fn TEMPLATEFILE::TEMPLATEFILE ( char *, char * )

\brief The constructor which initialises the variables and records the date and time.

The only actions in the constructor are to initialise all the variables and record the exact date and time of invokation. The real
work is done by the TOOL::Execute() method.

**/

//**********************************************************************************************************************************

TEMPLATEFILE::TEMPLATEFILE ( NODE     *ParentNode,
                             TEMPLATE *ParentTemplate,
                             FILE     *InputFile,
                             NUMBER   *LevelNumbers )
{
    long FileLength;

    Node = ParentNode;

    Parent = ParentTemplate;

    strcpy ( MacroPrefix, "$" );

    MasterFileNumber = -1;
    SourceFileNumber = -1;
    FilePointer      = 0;

    if ( InputFile != NULL )
    {
        fseek ( InputFile, 0, SEEK_END ); // go to the end of the file

        FileLength = ftell ( InputFile ); // the file pointer then gives the length

        fseek ( InputFile, 0, SEEK_SET ); // go back to the beginning of the file again

        FileStore = ( CHARACTER * ) malloc ( FileLength ); // allocate space for the whole file

        if ( FileStore != NULL )
        {
            FileSize = (long) fread ( FileStore, 1, FileLength, InputFile ); // and read it into memory if the space was allocated
        }
    }

    memset ( OutputString, 0, sizeof ( OutputString ) );

    memset ( &FunctionDefinition, 0, sizeof ( FunctionDefinition ) );
}

//**********************************************************************************************************************************

TEMPLATEFILE::TEMPLATEFILE ( NODE      *ParentNode,
                             TEMPLATE  *ParentTemplate,
                             CHARACTER *InputFileStore,
                             long       InputFileSize,
                             NUMBER    *LevelNumbers )
{
    Node = ParentNode;

    Parent = ParentTemplate;

    strcpy ( MacroPrefix, "$" );

    MasterFileNumber = -1;
    SourceFileNumber = -1;
    FilePointer      = 0;
    FileStore        = InputFileStore;
    FileSize         = InputFileSize;

    memset ( OutputString, 0, sizeof ( OutputString ) );

    memset ( &FunctionDefinition, 0, sizeof ( FunctionDefinition ) );
}

//**********************************************************************************************************************************

TEMPLATEFILE::TEMPLATEFILE ( NODE      *ParentNode,
                             TEMPLATE  *ParentTemplate,
                             CHARACTER *InputFileStore,
                             long       StartPosition,
                             long       EndPosition,
                             NUMBER    *LevelNumbers,
                             NUMBER     NewMasterFileNumber,
                             NUMBER     NewSourceFileNumber )
{
    Node = ParentNode;

    Parent = ParentTemplate;

    strcpy ( MacroPrefix, "$" );

    MasterFileNumber = NewMasterFileNumber;
    SourceFileNumber = NewSourceFileNumber;
    FilePointer      = 0;

    if ( InputFileStore != NULL )
    {
        FileSize = EndPosition - StartPosition + 1;

        FileStore = ( CHARACTER * ) malloc ( FileSize );

        if ( FileStore != NULL )
        {
            memcpy ( FileStore, &InputFileStore [ StartPosition ], FileSize );
        }
    }

    memset ( OutputString, 0, sizeof ( OutputString ) );

    memset ( &FunctionDefinition, 0, sizeof ( FunctionDefinition ) );
}

//**********************************************************************************************************************************

TEMPLATEFILE::~TEMPLATEFILE ( void )
{
    if ( FileStore != NULL )
    {
        free ( FileStore );
    }
}

//**********************************************************************************************************************************

CHARACTER TEMPLATEFILE::GetCharacter ( void )
{
    if ( FilePointer < FileSize )
    {
        return ( FileStore [ FilePointer++ ] );
    }
    else
    {
        return ( 0 );
    }
}

//**********************************************************************************************************************************

CHARACTER *TEMPLATEFILE::GetFileStore ( void )
{
    return ( FileStore );
}

long TEMPLATEFILE::GetFileSize ( void )
{
    return ( FileSize );
}

long TEMPLATEFILE::GetFilePointer ( void )
{
    return ( FilePointer );
}

void TEMPLATEFILE::SetFilePointer ( long NewFilePointer )
{
     FilePointer = NewFilePointer;
}

//**********************************************************************************************************************************

bool TEMPLATEFILE::EndOfFile ( void )
{
    if ( FilePointer < FileSize )
    {
        return ( FALSE );
    }
    else
    {
        return ( TRUE );
    }
}

//**********************************************************************************************************************************

void TEMPLATEFILE::Scan ( CHARACTER *NewMacroPrefix,
                          FILE      *OutputFile,
                          NUMBER     MasterFileIndex,
                          NUMBER     SourceFileIndex )
{
    NUMBER      NameIndex;
    CHARACTER   Character;
    MASTERFILE *MasterFile = NULL;
    SOURCEFILE *SourceFile = NULL;
    bool        Replaced;
	FILE       *DebugFile = NULL;

	DebugFile = GetTool()->GetDebugFile();

    strcpy ( MacroPrefix, NewMacroPrefix ); // store it away for the other functions

    // get the master and source file pointers for this node, if any

    if ( Node == NULL )
    {
        MasterFile = Node->GetMasterFile ( MasterFileIndex );

        SourceFile = Node->GetSourceFile ( SourceFileIndex );
    }

    while ( ! EndOfFile () )
    {
        Character = GetCharacter ();

        if ( Character == MacroPrefix [ 0 ] ) // just check the first letter for now
        {
            Character = GetCharacter (); // get the letter after the macro prefix and analyse

            if ( Character == '{' )
            {
                // get the macro name

                NameIndex = 0;

                while ( ( Character != '}' ) && ( ! EndOfFile () ) && ( NameIndex < sizeof ( MacroName ) ) )
                {
                    Character = GetCharacter ();

                    MacroName [ NameIndex++ ] = Character;
                }

                MacroName [ NameIndex - 1 ] = '\0';

                // now test all the different macro names

                OutputString [ 0 ] = '\0';

                Replaced = CheckStandardMacros ( MacroName );

                if ( ! Replaced )
                {
                    Replaced = CheckCompanyMacros ( MacroName );

                    if ( ! Replaced )
                    {
                        Replaced = CheckNameMacros ( MacroName );

                        if ( ! Replaced )
                        {
                            Replaced = CheckFileDescriptionMacros ( MacroName );

                            if ( ! Replaced )
                            {
                                if ( MasterFile != NULL )
                                {
                                    Replaced = CheckMasterFileMacros ( MacroName, MasterFile );
                                }

                                if ( ! Replaced )
                                {
                                    if ( SourceFile != NULL )
                                    {
                                        Replaced = CheckSourceFileMacros ( MacroName, SourceFile );
                                    }

                                    if ( ! Replaced )
                                    {
                                        Replaced = CheckRepetitionMacros ( MacroName, OutputFile );

                                        if ( ! Replaced )
                                        {
                                            if ( strstr ( MacroName, FunctionMacroNames [ PROCEDURE_HEADER ].MacroName ) == MacroName )
                                            {
                                                //InsertProcedureHeader ( OutputFile );
                                            }
                                            else
                                            {
                                                if ( strstr ( MacroName,FunctionMacroNames [ FUNCTION_HEADER ].MacroName ) == MacroName )
                                                {
                                                    //InsertFunctionHeader ( OutputFile );
                                                }
                                                else
                                                {
                                                    if ( strstr ( MacroName, FunctionMacroNames [ PROCEDURE_ENDING ].MacroName) == MacroName )
                                                    {
                                                        //InsertProcedureEnding ( OutputFile );
                                                    }
                                                    else
                                                    {
                                                        if ( strstr ( MacroName, FunctionMacroNames [ FUNCTION_ENDING ].MacroName ) == MacroName )
                                                        {
                                                            //InsertFunctionEnding ( OutputFile );
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }

                // only print out the macro expansions for the above macros

                if ( Replaced )
                {
                    fprintf ( DebugFile, "                Replaced extracted macroname: %s, with \"%s\"\n", MacroName, OutputString );

                    fprintf ( OutputFile, OutputString );
                }
                else
                {
                    fprintf ( DebugFile, "      No replacement for extracted macroname: %s\n", MacroName );
                }
            }
            else
            {
                putc ( MacroPrefix [ 0 ], OutputFile );

                putc ( Character, OutputFile );
            }
        }
        else
        {
            // Just write the character to the output

            putc ( Character, OutputFile );
        }
    }
}

//**********************************************************************************************************************************

bool TEMPLATEFILE::CheckMasterFileMacros ( CHARACTER  *MacroName,
                                           MASTERFILE *MasterFile )
{
    if ( ( MacroName [ 0 ] == 'M' ) && ( MacroName [ 1 ] == 'F' ) )
    {
        if ( strstr ( MacroName, MasterFileMacroNames [ GET_SOURCE_DIRECTORY_NAME      ].MacroName ) == MacroName ) { strcpy ( OutputString, MasterFile->GetSourceDirectoryName      () ); return ( TRUE ); }
        if ( strstr ( MacroName, MasterFileMacroNames [ GET_SOURCE_FILE_NAME           ].MacroName ) == MacroName ) { strcpy ( OutputString, MasterFile->GetSourceFileName           () ); return ( TRUE ); }
        if ( strstr ( MacroName, MasterFileMacroNames [ GET_DESTINATION_DIRECTORY_NAME ].MacroName ) == MacroName ) { strcpy ( OutputString, MasterFile->GetDestinationDirectoryName () ); return ( TRUE ); }
        if ( strstr ( MacroName, MasterFileMacroNames [ GET_DESTINATION_FILE_NAME      ].MacroName ) == MacroName ) { strcpy ( OutputString, MasterFile->GetDestinationFileName      () ); return ( TRUE ); }
        if ( strstr ( MacroName, MasterFileMacroNames [ GET_SOURCE_NAME                ].MacroName ) == MacroName ) { strcpy ( OutputString, MasterFile->GetSourceName               () ); return ( TRUE ); }
        if ( strstr ( MacroName, MasterFileMacroNames [ GET_SOURCE_EXTENSION           ].MacroName ) == MacroName ) { strcpy ( OutputString, MasterFile->GetSourceExtension          () ); return ( TRUE ); }
        if ( strstr ( MacroName, MasterFileMacroNames [ GET_DESTINATION_NAME           ].MacroName ) == MacroName ) { strcpy ( OutputString, MasterFile->GetDestinationName          () ); return ( TRUE ); }
        if ( strstr ( MacroName, MasterFileMacroNames [ GET_DESTINATION_EXTENSION      ].MacroName ) == MacroName ) { strcpy ( OutputString, MasterFile->GetDestinationExtension     () ); return ( TRUE ); }

        if ( strstr ( MacroName, MasterFileMacroNamesBackslash [ GET_SOURCE_DIRECTORY_NAME      ].MacroName ) == MacroName ) { strcpy ( OutputString, MasterFile->GetSourceDirectoryName      ( TRUE ) ); return ( TRUE ); }
        if ( strstr ( MacroName, MasterFileMacroNamesBackslash [ GET_SOURCE_FILE_NAME           ].MacroName ) == MacroName ) { strcpy ( OutputString, MasterFile->GetSourceFileName           ( TRUE ) ); return ( TRUE ); }
        if ( strstr ( MacroName, MasterFileMacroNamesBackslash [ GET_DESTINATION_DIRECTORY_NAME ].MacroName ) == MacroName ) { strcpy ( OutputString, MasterFile->GetDestinationDirectoryName ( TRUE ) ); return ( TRUE ); }
        if ( strstr ( MacroName, MasterFileMacroNamesBackslash [ GET_DESTINATION_FILE_NAME      ].MacroName ) == MacroName ) { strcpy ( OutputString, MasterFile->GetDestinationFileName      ( TRUE ) ); return ( TRUE ); }
        if ( strstr ( MacroName, MasterFileMacroNamesBackslash [ GET_SOURCE_NAME                ].MacroName ) == MacroName ) { strcpy ( OutputString, MasterFile->GetSourceName               ( TRUE ) ); return ( TRUE ); }
        if ( strstr ( MacroName, MasterFileMacroNamesBackslash [ GET_SOURCE_EXTENSION           ].MacroName ) == MacroName ) { strcpy ( OutputString, MasterFile->GetSourceExtension          ( TRUE ) ); return ( TRUE ); }
        if ( strstr ( MacroName, MasterFileMacroNamesBackslash [ GET_DESTINATION_NAME           ].MacroName ) == MacroName ) { strcpy ( OutputString, MasterFile->GetDestinationName          ( TRUE ) ); return ( TRUE ); }
        if ( strstr ( MacroName, MasterFileMacroNamesBackslash [ GET_DESTINATION_EXTENSION      ].MacroName ) == MacroName ) { strcpy ( OutputString, MasterFile->GetDestinationExtension     ( TRUE ) ); return ( TRUE ); }
    }

    return ( FALSE );
}

//**********************************************************************************************************************************

bool TEMPLATEFILE::CheckSourceFileMacros ( CHARACTER  *MacroName,
                                           SOURCEFILE *SourceFile )
{
    if ( ( MacroName [ 0 ] == 'S' ) && ( MacroName [ 1 ] == 'F' ) )
    {
        if ( strstr ( MacroName, SourceFileMacroNames [ GET_SOURCE_DIRECTORY_NAME      ].MacroName ) == MacroName ) { strcpy ( OutputString, SourceFile->GetSourceDirectoryName      () ); return ( TRUE ); }
        if ( strstr ( MacroName, SourceFileMacroNames [ GET_SOURCE_FILE_NAME           ].MacroName ) == MacroName ) { strcpy ( OutputString, SourceFile->GetSourceFileName           () ); return ( TRUE ); }
        if ( strstr ( MacroName, SourceFileMacroNames [ GET_DESTINATION_DIRECTORY_NAME ].MacroName ) == MacroName ) { strcpy ( OutputString, SourceFile->GetDestinationDirectoryName () ); return ( TRUE ); }
        if ( strstr ( MacroName, SourceFileMacroNames [ GET_DESTINATION_FILE_NAME      ].MacroName ) == MacroName ) { strcpy ( OutputString, SourceFile->GetDestinationFileName      () ); return ( TRUE ); }
        if ( strstr ( MacroName, SourceFileMacroNames [ GET_SOURCE_NAME                ].MacroName ) == MacroName ) { strcpy ( OutputString, SourceFile->GetSourceName               () ); return ( TRUE ); }
        if ( strstr ( MacroName, SourceFileMacroNames [ GET_SOURCE_EXTENSION           ].MacroName ) == MacroName ) { strcpy ( OutputString, SourceFile->GetSourceExtension          () ); return ( TRUE ); }
        if ( strstr ( MacroName, SourceFileMacroNames [ GET_DESTINATION_NAME           ].MacroName ) == MacroName ) { strcpy ( OutputString, SourceFile->GetDestinationName          () ); return ( TRUE ); }
        if ( strstr ( MacroName, SourceFileMacroNames [ GET_DESTINATION_EXTENSION      ].MacroName ) == MacroName ) { strcpy ( OutputString, SourceFile->GetDestinationExtension     () ); return ( TRUE ); }

        if ( strstr ( MacroName, SourceFileMacroNamesBackslash [ GET_SOURCE_DIRECTORY_NAME      ].MacroName ) == MacroName ) { strcpy ( OutputString, SourceFile->GetSourceDirectoryName      ( TRUE ) ); return ( TRUE ); }
        if ( strstr ( MacroName, SourceFileMacroNamesBackslash [ GET_SOURCE_FILE_NAME           ].MacroName ) == MacroName ) { strcpy ( OutputString, SourceFile->GetSourceFileName           ( TRUE ) ); return ( TRUE ); }
        if ( strstr ( MacroName, SourceFileMacroNamesBackslash [ GET_DESTINATION_DIRECTORY_NAME ].MacroName ) == MacroName ) { strcpy ( OutputString, SourceFile->GetDestinationDirectoryName ( TRUE ) ); return ( TRUE ); }
        if ( strstr ( MacroName, SourceFileMacroNamesBackslash [ GET_DESTINATION_FILE_NAME      ].MacroName ) == MacroName ) { strcpy ( OutputString, SourceFile->GetDestinationFileName      ( TRUE ) ); return ( TRUE ); }
        if ( strstr ( MacroName, SourceFileMacroNamesBackslash [ GET_SOURCE_NAME                ].MacroName ) == MacroName ) { strcpy ( OutputString, SourceFile->GetSourceName               ( TRUE ) ); return ( TRUE ); }
        if ( strstr ( MacroName, SourceFileMacroNamesBackslash [ GET_SOURCE_EXTENSION           ].MacroName ) == MacroName ) { strcpy ( OutputString, SourceFile->GetSourceExtension          ( TRUE ) ); return ( TRUE ); }
        if ( strstr ( MacroName, SourceFileMacroNamesBackslash [ GET_DESTINATION_NAME           ].MacroName ) == MacroName ) { strcpy ( OutputString, SourceFile->GetDestinationName          ( TRUE ) ); return ( TRUE ); }
        if ( strstr ( MacroName, SourceFileMacroNamesBackslash [ GET_DESTINATION_EXTENSION      ].MacroName ) == MacroName ) { strcpy ( OutputString, SourceFile->GetDestinationExtension     ( TRUE ) ); return ( TRUE ); }
    }

    return ( FALSE );
}

//**********************************************************************************************************************************

bool TEMPLATEFILE::CheckRepetitionMacros ( CHARACTER *MacroName,
                                           FILE      *OutputFile )
{
    if ( strstr ( MacroName, RepetitionMacroNames [ SEPERATOR ].MacroName ) == MacroName ) { InsertSeperator ( OutputFile ); return ( TRUE ); }

    //if ( strstr ( MacroName, RepetitionMacroNames [ REPEAT_GRP ].MacroName ) == MacroName ) { RepeatLevelOne ( OutputFile ); }

    //if ( strstr ( MacroName, RepetitionMacroNames [ REPEAT_APP ].MacroName ) == MacroName ) { RepeatApp ( OutputFile, LevelIndices [ 1 ] ); }

    //if ( strstr ( MacroName, RepetitionMacroNames [ REPEAT_MASTER_PRJ ].MacroName ) == MacroName ) { RepeatRootMasterFiles     ( OutputFile ); }
    //if ( strstr ( MacroName, RepetitionMacroNames [ REPEAT_MASTER_GRP ].MacroName ) == MacroName ) { RepeatLevelOneMasterFiles ( OutputFile ); }
    //if ( strstr ( MacroName, RepetitionMacroNames [ REPEAT_MASTER_APP ].MacroName ) == MacroName ) { RepeatLevelTwoMasterFiles ( OutputFile ); }

    //if ( strstr ( MacroName, RepetitionMacroNames [ REPEAT_SOURCE_PRJ ].MacroName ) == MacroName ) { RepeatRootSourceFiles     ( OutputFile ); }
    //if ( strstr ( MacroName, RepetitionMacroNames [ REPEAT_SOURCE_GRP ].MacroName ) == MacroName ) { RepeatLevelOneSourceFiles ( OutputFile ); }
    //if ( strstr ( MacroName, RepetitionMacroNames [ REPEAT_SOURCE_APP ].MacroName ) == MacroName ) { RepeatLevelTwoSourceFiles ( OutputFile ); }

    return ( FALSE );
}

//**********************************************************************************************************************************

bool TEMPLATEFILE::CheckCompanyMacros ( CHARACTER *MacroName )
{
    if ( ( MacroName [ 0 ] == 'C' ) && ( MacroName [ 1 ] == 'O' ) && ( MacroName [ 2 ] == 'M' ) )
    {
        if ( strstr ( MacroName, CompanyMacroNames [ LONG_ORIGONAL              ].MacroName ) == MacroName ) { strcpy ( OutputString, GetTool()->GetCompanyName ()->GetLong                     () ); return ( TRUE ); }
        if ( strstr ( MacroName, CompanyMacroNames [ LONG_LOWERCASE_UNDERSCORED ].MacroName ) == MacroName ) { strcpy ( OutputString, GetTool()->GetCompanyName ()->GetLongLowercaseUnderscored () ); return ( TRUE ); }
        if ( strstr ( MacroName, CompanyMacroNames [ LONG_UPPERCASE_UNDERSCORED ].MacroName ) == MacroName ) { strcpy ( OutputString, GetTool()->GetCompanyName ()->GetLongUppercaseUnderscored () ); return ( TRUE ); }
        if ( strstr ( MacroName, CompanyMacroNames [ LONG_LOWERCASE             ].MacroName ) == MacroName ) { strcpy ( OutputString, GetTool()->GetCompanyName ()->GetLongLowercase            () ); return ( TRUE ); }
        if ( strstr ( MacroName, CompanyMacroNames [ LONG_UPPERCASE             ].MacroName ) == MacroName ) { strcpy ( OutputString, GetTool()->GetCompanyName ()->GetLongUppercase            () ); return ( TRUE ); }
        if ( strstr ( MacroName, CompanyMacroNames [ LONG_CAPITALISED           ].MacroName ) == MacroName ) { strcpy ( OutputString, GetTool()->GetCompanyName ()->GetLongCapitalised          () ); return ( TRUE ); }
        if ( strstr ( MacroName, CompanyMacroNames [ SHORT_LOWERCASE            ].MacroName ) == MacroName ) { strcpy ( OutputString, GetTool()->GetCompanyName ()->GetShortLowercase           () ); return ( TRUE ); }
        if ( strstr ( MacroName, CompanyMacroNames [ SHORT_UPPERCASE            ].MacroName ) == MacroName ) { strcpy ( OutputString, GetTool()->GetCompanyName ()->GetShortUppercase           () ); return ( TRUE ); }
        if ( strstr ( MacroName, CompanyMacroNames [ SHORT_CAPITALISED          ].MacroName ) == MacroName ) { strcpy ( OutputString, GetTool()->GetCompanyName ()->GetShortCapitalised         () ); return ( TRUE ); }
    }

    return ( FALSE );
}

//**********************************************************************************************************************************

bool TEMPLATEFILE::CheckNameMacros ( CHARACTER *MacroName )
{
    NUMBER     Level;
    NUMBER     LevelNumber;
    NUMBER    *NodeNumbers;
    TOOL      *Tool;
    NAME      *Name = NULL;
    NODE      *NodeLevels     [ MAX_LEVELS ]; //!< A pointer to each node at each level for this branch of the tree
    CHARACTER *LevelMnemonics [ MAX_LEVELS ]; //!< The MNEMONIC name for each node level

    Tool = GetTool ();

    NodeNumbers = Node->GetNodeNumbers ();

    // populate the node levels and level mnemonics arrays from the level indices values

    NodeLevels [ 0 ] = Tool->GetRoot ();

    for ( Level = 0; Level < ( MAX_LEVELS - 1 ); Level++ )
    {
        LevelMnemonics [ Level ] = Tool->GetLevelMnemonic ( Level )->GetLong ();

        if ( NodeLevels [ Level ] != NULL ) // if this node is defined then get the sub node using the level index
        {
            if ( NodeNumbers [ Level + 1 ] != -1 ) // if the level index is valid then us it to get the sub node
            {
                NodeLevels [ Level + 1 ] = NodeLevels [ Level ]->GetSubNode ( NodeNumbers [ Level + 1 ] );
            }
            else
            {
                NodeLevels [ Level + 1 ] = NULL;
            }
        }
    }

    // check each level of the tree for macros for that level

    LevelNumber = Node->GetLevelNumber ();

    for ( Level = 0; Level <= LevelNumber; Level++ )
    {
        if ( ( MacroName [ 0 ] == LevelMnemonics [ Level ] [ 0 ] ) &&
             ( MacroName [ 1 ] == LevelMnemonics [ Level ] [ 1 ] ) &&
             ( MacroName [ 2 ] == LevelMnemonics [ Level ] [ 2 ] ) )
        {
			// Name = ? - this will depend on the level and the level names etc....
			
			/*
            if ( strstr ( MacroName, NameMacroNames [ LONG_ORIGONAL              ].MacroName ) != NULL ) { strcpy ( OutputString, Name->GetLong                     () ); return ( TRUE ); }
            if ( strstr ( MacroName, NameMacroNames [ LONG_LOWERCASE_UNDERSCORED ].MacroName ) != NULL ) { strcpy ( OutputString, Name->GetLongLowercaseUnderscored () ); return ( TRUE ); }
            if ( strstr ( MacroName, NameMacroNames [ LONG_UPPERCASE_UNDERSCORED ].MacroName ) != NULL ) { strcpy ( OutputString, Name->GetLongUppercaseUnderscored () ); return ( TRUE ); }
            if ( strstr ( MacroName, NameMacroNames [ LONG_LOWERCASE             ].MacroName ) != NULL ) { strcpy ( OutputString, Name->GetLongLowercase            () ); return ( TRUE ); }
            if ( strstr ( MacroName, NameMacroNames [ LONG_UPPERCASE             ].MacroName ) != NULL ) { strcpy ( OutputString, Name->GetLongUppercase            () ); return ( TRUE ); }
            if ( strstr ( MacroName, NameMacroNames [ LONG_CAPITALISED           ].MacroName ) != NULL ) { strcpy ( OutputString, Name->GetLongCapitalised          () ); return ( TRUE ); }
            if ( strstr ( MacroName, NameMacroNames [ SHORT_LOWERCASE            ].MacroName ) != NULL ) { strcpy ( OutputString, Name->GetShortLowercase           () ); return ( TRUE ); }
            if ( strstr ( MacroName, NameMacroNames [ SHORT_UPPERCASE            ].MacroName ) != NULL ) { strcpy ( OutputString, Name->GetShortUppercase           () ); return ( TRUE ); }
            if ( strstr ( MacroName, NameMacroNames [ SHORT_CAPITALISED          ].MacroName ) != NULL ) { strcpy ( OutputString, Name->GetShortCapitalised         () ); return ( TRUE ); }
			*/
        }
    }

    return ( FALSE );
}

//**********************************************************************************************************************************

bool TEMPLATEFILE::CheckStandardMacros ( CHARACTER *MacroName )
{
    if ( strstr ( MacroName, StandardMacroNames [ AUTHOR_NAME     ].MacroName ) == MacroName ) { strcpy ( OutputString, GetTool()->GetAuthorName        () ); return ( TRUE ); }
    if ( strstr ( MacroName, StandardMacroNames [ AUTHOR_INITIALS ].MacroName ) == MacroName ) { strcpy ( OutputString, GetTool()->GetAuthorInitials    () ); return ( TRUE ); }
    if ( strstr ( MacroName, StandardMacroNames [ FILE_DATE       ].MacroName ) == MacroName ) { strcpy ( OutputString, GetTool()->GetFileDate          () ); return ( TRUE ); }
    if ( strstr ( MacroName, StandardMacroNames [ FILE_LONGDATE   ].MacroName ) == MacroName ) { strcpy ( OutputString, GetTool()->GetFileTimeLong      () ); return ( TRUE ); }
    if ( strstr ( MacroName, StandardMacroNames [ FILE_TIME       ].MacroName ) == MacroName ) { strcpy ( OutputString, GetTool()->GetFileTime          () ); return ( TRUE ); }
    if ( strstr ( MacroName, StandardMacroNames [ FILE_YEAR       ].MacroName ) == MacroName ) { strcpy ( OutputString, GetTool()->GetFileYear          () ); return ( TRUE ); }
    if ( strstr ( MacroName, StandardMacroNames [ COMPANY_URL     ].MacroName ) == MacroName ) { strcpy ( OutputString, GetTool()->GetCompanyWebAddress () ); return ( TRUE ); }

    //if ( strstr ( MacroName, StandardMacroNames [ DEST_NAME ].MacroName ) == MacroName ) { strcpy ( OutputString, Parent->GetDestinationName      () ); return ( TRUE ); }
    //if ( strstr ( MacroName, StandardMacroNames [ DEST_EXT  ].MacroName ) == MacroName ) { strcpy ( OutputString, Parent->GetDestinationExtension () ); return ( TRUE ); }

    //if ( NodeLevels [ 2 ] )
    //{
    //    if ( strstr ( MacroName, StandardMacroNames [ DIRECTORY_NAME ].MacroName ) == MacroName ) // actual directory name used when creating directories
    //    {
    //        if ( NodeLevels [ 1 ]->GetUseMnemonicNames() )
    //        {
    //            strcpy ( OutputString, NodeLevels [ 2 ]->GetMnemonic () );
    //        }
    //        else
    //        {
    //            strcpy ( OutputString, NodeLevels [ 2 ]->GetName ()->GetShortCapitalised () );
    //        }
    //    }
    //    else
    //    {
    //        if ( strstr ( MacroName, StandardMacroNames [ MNEMONIC ].MacroName ) == MacroName )
    //        {
    //            strcpy ( OutputString, NodeLevels [ 2 ]->GetMnemonic () );
    //        }
    //    }
    //}

    return ( FALSE );
}

//**********************************************************************************************************************************

bool TEMPLATEFILE::CheckFileDescriptionMacros ( CHARACTER *MacroName )
{
    if ( ( MacroName [ 0 ] == 'F' ) && ( MacroName [ 1 ] == 'D' ) )
    {
        if ( strstr ( MacroName, FileDescriptionMacroNames [ LONG_ORIGONAL              ].MacroName ) == MacroName ) { strcpy ( OutputString, Parent->GetTemplatePurpose()->GetLong                     () ); return ( TRUE ); }
        if ( strstr ( MacroName, FileDescriptionMacroNames [ LONG_LOWERCASE_UNDERSCORED ].MacroName ) == MacroName ) { strcpy ( OutputString, Parent->GetTemplatePurpose()->GetLongLowercaseUnderscored () ); return ( TRUE ); }
        if ( strstr ( MacroName, FileDescriptionMacroNames [ LONG_UPPERCASE_UNDERSCORED ].MacroName ) == MacroName ) { strcpy ( OutputString, Parent->GetTemplatePurpose()->GetLongUppercaseUnderscored () ); return ( TRUE ); }
        if ( strstr ( MacroName, FileDescriptionMacroNames [ LONG_LOWERCASE             ].MacroName ) == MacroName ) { strcpy ( OutputString, Parent->GetTemplatePurpose()->GetLongLowercase            () ); return ( TRUE ); }
        if ( strstr ( MacroName, FileDescriptionMacroNames [ LONG_UPPERCASE             ].MacroName ) == MacroName ) { strcpy ( OutputString, Parent->GetTemplatePurpose()->GetLongUppercase            () ); return ( TRUE ); }
        if ( strstr ( MacroName, FileDescriptionMacroNames [ LONG_CAPITALISED           ].MacroName ) == MacroName ) { strcpy ( OutputString, Parent->GetTemplatePurpose()->GetLongCapitalised          () ); return ( TRUE ); }
        if ( strstr ( MacroName, FileDescriptionMacroNames [ SHORT_LOWERCASE            ].MacroName ) == MacroName ) { strcpy ( OutputString, Parent->GetTemplatePurpose()->GetShortLowercase           () ); return ( TRUE ); }
        if ( strstr ( MacroName, FileDescriptionMacroNames [ SHORT_UPPERCASE            ].MacroName ) == MacroName ) { strcpy ( OutputString, Parent->GetTemplatePurpose()->GetShortUppercase           () ); return ( TRUE ); }
        if ( strstr ( MacroName, FileDescriptionMacroNames [ SHORT_CAPITALISED          ].MacroName ) == MacroName ) { strcpy ( OutputString, Parent->GetTemplatePurpose()->GetShortCapitalised         () ); return ( TRUE ); }
    }

    return ( FALSE );
}

//**********************************************************************************************************************************

void TEMPLATEFILE::InsertSeperator ( FILE *OutputFile )
{
    NUMBER    CharacterCount;
    NUMBER    SeperatorLineWidth;
    CHARACTER SeperatorCharacter;
    CHARACTER SeperatorLine [ 2000 ];

    SeperatorLineWidth = GetTool()->GetSourceFileWidth ();

    SeperatorCharacter = GetTool()->GetSeperatorCharacter ();

    for ( CharacterCount = 0; CharacterCount < SeperatorLineWidth - 2; CharacterCount++ )
    {
        SeperatorLine [ CharacterCount ]  = SeperatorCharacter;
    }

    SeperatorLine [ CharacterCount ] = '\0';

    fprintf ( OutputFile, SeperatorLine );
}

//**********************************************************************************************************************************

void TEMPLATEFILE::InsertStartOfComment ( FILE *OutputFile )

{
    fprintf ( OutputFile, "/");
    fprintf ( OutputFile, "/");
}

//**********************************************************************************************************************************

void TEMPLATEFILE::InsertBlankComment ( FILE *OutputFile )

{
    fprintf ( OutputFile, "/");
    fprintf ( OutputFile, "/");

    fprintf ( OutputFile, "\n" );
}

//**********************************************************************************************************************************

void TEMPLATEFILE::InsertFullSeperator ( FILE *OutputFile )

{
    InsertStartOfComment ( OutputFile );

    InsertSeperator ( OutputFile );

    fprintf ( OutputFile, "\n" );
}

//**********************************************************************************************************************************

void TEMPLATEFILE::InsertProcedureHeader ( FILE *OutputFile )
{
    NUMBER    Count;
    NUMBER    ArgumentNumber;
    NUMBER    WhiteSpace;
    NUMBER    LongestReturn;
    NUMBER    LongestType;
    NUMBER    LongestName;
    NUMBER    ParameterLength;
    NUMBER    NumberOfPointerRedirections;
    NUMBER    LongestPointerRedirection;
    CHARACTER HeaderFirstLine [ 1000 ];

    ExtractFunctionDefinition ( MacroName );

    // first part is seperator

    InsertFullSeperator ( OutputFile );

    fprintf ( OutputFile, "//\n" );
    fprintf ( OutputFile, "//!  Function Name: %s\n", FunctionDefinition.FunctionName );
    fprintf ( OutputFile, "//\n" );
    fprintf ( OutputFile, "//!        Purpose: %s\n", FunctionDefinition.FunctionPurpose );
    fprintf ( OutputFile, "//\n" );
    fprintf ( OutputFile, "//!        Authors: %s (%s)\n", GetTool()->GetAuthorName (), GetTool()->GetAuthorInitials () );
    fprintf ( OutputFile, "//\n" );

    InsertFullSeperator ( OutputFile );

    LongestType               = 0;
    LongestName               = 0;
    LongestPointerRedirection = 0;

    fprintf ( OutputFile, "//\n" );
    fprintf ( OutputFile, "//!  Parameters\n" );
    fprintf ( OutputFile, "//!  ----------\n" );
    fprintf ( OutputFile, "//\n" );

    if ( FunctionDefinition.NumberOfArguments == 0 ) // function with old style single arguments, or just "void"
    {
        fprintf ( OutputFile, "//  NONE\n" );
        fprintf ( OutputFile, "//\n" );
    }
    else
    {
        // find out the longest type and name etc

        for ( ArgumentNumber = 0; ArgumentNumber < FunctionDefinition.NumberOfArguments; ArgumentNumber++ )
        {
            ParameterLength = (NUMBER) strlen ( FunctionDefinition.FunctionArguments [ ArgumentNumber ].ArgumentType );

            if ( ParameterLength > LongestType ) LongestType = ParameterLength;

            ParameterLength = (NUMBER) strlen ( FunctionDefinition.FunctionArguments [ ArgumentNumber ].ArgumentName );

            if ( ParameterLength > LongestName ) LongestName = ParameterLength;

            NumberOfPointerRedirections = FunctionDefinition.FunctionArguments [ ArgumentNumber ].NumberOfPointerRedirections;

            if ( NumberOfPointerRedirections > LongestPointerRedirection ) LongestPointerRedirection = NumberOfPointerRedirections;
        }

        fprintf ( OutputFile, "//!  %-*s  ", LongestType, "Type" );

        for ( Count = 0; Count < LongestPointerRedirection; Count++ ) fprintf ( OutputFile, " " );

        fprintf ( OutputFile, "%-*s  Purpose\n", LongestName, "Name" );

        fprintf ( OutputFile, "//!  %-*s  ", LongestType, "----" );

        for ( Count = 0; Count < LongestPointerRedirection; Count++ ) fprintf ( OutputFile, " " );

        fprintf ( OutputFile, "%-*s  -------\n", LongestName, "----" );

        fprintf ( OutputFile, "//\n" );

        for ( ArgumentNumber = 0; ArgumentNumber < FunctionDefinition.NumberOfArguments; ArgumentNumber++ )
        {
            fprintf ( OutputFile, "//!  %-*s  %+*s%-*s  NONE\n//\n",
                      LongestType,
                      FunctionDefinition.FunctionArguments [ ArgumentNumber ].ArgumentType,
                      LongestPointerRedirection,
                      FunctionDefinition.FunctionArguments [ ArgumentNumber ].RedirectionString,
                      LongestName,
                      FunctionDefinition.FunctionArguments [ ArgumentNumber ].ArgumentName );
        }
    }

    InsertFullSeperator ( OutputFile );

    fprintf ( OutputFile, "//\n" );
    fprintf ( OutputFile, "//!  Return Value\n" );
    fprintf ( OutputFile, "//!  ------------\n" );
    fprintf ( OutputFile, "//\n" );

    if ( strstr ( FunctionDefinition.FunctionReturnType, "void" ) != NULL ) // if "void" return type then no need to specify type and purpose
    {
        fprintf ( OutputFile, "//  NONE\n" );
    }
    else
    {
        LongestReturn = (NUMBER) strlen ( FunctionDefinition.FunctionReturnType );

        if ( LongestReturn < 4 ) LongestReturn = 4;

        fprintf ( OutputFile, "//!  %-*s  %s\n", LongestReturn, "Type", "Purpose" );
        fprintf ( OutputFile, "//!  %-*s  %s\n", LongestReturn, "----", "-------" );
        fprintf ( OutputFile, "//\n" );
        fprintf ( OutputFile, "//!  %-*s  %s\n", LongestReturn, FunctionDefinition.FunctionReturnType, "NONE" );
    }

    fprintf ( OutputFile, "//\n" );

    InsertFullSeperator ( OutputFile );

    fprintf ( OutputFile, "//\n" );
    fprintf ( OutputFile, "//!  Variables\n" );
    fprintf ( OutputFile, "//!  ---------\n" );
    fprintf ( OutputFile, "//\n" );
    fprintf ( OutputFile, "//  NONE\n" );
    fprintf ( OutputFile, "//\n" );

    InsertFullSeperator ( OutputFile );

    fprintf ( OutputFile, "//\n" );
    fprintf ( OutputFile, "//!  Description\n" );
    fprintf ( OutputFile, "//!  -----------\n" );
    fprintf ( OutputFile, "//\n" );
    fprintf ( OutputFile, "//\n" );

    InsertFullSeperator ( OutputFile );

    // now print the actual function declaration

    fprintf ( OutputFile, "\n" );

    sprintf ( HeaderFirstLine, "%s %s ( ",
              FunctionDefinition.FunctionReturnType,
              FunctionDefinition.FunctionName );

    WhiteSpace = (NUMBER) strlen ( HeaderFirstLine );

    fprintf ( OutputFile, HeaderFirstLine );

    if ( FunctionDefinition.NumberOfArguments == 0 )
    {
        fprintf ( OutputFile, "void )\n" );
    }
    else
    {
        fprintf ( OutputFile, "%-*s %+*s%s,\n",
                  LongestType,
                  FunctionDefinition.FunctionArguments [ 0 ].ArgumentType,
                  LongestPointerRedirection,
                  FunctionDefinition.FunctionArguments [ 0 ].RedirectionString,
                  FunctionDefinition.FunctionArguments [ 0 ].ArgumentName );

        for ( ArgumentNumber = 1; ArgumentNumber < FunctionDefinition.NumberOfArguments; ArgumentNumber++ )
        {
            for ( Count = 0; Count < WhiteSpace; Count++ ) fprintf ( OutputFile, " " );

            fprintf ( OutputFile, "%-*s %+*s%s",
                      LongestType,
                      FunctionDefinition.FunctionArguments [ ArgumentNumber ].ArgumentType,
                      LongestPointerRedirection,
                      FunctionDefinition.FunctionArguments [ ArgumentNumber ].RedirectionString,
                      FunctionDefinition.FunctionArguments [ ArgumentNumber ].ArgumentName );

            if ( ArgumentNumber < ( FunctionDefinition.NumberOfArguments - 1 ) )
            {
                fprintf ( OutputFile, ",\n" );
            }
            else
            {
                fprintf ( OutputFile, " )\n" );
            }
        }
    }

    fprintf ( OutputFile, "{\n" );

    fprintf ( OutputFile, "    ENTERING ( \"%s\" )", FunctionDefinition.FunctionName );
}

//**********************************************************************************************************************************

void TEMPLATEFILE::InsertFunctionEnding ( FILE *OutputFile )
{
    fprintf ( OutputFile, "    LEAVING ()\n" );

    fprintf ( OutputFile, "}" );
}

//**********************************************************************************************************************************

void TEMPLATEFILE::InsertProcedureEnding ( FILE *OutputFile )
{
    ExtractFunctionDefinition ( MacroName );

    fprintf ( OutputFile, "    LEAVING ()\n" );

    fprintf ( OutputFile, "\n" );

    fprintf ( OutputFile, "    return ( %s );\n", FunctionDefinition.FunctionPurpose );

    fprintf ( OutputFile, "}" );
}

//**********************************************************************************************************************************

void TEMPLATEFILE::RepeatLevelOne ( FILE *OutputFile )
{
    //NUMBER LevelOneIndex;
    //NUMBER NumberOfLevelOnes;
    //long   StartPosition;
    //long   EndPosition;

    //// start by reading the characters up to the next ${ENDGRP} macro (nested GROUP's not allowed)

    //StartPosition = GetFilePointer ();

    //FindEndOfMacro ( RepetitionMacroNames [ END_REPEAT_GRP ].MacroName );

    //EndPosition = GetFilePointer ();

    //// create a new TemplateFile for this subtext

    //NumberOfLevelOnes = GetTool()->GetRoot()->GetNumberOfSubNodes ();

    //if ( EndPosition > StartPosition )
    //{
    //    for ( LevelOneIndex = 0; LevelOneIndex < NumberOfLevelOnes; LevelOneIndex++ )
    //    {
    //        //TEMPLATEFILE *TemplateFile = new TEMPLATEFILE ( Parent,
    //        //                                                FileStore,
    //        //                                                StartPosition,
    //        //                                                EndPosition,
    //        //                                                LevelOneIndex,
    //        //                                                LevelTwoNumber );

    //        //TemplateFile->Scan ( MacroPrefix,
    //        //                     OutputFile,
    //        //                     LevelOneIndex,
    //        //                     LevelTwoNumber );

    //        //delete TemplateFile;
    //    }
    //}
}

//**********************************************************************************************************************************

void TEMPLATEFILE::RepeatApp ( FILE   *OutputFile,
                               NUMBER  LevelOneNumber )
{
    //NUMBER LevelTwoIndex;
    //NUMBER NumberOfLevelOnes;
    //NUMBER NumberOfLevelTwos;
    //long   StartPosition;
    //long   EndPosition;

    //// start by reading the characters up to the next ${ENDAPP} macro (nested GROUP's not allowed)

    //StartPosition = GetFilePointer ();

    //FindEndOfMacro ( RepetitionMacroNames [ END_REPEAT_APP ].MacroName );

    //EndPosition = GetFilePointer ();

    //// create a new TemplateFile for this subtext

    //NumberOfLevelOnes = GetTool()->GetRoot()->GetNumberOfSubNodes ();

    //if ( NumberOfLevelOnes > 0 )
    //{
    //    NumberOfLevelTwos = GetTool()->GetRoot()->GetSubNode ( LevelOneNumber )->GetNumberOfSubNodes ();

    //    if ( EndPosition > StartPosition )
    //    {
    //        for ( LevelTwoIndex = 0; LevelTwoIndex < NumberOfLevelTwos; LevelTwoIndex++ )
    //        {
    //            //TEMPLATEFILE *TemplateFile = new TEMPLATEFILE ( Parent,
    //            //                                                FileStore,
    //            //                                                StartPosition,
    //            //                                                EndPosition,
    //            //                                                LevelOneNumber,
    //            //                                                LevelTwoIndex );

    //            //TemplateFile->Scan ( MacroPrefix,
    //            //                     OutputFile,
    //            //                     LevelOneNumber,
    //            //                     LevelTwoIndex );

    //            //delete TemplateFile;
    //        }
    //    }
    //}
}

//**********************************************************************************************************************************

void TEMPLATEFILE::RepeatRootMasterFiles ( FILE *OutputFile )
{
    //NUMBER MasterFileIndex;
    //NUMBER NumberOfMasterFiles;
    //long   StartPosition;
    //long   EndPosition;

    //StartPosition = GetFilePointer ();

    //FindEndOfMacro ( RepetitionMacroNames [ END_REPEAT_MASTER_PRJ ].MacroName );

    //EndPosition = GetFilePointer ();

    //// create a new TemplateFile for this subtext

    //NumberOfMasterFiles = GetTool()->GetRoot()->GetNumberOfMasterFiles ();

    //if ( EndPosition > StartPosition )
    //{
    //    for ( MasterFileIndex = 0; MasterFileIndex < NumberOfMasterFiles; MasterFileIndex++ )
    //    {
    //        //TEMPLATEFILE *TemplateFile = new TEMPLATEFILE ( Parent,
    //        //                                                FileStore,
    //        //                                                StartPosition,
    //        //                                                EndPosition,
    //        //                                                LevelOneNumber,
    //        //                                                LevelTwoNumber,
    //        //                                                MasterFileIndex,
    //        //                                                SourceFileNumber );

    //        //TemplateFile->Scan ( MacroPrefix,
    //        //                     OutputFile,
    //        //                     LevelOneNumber,
    //        //                     LevelTwoNumber,
    //        //                     MasterFileIndex,
    //        //                     SourceFileNumber );

    //        //delete TemplateFile;
    //    }
    //}
}

//**********************************************************************************************************************************

void TEMPLATEFILE::RepeatLevelOneMasterFiles ( FILE *OutputFile )
{
    //NUMBER  MasterFileIndex;
    //NUMBER  NumberOfMasterFiles;
    //long    StartPosition;
    //long    EndPosition;
    //NODE   *LevelOne;

    //StartPosition = GetFilePointer ();

    //FindEndOfMacro ( RepetitionMacroNames [ END_REPEAT_MASTER_GRP ].MacroName );

    //EndPosition = GetFilePointer ();

    //// create a new TemplateFile for this subtext

    //LevelOne = GetTool()->GetRoot()->GetSubNode ( LevelOneNumber );

    //if ( LevelOne )
    //{
    //    NumberOfMasterFiles = LevelOne->GetNumberOfMasterFiles ();

    //    if ( EndPosition > StartPosition )
    //    {
    //        for ( MasterFileIndex = 0; MasterFileIndex < NumberOfMasterFiles; MasterFileIndex++ )
    //        {
    //            //TEMPLATEFILE *TemplateFile = new TEMPLATEFILE ( Parent,
    //            //                                                FileStore,
    //            //                                                StartPosition,
    //            //                                                EndPosition,
    //            //                                                LevelOneNumber,
    //            //                                                LevelTwoNumber,
    //            //                                                MasterFileIndex,
    //            //                                                SourceFileNumber );

    //            //TemplateFile->Scan ( MacroPrefix,
    //            //                     OutputFile,
    //            //                     LevelOneNumber,
    //            //                     LevelTwoNumber,
    //            //                     MasterFileIndex,
    //            //                     SourceFileNumber );

    //            //delete TemplateFile;
    //        }
    //    }
    //}
}

//**********************************************************************************************************************************

void TEMPLATEFILE::RepeatLevelTwoMasterFiles ( FILE *OutputFile )
{
    //NUMBER  MasterFileIndex;
    //NUMBER  NumberOfMasterFiles;
    //long    StartPosition;
    //long    EndPosition;
    //NODE   *LevelOne;
    //NODE   *LevelTwo;

    //StartPosition = GetFilePointer ();

    //FindEndOfMacro ( RepetitionMacroNames [ END_REPEAT_MASTER_APP ].MacroName );

    //EndPosition = GetFilePointer ();

    //// create a new TemplateFile for this subtext

    //LevelOne = GetTool()->GetRoot()->GetSubNode ( LevelOneNumber );

    //if ( LevelOne )
    //{
    //    LevelTwo = LevelOne->GetSubNode ( LevelTwoNumber );

    //    if ( LevelTwo )
    //    {
    //        NumberOfMasterFiles = LevelTwo->GetNumberOfMasterFiles ();

    //        if ( EndPosition > StartPosition )
    //        {
    //            for ( MasterFileIndex = 0; MasterFileIndex < NumberOfMasterFiles; MasterFileIndex++ )
    //            {
    //                TEMPLATEFILE *TemplateFile = new TEMPLATEFILE ( Parent,
    //                                                                FileStore,
    //                                                                StartPosition,
    //                                                                EndPosition,
    //                                                                LevelOneNumber,
    //                                                                LevelTwoNumber,
    //                                                                MasterFileIndex,
    //                                                                SourceFileNumber );

    //                TemplateFile->Scan ( MacroPrefix,
    //                                     OutputFile,
    //                                     LevelOneNumber,
    //                                     LevelTwoNumber,
    //                                     MasterFileIndex,
    //                                     SourceFileNumber );

    //                delete TemplateFile;
    //            }
    //        }
    //    }
    //}
}

//**********************************************************************************************************************************

void TEMPLATEFILE::RepeatRootSourceFiles ( FILE *OutputFile )
{
    //NUMBER SourceFileIndex;
    //NUMBER NumberOfSourceFiles;
    //long   StartPosition;
    //long   EndPosition;

    //StartPosition = GetFilePointer ();

    //FindEndOfMacro ( RepetitionMacroNames [ END_REPEAT_SOURCE_PRJ ].MacroName );

    //EndPosition = GetFilePointer ();

    //// create a new TemplateFile for this subtext

    //NumberOfSourceFiles = GetTool()->GetRoot()->GetNumberOfSourceFiles ();

    //if ( EndPosition > StartPosition )
    //{
    //    for ( SourceFileIndex = 0; SourceFileIndex < NumberOfSourceFiles; SourceFileIndex++ )
    //    {
    //        TEMPLATEFILE *TemplateFile = new TEMPLATEFILE ( Parent,
    //                                                        FileStore,
    //                                                        StartPosition,
    //                                                        EndPosition,
    //                                                        LevelOneNumber,
    //                                                        LevelTwoNumber,
    //                                                        MasterFileNumber,
    //                                                        SourceFileIndex );

    //        TemplateFile->Scan ( MacroPrefix,
    //                             OutputFile,
    //                             LevelOneNumber,
    //                             LevelTwoNumber,
    //                             MasterFileNumber,
    //                             SourceFileIndex );

    //        delete TemplateFile;
    //    }
    //}
}

//**********************************************************************************************************************************

void TEMPLATEFILE::RepeatLevelOneSourceFiles ( FILE *OutputFile )
{
    //NUMBER  SourceFileIndex;
    //NUMBER  NumberOfSourceFiles;
    //long    StartPosition;
    //long    EndPosition;
    //NODE   *LevelOne;

    //StartPosition = GetFilePointer ();

    //FindEndOfMacro ( RepetitionMacroNames [ END_REPEAT_SOURCE_GRP ].MacroName );

    //EndPosition = GetFilePointer ();

    //// create a new TemplateFile for this subtext

    //LevelOne = GetTool()->GetRoot()->GetSubNode ( LevelOneNumber );

    //if ( LevelOne )
    //{
    //    NumberOfSourceFiles = LevelOne->GetNumberOfSourceFiles ();

    //    if ( EndPosition > StartPosition )
    //    {
    //        for ( SourceFileIndex = 0; SourceFileIndex < NumberOfSourceFiles; SourceFileIndex++ )
    //        {
    //            TEMPLATEFILE *TemplateFile = new TEMPLATEFILE ( Parent,
    //                                                            FileStore,
    //                                                            StartPosition,
    //                                                            EndPosition,
    //                                                            LevelOneNumber,
    //                                                            LevelTwoNumber,
    //                                                            MasterFileNumber,
    //                                                            SourceFileIndex );

    //            TemplateFile->Scan ( MacroPrefix,
    //                                 OutputFile,
    //                                 LevelOneNumber,
    //                                 LevelTwoNumber,
    //                                 MasterFileNumber,
    //                                 SourceFileIndex );

    //            delete TemplateFile;
    //        }
    //    }
    //}
}

//**********************************************************************************************************************************

void TEMPLATEFILE::RepeatLevelTwoSourceFiles ( FILE *OutputFile )
{
    //NUMBER  SourceFileIndex;
    //NUMBER  NumberOfSourceFiles;
    //long    StartPosition;
    //long    EndPosition;
    //NODE   *LevelOne;
    //NODE   *LevelTwo;

    //StartPosition = GetFilePointer ();

    //FindEndOfMacro ( RepetitionMacroNames [ END_REPEAT_SOURCE_APP ].MacroName );

    //EndPosition = GetFilePointer ();

    //// create a new TemplateFile for this subtext

    //LevelOne = GetTool()->GetRoot()->GetSubNode ( LevelOneNumber );

    //if ( LevelOne )
    //{
    //    LevelTwo = LevelOne->GetSubNode ( LevelTwoNumber );

    //    if ( LevelTwo )
    //    {
    //        NumberOfSourceFiles = LevelTwo->GetNumberOfSourceFiles ();

    //        if ( EndPosition > StartPosition )
    //        {
    //            for ( SourceFileIndex = 0; SourceFileIndex < NumberOfSourceFiles; SourceFileIndex++ )
    //            {
    //                TEMPLATEFILE *TemplateFile = new TEMPLATEFILE ( Parent,
    //                                                                FileStore,
    //                                                                StartPosition,
    //                                                                EndPosition,
    //                                                                LevelOneNumber,
    //                                                                LevelTwoNumber,
    //                                                                MasterFileNumber,
    //                                                                SourceFileIndex );

    //                TemplateFile->Scan ( MacroPrefix,
    //                                     OutputFile,
    //                                     LevelOneNumber,
    //                                     LevelTwoNumber,
    //                                     MasterFileNumber,
    //                                     SourceFileIndex );

    //                delete TemplateFile;
    //            }
    //        }
    //    }
    //}
}

//**********************************************************************************************************************************

void TEMPLATEFILE::FindEndOfMacro ( CHARACTER *MacroName )

{
    NUMBER    NameIndex;
    CHARACTER Character;
    CHARACTER EndMacroName [ 100 ];

    while ( ! EndOfFile () )
    {
        Character = GetCharacter ();

        if ( Character == MacroPrefix [ 0 ] )
        {
            Character = GetCharacter ();

            if ( Character == '{' )
            {
                // get macro

                NameIndex = 0;

                while ( ( Character != '}' ) && ( ! EndOfFile () ) && ( NameIndex < sizeof ( EndMacroName ) ) )
                {
                    Character = GetCharacter ();

                    EndMacroName [ NameIndex++ ] = Character;
                }

                EndMacroName [ NameIndex ] = '\0';

                if ( strstr ( EndMacroName, MacroName ) == EndMacroName ) break;
            }
        }
    }
}

//**********************************************************************************************************************************

void TEMPLATEFILE::ExtractFunctionDefinition ( CHARACTER *MacroName )
{
    NUMBER     Count;
    NUMBER     Index;
    NUMBER     ArgumentLength;
    NUMBER     NumberOfPointerRedirections;
    CHARACTER *FirstBracket;
    CHARACTER *LastBracket;
    CHARACTER *CharPointer;
    CHARACTER *ArgumentPointer;
    CHARACTER *ArgumentType;
    CHARACTER *ArgumentName;
    CHARACTER *ParameterPointer;
    CHARACTER  MacroCopy       [ 100000 ];
    CHARACTER  ParameterString [ 100000 ];
    CHARACTER  ParameterCopy   [ 100000 ];

    // make a copy of the macro name and remove any white space from the end of it

    strcpy ( MacroCopy, MacroName );

    ArgumentPointer = &MacroCopy [ strlen ( MacroCopy ) - 1 ];

    while ( ( *ArgumentPointer == '\r' ) || ( *ArgumentPointer == '\n' ) || ( *ArgumentPointer == '\t' ) || ( *ArgumentPointer == ' ' ) )
    {
        *ArgumentPointer-- = '\0';
    }

    // extract the macro text

    FirstBracket = strchr ( MacroCopy, '(' );

    if ( FirstBracket != NULL )
    {
        LastBracket = strchr ( MacroCopy, ')' );

        if ( LastBracket != NULL )
        {
            // make a copy of the string for strtok to process (but without the brackets)

            ParameterPointer = &ParameterString [ 0 ];

            CharPointer = &FirstBracket [ 1 ];

            while ( CharPointer < LastBracket )
            {
                *ParameterPointer++ = *CharPointer++;
            }

            *ParameterPointer = '\0';

            // clear the parameter definition from last time

            memset ( FunctionDefinition.FunctionPurpose,    0, sizeof ( FunctionDefinition.FunctionPurpose    ) );
            memset ( FunctionDefinition.FunctionReturnType, 0, sizeof ( FunctionDefinition.FunctionReturnType ) );
            memset ( FunctionDefinition.FunctionName,       0, sizeof ( FunctionDefinition.FunctionName       ) );

            FunctionDefinition.NumberOfArguments = 0;

            // now extract the parameters themselves

            CharPointer = strtok ( ParameterString, "," );

            if ( CharPointer != NULL )
            {
                strcpy ( ParameterCopy, CharPointer );

                ArgumentPointer = CleanParameter ( ParameterCopy );

                strcpy ( FunctionDefinition.FunctionPurpose, ArgumentPointer );

                CharPointer = strtok ( NULL, "," );

                if ( CharPointer != NULL )
                {
                    strcpy ( ParameterCopy, CharPointer );

                    ArgumentPointer = CleanParameter ( ParameterCopy );

                    strcpy ( FunctionDefinition.FunctionReturnType, ArgumentPointer );

                    CharPointer = strtok ( NULL, "," );

                    if ( CharPointer != NULL )
                    {
                        strcpy ( ParameterCopy, CharPointer );

                        ArgumentPointer = CleanParameter ( ParameterCopy );

                        strcpy ( FunctionDefinition.FunctionName, ArgumentPointer );

                        printf ( "ProcedureDefinition is: %s\n", FunctionDefinition.FunctionPurpose    );
                        printf ( "                        %s\n", FunctionDefinition.FunctionReturnType );
                        printf ( "                        %s\n", FunctionDefinition.FunctionName       );

                        // check function arguments if any

                        CharPointer = strtok ( NULL, "," );

                        if ( CharPointer != NULL )
                        {
                            if ( strstr ( CharPointer, "void" ) == NULL ) // if the first argument is void then stop here
                            {
                                // get and clean the argument type

                                strcpy ( ParameterCopy, CharPointer );

                                ArgumentPointer = CleanParameter ( ParameterCopy );

                                strcpy ( FunctionDefinition.FunctionArguments [ FunctionDefinition.NumberOfArguments ].ArgumentType, ArgumentPointer );

                                CharPointer = strtok ( NULL, "," );

                                if ( CharPointer != NULL )
                                {
                                    // get and clean the argument name

                                    strcpy ( ParameterCopy, CharPointer );

                                    ArgumentPointer = CleanParameter ( ParameterCopy );

                                    strcpy ( FunctionDefinition.FunctionArguments [ FunctionDefinition.NumberOfArguments ].ArgumentName, ArgumentPointer );
                                }

                                FunctionDefinition.NumberOfArguments++;

                                while ( CharPointer != NULL )
                                {
                                    CharPointer = strtok ( NULL, "," );

                                    if ( CharPointer != NULL )
                                    {
                                        // get and clean the argument type

                                        strcpy ( ParameterCopy, CharPointer );

                                        ArgumentPointer = CleanParameter ( ParameterCopy );

                                        strcpy ( FunctionDefinition.FunctionArguments [ FunctionDefinition.NumberOfArguments ].ArgumentType, ArgumentPointer );

                                        CharPointer = strtok ( NULL, "," );

                                        if ( CharPointer != NULL )
                                        {
                                            // get and clean the argument name

                                            strcpy ( ParameterCopy, CharPointer );

                                            ArgumentPointer = CleanParameter ( ParameterCopy );

                                            strcpy ( FunctionDefinition.FunctionArguments [ FunctionDefinition.NumberOfArguments ].ArgumentName, ArgumentPointer );
                                        }

                                        FunctionDefinition.NumberOfArguments++;
                                    }
                                }

                                printf ( "                        %d arguments\n", FunctionDefinition.NumberOfArguments );

                                // now process the arguments to extact pointer redirections etc

                                for ( Index = 0; Index < FunctionDefinition.NumberOfArguments; Index++ )
                                {
                                    ArgumentType = FunctionDefinition.FunctionArguments [ Index ].ArgumentType;

                                    printf ( "                        argument type [%d] = %s\n", Index, ArgumentType );

                                    // check the argument type for pointer redirections

                                    NumberOfPointerRedirections = 0;

                                    ArgumentLength = (NUMBER) strlen ( ArgumentType );

                                    if ( ArgumentLength > 0 )
                                    {
                                        CharPointer = &ArgumentType [ ArgumentLength - 1 ];

                                        while ( *CharPointer == '*' )
                                        {
                                            NumberOfPointerRedirections++;

                                            *CharPointer-- = '\0';
                                        }

                                        // now clean the processed argument type

                                        strcpy ( ParameterCopy, ArgumentType );

                                        ArgumentPointer = CleanParameter ( ParameterCopy );

                                        strcpy ( ArgumentType, ArgumentPointer );
                                    }

                                    // check the argument name for array brackets and change them to pointer redirections

                                    ArgumentName = FunctionDefinition.FunctionArguments [ Index ].ArgumentName;

                                    printf ( "                        argument name [%d] = %s\n", Index, ArgumentName );

                                    ArgumentLength = (NUMBER) strlen ( ArgumentName );

                                    if ( ArgumentLength > 0 )
                                    {
                                        ArgumentPointer = strchr ( ArgumentName, '[' );

                                        if ( ArgumentPointer != NULL )
                                        {
                                            *ArgumentPointer = '\0';

                                            NumberOfPointerRedirections++;
                                        }

                                        // now clean the processed argument name

                                        strcpy ( ParameterCopy, ArgumentName );

                                        ArgumentPointer = CleanParameter ( ParameterCopy );

                                        strcpy ( ArgumentName, ArgumentPointer );
                                    }

                                    // save the number of pointer rredirections found and build a suitable text string

                                    FunctionDefinition.FunctionArguments [ Index ].NumberOfPointerRedirections = NumberOfPointerRedirections;

                                    for ( Count = 0; Count < NumberOfPointerRedirections; Count++ )
                                    {
                                        FunctionDefinition.FunctionArguments [ Index ].RedirectionString [ Count ] = '*';
                                    }

                                    FunctionDefinition.FunctionArguments [ Index ].RedirectionString [ Count ] = '\0';
                                }
                            }
                        }
                        else
                        {
                            fprintf ( GetTool()->GetLogFile(), "                Parameterised Macro %s has function arguments\n", MacroCopy );
                        }
                    }
                    else
                    {
                        fprintf ( GetTool()->GetLogFile(), "                Parameterised Macro %s has no function name\n", MacroCopy );
                    }
                }
                else
                {
                    fprintf ( GetTool()->GetLogFile(), "                Parameterised Macro %s has no return type\n", MacroCopy );
                }
            }
            else
            {
                fprintf ( GetTool()->GetLogFile(), "                Parameterised Macro %s has no function purpose\n", MacroCopy );
            }
        }
        else
        {
            fprintf ( GetTool()->GetLogFile(), "                Parameterised Macro %s does not have an ending bracket\n", MacroCopy );
        }
    }
    else
    {
        fprintf ( GetTool()->GetLogFile(), "                Parameterised Macro %s does not have a starting bracket\n", MacroCopy );
    }
}

//**********************************************************************************************************************************

CHARACTER *TEMPLATEFILE::CleanParameter ( CHARACTER *Parameter )
{
    CHARACTER *StartOfParameter;
    CHARACTER *EndOfParameter;

    StartOfParameter = &Parameter [ 0 ];
    EndOfParameter   = &Parameter [ strlen ( Parameter ) - 1 ];

    // remove any extra spaces and tabs and new lines from end of the parameter

    while ( ( *EndOfParameter == 0x09 ) || ( *EndOfParameter == 0x0A ) || ( *EndOfParameter == 0x0D ) || ( *EndOfParameter == 0x20 ) )
    {
        *EndOfParameter = '\0';

        EndOfParameter--;

//        printf ( "In CleanParameter [scanning end], Parameter=%s\n", Parameter );

        if ( EndOfParameter < StartOfParameter ) break; // don't go past the beggining of the parameter if it is actually just all white space
    }

    // remove any extra spaces and tabs and new lines at the start of the parameter

    while ( ( *StartOfParameter == 0x09 ) || ( *StartOfParameter == 0x0A ) || ( *StartOfParameter == 0x0D ) || ( *StartOfParameter == 0x20 ) )
    {
        *StartOfParameter = '\0';

        StartOfParameter++;

//        printf ( "In CleanParameter [ scanning start], Parameter=%s\n", StartOfParameter );

        if ( StartOfParameter > EndOfParameter ) break; // don't go past the end of the parameter if it is actually just all white space
    }

    return ( StartOfParameter );
}

//**********************************************************************************************************************************

void PrintMacroNames ( void )
{
   NUMBER Index;

   printf ( "\n" );
   printf ( "Master File Macro Names\n" );
   printf ( "-----------------------\n" );
   printf ( "\n" );

   for ( Index = 0; Index < MAX_MACRO_NAMES_PER_TABLE; Index++ )
   {
      if ( MasterFileMacroNames [ Index ].MacroName [ 0 ] == 'Z' ) break;

      printf ( "%+*s: %s\n", 15, MasterFileMacroNames [ Index ].MacroName, MasterFileMacroNames [ Index ].MethodName );
   }

   printf ( "\n" );
   printf ( "Master File Macro Names (Append Backslash)\n" );
   printf ( "------------------------------------------\n" );
   printf ( "\n" );

   for ( Index = 0; Index < MAX_MACRO_NAMES_PER_TABLE; Index++ )
   {
      if ( MasterFileMacroNamesBackslash [ Index ].MacroName [ 0 ] == 'Z' ) break;

      printf ( "%+*s: %s\n", 15, MasterFileMacroNamesBackslash [ Index ].MacroName, MasterFileMacroNamesBackslash [ Index ].MethodName );
   }

   printf ( "\n" );
   printf ( "Source File Macro Names\n" );
   printf ( "-----------------------\n" );
   printf ( "\n" );

   for ( Index = 0; Index < MAX_MACRO_NAMES_PER_TABLE; Index++ )
   {
      if ( SourceFileMacroNames [ Index ].MacroName [ 0 ] == 'Z' ) break;

      printf ( "%+*s: %s\n", 15, SourceFileMacroNames [ Index ].MacroName, SourceFileMacroNames [ Index ].MethodName );
   }

   printf ( "\n" );
   printf ( "Source File Macro Names (Append Backslash)\n" );
   printf ( "------------------------------------------\n" );
   printf ( "\n" );

   for ( Index = 0; Index < MAX_MACRO_NAMES_PER_TABLE; Index++ )
   {
      if ( SourceFileMacroNamesBackslash [ Index ].MacroName [ 0 ] == 'Z' ) break;

      printf ( "%+*s: %s\n", 15, SourceFileMacroNamesBackslash [ Index ].MacroName, SourceFileMacroNamesBackslash [ Index ].MethodName );
   }

   printf ( "\n" );
   printf ( "Repetition Macro Names\n" );
   printf ( "----------------------\n" );
   printf ( "\n" );

   for ( Index = 0; Index < MAX_MACRO_NAMES_PER_TABLE; Index++ )
   {
      if ( RepetitionMacroNames [ Index ].MacroName [ 0 ] == 'Z' ) break;

      printf ( "%+*s: %s\n", 15, RepetitionMacroNames [ Index ].MacroName, RepetitionMacroNames [ Index ].MethodName );
   }

   printf ( "\n" );
   printf ( "Function Macro Names\n" );
   printf ( "--------------------\n" );
   printf ( "\n" );

   for ( Index = 0; Index < MAX_MACRO_NAMES_PER_TABLE; Index++ )
   {
      if ( FunctionMacroNames [ Index ].MacroName [ 0 ] == 'Z' ) break;

      printf ( "%+*s: %s\n", 15, FunctionMacroNames [ Index ].MacroName, FunctionMacroNames [ Index ].MethodName );
   }

   printf ( "\n" );
   printf ( "Company Macro Names\n" );
   printf ( "-------------------\n" );
   printf ( "\n" );

   for ( Index = 0; Index < MAX_MACRO_NAMES_PER_TABLE; Index++ )
   {
      if ( CompanyMacroNames [ Index ].MacroName [ 0 ] == 'Z' ) break;

      printf ( "%+*s: %s\n", 15, CompanyMacroNames [ Index ].MacroName, CompanyMacroNames [ Index ].MethodName );
   }

   printf ( "\n" );
   printf ( "Name Macro Names\n" );
   printf ( "----------------\n" );
   printf ( "\n" );

   for ( Index = 0; Index < MAX_MACRO_NAMES_PER_TABLE; Index++ )
   {
      if ( NameMacroNames [ Index ].MacroName [ 0 ] == 'Z' ) break;

      printf ( "%+*s: %s\n", 15, NameMacroNames [ Index ].MacroName, NameMacroNames [ Index ].MethodName );
   }

   printf ( "\n" );
   printf ( "Standard Macro Names\n" );
   printf ( "--------------------\n" );
   printf ( "\n" );

   for ( Index = 0; Index < MAX_MACRO_NAMES_PER_TABLE; Index++ )
   {
      if ( StandardMacroNames [ Index ].MacroName [ 0 ] == 'Z' ) break;

      printf ( "%+*s: %s\n", 15, StandardMacroNames [ Index ].MacroName, StandardMacroNames [ Index ].MethodName );
   }

   printf ( "\n" );
   printf ( "File Description Macro Names\n" );
   printf ( "----------------------------\n" );
   printf ( "\n" );

   for ( Index = 0; Index < MAX_MACRO_NAMES_PER_TABLE; Index++ )
   {
      if ( FileDescriptionMacroNames [ Index ].MacroName [ 0 ] == 'Z' ) break;

      printf ( "%+*s: %s\n", 15, FileDescriptionMacroNames [ Index ].MacroName, FileDescriptionMacroNames [ Index ].MethodName );
   }
}

//**********************************************************************************************************************************
