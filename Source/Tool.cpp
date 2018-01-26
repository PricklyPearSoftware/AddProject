
//**********************************************************************************************************************************
//
//   Purpose: Add Project Utility source code file
//
//   Project: Development Utilities
//
//  Filename: Tool.cpp
//
//   Authors: Caroline.M.Mathieson (CMM)
//
//**********************************************************************************************************************************
//
//  Description
//  -----------
//
//! \file Tool.cpp
//! \brief Contains the implementation of the TOOL Object.
//!
//! This file contains the implementation of the TOOL Object.
//!
//**********************************************************************************************************************************

#include "AddProject.h"

//**********************************************************************************************************************************

static char *WeekDays [] =
{
    "Sunday",
    "Monday",
    "Tuesday",
    "Wednesday",
    "Thursday",
    "Friday",
    "Saturday"
};

//**********************************************************************************************************************************

static char *MonthDays [] =
{
    "0th",  "1st",  "2nd",  "3rd",  "4th",  "5th",  "6th",  "7th",  "8th",  "9th",
    "10th", "11th", "12th", "13th", "14th", "15th", "16th", "17th", "18th", "19th",
    "20th", "21st", "22nd", "23rd", "24th", "25th", "26th", "27th", "28th", "29th",
    "30th", "31st"
};

//**********************************************************************************************************************************

static char *Months [] =
{
    "January",
    "February",
    "March",
    "April",
    "May",
    "June",
    "July",
    "August",
    "September",
    "October",
    "November",
    "December",
};

//**********************************************************************************************************************************

static char *DefaultLevelNames [] =
{
    "DefaultRoot",
    "DefaultLevelOne",
    "DefaultLevelTwo",
    "DefaultLevelThree",
    "DefaultLevelFour",
    "DefaultLevelFive",
    "DefaultLevelSix",
    "DefaultLevelSeven",
    "DefaultLevelEight",
    "DefaultLevelNine",
};

//**********************************************************************************************************************************

static char *DefaultMnemonicNames [] = /* Default Level Mnemonic # */
{
    "DLM0",
    "DLM1",
    "DLM2",
    "DLM3",
    "DLM4",
    "DLM5",
    "DLM6",
    "DLM7",
    "DLM8",
    "DLM9",
};

//**********************************************************************************************************************************
/**
\brief The constructor which initialises the variables and records the date and time.

\param NewProjectSpecificationFileName The full filename of the project specification file

\param NewTemplateDirectoryFileName The full filename of the template directory

The only actions in the constructor are to initialise all the variables and record the exact date and time of invokation. The real
work is done by the TOOL::Execute() method.

**/
//**********************************************************************************************************************************

TOOL::TOOL ( CHARACTER *NewProjectSpecificationFileName,
             CHARACTER *NewTemplateDirectoryFileName,
             CHARACTER *NewOutputDirectoryFileName )
{
    NUMBER     LevelNumber;
    time_t     CurrentTime;
    struct tm *LocalTime;

    //
    // Initialise all the variables
    //

    strcpy ( ProjectSpecificationFileName, NewProjectSpecificationFileName );
    strcpy ( TemplateDirectoryFileName,    NewTemplateDirectoryFileName    );
	strcpy ( OutputDirectoryFileName,      NewOutputDirectoryFileName      );

    memset ( LogFileName,   0, sizeof ( LogFileName   ) );
    memset ( DebugFileName, 0, sizeof ( DebugFileName ) );

    LogFile = NULL;

    DebugFile = NULL;

    Root = NULL;

    IndentStep = 4;

    IndentLevel = 30;

    CompanyName = NULL;

    memset ( CompanyWebAddress, 0, sizeof ( CompanyWebAddress ) );
    memset ( AuthorName,        0, sizeof ( AuthorName        ) );
    memset ( AuthorInitials,    0, sizeof ( AuthorInitials    ) );

    SourceFileWidth = 132;

    SeperatorCharacter = '*';

    NumberOfLevels = 0;

    for ( LevelNumber = 0; LevelNumber < MAX_LEVELS; LevelNumber++ )
    {
        LevelNames [ LevelNumber ] = NULL;

        LevelMnemonics [ LevelNumber ] = NULL;
    }

    //
    // Generate the date and time automatically
    //

    time ( &CurrentTime );

    LocalTime = localtime ( &CurrentTime );

    sprintf ( FileDate,
              "%02d.%02d.%02d",
              LocalTime->tm_mday,
              LocalTime->tm_mon + 1,
              LocalTime->tm_year % 100 );

    sprintf ( FileTime,
              "%02d:%02d:%02d",
              LocalTime->tm_hour,
              LocalTime->tm_min,
              LocalTime->tm_sec );

    sprintf ( FileTimeLong,
              "%s %s of %s %4d",
              WeekDays  [ LocalTime->tm_wday ],
              MonthDays [ LocalTime->tm_mday ],
              Months    [ LocalTime->tm_mon  ],
              LocalTime->tm_year + 1900 );

    sprintf ( FileYear,
              "%4d",
              LocalTime->tm_year + 1900 );
}

