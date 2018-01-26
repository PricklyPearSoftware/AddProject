
#ifndef addproject_header
#define addproject_header

//**********************************************************************************************************************************
//
//   Purpose: Add Project Utility header file
//
//   Project: Development Utilities
//
//  Filename: AddProject.h
//
//   Authors: Caroline.M.Mathieson (CMM)
//
//**********************************************************************************************************************************
//
//  Description
//  -----------
//
//! \file AddProject.h
//! \brief Contains all the classes and other definitions of the AddProject Utility.
//!
//**********************************************************************************************************************************

#include <assert.h>
#include <conio.h>
#include <ctype.h>
#include <direct.h> // was dir.h
#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <windows.h>

//**********************************************************************************************************************************

#define ASSERTION(a) Assertion ( a, __FILE__, __LINE__ )

//**********************************************************************************************************************************

// doxygen will generate a lot of warnings unless the typedef approach is used for these two fundamental types.

typedef char CHARACTER;
typedef int  NUMBER;

//**********************************************************************************************************************************

#define MAX_LEVELS (10)

#define MAX_NAME_LENGTH (100)

#define MAX_DRIVE     (2+1)
#define MAX_DIR       (255+1)
#define MAX_PATH_NAME (1000)

#define MAX_TEMPLATES          (100)
#define MAX_SUB_NODES          (100)
#define MAX_FUNCTION_ARGUMENTS (20)

#define MakeDirectory _mkdir

#ifdef VCPP

   #define GetWorkingDirectory _getcwd
   #define ChangeDirectory     _chdir
   #define StringLower         _strlwr
   #define StringUpper         _strupr
   #define FileNameSplit       _splitpath

#else

   #define GetWorkingDirectory getcwd
   #define ChangeDirectory     chdir
   #define StringLower         strlwr
   #define StringUpper         strupr
   #define FileNameSplit       fnsplit   // built in function

#endif

//**********************************************************************************************************************************

/**

\struct function_argument

\brief Used to store the textual definition of a function argument

A function argument takes the form:

  ArgumentType RedirectionString ArgumentName ## ArgumentPurpose

The number of redirection characters is needed for allignment of this argument with the other arguments, if any.

**/

typedef struct function_argument
{
   NUMBER NumberOfPointerRedirections; //!< The number of '*' characters are used in the redirection string, if any

   // arguments are of the form: Type <redirection string> Name

   CHARACTER ArgumentType      [ MAX_NAME_LENGTH ]; //!< the fundamental type of the argument ( e.g. NUMBER )
   CHARACTER RedirectionString [ MAX_NAME_LENGTH ]; //!< any redirection characters before the name
   CHARACTER ArgumentName      [ MAX_NAME_LENGTH ]; //!< the argument name
   CHARACTER ArgumentPurpose   [ MAX_NAME_LENGTH ]; //!< the purpose of the argument

} FUNCTION_ARGUMENT;

//**********************************************************************************************************************************

/**

\struct function_definition

\brief Used to store the textual definition of a function

When inserted into a source code file, a function declaration should be neatly formatted with all arguments alligned etc.

To acheive this, the function definition needs to be stored in its component parts so that the lengths can be calculated for
formatting purposes.

Note that the function code itself is in the template file and its formatting is retained.

An example output from a function definition might be:-

  <PRE><CODE>

  ##************************************************* ...
  ##
  ## FunctionPurpose
  ##
  ## %param ArgumentName1
  ##        ArgumentPurpose1
  ##
  ## %param ArgumentName2
  ##        ArgumentPurpose2
  ##
  ##  ...
  ##
  ## %param ArgumentNameN
  ##        ArgumentPurposeN
  ##
  ## %returns FunctionReturnType
  ##          FunctionReturnDescription
  ##
  ##************************************************* ...

  FunctionReturnType FunctionName ( ArgumentType1 RedirectionString1 ArgumentName1,
                                    ArgumentType2 RedirectionString2 ArgumentName2,

                                     ...

                                    ArgumentTypeN RedirectionStringN ArgumentNameN )
  {
     code
  }

  </CODE></PRE>

**/

