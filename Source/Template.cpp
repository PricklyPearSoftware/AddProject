
//**********************************************************************************************************************************
//
//   Purpose: Add Project Utility source code file
//
//   Project: Development Utilities
//
//  Filename: Template.cpp
//
//   Authors: Caroline.M.Mathieson (CMM)
//
//**********************************************************************************************************************************
//
//  Description
//  -----------
//
//! \file Template.cpp
//! \brief Contains the implementation of the TEMPLATE object.
//!
//! This file contains the implementation of the TEMPLATE Object.
//!
//**********************************************************************************************************************************

#include "AddProject.h"

NUMBER NumberOfTemplates = 0;

//**********************************************************************************************************************************
//
//  TEMPLATE
//
//**********************************************************************************************************************************

//*********************************************************************************************************************************
/**
\fn TEMPLATE::TEMPLATE ( char *, char *, char *, char *, char *, char * )

\brief The constructor which initialises the variables.

**/
//**********************************************************************************************************************************

TEMPLATE::TEMPLATE ( NODE      *ParentNode,
                     CHARACTER *NewSourceDirectoryName,
                     CHARACTER *NewSourceFileName,
                     CHARACTER *NewDestinationDirectoryName,
                     CHARACTER *NewDestinationFileName,
                     CHARACTER *NewTemplatePurpose,
                     CHARACTER *NewTemplateType )
{
    CHARACTER Drive     [ 2000 ];
    CHARACTER Directory [ 2000 ];

    // public:

    Node = ParentNode;

    InputFile  = NULL;
    OutputFile = NULL;

    memset ( InputFileName,  0, sizeof ( InputFileName  ) );
    memset ( OutputFileName, 0, sizeof ( OutputFileName ) );

    // private:

    TemplatePurpose = new NAME ( NewTemplatePurpose );

    TemplateType = new NAME ( NewTemplateType );

    strcpy ( SourceDirectoryName, NewSourceDirectoryName );
    strcpy ( SourceFileName,      NewSourceFileName      );

    strcpy ( DestinationDirectoryName, NewDestinationDirectoryName );
    strcpy ( DestinationFileName,      NewDestinationFileName      );

    FileNameSplit ( SourceFileName, Drive, Directory, SourceName, SourceExtension );

    FileNameSplit ( DestinationFileName, Drive, Directory, DestinationName, DestinationExtension );

    memset ( ReturnedName, 0, sizeof ( ReturnedName ) );

    TemplateNumber = NumberOfTemplates++;
}

//**********************************************************************************************************************************

TEMPLATE::~TEMPLATE ( void )
{
    delete TemplatePurpose;

    delete TemplateType;
}

//**********************************************************************************************************************************

void TEMPLATE::Debug ( void )
{
    TOOL   *Tool;
    FILE   *DebugFile;
    NUMBER  IndentLevel;

    Tool = GetTool ();

    DebugFile = Tool->GetDebugFile ();

    IndentLevel = Tool->GetIndentLevel ();

    fprintf ( DebugFile, "\n" );

    fprintf ( DebugFile, "%*s TEMPLATE (%07d)\n", IndentLevel, "", TemplateNumber );

    fprintf ( DebugFile, "%*s ------------------\n", IndentLevel, "" );

    fprintf ( DebugFile, "%+*s: %s\n", IndentLevel, "InputFileName",  InputFileName  );
    fprintf ( DebugFile, "%+*s: %s\n", IndentLevel, "OutputFileName", OutputFileName );

    fprintf ( DebugFile, "%+*s: %s\n", IndentLevel, "TemplatePurpose", TemplatePurpose->GetLong () );
    fprintf ( DebugFile, "%+*s: %s\n", IndentLevel, "TemplateType",    TemplateType->GetLong    () );

    fprintf ( DebugFile, "%+*s: %s\n", IndentLevel, "SourceDirectoryName",      SourceDirectoryName      );
    fprintf ( DebugFile, "%+*s: %s\n", IndentLevel, "SourceFileName",           SourceFileName           );
    fprintf ( DebugFile, "%+*s: %s\n", IndentLevel, "DestinationDirectoryName", DestinationDirectoryName );
    fprintf ( DebugFile, "%+*s: %s\n", IndentLevel, "DestinationFileName",      DestinationFileName      );

    fprintf ( DebugFile, "%+*s: %s\n", IndentLevel, "SourceName",      SourceName      );
    fprintf ( DebugFile, "%+*s: %s\n", IndentLevel, "SourceExtension", SourceExtension );

    fprintf ( DebugFile, "%+*s: %s\n", IndentLevel, "DestinationName",      DestinationName      );
    fprintf ( DebugFile, "%+*s: %s\n", IndentLevel, "DestinationExtension", DestinationExtension );

    fprintf ( DebugFile, "%+*s: %s\n", IndentLevel, "ReturnedName", ReturnedName );
}

