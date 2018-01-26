
//**********************************************************************************************************************************
//
//   Purpose: Add Project Utility source code file
//
//   Project: Development Utilities
//
//  Filename: Node.cpp
//
//   Authors: Caroline.M.Mathieson (CMM)
//
//**********************************************************************************************************************************
//
//  Description
//  -----------
//
//! \file Node.cpp
//! \brief Contains the implementation of the NODE object.
//!
//! This file contains the implementation of the NODE Object.
//!
//**********************************************************************************************************************************

#include "AddProject.h"

NUMBER NumberOfNodes = 0;

//*********************************************************************************************************************************

/**

\brief The constructor which initialises the variables.

**/

//**********************************************************************************************************************************

NODE::NODE ( NODE      *NewParent,
             CHARACTER *NewNodeName,
             NUMBER     NewNodeNumber,
             NUMBER    *NewNodeNumbers,
             NUMBER     NewLevelNumber )
{
    NUMBER Index;
    NUMBER SubNodeNumber;
    NUMBER MasterFileNumber;
    NUMBER SourceFileNumber;

    // private:

    Parent    = NewParent;
    IsLeaf    = FALSE;
    IsBuilt   = FALSE;
    IsCommon  = FALSE;
    IsTarget  = FALSE;
    IsLibrary = FALSE;

    UseMnemonicNames = FALSE;

    memset ( Mnemonic, 0, sizeof ( Mnemonic ) );
    memset ( Comment,  0, sizeof ( Comment  ) );

    ASSERTION ( NewNodeName != NULL );

    Name = new NAME ( NewNodeName );

    Number = NewNodeNumber;

    LevelNumber = NewLevelNumber;

    /* record the node numbers of all ancestors of this node and set remainder to -1 */

    memset ( NodeNumbers, -1, sizeof ( NUMBER ) * MAX_LEVELS );

    for ( Index = 0; Index < LevelNumber; Index++ ) /* levels above this one */
    {
        NodeNumbers [ Index ] = NewNodeNumbers [ Index ];
    }

    NodeNumbers [ LevelNumber ] = NewNodeNumber; /* this level */

    Type = NULL;

    // protected:

    memset ( TargetDirectoryName, 0, sizeof ( TargetDirectoryName ) );

    NumberOfSubNodes = 0;

    for ( SubNodeNumber = 0; SubNodeNumber < MAX_SUB_NODES; SubNodeNumber++ )
    {
        SubNodes [ SubNodeNumber ] = NULL;
    }

    NumberOfMasterFiles = 0;

    for ( MasterFileNumber = 0; MasterFileNumber < MAX_TEMPLATES; MasterFileNumber++ )
    {
        MasterFiles [ MasterFileNumber ] = NULL;
    }

    NumberOfSourceFiles = 0;

    for ( SourceFileNumber = 0; SourceFileNumber < MAX_TEMPLATES; SourceFileNumber++ )
    {
        SourceFiles [ SourceFileNumber ] = NULL;
    }

    NodeNumber = NumberOfNodes++;
	
    printf ( "  Created NODE (%d)\r", NodeNumber );
}

//**********************************************************************************************************************************

NODE::~NODE ( void )
{
    NUMBER SubNodeNumber;
    NUMBER MasterFileNumber;
    NUMBER SourceFileNumber;

    for ( SubNodeNumber = 0; SubNodeNumber < MAX_SUB_NODES; SubNodeNumber++ )
    {
        if ( SubNodes [ SubNodeNumber ] != NULL ) delete SubNodes [ SubNodeNumber ];
    }

    for ( MasterFileNumber = 0; MasterFileNumber < NumberOfMasterFiles; MasterFileNumber++ )
    {
        if ( MasterFiles [ MasterFileNumber ] != NULL ) delete MasterFiles [ MasterFileNumber ];
    }

    for ( SourceFileNumber = 0; SourceFileNumber < NumberOfSourceFiles; SourceFileNumber++ )
    {
        if ( SourceFiles [ SourceFileNumber ] != NULL ) delete SourceFiles [ SourceFileNumber ];
    }

    if ( ! IsBuilt )
    {
        fprintf ( stdout, "Node (%s) was not built!\n", Name->GetLong () );
    }

    delete Name;

    delete Type;
}

//**********************************************************************************************************************************