typedef struct function_definition
{
   CHARACTER FunctionPurpose           [ 1000 ];            //!< description of the purpose of this function
   CHARACTER FunctionReturnType        [ MAX_NAME_LENGTH ]; //!< the full return type including any redirections ( e.g. NUMBER * )
   CHARACTER FunctionReturnDescription [ MAX_NAME_LENGTH ]; //!< the description of the returned value, if any
   CHARACTER FunctionName              [ MAX_NAME_LENGTH ]; //!< the name of the function itself

   NUMBER NumberOfArguments; //!< The number of parameters in the detected function declaration

   FUNCTION_ARGUMENT FunctionArguments [ MAX_FUNCTION_ARGUMENTS ]; //!< All the arguments for the function

} FUNCTION_DEFINITION;

//**********************************************************************************************************************************

/**

\struct initialisation_table_entry

\brief Used to store the textual definition of an initialisation table entry

Each initialisation table entry needs to store the name of the key in the initialisation file for this entry, the type of the entry
which will affect how it is processed and finally the directory within a NODE where the processed template file will be placed.

**/

typedef struct initialisation_table_entry
{
   CHARACTER *KeyName;      //!< The name of the key to seach for in the initialisation file
   CHARACTER *TemplateType; //!< The textual form of the type of file ( e.g. "Header" )
   CHARACTER *Directory;    //!< The path to the directory within a node directory to place this file

} INITIALISATION_TABLE_ENTRY;

//**********************************************************************************************************************************

/**

\class NAME

\brief The major class in the system. Used to store the names of things in their many different formats.

For flexibility, the name of a thing in the source code can take many forms. There can be a lowercase version, an uppercase version
or a version with underscores between the components of the name. For instance the project name could be:

   "Example Project Name"

This is the origonal format of the name, and is the form which might be used in the source code description. The form
"EXAMPLE_PROJECT_NAME" is the format called "long upper case with underscores". The form "exampleprojectname" is called "short
lower case" as all the spaces and capitalisation have been removed.

It is normal throughout a project that similar things will have a similar name, so the origonal is stored along with the name
converted to all the other formats. These different formats are accessable through the use of the MACROS in the template files. For
instance a class will be defined with an uppercase name with underscores e.g. "EXAMPLE_PROJECT_CLASS" but a reference to it will use
the short capitalised form "ExampleRootClass". That's a convention I have used throughout all my code. It is entirely up to the
author of the template files which format to use.

Click on the names of the classes in the above description to find out more information about that class.

**/

//**********************************************************************************************************************************

class NAME
{
   private:

      NUMBER NameNumber; //!< Keep a track of the number of these objects created
/*
      CHARACTER Long                     [ MAX_NAME_LENGTH ]; //!< Origonal format with any capitalisation and spacing intact
      CHARACTER LongUppercase            [ MAX_NAME_LENGTH ]; //!< This format gives: "THIS IS THE NAME"
      CHARACTER LongLowercase            [ MAX_NAME_LENGTH ]; //!< This format gives: "this is the name"
      CHARACTER LongCapitalised          [ MAX_NAME_LENGTH ]; //!< This format gives: "This Is The Name"
      CHARACTER LongUppercaseUnderscored [ MAX_NAME_LENGTH ]; //!< This format gives: "THIS_IS_THE_NAME"
      CHARACTER LongLowercaseUnderscored [ MAX_NAME_LENGTH ]; //!< This format gives: "this_is_the_name"
      CHARACTER ShortLowercase           [ MAX_NAME_LENGTH ]; //!< This format gives: "thisisthename"
      CHARACTER ShortUppercase           [ MAX_NAME_LENGTH ]; //!< This format gives: "THISISTHENAME"
      CHARACTER ShortCapitalised         [ MAX_NAME_LENGTH ]; //!< This format gives: "ThisIsTheName"
*/
	  CHARACTER *Long;                     //!< Origonal format with any capitalisation and spacing intact
	  CHARACTER *LongUppercase;            //!< This format gives: "THIS IS THE NAME"
	  CHARACTER *LongLowercase;            //!< This format gives: "this is the name"
	  CHARACTER *LongCapitalised;          //!< This format gives: "This Is The Name"
	  CHARACTER *LongUppercaseUnderscored; //!< This format gives: "THIS_IS_THE_NAME"
	  CHARACTER *LongLowercaseUnderscored; //!< This format gives: "this_is_the_name"
	  CHARACTER *ShortLowercase;           //!< This format gives: "thisisthename"
	  CHARACTER *ShortUppercase;           //!< This format gives: "THISISTHENAME"
	  CHARACTER *ShortCapitalised;         //!< This format gives: "ThisIsTheName"
public:

      NAME ( CHARACTER *NewName );

	  ~NAME ( void );

      void Debug ( void );

   public:

      CHARACTER *GetLong                     ( void );
      CHARACTER *GetLongUppercase            ( void );
      CHARACTER *GetLongLowercase            ( void );
      CHARACTER *GetLongCapitalised          ( void );
      CHARACTER *GetLongUppercaseUnderscored ( void );
      CHARACTER *GetLongLowercaseUnderscored ( void );
      CHARACTER *GetShortLowercase           ( void );
      CHARACTER *GetShortUppercase           ( void );
      CHARACTER *GetShortCapitalised         ( void );
};

//**********************************************************************************************************************************

/**

\class TEMPLATE

\brief A key class in the system. Used to store the main attributes of the files being manipulated by the system.

This utility is all about manipulating template files. These are stored in the directory \ref TOOL::TemplateDirectoryFileName,
which is the second parameter passed to the utility when invoked. These files are processed as they are copied from the source
directory to the destination directory. As any given source template file could be copied multiple times to different destinations,
then each copy needs an instance of this class to define the destination names and final filename etc.

When an instance of this class is created, the full source and directory names of the template file are provided along with the
purpose of the template file, which depends on context, and also what type of template this will be i.e. how to process the template.

There are several template types:-

- \ref RootFileTypes
- \ref MasterFileTypes
- \ref TemplateFileTypes
- \ref SourceFileTypes

\section RootFileTypes The Root File Type

These are files which are intended to go into the root of the target file tree.

\section MasterFileTypes The Master File Type

These files will be used as is without a change of name.

\section TemplateFileTypes The Template File Type

These files will be used as template files in the project at a later stage.

\section SourceFileTypes The Source File Type

These files will be used with a new name (usually the existing name prefixed with another context specific name).

Click on the names of the classes in the above description to find out more information about that class.

**/

//**********************************************************************************************************************************

class TEMPLATE
{
   public:

      NUMBER TemplateNumber; //!< Keep a track of the number of these objects created

      class NODE *Node;

      FILE *InputFile;  //!< Used to access the source file
      FILE *OutputFile; //!< Used to access the destination file

      CHARACTER InputFileName  [ MAX_PATH_NAME ]; //!< The full path and filename of the source file (derived)
      CHARACTER OutputFileName [ MAX_PATH_NAME ]; //!< The full path and filename of the destination file (derived)

   private:

      NAME *TemplatePurpose; //!< A brief description of the purpose of the source file e.g. "source file"
      NAME *TemplateType;    //!< Defines how the source file should be processed on its way to the destination.

      CHARACTER SourceDirectoryName      [ MAX_PATH_NAME ]; //!< The directory part of the full source filename
      CHARACTER SourceFileName           [ MAX_PATH_NAME ]; //!< The filename part of the full source file name
      CHARACTER DestinationDirectoryName [ MAX_PATH_NAME ]; //!< The directory part of the full destination filename
      CHARACTER DestinationFileName      [ MAX_PATH_NAME ]; //!< The filename part of the full destination filename

      // these are generated by fnsplit

      CHARACTER SourceName      [ 500 ];      //!< The name part of the source filename
      CHARACTER SourceExtension [ 100 ];      //!< The extension part of the source filename

      CHARACTER DestinationName      [ 500 ]; //!< The name part of the destination filename
      CHARACTER DestinationExtension [ 100 ]; //!< The extension part of the destination filename

      CHARACTER ReturnedName [ MAX_PATH_NAME ];

      // so the full filenames of the source and target are:-
      //
      //   SourceDirectoryName\SourceName.SourceExtension
      //
      //   DestinationDirectoryName\DestinationName.DestinationExtension
      //

   public:

      TEMPLATE ( NODE       *ParentNode,
                 CHARACTER  *NewSourceDirectoryName,
                 CHARACTER  *NewSourceFileName,
                 CHARACTER  *NewDestinationDirectoryName,
                 CHARACTER  *NewDestinationFileName,
                 CHARACTER  *NewTemplatePurpose,
                 CHARACTER  *NewTemplateType );