//**********************************************************************************************************************************

void TEMPLATE::Build ( CHARACTER *MacroPrefix,
                       NUMBER    *LevelNumbers )
{
    TOOL *Tool;
    NODE *Root;
    FILE *LogFile;

    Tool = GetTool ();

    Root = Tool->GetRoot ();

    LogFile = Tool->GetLogFile ();

    // Get the target directory

    strcpy ( OutputFileName, Node->GetTargetDirectoryName () );
    strcat ( OutputFileName, "\\" );

    // and append the directory path within the node

    if ( strlen ( GetDestinationDirectoryName () ) > 0 ) // don't append empty directories as it generates \\\\ sequence
    {
        strcat ( OutputFileName, GetDestinationDirectoryName () );
        strcat ( OutputFileName, "\\" );
    }

    // and finally append the filename

    strcat ( OutputFileName, GetDestinationFileName () );

    // try to create this file

    OutputFile = fopen ( OutputFileName, "wb" );

    if ( OutputFile != NULL )
    {
        TEMPLATEFILE *TemplateFile = new TEMPLATEFILE ( Node, this, InputFile, LevelNumbers );

        TemplateFile->Scan ( MacroPrefix, OutputFile );

        delete TemplateFile;

        fprintf ( LogFile,
                  "      Created destination file: %s, \"%s %s\"\n",
                  OutputFileName,
                  GetTemplatePurpose()->GetLong(),
                  GetTemplateType   ()->GetLong() );

        fclose ( OutputFile );
    }
    else
    {
        fprintf ( LogFile, "Cannot create destination file: %s\n", OutputFileName );
    }

    fclose ( InputFile );
}

//**********************************************************************************************************************************

CHARACTER *TEMPLATE::GetSourceDirectoryName ( BOOLEAN AddBackSlash )
{
    ProcessReturnedName ( SourceDirectoryName, AddBackSlash );

    return ( ReturnedName );
}

CHARACTER *TEMPLATE::GetSourceFileName ( BOOLEAN AddBackSlash )
{
    ProcessReturnedName ( SourceFileName, AddBackSlash );

    return ( ReturnedName );
}

CHARACTER *TEMPLATE::GetDestinationDirectoryName ( BOOLEAN AddBackSlash )
{
    ProcessReturnedName ( DestinationDirectoryName, AddBackSlash );

    return ( ReturnedName );
}

CHARACTER *TEMPLATE::GetDestinationFileName ( BOOLEAN AddBackSlash )
{
    ProcessReturnedName ( DestinationFileName, AddBackSlash );

    return ( ReturnedName );
}

CHARACTER *TEMPLATE::GetSourceName ( BOOLEAN AddBackSlash )
{
    ProcessReturnedName ( SourceName, AddBackSlash );

    return ( ReturnedName );
}

CHARACTER *TEMPLATE::GetSourceExtension ( BOOLEAN AddBackSlash )
{
    ProcessReturnedName ( SourceExtension, AddBackSlash );

    return ( ReturnedName );
}

CHARACTER *TEMPLATE::GetDestinationName ( BOOLEAN AddBackSlash )
{
    ProcessReturnedName ( DestinationName, AddBackSlash );

    return ( ReturnedName );
}

CHARACTER *TEMPLATE::GetDestinationExtension ( BOOLEAN AddBackSlash )
{
    ProcessReturnedName ( DestinationExtension, AddBackSlash );

    return ( ReturnedName );
}