void NODE::Debug ( void )
{
    TOOL      *Tool;
    FILE      *DebugFile;
    NUMBER     Index;
    NUMBER     IndentLevel;
    NUMBER     SubNodeNumber;
    NUMBER     SourceFileNumber;
    NUMBER     MasterFileNumber;
    CHARACTER  LongString  [ 2000 ];
    CHARACTER  ShortString [ 20   ];

    Tool = GetTool ();

    DebugFile = Tool->GetDebugFile();

    IndentLevel = Tool->GetIndentLevel ();

    //
    // Heading
    //

    fprintf ( DebugFile, "\n");
    fprintf ( DebugFile, "%*s NODE (%07d)\n", IndentLevel, "", NodeNumber );
    fprintf ( DebugFile, "%*s --------------\n", IndentLevel, "" );

    //
    // private:
    //

    if ( IsLeaf    ) fprintf ( DebugFile, "%+*s: TRUE\n", IndentLevel, "IsLeaf"    ); else fprintf ( DebugFile, "%+*s: FALSE\n", IndentLevel, "IsLeaf"    );
    if ( IsBuilt   ) fprintf ( DebugFile, "%+*s: TRUE\n", IndentLevel, "IsBuilt"   ); else fprintf ( DebugFile, "%+*s: FALSE\n", IndentLevel, "IsBuilt"   );
    if ( IsCommon  ) fprintf ( DebugFile, "%+*s: TRUE\n", IndentLevel, "IsCommon"  ); else fprintf ( DebugFile, "%+*s: FALSE\n", IndentLevel, "IsCommon"  );
    if ( IsTarget  ) fprintf ( DebugFile, "%+*s: TRUE\n", IndentLevel, "IsTarget"  ); else fprintf ( DebugFile, "%+*s: FALSE\n", IndentLevel, "IsTarget"  );
    if ( IsLibrary ) fprintf ( DebugFile, "%+*s: TRUE\n", IndentLevel, "IsLibrary" ); else fprintf ( DebugFile, "%+*s: FALSE\n", IndentLevel, "IsLibrary" );

    if ( UseMnemonicNames ) fprintf ( DebugFile, "%+*s: TRUE\n", IndentLevel, "UseMnemonicNames" ); else fprintf ( DebugFile, "%+*s: FALSE\n", IndentLevel, "UseMnemonicNames" );

    fprintf ( DebugFile, "%+*s: %s\n", IndentLevel, "Mnemonic", Mnemonic );
    fprintf ( DebugFile, "%+*s: %s\n", IndentLevel, "Comment",  Comment );

    fprintf ( DebugFile, "%+*s: %d\n", IndentLevel, "Number",  Number );

    fprintf ( DebugFile, "%+*s: %d\n", IndentLevel, "LevelNumber",  LevelNumber );

    LongString [ 0 ] = '\0';

    for ( Index = 0; Index < MAX_LEVELS; Index++ )
    {
        sprintf ( ShortString, " %d", NodeNumbers [ Index ] );

        strcat ( LongString, ShortString );
    }

    fprintf ( DebugFile, "%+*s: %s\n", IndentLevel, "NodeNumbers", LongString );

    Tool->Indent ();

    if ( Name != NULL ) Name->Debug ();

    Tool->Outdent ();

    //
    // protected:
    //

    Tool->Indent ();

    if ( Type != NULL ) Type->Debug ();

    fprintf ( DebugFile, "\n");

    fprintf ( DebugFile, "%+*s: %d\n", IndentLevel, "NumberOfSubNodes", NumberOfSubNodes );

    for ( SubNodeNumber = 0; SubNodeNumber < NumberOfSubNodes; SubNodeNumber++ )
    {
        SubNodes [ SubNodeNumber ]->Debug ();
    }

    fprintf ( DebugFile, "\n");

    fprintf ( GetTool()->GetDebugFile(), "%+*s: %s\n", IndentLevel, "TargetDirectoryName", TargetDirectoryName );

    fprintf ( DebugFile, "\n");

    fprintf ( DebugFile, "%+*s: %d\n", IndentLevel, "NumberOfMasterFiles", NumberOfMasterFiles );

    for ( MasterFileNumber = 0; MasterFileNumber < NumberOfMasterFiles; MasterFileNumber++ )
    {
        MasterFiles [ MasterFileNumber ]->Debug ();
    }

    fprintf ( DebugFile, "\n");

    fprintf ( DebugFile, "%+*s: %d\n", IndentLevel, "NumberOfSourceFiles", NumberOfSourceFiles );

    for ( SourceFileNumber = 0; SourceFileNumber < NumberOfSourceFiles; SourceFileNumber++ )
    {
        SourceFiles [ SourceFileNumber ]->Debug ();
    }

    Tool->Outdent ();
}

//**********************************************************************************************************************************