      ~TEMPLATE ( void );

      void Debug ( void );

      void Build ( CHARACTER *MacroPrefix,
                   NUMBER    *LevelNumbers );

   public:

      CHARACTER *GetSourceDirectoryName      ( BOOLEAN AddBackSlash = FALSE );
      CHARACTER *GetSourceFileName           ( BOOLEAN AddBackSlash = FALSE );
      CHARACTER *GetDestinationDirectoryName ( BOOLEAN AddBackSlash = FALSE );
      CHARACTER *GetDestinationFileName      ( BOOLEAN AddBackSlash = FALSE );
      CHARACTER *GetSourceName               ( BOOLEAN AddBackSlash = FALSE );
      CHARACTER *GetSourceExtension          ( BOOLEAN AddBackSlash = FALSE );
      CHARACTER *GetDestinationName          ( BOOLEAN AddBackSlash = FALSE );
      CHARACTER *GetDestinationExtension     ( BOOLEAN AddBackSlash = FALSE );

      NAME *GetTemplatePurpose ( void );

      NAME *GetTemplateType ( void );

   private:

      void ProcessReturnedName ( CHARACTER *String,
                                 BOOLEAN    AddBackSlash );
};

//**********************************************************************************************************************************

/**

\class MASTERFILE

\brief Encapsulates a file which will not change its name as it is copied into the project.

Some files in the system do not need to change name, these are existing files known to be needed when building the project. These
will depend on context so thay are specified in the project specification file.

**/

class MASTERFILE : public TEMPLATE
{
   private:

      public:

      MASTERFILE ( NODE       *ParentNode,
                   CHARACTER  *SourceDirectoryName,
                   CHARACTER  *SourceFileName,
                   CHARACTER  *DestinationDirectoryName,
                   CHARACTER  *DestinationFileName,
                   CHARACTER  *FileTypeComment,
                   CHARACTER  *MasterTemplateType );

      void Build ( NUMBER *LevelNumbers );

   public:
};

//**********************************************************************************************************************************

/**

\class SOURCEFILE

\brief Encapsulates a file which will usually change its name as it is copied into the project.

The difference between the master file and a source file is that a source file is known to be needed but is also known to be used
in a modified format in each instance of a NODE in a project.

An example will be that there are many application classes in the system all derived from a top level APPLICATION class. So for each
NODE there will need to be a APPLICATION derived class for that NODE.

Having source files allows for this derivation to be named by prefixing a standard name such as "ApplicationClass" with the context
specific name e.g. "InterfaceApplicationClass".

**/

class SOURCEFILE : public TEMPLATE
{
   private:

      public:

      SOURCEFILE ( NODE       *ParentNode,
                   CHARACTER  *SourceDirectoryName,
                   CHARACTER  *SourceFileName,
                   CHARACTER  *DestinationDirectoryName,
                   CHARACTER  *DestinationFileName,
                   CHARACTER  *FileDescription,
                   CHARACTER  *SourceTemplateType );

      void Build ( NUMBER *LevelNumbers );

   public:
};

//**********************************************************************************************************************************

/**

\class NODE

\brief Used to represent a single point in the project directory tree. Can contain sub nodes and has a parent node.

When the project layout is being derived, this object is used to represent a single point in the project directory tree.

It can therefore contain one or more sub NODE objects in the same way that a directory can cantain subdirectories.

The NODE object behaves differently depending on a set of internal flags set when the initialisation file is scanned.

A NODE contains references to its parent and its children, but not to its siblings.

At some level, the source code inside a node an all its sub nodes will be combined together into a single target.

The distinction between Name and Type is that the Name is the context specific name of this instance of a NODE and the
Type is the general name of all nodes with a similar purpose to this one. The Name is used when generating the
directories and files, the Type is used when finding the template files.

e.g. The context specific NODE name might be "Interface" but the generic type is "Group".

**/

class NODE
{
   private:

      NODE *Parent;                      //!< The NODE containing this NODE, if any

      bool IsLeaf;                       //!< If TRUE, then this is the last node in a tree;

      bool IsBuilt;                      //!< If TRUE, then this node has already been built and does not need rebuilding

      bool IsCommon;                     //!< If TRUE, then this is a common shared NODE