NAME *TEMPLATE::GetTemplatePurpose ( void )
{
    return ( TemplatePurpose );
}

NAME *TEMPLATE::GetTemplateType ( void )
{
    return ( TemplateType );
}

void TEMPLATE::ProcessReturnedName ( CHARACTER *String,
                                     BOOLEAN    AddBackSlash )
{
    memset ( ReturnedName, 0, sizeof ( ReturnedName ) );

    if ( strlen ( String ) > 1 )
    {
        if ( String [ 0 ] != '\\' )
        {
            strcpy ( ReturnedName, String );
        }
        else
        {
            strcpy ( ReturnedName, &String [ 1 ] );
        }

        if ( AddBackSlash )
        {
            strcat ( ReturnedName, "\\" );
        }
    }
}

//**********************************************************************************************************************************
//
//  MASTERFILE
//
//**********************************************************************************************************************************

MASTERFILE::MASTERFILE ( NODE      *ParentNode,
                         CHARACTER *NewSourceDirectoryName,
                         CHARACTER *NewSourceFileName,
                         CHARACTER *NewDestinationDirectoryName,
                         CHARACTER *NewDestinationFileName,
                         CHARACTER *NewFileTypeComment,
                         CHARACTER *MasterTemplateType ) :

                         TEMPLATE ( ParentNode,
                                    NewSourceDirectoryName,
                                    NewSourceFileName,
                                    NewDestinationDirectoryName,
                                    NewDestinationFileName,
                                    NewFileTypeComment,
                                    MasterTemplateType )
{
}

//**********************************************************************************************************************************

void MASTERFILE::Build ( NUMBER *LevelNumbers )
{
    InputFileName [ 0 ] = '\0';

    if ( strlen ( GetSourceDirectoryName () ) > 0 ) // don't append empty directories as it generates \\\\ sequence
    {
        strcat ( InputFileName, GetSourceDirectoryName () );
        strcat ( InputFileName, "\\" );
    }

    strcat ( InputFileName, GetSourceFileName () );

    InputFile = fopen ( InputFileName, "rb" );

    if ( InputFile != NULL )
    {
        TEMPLATE::Build ( "$", LevelNumbers );
    }
    else
    {
        fprintf ( GetTool()->GetLogFile(), "       Cannot open master file: %s\n", InputFileName );
    }
}

//**********************************************************************************************************************************
//
//  SOURCEFILE
//
//**********************************************************************************************************************************

SOURCEFILE::SOURCEFILE ( NODE      *ParentNode,
                         CHARACTER *NewSourceDirectoryName,
                         CHARACTER *NewSourceFileName,
                         CHARACTER *NewDestinationDirectoryName,
                         CHARACTER *NewDestinationFileName,
                         CHARACTER *NewFileDescription,
                         CHARACTER *MasterTemplateType ) :

                         TEMPLATE ( ParentNode,
                                    NewSourceDirectoryName,
                                    NewSourceFileName,
                                    NewDestinationDirectoryName,
                                    NewDestinationFileName,
                                    NewFileDescription,
                                    MasterTemplateType )
{
}

//**********************************************************************************************************************************

void SOURCEFILE::Build ( NUMBER *LevelNumbers )
{
    strcpy ( InputFileName, GetTool()->GetRoot()->GetName()->GetShortCapitalised () );
    strcat ( InputFileName, "\\" );

    if ( strlen ( GetSourceDirectoryName () ) > 0 ) // don't append empty directories as it generates \\\\ sequence
    {
        strcat ( InputFileName, GetSourceDirectoryName () );
        strcat ( InputFileName, "\\" );
    }

    strcat ( InputFileName, GetSourceFileName () );

    InputFile = fopen ( InputFileName, "rb" );

    if ( InputFile != NULL )
    {
        TEMPLATE::Build ( "$", LevelNumbers );
    }
    else
    {
        fprintf ( GetTool()->GetLogFile(), "       Cannot open source file: %s\n", InputFileName );
    }
}

//**********************************************************************************************************************************

NUMBER GetNumberOfTemplates ( void )
{
    return ( NumberOfTemplates );
}

//**********************************************************************************************************************************