void NODE::Load ( void )
{
    TOOL      *Tool;
    CHARACTER *LevelName;
    CHARACTER *SectionName;
    CHARACTER *InitialisationFileName;
    NUMBER     SubNodeNumber;
    CHARACTER  LongName     [ MAX_NAME_LENGTH ];
    CHARACTER  KeyName      [ MAX_NAME_LENGTH ];
    CHARACTER  DefaultValue [ MAX_NAME_LENGTH ];

    Tool = GetTool ();

    SectionName = GetName ()->GetLong(); /* get the name of this node */

    InitialisationFileName = Tool->GetProjectSpecificationFileName ();

    GetPrivateProfileString ( SectionName,
                              "MnemonicName",
                              "NONE",
                              Mnemonic,
                              sizeof ( Mnemonic ),
                              InitialisationFileName );

    GetPrivateProfileString ( SectionName,
                              "Comment",
                              "None",
                              Comment,
                              sizeof ( Comment ),
                              InitialisationFileName );

    UseMnemonicNames = ( bool ) GetPrivateProfileInt ( SectionName,
                                                       "UseMnemonicName",
                                                       0,
                                                       InitialisationFileName );

    //
    // Get the sub nodes
    //

    LevelName = Tool->GetLevelName ( LevelNumber + 1 )->GetLong(); /* get the level name of the sub nodes */

    sprintf ( KeyName, "NumberOf%ss", LevelName );

    NumberOfSubNodes = GetPrivateProfileInt ( SectionName,
                                              KeyName,
                                              0,
                                              InitialisationFileName );

    for ( SubNodeNumber = 0; SubNodeNumber < NumberOfSubNodes; SubNodeNumber++ )
    {
        sprintf ( KeyName, "%s%d", LevelName, SubNodeNumber + 1 );

        sprintf ( DefaultValue, "Default %s Name", LevelName );

        GetPrivateProfileString ( SectionName,
                                  KeyName,
                                  DefaultValue,
                                  LongName,
                                  sizeof ( LongName ),
                                  InitialisationFileName );

        SubNodes [ SubNodeNumber ] = new NODE ( this, LongName, SubNodeNumber, &NodeNumbers [ 0 ], LevelNumber + 1 );

        SubNodes [ SubNodeNumber ]->Load ();
				
	    //DisplayProgress ( SubNodeNumber, NumberOfSubNodes );
    }
}

//**********************************************************************************************************************************

void NODE::AddRootFiles ( void )
{
    TOOL      *Tool;
    NUMBER     SubNodeNumber;
    CHARACTER *LevelName;
    CHARACTER  SectionName [ MAX_NAME_LENGTH ];

	printf ( "  Adding Root Files for NODE (%d)\r", NodeNumber );
    
    Tool = GetTool ();

    LevelName = Tool->GetLevelName ( LevelNumber )->GetLong ();

    sprintf ( SectionName, "%s Core Files", LevelName );

    // Get the root file names from the initialisation file

    AddFiles ( GetNumberOfRootFileTypes (), GetRootFileTypesTable (), SectionName, TRUE, FALSE, TRUE );

    for ( SubNodeNumber = 0; SubNodeNumber < NumberOfSubNodes; SubNodeNumber++ )
    {
        SubNodes [ SubNodeNumber ]->AddRootFiles ();
    
	    //DisplayProgress ( SubNodeNumber, NumberOfSubNodes );
    }
}

//**********************************************************************************************************************************

void NODE::AddMasterFiles ( void )
{
    TOOL      *Tool;
    NUMBER     SubNodeNumber;
    CHARACTER *LevelName;
    CHARACTER  SectionName [ MAX_NAME_LENGTH ];

	printf ( "  Adding Master Files for NODE (%d)\r", NodeNumber );
    
    Tool = GetTool ();

    LevelName = Tool->GetLevelName ( LevelNumber )->GetLong ();

    sprintf ( SectionName, "%s Master Files", LevelName );

    // Get the master file names from the initialisation file

    AddFiles ( GetNumberOfMasterFileTypes (), GetMasterFileTypesTable (), SectionName, TRUE, FALSE, FALSE );

    for ( SubNodeNumber = 0; SubNodeNumber < NumberOfSubNodes; SubNodeNumber++ )
    {
        SubNodes [ SubNodeNumber ]->AddMasterFiles ();

		//DisplayProgress ( SubNodeNumber, NumberOfSubNodes );
    }
}

//**********************************************************************************************************************************

void NODE::AddSourceFiles ( void )
{
    TOOL      *Tool;
    NUMBER     SubNodeNumber;
    CHARACTER *LevelName;
    CHARACTER  SectionName [ MAX_NAME_LENGTH ];

	printf ( "  Adding Source Files for NODE (%d)\r", NodeNumber );
    
    Tool = GetTool ();

    LevelName = Tool->GetLevelName ( LevelNumber )->GetLong ();

    sprintf ( SectionName, "%s Source Files", LevelName );

    // Get the source file names from the initialisation file

    AddFiles ( GetNumberOfSourceFileTypes (), GetSourceFileTypesTable (), SectionName, FALSE, FALSE, FALSE );

    for ( SubNodeNumber = 0; SubNodeNumber < NumberOfSubNodes; SubNodeNumber++ )
    {
        SubNodes [ SubNodeNumber ]->AddSourceFiles ();

		//DisplayProgress ( SubNodeNumber, NumberOfSubNodes );
    }
}