//**********************************************************************************************************************************

TOOL::~TOOL ( void )
{
    NUMBER LevelNumber;

    if ( CompanyName != NULL ) delete CompanyName;

    for ( LevelNumber = 0; LevelNumber < MAX_LEVELS; LevelNumber++ )
    {
        if ( LevelNames [ LevelNumber ] != NULL ) delete ( LevelNames [ LevelNumber ] );
    }
}

//**********************************************************************************************************************************
/**
\brief The key method of the class. The one which doeas all the work.

An attempt is then made to create a log file with the same name as the project specification file but with the extension ".log". If
this attempt is successful then an attempt is made to create a debug file. If this attempt is also successful then an attempt is
made to create the topmost NODE. If any of these attempts fail then suitable error messages are generated.

If all attempts are succesful then the various member functions of the PROJECT object are called in the right order to create the
desired system. Copious logs are kept of the process.

The log file and debug file are opened first so that any complex error messages from the creation of objects can be recorded.

**/
//**********************************************************************************************************************************

void TOOL::Execute ( void )
{
    CHARACTER Drive            [ MAX_PATH_NAME ];
    CHARACTER Directory        [ MAX_PATH_NAME ];
    CHARACTER SourceName       [ MAX_PATH_NAME ];
    CHARACTER SourceExtension  [ MAX_PATH_NAME ];
    CHARACTER TemplateFileName [ MAX_PATH_NAME ];

    //
    // Check that the initialisation file exists first
    //

    FILE *InitialisationFile = fopen ( ProjectSpecificationFileName, "rt" );

    if ( InitialisationFile != NULL )
    {
       // we are finished with the file for now so close it

       fclose ( InitialisationFile );

       //
       // Check that the templates directory exists
       //

       sprintf ( TemplateFileName, "%s\\index.html", TemplateDirectoryFileName ); // this file should exist

       FILE *TemplateFile = fopen ( TemplateFileName, "rt" );

       if ( TemplateFile != NULL )
       {
          // we are finished with the file for now so close it

          fclose ( TemplateFile );

          //
          // Try to create the log file based on the project name
          //

          FileNameSplit ( ProjectSpecificationFileName,
                          Drive,
                          Directory,
                          SourceName,
                          SourceExtension );

          sprintf ( LogFileName, "%s\\%s\\%s.log", Drive, Directory, SourceName );

          LogFile = fopen ( LogFileName, "wt" );

          if ( LogFile != NULL )
          {
              //
              // OK, so try creating the debug file
              //

              sprintf ( DebugFileName, "%s\\%s\\Debug.log", Drive, Directory );

              DebugFile = fopen ( DebugFileName, "wt" );

              if ( DebugFile != NULL )
              {
                  //
                  // OK, so try to create the root NODE object
                  //

                  fprintf ( LogFile, GetTitleText () );

                  Root = new NODE ( NULL, DefaultLevelNames [ 0 ], 0, Root->GetNodeNumbers(), 0 );

                  if ( Root != NULL )
                  {
                      printf ( "Creating Object Tree from INI file\n" );

                      Load (); // this will create any required sub nodes of project

                      printf ( "\nAdding Root Files\n" );

                      Root->AddRootFiles ();

                      printf ( "\nAdding Master Files\n" );

                      Root->AddMasterFiles ();

                      printf ( "\nAdding Template Files\n" );

                      Root->AddTemplateFiles ();

                      printf ( "\nAdding Source Files\n" );

                      Root->AddSourceFiles ();

                      //
                      // Now make a full debug record of what we have so far
                      //

                      fprintf ( DebugFile, GetTitleText () );

                      fprintf ( DebugFile, "%+*s: %s\n", IndentLevel, "ProjectSpecificationFileName", ProjectSpecificationFileName );
                      fprintf ( DebugFile, "%+*s: %s\n", IndentLevel, "TemplateDirectoryFileName",    TemplateDirectoryFileName    );
                      fprintf ( DebugFile, "%+*s: %s\n", IndentLevel, "OutputDirectoryFileName",      OutputDirectoryFileName      );

                      CompanyName->Debug ();

                      fprintf ( DebugFile, "%+*s: %s\n", IndentLevel, "CompanyWebAddress", CompanyWebAddress );

                      fprintf ( DebugFile, "%+*s: %s\n", IndentLevel, "AuthorName", AuthorName );

                      fprintf ( DebugFile, "%+*s: %s\n", IndentLevel, "AuthorInitials", AuthorInitials );

                      fprintf ( DebugFile, "%+*s: %d\n", IndentLevel, "SourceFileWidth", SourceFileWidth );

                      fprintf ( DebugFile, "%+*s: %c\n", IndentLevel, "SeperatorCharacter", SeperatorCharacter );

                      Root->Debug ();

                      //
                      // Now we start generating the files and directories of the desired system
                      //

                      ChangeDirectory ( OutputDirectoryFileName );

                      printf ( "\nCreating Directory Tree\n" );

                      fprintf ( LogFile, "\n" );
                      fprintf ( LogFile, "Directory Tree\n" );
                      fprintf ( LogFile, "--------------\n" );
                      fprintf ( LogFile, "\n" );

                      Root->CreateDirectories ();

                      printf ( "Populating Project Tree\n" );

                      Root->Build ();

                      printf ( "(%d) Names, (%d) Nodes and (%d) Templates created!\n",
                               GetNumberOfNames (),
                               GetNumberOfNodes (),
                               GetNumberOfTemplates () );

                      delete Root;
                  }
                  else
                  {
                      printf ( "Cannot create root NODE object\n" );
                  }

                  fclose ( DebugFile );
              }
              else
              {
                  printf ( "Cannot open DebugFile: %s\n", DebugFileName );
              }

              fclose ( LogFile );
          }
          else
          {
              printf ( "Cannot open LogFile: %s\n", LogFileName );
          }
       }
       else
       {
           printf ( "Template Directory %s does not exist\n", TemplateDirectoryFileName );
       }
    }
    else
    {
        printf ( "Cannot open ProjectSpecificationFile: %s\n", ProjectSpecificationFileName );
    }

    printf ( "Press ENTER key to finish\n" );

    getchar ();
}