      bool IsTarget;                     //!< If TRUE, then this NODE and all its sub nodes are combined into a single build target

      bool IsLibrary;                    //!< If TRUE, then this NODE and all sub nodes are built as a DLL rather than an EXE

      bool UseMnemonicNames;             //!< If TRUE, then always use the mnemonic name rather than the full name

      CHARACTER Mnemonic [ 10 ];        //!< The short name (abbreviation) for this NODE

      CHARACTER Comment [ 1000 ];        //!< A short description of the purpose of this NODE

      NAME *Name;                        //!< The name of this node to be used in processing files

      NUMBER Number;                     //!< The index of this node in the parent's subnode list

      NUMBER LevelNumber;                //!< The level in the node hierarchy for this node (level 0 is the root)

      NUMBER NodeNumbers [ MAX_LEVELS ]; //!< The node indices for all the nodes above this one in the node hierarchy

      NUMBER NodeNumber;                 //!< Keep a track of the number of these nodes created

   protected: // these are accessed by NODE derivatives

      NAME *Type;                                      //!< Should be set to the node derived class name ( e.g. "Group", "Application" etc )

      FILE *VisualStudioRootFile;                      //!< The visual studio project file to be built for this node

      NUMBER NumberOfSubNodes;                         //!< The number of sub nodes this node contains

      NODE *SubNodes [ MAX_SUB_NODES ];                //!< The sub nodes themselves

      CHARACTER TargetDirectoryName [ MAX_PATH_NAME ]; //!< The full path prefix for all target files in this node

      NUMBER NumberOfMasterFiles;                      //!< The number of master files defined for this NODE

      MASTERFILE *MasterFiles [ MAX_TEMPLATES ];       //!< The master files themselves

      NUMBER NumberOfSourceFiles;                      //!< The number of source files defined for this NODE

      SOURCEFILE *SourceFiles [ MAX_TEMPLATES ];       //!< The source files themselves

   public:

      NODE ( NODE      *NewParent,
             CHARACTER *NewNodeName,
             NUMBER     NewNodeNumber,
             NUMBER    *NewNodeNumbers,
             NUMBER     NewLevelNumber );

      ~NODE ( void );

   public:

      void Debug ( void );

      void Load ( void );

      void AddRootFiles ( void );

      void AddMasterFiles ( void );

      void AddSourceFiles ( void );

      void AddTemplateFiles ( void );

      void CreateDirectories ( void );

      void Build ( void );

   public:

      CHARACTER *GetTargetDirectoryName ( void );

      NUMBER GetNumberOfSubNodes ( void );

      NODE *GetSubNode ( NUMBER SubNodeNumber );

      NUMBER GetNumberOfMasterFiles ( void );

      MASTERFILE *GetMasterFile ( NUMBER MasterFileNumber );

      NUMBER GetNumberOfSourceFiles ( void );

      SOURCEFILE *GetSourceFile ( NUMBER SourceFileNumber );

   public:

      NODE *GetParent ( void );

      bool GetIsLeaf ( void );

      bool GetIsBuilt ( void );

      bool GetIsCommon ( void );

      bool GetUseMnemonicNames ( void );

      CHARACTER *GetMnemonic ( void );

      CHARACTER *GetComment ( void );

      NUMBER GetNumber ( void );

      NUMBER GetLevelNumber ( void );

      NUMBER *GetNodeNumbers ( void );

      NAME *GetName ( void );

      NAME *SetName ( CHARACTER *NewName );

      NAME *GetType ( void );

      NAME *SetType ( CHARACTER *NewType );

   private:

      void AddFiles ( NUMBER                      NumberOfTableEntries,
                      INITIALISATION_TABLE_ENTRY *InitialisationTable,
                      CHARACTER                  *SectionName,
                      BOOLEAN                     MasterFile,
                      BOOLEAN                     TemplateFile,
                      BOOLEAN                     RootFile );

      void AddFile ( NUMBER     EntryNumber,
                     CHARACTER *SectionName,
                     CHARACTER *MasterTemplateType,
                     CHARACTER *Directory,
                     BOOLEAN    MasterFile   = FALSE,
                     BOOLEAN    TemplateFile = FALSE,
                     BOOLEAN    RootFile     = FALSE );