//**********************************************************************************************************************************

void NODE::AddTemplateFiles ( void )
{
    TOOL      *Tool;
    NUMBER     SubNodeNumber;
    CHARACTER *LevelName;
    CHARACTER  SectionName [ MAX_NAME_LENGTH ];

	printf ( "  Adding Template Files for NODE (%d)\r", NodeNumber );
    
    Tool = GetTool ();

    LevelName = Tool->GetLevelName ( LevelNumber )->GetLong ();

    sprintf ( SectionName, "%s Template Files", LevelName );

    AddFiles ( GetNumberOfTemplateFileTypes (), GetTemplateFileTypesTable (), SectionName, TRUE, TRUE, FALSE );

    for ( SubNodeNumber = 0; SubNodeNumber < NumberOfSubNodes; SubNodeNumber++ )
    {
        SubNodes [ SubNodeNumber ]->AddTemplateFiles ();

		//DisplayProgress ( SubNodeNumber, NumberOfSubNodes );
    }
}

//**********************************************************************************************************************************
//
//  Create the following directory structure for this node within a directory derived from the node name
//
//    NodeName
//    |
//    |-- Documentation        \
//    |   |-- User              |
//    |   \-- Design            |
//    |                         |
//    |-- Implementation        |
//    |   |-- Common            |
//    |   |-- Export            |
//    |   |-- Header            |
//    |   |-- Model             |
//    |   |-- Object            |
//    |   |-- Source             \______ & = repeated directory structure within each NODE
//    |   |-- Test               /
//    |   |-- Tools             |
//    |   \-- Template          |
//    |       |-- Common        |
//    |       |-- Export        |
//    |       |-- Header        |
//    |       |-- Model         |
//    |       |-- Source        |
//    |       |-- Test          |
//    |       |-- Tools         |
//    |       \-- Documentation |
//    |           |-- User      |
//    |           \-- Design    |
//    \-- Testing              /
//
//**********************************************************************************************************************************

void NODE::CreateDirectories ( void )
{
    TOOL      *Tool;
    FILE      *LogFile;
    NUMBER     NodeNumber;
    CHARACTER  WorkingDirectoryName [ MAX_PATH_NAME ];

    Tool = GetTool ();

    LogFile = Tool->GetLogFile();

    // note where we are in the directory tree right now

    GetWorkingDirectory ( WorkingDirectoryName, sizeof ( WorkingDirectoryName ) );

    // then construct the directory name to be created and record it

    if ( GetUseMnemonicNames () )
    {
        sprintf ( TargetDirectoryName, "%s\\%s", WorkingDirectoryName, Mnemonic );
    }
    else
    {
        sprintf ( TargetDirectoryName, "%s\\%s", WorkingDirectoryName, GetName()->GetShortCapitalised () );
    }

    // create this directory

    fprintf ( LogFile, "Creating directory %s\n", TargetDirectoryName );

    MakeDirectory   ( TargetDirectoryName );
    ChangeDirectory ( TargetDirectoryName );

    // then create all the sub directories

    MakeDirectory   ( "Documentation" );
    ChangeDirectory ( "Documentation" );

    MakeDirectory ( "Design" );
    MakeDirectory ( "User"   );

    ChangeDirectory ( ".." ); // from documentation directory

    MakeDirectory ( "Testing" );

    MakeDirectory   ( "Implementation" );
    ChangeDirectory ( "Implementation" );

    MakeDirectory ( "Common" );
    MakeDirectory ( "Export" );
    MakeDirectory ( "Header" );
    MakeDirectory ( "Model"  );
    MakeDirectory ( "Object" );
    MakeDirectory ( "Source" );
    MakeDirectory ( "Test"   );
    MakeDirectory ( "Tools"  );

    MakeDirectory   ( "Template" );
    ChangeDirectory ( "Template" );

    MakeDirectory ( "Common" );
    MakeDirectory ( "Export" );
    MakeDirectory ( "Header" );
    MakeDirectory ( "Model"  );
    MakeDirectory ( "Source" );
    MakeDirectory ( "Test"   );
    MakeDirectory ( "Tools"  );

    MakeDirectory   ( "Documentation" );
    ChangeDirectory ( "Documentation" );

    MakeDirectory ( "Design" );
    MakeDirectory ( "User"   );

    ChangeDirectory ( ".." ); // from template documentation directory

    ChangeDirectory ( ".." ); // from Template directory

    ChangeDirectory ( ".." ); // from implementation directory

    // create the directories for the sub nodes, if any

    for ( NodeNumber = 0; NodeNumber < GetNumberOfSubNodes(); NodeNumber++ )
    {
        GetSubNode ( NodeNumber )->CreateDirectories ();
    }

    ChangeDirectory ( ".." ); // from NodeName directory
}

//**********************************************************************************************************************************