//**********************************************************************************************************************************

void TOOL::Load ( void )
{
    NUMBER     LevelNumber;
    CHARACTER *SectionName;
    CHARACTER  LevelNameKey             [ MAX_NAME_LENGTH ];
    CHARACTER  MnemonicKey              [ MAX_NAME_LENGTH ];
    CHARACTER  LevelNameString          [ MAX_NAME_LENGTH ];
    CHARACTER  MnemonicString           [ MAX_NAME_LENGTH ];
    CHARACTER  RootLongName             [ MAX_NAME_LENGTH ];
    CHARACTER  CompanyLongName          [ MAX_NAME_LENGTH ];
    CHARACTER  SourceFileWidthString    [ MAX_NAME_LENGTH ];
    CHARACTER  SeperatorCharacterString [ MAX_NAME_LENGTH ];

    //
    // get the names of each level in the hierarchy
    //

    NumberOfLevels = GetPrivateProfileInt ( "Levels",
                                            "NumberOfLevels",
                                            0,
                                            ProjectSpecificationFileName );

    for ( LevelNumber = 0; LevelNumber < MAX_LEVELS; LevelNumber++ )
    {
        sprintf ( LevelNameKey, "Level%d", LevelNumber );

        GetPrivateProfileString ( "Level Names",
                                  LevelNameKey,
                                  DefaultLevelNames [ LevelNumber ],
                                  LevelNameString,
                                  sizeof ( LevelNameString ),
                                  ProjectSpecificationFileName );

        LevelNames [ LevelNumber ] = new NAME ( LevelNameString );
    }

    for ( LevelNumber = 0; LevelNumber < MAX_LEVELS; LevelNumber++ )
    {
        sprintf ( MnemonicKey, "Mnemonic%d", LevelNumber );

        GetPrivateProfileString ( "Level Mnemonics",
                                  MnemonicKey,
                                  DefaultMnemonicNames [ LevelNumber ],
                                  MnemonicString,
                                  sizeof ( MnemonicString ),
                                  ProjectSpecificationFileName );

        LevelMnemonics [ LevelNumber ] = new NAME ( MnemonicString );
    }

    SectionName = GetLevelName ( 0 )->GetLong ();

    //
    // Get the top level details of the project to be generated based on the level names just loaded
    //

    GetPrivateProfileString ( LevelNames [ 0 ]->GetLong (),
                              "ProjectName",
                              "Default Root Name",
                              RootLongName,
                              sizeof ( RootLongName ),
                              ProjectSpecificationFileName );

    GetPrivateProfileString ( RootLongName,
                              "CompanyName",
                              "Wibbly Wobbly Widgets",
                              CompanyLongName,
                              sizeof ( CompanyLongName ),
                              ProjectSpecificationFileName );

    CompanyName = new NAME ( CompanyLongName );

    GetPrivateProfileString ( RootLongName,
                              "CompanyWebAddress",
                              "wibblywobblywidgets.co.uk",
                              CompanyWebAddress,
                              sizeof ( CompanyWebAddress ),
                              ProjectSpecificationFileName );

    GetPrivateProfileString ( RootLongName,
                              "AuthorName",
                              "Mickey.M.Mouse",
                              AuthorName,
                              sizeof ( AuthorName ),
                              ProjectSpecificationFileName );

    GetPrivateProfileString ( RootLongName,
                              "AuthorInitials",
                              "MMM",
                              AuthorInitials,
                              sizeof ( AuthorInitials ),
                              ProjectSpecificationFileName );

    // make sure the initials are three characters (pad with spaces)

    if ( strlen ( AuthorInitials ) == 1 )
    {
        AuthorInitials [ 1 ] = ' ';
        AuthorInitials [ 2 ] = ' ';
        AuthorInitials [ 3 ] = '\0';
    }
    else
    {
        if ( strlen ( AuthorInitials ) == 2 )
        {
            AuthorInitials [ 2 ] = ' ';
            AuthorInitials [ 3 ] = '\0';
        }
        else
        {
            if ( strlen ( AuthorInitials ) > 3 )
            {
                AuthorInitials [ 3 ] = '\0'; // truncate
            }
        }
    }

    GetPrivateProfileString ( RootLongName,
                              "SourceFileWidth",
                              "50",
                              SourceFileWidthString,
                              sizeof ( SourceFileWidthString ),
                              ProjectSpecificationFileName );

    SourceFileWidth = atoi ( SourceFileWidthString );

    GetPrivateProfileString ( RootLongName,
                              "SeperatorCharacter",
                              "!",
                              SeperatorCharacterString,
                              sizeof ( SeperatorCharacterString ),
                              ProjectSpecificationFileName );

    SeperatorCharacter = SeperatorCharacterString [ 0 ];

    //
    // Load the root node and all its sub nodes
    //

    Root->SetName ( RootLongName );

    Root->Load ();
}