      //void GetMasterFile ( NUMBER     EntryNumber,
      //                     CHARACTER *SectionName,
      //                     CHARACTER *MasterTemplateType,
      //                     CHARACTER *Directory,
      //                     BOOLEAN    MasterFile   = FALSE,
      //                     BOOLEAN    TemplateFile = FALSE,
      //                     BOOLEAN    RootFile     = FALSE );

      //void GetSourceFile ( NUMBER     EntryNumber,
      //                     CHARACTER *SectionName,
      //                     CHARACTER *SourceTemplateType,
      //                     CHARACTER *Directory );

      void AddMasterFile ( CHARACTER *SourceFileName,
                           CHARACTER *TargetFileName,
                           CHARACTER *FileTypeComment,
                           CHARACTER *MasterTemplateType );

      void AddSourceFile ( CHARACTER *SourceFileName,
                           CHARACTER *TargetFileName,
                           CHARACTER *FileDescription,
                           CHARACTER *SourceTemplateType );

      void BuildMasterFiles ( void );

      void BuildSourceFiles ( void );

      void GenerateVisualStudioRoot ( void );
};

//**********************************************************************************************************************************

/**

\class TEMPLATEFILE

\brief Represents a single file in the project. The file may be any of the types specified for a TEMPLATE.

Each file as read as it is copied by the utility from the template directory to the target directory. If the file contains any
recognized macros then these are processed during the copy.

Because some macros can specify a repetition of a specific sub node value then a record needs to be kept of where this particular
file fits into the project tree. Because the number of levels is variable then the arrays recording the indices at each level have
to be maximum length.

**/

class TEMPLATEFILE
{
   private:

      NODE *Node;  //!< The node containing this templatefile

      TEMPLATE *Parent; //!< The template containing this templatefile

      CHARACTER *FileStore; //!< The whole of the template file is loaded into the memory allocated here

      long FileSize;    //!< This is how big the loaded template file is
      long FilePointer; //!< This is where we are in the memory copy of the file

      NUMBER LevelNumbers [ MAX_LEVELS ]; //!< The index numbers for each level in the hierarchy for the parent node

      NUMBER MasterFileNumber; //!< Index of this templatefile in the parent masterfile array
      NUMBER SourceFileNumber; //!< Index of this templatefile in the parent sourcefile array

      CHARACTER MacroPrefix  [      3 ]; //!< the character which starts all macros ( e.g. ${MACRONAME} )
      CHARACTER MacroName    [    100 ]; //!< the macro name extracted from the template file.
      CHARACTER OutputString [ 100000 ]; //!< for storing the result of the macro expansion

      FUNCTION_DEFINITION FunctionDefinition; //!< all the parts of a function definition

   public:

      TEMPLATEFILE ( NODE     *ParentNode,
                     TEMPLATE *ParentTemplate,
                     FILE     *InputFile,
                     NUMBER   *LevelNumbers );

      TEMPLATEFILE ( NODE      *ParentNode,
                     TEMPLATE  *ParentTemplate,
                     CHARACTER *InputFileStore,
                     long       InputFileSize,
                     NUMBER    *LevelNumbers );

      TEMPLATEFILE ( NODE      *ParentNode,
                     TEMPLATE  *ParentTemplate,
                     CHARACTER *InputFileStore,
                     long       StartPosition,
                     long       EndPosition,
                     NUMBER    *LevelNumbers,
                     NUMBER     NewMasterFileNumber,
                     NUMBER     NewSourceFileNumber );

      ~TEMPLATEFILE ( void );

   private:

      CHARACTER GetCharacter ( void );

      bool EndOfFile ( void );

      bool CheckMasterFileMacros ( CHARACTER  *MacroName,
                                   MASTERFILE *MasterFile );

      bool CheckSourceFileMacros ( CHARACTER  *MacroName,
                                   SOURCEFILE *SourceFile );

      bool CheckRepetitionMacros ( CHARACTER *MacroName,
                                   FILE      *OutputFile );

      bool CheckCompanyMacros ( CHARACTER *MacroName );

      bool CheckNameMacros ( CHARACTER *MacroName );

      bool CheckStandardMacros ( CHARACTER *MacroName );

      bool CheckFileDescriptionMacros ( CHARACTER *MacroName );