void NODE::Build ( void )
{
    fprintf ( GetTool()->GetLogFile(), "\n" );
    fprintf ( GetTool()->GetLogFile(), "Populated Files\n" );
    fprintf ( GetTool()->GetLogFile(), "---------------\n" );
    fprintf ( GetTool()->GetLogFile(), "\n" );

    printf ( "  Building Master Files\n" );

    BuildMasterFiles ();

    printf ( "  Building Source Files\n" );

    BuildSourceFiles ();
}

//**********************************************************************************************************************************
//
// PRIVATE
//
//**********************************************************************************************************************************

void NODE::AddFiles ( NUMBER                      NumberOfTableEntries,
                      INITIALISATION_TABLE_ENTRY *InitialisationTable,
                      CHARACTER                  *SectionName,
                      BOOLEAN                     MasterFile,
                      BOOLEAN                     TemplateFile,
                      BOOLEAN                     RootFile )
{
   NUMBER FileNumber;
   NUMBER TypeNumber;
   NUMBER NumberOfFiles;

   // scan though the initialisation table and process each of the file types listed

   for ( TypeNumber = 0; TypeNumber < NumberOfTableEntries; TypeNumber++ )
   {
      // find out how many files of this type are to be processed

      NumberOfFiles = GetPrivateProfileInt ( SectionName,
                                             InitialisationTable [ TypeNumber ].KeyName,
                                             0,
                                             GetTool()->GetProjectSpecificationFileName () );

      // then get each file

      for ( FileNumber = 0; FileNumber < NumberOfFiles; FileNumber++ )
      {
         AddFile ( FileNumber + 1,
                   SectionName,
                   InitialisationTable [ TypeNumber ].TemplateType,
                   InitialisationTable [ TypeNumber ].Directory,
                   MasterFile,
                   TemplateFile,
                   RootFile );
      }
   }
}

//**********************************************************************************************************************************