//**********************************************************************************************************************************

CHARACTER *TOOL::GetProjectSpecificationFileName ( void )
{
    return ( ProjectSpecificationFileName );
}

CHARACTER *TOOL::GetTemplateDirectoryFileName ( void )
{
    return ( TemplateDirectoryFileName );
}

CHARACTER *TOOL::GetOutputDirectoryFileName ( void )
{
    return ( OutputDirectoryFileName );
}

NAME *TOOL::GetCompanyName ( void )
{
    return ( CompanyName );
}

CHARACTER *TOOL::GetCompanyWebAddress ( void )
{
    return ( CompanyWebAddress );
}

CHARACTER *TOOL::GetAuthorName ( void )
{
    return ( AuthorName );
}

CHARACTER *TOOL::GetAuthorInitials ( void )
{
    return ( AuthorInitials );
}

NUMBER TOOL::GetSourceFileWidth ( void )
{
    return ( SourceFileWidth );
}

CHARACTER TOOL::GetSeperatorCharacter ( void )
{
    return ( SeperatorCharacter );
}

//**********************************************************************************************************************************

FILE *TOOL::GetLogFile ( void )
{
    return ( LogFile );
}

FILE *TOOL::GetDebugFile ( void )
{
    return ( DebugFile );
}

NUMBER TOOL::GetIndentLevel ( void )
{
    return ( IndentLevel );
}

NUMBER TOOL::Indent ( void )
{
    IndentLevel += IndentStep;

    return ( IndentLevel );
}

NUMBER TOOL::Outdent ( void )
{
    if ( IndentLevel > IndentStep ) { IndentLevel -= IndentStep; }

    return ( IndentLevel );
}

void TOOL::ApplyIndent ( void )
{
    NUMBER Count;

    for ( Count = 0; Count < IndentLevel; Count++ )
    {
       putc ( ' ', DebugFile );
    }
}

//**********************************************************************************************************************************

NUMBER TOOL::GetNumberOfLevels ( void )
{
    return ( NumberOfLevels );
}

//**********************************************************************************************************************************

NAME *TOOL::GetLevelName ( NUMBER Level )
{
    if ( Level < NumberOfLevels )
    {
        return ( LevelNames [ Level ] );
    }
    else
    {
        return ( NULL );
    }
}

//**********************************************************************************************************************************

NAME *TOOL::GetLevelMnemonic ( NUMBER Level )
{
    if ( Level < NumberOfLevels )
    {
        return ( LevelMnemonics [ Level ] );
    }
    else
    {
        return ( NULL );
    }
}

//**********************************************************************************************************************************

NODE *TOOL::GetRoot ( void )
{
    return ( Root );
}

//**********************************************************************************************************************************

CHARACTER *TOOL::GetFileDate ( void )
{
    return ( FileDate );
}

CHARACTER *TOOL::GetFileTime ( void )
{
    return ( FileTime );
}

CHARACTER *TOOL::GetFileTimeLong ( void )
{
    return ( FileTimeLong );
}

CHARACTER *TOOL::GetFileYear ( void )
{
    return ( FileYear );
}

//**********************************************************************************************************************************