      void InsertSeperator ( FILE *OutputFile );

      void InsertStartOfComment ( FILE *OutputFile );

      void InsertBlankComment ( FILE *OutputFile );

      void InsertFullSeperator ( FILE *OutputFile );

      void InsertProcedureHeader ( FILE *OutputFile );

      void InsertFunctionEnding ( FILE *OutputFile );

      void InsertProcedureEnding ( FILE *OutputFile );

      void RepeatLevelOne ( FILE *OutputFile );

      void RepeatApp ( FILE   *OutputFile,
                       NUMBER  LevelOneNumber );

      void RepeatRootMasterFiles ( FILE *OutputFile );

      void RepeatLevelOneMasterFiles ( FILE *OutputFile );

      void RepeatLevelTwoMasterFiles ( FILE *OutputFile );

      void RepeatRootSourceFiles ( FILE *OutputFile );

      void RepeatLevelOneSourceFiles ( FILE *OutputFile );

      void RepeatLevelTwoSourceFiles ( FILE *OutputFile );

      void FindEndOfMacro ( CHARACTER *MacroName );

      void ExtractFunctionDefinition ( CHARACTER *MacroName );

      CHARACTER *CleanParameter ( CHARACTER *Parameter );

   public:

      void Scan ( CHARACTER *NewMacroPrefix,
                  FILE      *OutputFile,
                  NUMBER     MasterFileIndex  = -1,
                  NUMBER     SourceFileIndex  = -1 );

      CHARACTER *GetFileStore ( void );

      long GetFileSize ( void );

      long GetFilePointer ( void );

      void SetFilePointer ( long NewFilePointer );
};

//**********************************************************************************************************************************

/**

\class TOOL

\brief The working object in the system. which ties all the other objects together.

In any object oriented program, there needs to be one global object which ties all the other objects together. This is that object
for this utility. I prefer to keep the main function as simple as possible and do all the work in the objects. I keep only the one
global variable, in this case \ref Tool which is a pointer to this object. Because this object definition indirectly uses all the
other object definitions, it needs to be defined last in the header file.

The main attributes of this object are the record of the two command line arguments, \ref ProjectSpecificationFileName and \ref
TemplateDirectoryFileName. The two logfile variables \ref LogFileName and \ref LogFile. The two debug file variables \ref
DebugFileName and \ref DebugFile. The variable \ref IndentLevel is used for indentation in the log files.

Finally the most important variable in the object is the pointer \ref Root which is the topmost NODE in the NODE hierarchy.

When the %TOOL object is instantiated, the exact date and time is recorded for use when creating the timestamps for all the created
files. Also the command line arguments are recorded for later use but are not yet verified.

**/

//**********************************************************************************************************************************

class TOOL
{
   private:

      CHARACTER ProjectSpecificationFileName [ MAX_PATH_NAME ]; //!< The full filename of the project specification file
      CHARACTER TemplateDirectoryFileName    [ MAX_PATH_NAME ]; //!< The full filename of the template directory
	  CHARACTER OutputDirectoryFileName      [ MAX_PATH_NAME ]; //!< The full filename of the output directory

      CHARACTER LogFileName [ MAX_PATH_NAME ];                  //!< The full filename of the log file

      CHARACTER DebugFileName [ MAX_PATH_NAME ];                //!< the full filename of the debug file

      FILE *LogFile;                                            //!< used to access the log file

      FILE *DebugFile;                                          //!< used to access the debug file

      NUMBER IndentStep;  //!< how much to adjust the indent level when using the Indent() and Outdent() functions.

      NUMBER IndentLevel; //!< \brief Specifies the amount of white space to be used at the start of a log file entry
                          //!<
                          //!< This can be used to indent or outdent entries on the log file in order to show their hierarchy. This
                          //!< number of space characters are put at the beginning of all entries in the log file. If no
                          //!< indentation is required then it should be set to zero.

      NAME *CompanyName;                   //!< The full name of the company owning the project

      CHARACTER CompanyWebAddress [ 200 ]; //!< The full URL for the company owning the project

      CHARACTER AuthorName [ 100 ];        //!< The full name of the first author of all files (e.g. "Joe.B.Bloggs")

      CHARACTER AuthorInitials [ 10 ];     //!< Up to three letters for the initials of the first author of all files (e.g. "JBB" )