void NODE::AddFile ( NUMBER     EntryNumber,
                     CHARACTER *SectionName,
                     CHARACTER *TemplateType,
                     CHARACTER *Directory,
                     BOOLEAN    MasterFile,
                     BOOLEAN    TemplateFile,
                     BOOLEAN    RootFile )
{
    TOOL      *Tool;
    FILE      *LogFile;
    NAME      *TemplateTypeName;
    CHARACTER *LevelName;
    CHARACTER  SourceFileName      [ 2000 ];
    CHARACTER  DestinationFileName [ 2000 ];
    CHARACTER  KeyName             [ 2000 ];
    CHARACTER  KeyValue            [ 2000 ];
    CHARACTER  FileName            [ 2000 ];
    CHARACTER  Purpose             [ 2000 ];
    CHARACTER *Position1;
    CHARACTER *Position2;

    Tool = GetTool ();

    LogFile = Tool->GetLogFile ();

    LevelName = Tool->GetLevelName ( LevelNumber )->GetLong ();

    // work out the name of the key of the key/value pair containing the filename etc

    TemplateTypeName = new NAME ( TemplateType );

    sprintf ( KeyName, "%s%d", TemplateTypeName->GetShortCapitalised (), EntryNumber );

    // get the value

    GetPrivateProfileString ( SectionName,
                              KeyName,
                              "",
                              KeyValue,
                              sizeof ( KeyValue ),
                              Tool->GetProjectSpecificationFileName () );

    if ( strlen ( KeyValue ) > 0 )
    {
        // extract the filename and purpose strings

        if ( ( Position1 = strchr ( KeyValue, ',' ) ) != NULL )
        {
            *Position1 = '\0';

            strcpy ( FileName, KeyValue );

            if ( ( Position2 = strchr ( &Position1 [ 1 ], '"' ) ) != NULL )
            {
                strcpy ( Purpose, &Position2 [ 1 ] );

                Purpose [ strlen ( Purpose ) - 1 ] = '\0';

                // now work out the source and destination of the file

                if ( RootFile )
                {
                    if ( strlen ( Directory ) > 0 )
                    {
                        // the source filename is dependent on the level name

                        strcpy ( SourceFileName, LevelName );
                        strcat ( SourceFileName, "\\"      );
                        strcat ( SourceFileName, Directory );
                        strcat ( SourceFileName, "\\"      );
                        strcat ( SourceFileName, FileName  );

                        // the destination filename depends on where its to be placed

                        if ( strstr ( Directory, "Documentation" ) == NULL )
                        {
                            // source files

                            strcpy ( DestinationFileName, "Implementation\\" );
                            strcat ( DestinationFileName, Directory          );
                            strcat ( DestinationFileName, "\\"               );
                            strcat ( DestinationFileName, FileName           );
                        }
                        else
                        {
                            // documentation files

                            strcpy ( DestinationFileName, Directory );
                            strcat ( DestinationFileName, "\\"      );
                            strcat ( DestinationFileName, FileName  );
                        }
                    }
                    else
                    {
                        // don't change the name of core files in any way

                        strcpy ( SourceFileName, Directory );
                        strcat ( SourceFileName, "\\"      );
                        strcat ( SourceFileName, FileName  );

                        strcpy ( DestinationFileName, Directory );
                        strcat ( DestinationFileName, "\\"      );
                        strcat ( DestinationFileName, FileName  );
                    }
                }
                else
                {
                    if ( TemplateFile )
                    {
                        // get the source from the template directory

                        strcpy ( SourceFileName, LevelName  );
                        strcat ( SourceFileName, "\\"       );
                        strcat ( SourceFileName, "Template" );
                        strcat ( SourceFileName, "\\"       );
                        strcat ( SourceFileName, Directory  );
                        strcat ( SourceFileName, "\\"       );
                        strcat ( SourceFileName, FileName   );

                        strcpy ( DestinationFileName, "Implementation" );
                        strcat ( DestinationFileName, "\\"             );
                        strcat ( DestinationFileName, "Template"       );
                        strcat ( DestinationFileName, "\\"             );
                        strcat ( DestinationFileName, Directory        );
                        strcat ( DestinationFileName, "\\"             );
                        strcat ( DestinationFileName, FileName         );
                    }
                    else
                    {
                        strcpy ( SourceFileName, LevelName );
                        strcat ( SourceFileName, "\\"      );
                        strcat ( SourceFileName, Directory );
                        strcat ( SourceFileName, "\\"      );
                        strcat ( SourceFileName, LevelName );
                        strcat ( SourceFileName, FileName  );

                        // the destination filename depends on where its to be placed

                        if ( strstr ( Directory, "Documentation" ) == NULL )
                        {
                            // source files

                            strcpy ( DestinationFileName, "Implementation" );
                            strcat ( DestinationFileName, "\\" );
                            strcat ( DestinationFileName, Directory );
                            strcat ( DestinationFileName, "\\" );
                            strcat ( DestinationFileName, GetName()->GetShortCapitalised () );
                            strcat ( DestinationFileName, FileName );
                        }
                        else
                        {
                            // documentation files

                            strcpy ( DestinationFileName, Directory );
                            strcat ( DestinationFileName, "\\" );
                            strcat ( DestinationFileName, GetName()->GetShortCapitalised () );
                            strcat ( DestinationFileName, FileName );
                        }
                    }
                }

                // add this file to the appropriate list

                if ( MasterFile )
                {
                   AddMasterFile ( SourceFileName,
                                   DestinationFileName,
                                   Purpose,
                                   TemplateType );
                }
                else
                {
                   AddSourceFile ( SourceFileName,
                                   DestinationFileName,
                                   Purpose,
                                   TemplateType );
                }
            }
            else
            {
                fprintf ( LogFile, "No purpose for [%s] %s\n", SectionName, KeyName );
            }
        }
        else
        {
            fprintf ( LogFile, "No FileName for [%s] %s\n", SectionName, KeyName );
        }
    }
    else
    {
        fprintf ( LogFile, "No KeyValue for [%s] %s\n", SectionName, KeyName );
    }
}

//**********************************************************************************************************************************