      NUMBER SourceFileWidth;              //!< How wide should the seperator lines be?

      CHARACTER SeperatorCharacter;        //!< What character should be used for the seperator

      NUMBER NumberOfLevels;               //!< The number of levels in the node hierarchy

      NAME *LevelNames [ MAX_LEVELS ];     //!< The name of each node level ( "Project", "Group" etc)

      NAME *LevelMnemonics [ MAX_LEVELS ]; //!< The abbreviation for the node level to be used in macros

      NODE *Root;                          //!< The topmost object in the NODE hierarchy, contains all the other NODES.

      CHARACTER FileDate [  50 ];          //!< The exact date in ASCII of the time of invokation of the TOOL object.
      CHARACTER FileTime [  50 ];          //!< The exact time in ASCII of the time of invokation of the TOOL object.
      CHARACTER FileYear [  10 ];          //!< The exact year in ASCII of the time of invokation of the TOOL object.

      CHARACTER FileTimeLong [ 100 ];      //!< The exact time in full format with the day of the week and month etc.

   public:

      TOOL ( CHARACTER *NewProjectSpecificationFileName,
             CHARACTER *NewTemplateDirectoryFileName,
             CHARACTER *NewOutputDirectoryFileName );

      ~TOOL ( void );

      void Execute ( void );

      void Load ( void );

      void Debug ( void );

   public:

      CHARACTER *GetProjectSpecificationFileName ( void );

      CHARACTER *GetTemplateDirectoryFileName ( void );

      CHARACTER *GetOutputDirectoryFileName ( void );

      FILE *GetLogFile ( void );

      FILE *GetDebugFile ( void );

      NUMBER GetIndentLevel ( void );

      NUMBER Indent ( void );

      NUMBER Outdent ( void );

      void ApplyIndent ( void );

      NAME *GetCompanyName ( void );

      CHARACTER *GetCompanyWebAddress ( void );

      CHARACTER *GetAuthorName ( void );

      CHARACTER *GetAuthorInitials ( void );

      NUMBER GetSourceFileWidth ( void );

      CHARACTER GetSeperatorCharacter ( void );

      NUMBER GetNumberOfLevels ( void );

      NAME *GetLevelName ( NUMBER Level );

      NAME *GetLevelMnemonic ( NUMBER Level );

      NODE *GetRoot ( void );

      CHARACTER *GetFileDate     ( void );
      CHARACTER *GetFileTime     ( void );
      CHARACTER *GetFileYear     ( void );
      CHARACTER *GetFileTimeLong ( void );
};

//**********************************************************************************************************************************

int main ( NUMBER     ArgumentCount,
           CHARACTER *ArgumentVectors [] );

//**********************************************************************************************************************************

CHARACTER *GetTitleText ( void );

CHARACTER *GetHelpText ( void );

TOOL *GetTool ( void );

void PrintMacroNames ( void );

//**********************************************************************************************************************************

NUMBER GetNumberOfRootFileTypes ( void );

NUMBER GetNumberOfMasterFileTypes ( void );

NUMBER GetNumberOfTemplateFileTypes ( void );

NUMBER GetNumberOfSourceFileTypes ( void );

//**********************************************************************************************************************************

INITIALISATION_TABLE_ENTRY *GetRootFileTypesTable ( void );

INITIALISATION_TABLE_ENTRY *GetMasterFileTypesTable ( void );

INITIALISATION_TABLE_ENTRY *GetTemplateFileTypesTable ( void );

INITIALISATION_TABLE_ENTRY *GetSourceFileTypesTable ( void );

//**********************************************************************************************************************************

NUMBER GetNumberOfNames ( void );

NUMBER GetNumberOfNodes ( void );

NUMBER GetNumberOfTemplates ( void );

//**********************************************************************************************************************************

void Assertion ( bool  Condition,
                 char *FileName,
                 int   LineNumber );

void fnsplit ( CHARACTER *SourceFileName,
               CHARACTER *Drive,
               CHARACTER *Directory,
               CHARACTER *SourceName,
               CHARACTER *SourceExtension );

void OperatorConfidence ( void );

void DisplayProgress ( NUMBER Current, 
	                   NUMBER Total );

//**********************************************************************************************************************************

#endif // addproject_header