//void NODE::GetSourceFile ( NUMBER     EntryNumber,
//                           CHARACTER *SectionName,
//                           CHARACTER *SourceTemplateType,
//                           CHARACTER *Directory )
//{
//    CHARACTER  SourceFileName      [ 2000 ];
//    CHARACTER  DestinationFileName [ 2000 ];
//    CHARACTER  FileDescription     [ 2000 ];
//    CHARACTER  KeyName             [ 2000 ];
//    CHARACTER  KeyValue            [ 2000 ];
//    CHARACTER  Source              [ 2000 ];
//    CHARACTER  Target              [ 2000 ];
//    CHARACTER  Purpose             [ 2000 ];
//    CHARACTER *Position1;
//    CHARACTER *Position2;
//    CHARACTER *Position3;
//
//    NAME *TemplateType = new NAME ( SourceTemplateType );
//
//    sprintf ( KeyName, "%s%d", TemplateType->GetShortCapitalised (), EntryNumber );
//
//    GetPrivateProfileString ( SectionName,
//                              KeyName,
//                              "",
//                              KeyValue,
//                              sizeof ( KeyValue ),
//                              GetTool()->GetProjectSpecificationFileName () );
//
//    if ( strlen ( KeyValue ) > 0 )
//    {
//        // extract the filenames and purpose strings
//
//        if ( ( Position1 = strchr ( KeyValue, ',' ) ) != NULL )
//        {
//            *Position1++ = '\0';
//
//            strcpy ( Source, KeyValue );
//
//            if ( ( Position2 = strchr ( Position1, ',' ) ) != NULL )
//            {
//                *Position2++ = '\0';
//
//                while ( *Position1++ == ' ' )            // scan past white space
//
//                strcpy ( Target, &Position1 [ 1 ] );     // ignore first quote
//
//                Target [ strlen ( Target ) - 1 ] = '\0'; // get rid of last qoute
//
//                if ( ( Position3 = strchr ( Position2, '"' ) ) != NULL )
//                {
//                    strcpy ( Purpose, &Position3 [ 1 ] );      // ignore first quote
//
//                    Purpose [ strlen ( Purpose ) - 1 ] = '\0'; // get rid of last qoute
//
//                    // source files are built from files in the template directory
//
//                    strcpy ( SourceFileName, "Implementation" );
//                    strcat ( SourceFileName, "\\"             );
//                    strcat ( SourceFileName, "Template"       );
//                    strcat ( SourceFileName, "\\"             );
//                    strcat ( SourceFileName, Directory        );
//                    strcat ( SourceFileName, "\\"             );
//                    strcat ( SourceFileName, Source           );
//
//                    if ( strstr ( Directory, "Documentation" ) == NULL )
//                    {
//                        strcpy ( DestinationFileName, "Implementation" );
//                        strcat ( DestinationFileName, "\\"             );
//                        strcat ( DestinationFileName, Directory        );
//                    }
//                    else
//                    {
//                        strcpy ( DestinationFileName, Directory );
//                    }
//
//                    strcat ( DestinationFileName, "\\" );
//                    strcat ( DestinationFileName, GetName()->GetShortCapitalised () );
//                    strcat ( DestinationFileName, Target );
//
//                    strcpy ( FileDescription, GetName()->GetLongCapitalised () );
//                    strcat ( FileDescription, " "     );
//                    strcat ( FileDescription, Purpose );
//
//                    AddSourceFile ( SourceFileName,
//                                    DestinationFileName,
//                                    FileDescription,
//                                    SourceTemplateType );
//                }
//                else
//                {
//                    fprintf ( GetTool()->GetLogFile(), "No purpose for [%s] %s\n", SectionName, KeyName );
//                }
//            }
//            else
//            {
//                fprintf ( GetTool()->GetLogFile(), "No target for [%s] %s\n", SectionName, KeyName );
//            }
//        }
//        else
//        {
//            fprintf ( GetTool()->GetLogFile(), "No source for [%s] %s\n", SectionName, KeyName );
//        }
//    }
//    else
//    {
//        fprintf ( GetTool()->GetLogFile(), "No KeyValue for [%s] %s\n", SectionName, KeyName );
//    }
//}

//**********************************************************************************************************************************

void NODE::AddMasterFile ( CHARACTER *SourceFileName,
                           CHARACTER *TargetFileName,
                           CHARACTER *FileDescription,
                           CHARACTER *TemplateType )
{
    if ( NumberOfMasterFiles <= MAX_TEMPLATES )
    {
        MasterFiles [ NumberOfMasterFiles++ ] = new MASTERFILE ( this,
                                                                 GetTool()->GetTemplateDirectoryFileName (),
                                                                 SourceFileName,
                                                                 TargetDirectoryName,
                                                                 TargetFileName,
                                                                 FileDescription,
                                                                 TemplateType );
    }
	else
	{
		fprintf ( GetTool()->GetLogFile(), "Cannot add MasterFile (%s), Maximum Number of MasterFiles (%d) reached\n", TargetFileName, MAX_TEMPLATES );
	}
}

//**********************************************************************************************************************************

void NODE::AddSourceFile ( CHARACTER *SourceFileName,
                           CHARACTER *TargetFileName,
                           CHARACTER *FileDescription,
                           CHARACTER *TemplateType )
{
    if ( NumberOfSourceFiles < MAX_TEMPLATES )
    {
        SourceFiles [ NumberOfSourceFiles++ ] = new SOURCEFILE ( this,
                                                                 GetTool()->GetTemplateDirectoryFileName (),
                                                                 SourceFileName,
                                                                 TargetDirectoryName,
                                                                 TargetFileName,
                                                                 FileDescription,
                                                                 TemplateType );
    }
	else
	{
		fprintf ( GetTool()->GetLogFile(), "Cannot add SourceFile (%s), Maximum Number of SourceFiles (%d) reached\n", TargetFileName, MAX_TEMPLATES );
	}
}

//**********************************************************************************************************************************

void NODE::BuildMasterFiles ( void )
{
    NUMBER SubNodeNumber;
    NUMBER MasterFileNumber;

    /* build the master files in this node */

	printf ( "    Building Master Files for NODE (%d)\n", NodeNumber );
    
	for ( MasterFileNumber = 0; MasterFileNumber < NumberOfMasterFiles; MasterFileNumber++ )
    {
        MasterFiles [ MasterFileNumber ]->Build ( NodeNumbers );

		DisplayProgress ( MasterFileNumber, NumberOfMasterFiles );
    }

    /* build the master files in the sub nodes */

    for ( SubNodeNumber = 0; SubNodeNumber < NumberOfSubNodes; SubNodeNumber++ )
    {
		SubNodes [ SubNodeNumber ]->BuildMasterFiles ();
 	}

    IsBuilt = TRUE;
}

//**********************************************************************************************************************************

void NODE::BuildSourceFiles ( void )
{
    NUMBER SubNodeNumber;
    NUMBER SourceFileNumber;
	NUMBER NumberOfSourceFiles;

    /* build the source files in this node */

	printf ( "    Building Source Files for NODE (%d)\n", NodeNumber );
    
	NumberOfSourceFiles = GetNumberOfSourceFiles();

    for ( SourceFileNumber = 0; SourceFileNumber < NumberOfSourceFiles; SourceFileNumber++ )
    {
        SourceFiles [ SourceFileNumber ]->Build ( NodeNumbers );

		DisplayProgress ( SourceFileNumber, NumberOfSourceFiles );
    }

    /* build the source files in the sub nodes */

    for ( SubNodeNumber = 0; SubNodeNumber < NumberOfSubNodes; SubNodeNumber++ )
    {
        SubNodes [ SubNodeNumber ]->BuildSourceFiles ();
    }

    IsBuilt = TRUE;
}

//**********************************************************************************************************************************

NAME *NODE::GetType ( void )
{
    return ( Type );
}

//**********************************************************************************************************************************

NAME *NODE::SetType ( CHARACTER *NewType )
{
    if ( Type != NULL ) delete Type;

    Type = new NAME ( NewType );

    return ( Type );
}

//**********************************************************************************************************************************

NAME *NODE::GetName ( void )
{
    return ( Name );
}

//**********************************************************************************************************************************

NAME *NODE::SetName ( CHARACTER *NewName )
{
    if ( Name != NULL ) delete Name;

    Name = new NAME ( NewName );

    return ( Name );
}

//**********************************************************************************************************************************

CHARACTER *NODE::GetMnemonic ( void )
{
    return ( Mnemonic );
}

//**********************************************************************************************************************************

bool NODE::GetUseMnemonicNames ( void )
{
    return ( UseMnemonicNames );
}

//**********************************************************************************************************************************

NODE *NODE::GetParent ( void )
{
    return ( Parent );
}

//**********************************************************************************************************************************

CHARACTER *NODE::GetComment ( void )
{
    return ( Comment );
}

//**********************************************************************************************************************************

NUMBER NODE::GetNumber ( void )
{
    return ( Number );
}

//**********************************************************************************************************************************

NUMBER NODE::GetLevelNumber ( void )
{
    return ( LevelNumber );
}

//**********************************************************************************************************************************

NUMBER *NODE::GetNodeNumbers ( void )
{
    return ( &NodeNumbers [ 0 ] );
}

//**********************************************************************************************************************************

NUMBER NODE::GetNumberOfSubNodes ( void )
{
    return ( NumberOfSubNodes );
}

//**********************************************************************************************************************************

NODE *NODE::GetSubNode ( NUMBER SubNodeNumber )
{
    if ( ( SubNodeNumber >= 0 ) && ( SubNodeNumber < NumberOfSubNodes ) )
    {
        return ( SubNodes [ SubNodeNumber ] );
    }
    else
    {
        return ( NULL );
    }
}

//**********************************************************************************************************************************

CHARACTER *NODE::GetTargetDirectoryName ( void )
{
    return ( TargetDirectoryName );
}

//**********************************************************************************************************************************

NUMBER NODE::GetNumberOfMasterFiles ( void )
{
    return ( NumberOfMasterFiles );
}

//**********************************************************************************************************************************

MASTERFILE *NODE::GetMasterFile ( NUMBER MasterFileNumber )
{
    if ( ( MasterFileNumber >= 0 ) && ( MasterFileNumber < NumberOfMasterFiles ) )
    {
        return ( MasterFiles [ MasterFileNumber ] );
    }
    else
    {
       return ( NULL );
    }
}

//**********************************************************************************************************************************

NUMBER NODE::GetNumberOfSourceFiles ( void )
{
    return ( NumberOfSourceFiles );
}

//**********************************************************************************************************************************

SOURCEFILE *NODE::GetSourceFile ( NUMBER SourceFileNumber )
{
    if ( ( SourceFileNumber >= 0 ) && ( SourceFileNumber < NumberOfSourceFiles ) )
    {
        return ( SourceFiles [ SourceFileNumber ] );
    }
    else
    {
       return ( NULL );
    }
}

//**********************************************************************************************************************************

NUMBER GetNumberOfNodes ( void )
{
    return ( NumberOfNodes );
}

//**********************************************************************************************************************************

