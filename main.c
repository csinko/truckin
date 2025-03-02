/*
  ...... ......                                             ..---.
 -######-#####-                                           .+#######-
  -####-.##-.        ....      ....       ..--.          .###+..####-    .. .....
  -#######-        -+#####-. .+#####+..-+#######-.      .####-  .####..+###+#####-
  -#######-       -###--###..###--###++####--####-      -####+.  ####-.+####--###-
  -####--##+     .+###.---. +###-.--.. +###-.####-      -#####+-+####+. ###+..###-
 .#####+.+###-   .####+..--.####+. --. +########+.      -############+..####..###+.
.+######-.#####-..+########--########+.#####+--          +###########- +####--####-
-########..######+.+######+. -######+..#####.             -#########. -#####.######
 .------.  .-----.  .----.    .----.  -#####-.              .--+--.    ----.  ----.
                                      +#####+.
                                       ......                                                              .
                           .+#++++++#-                                  .-+-.        .+##-               -###+
                           ###########-                               .####+.       .####+              .+####.
                          .###-####.##- ..-..--. ..--. ..--.   ..--..  .###-. ----.  ..---   ..-..---.   .###+
                              -####.  .#########-####..####+ .-######+. ###-.-####. .#####..+#########-  .+##.
                              -####-   -####+###.+### .-###-.-###-.-#-. #######+.   .+###+  -####--###-  -+-
                             .+####+   .###+...  -###..+###..####       ########-.   -###+  .+###..###+
                             .######.  -####.   .-#########-.#####++++..####-#####- .-####. -####--####-       -###. .+##+. .+##-
                            .#######+..######.  .+##########.+########.+####--#####--#####+.+####+-####+.     -#####.+####+.#####-
                            .########.-######-   .+########.  -######..#####. #####+-######-#####--#####-     .#####.-####-.+####.

                                  .-.
                               -++#+.                           .----.
                            -+#####+.                       .-+++.-+++-.
                          .+-.-+-.#+                       .#-.#-+#+..-#.
               .++-....  -#-+.........-++.                 +-.+-  -###+.
              -#.    .+#####...-###+     -#-+#.         -##.  .#. .+#+-#
             .##- ..        ..-##- -##++##++##+..     -##+-#. -#-.. .--
             .##- .-##-     ..+#--+#+---###+.-###+..-+....#--+-+#-++++.
             .##-.  .+##-..#####+##-  ..-+##+#+ .##+-....  ++-+-.
             .###-    +###########+++-++##-.+#++-. .-#+++++###-.
             .####-    -#-.---######+--.....+##+###-   .--##.
             .#####-   .+.     .+#####+##+..+#-##-..       .--
             .+####-   .+.        -###-.       #+.         .##-
               +###+-..-+         +##+..       ++.-#######+. .-.
         -#+-..##-....-#-        +####++-...-+##-             --.
         -+. -#-     ..+.        .++#####++-.               ..  .---.
          .++-.       .+.           -########--.           .        .---.                              .--++-----..
          .-+---     .+.           -###+-+##-.                         .-+-.                        -+###############-.
           .-+##+..-+-           .####-..                    .-.         .-#-..                   -############++######.
               .....           .+####-                  .+##++#-.           ...-##+.             -############...+.+-##+
                             .-+....                 +########.               ...  -#.          .######+-###.------.-###
                            .#. .             ..-+#############.       ..         .+####++-...  +######-+##---.... .+##+
                           +#.                .####-  .-########-.. .-##..--.             ..-+#+#######-+#####+-+#++---+
                          -+               .-##+-.------..++######+####+--#+. ...              .#######--+###+..-+...+#+
                         .+.             .+##-        . --  ..-+###########.  ...               #######+-+--...... .+##-
                         .+.          .+####+.       -- .###-.    .-#######..#-..               ########.+---+.. ...##+.
                          +-           .##+-.        +- -#####+-.  .-########--...              +#######.+##-....--###-
                           ++...   -####--##-.+.  .-#. -######-.+     .#####---.      ..        -#######.+#-..--. .--+.
                            .++--...----+####+-###+-+--#######+.#       -####.-+--.            .-######+.+#+--+--+###-
                              .-############+###--############+.#         -#####---.  . .. ..-++.+#####+.##+..-.-+##-
                                 ..---++######################+-+          .+#####--..  . .-+-.  -#####+-##+-...+##-
                                     .--+#####################+#-            -######-....-#-.  .-######-+#+.--.+##-
                                  -+#########################+++.             .-###--...+#-  .+#######+-###--####-
                                 +##########################-+-.                .+##+#++#. .-##-+#####.####-####+.
                                .#########################.++.                    .#####-..-##--#####-.###########-
                                 +#####################+.-################-........-###-+############. .-###+----#
                                 +#################+--+################################+#####+#######+.+++. ..+##.
                        .----++++####--++++++++--+#####################++##################+############+- ..-##.
                       .++#############################################################################+-...+#+.
                           ....------+-------------.-###################################################+#++##----------.
                                                     ..--+++++++++++++++++--...-+-+++++++--......-++++++++++++++++++++++.

*/
#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <fnmatch.h>
#include <getopt.h>
#include <limits.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#include "version.h"

// Color strings (modifiable at runtime)
static const char *color_reset;
static const char *color_header;
static const char *color_total;
static const char *color_filename;
static const char *color_tokens;
static const char *color_percent;

// Constants
typedef enum {
    kBufSize_bytes           = 8192,
    kThreadThreshold_files   = 10,
    kMaxNumPatterns          = 64,
    kMaxNumExcludes          = 64,
    kInitialFileListCapacity = 128,
    kInitialBufferSize_bytes = 1024,
    kMaxFileColWidth_chars   = 60
} Constants_t;

// Typedefs
typedef struct {
    char *pszPath;
    char *pszRelPath;
} FileEntry_t;

typedef struct {
    int     iStart;
    int     iEnd;
    char   *pszBuffer;
    size_t  uBufferLength;
    size_t  uBufferCapacity;
} ThreadArgs_t;

typedef struct IgnorePatterns {
    char                   *pszDir;
    char                  **ppSzPatterns;
    int                     iPatternCount;
    struct IgnorePatterns  *pParent;
} IgnorePatterns_t;

// Global variables
static int          iVerbose       = 0;
static char        *pszOutputFile  = NULL;
static char        *pszBasePath    = NULL;
static FileEntry_t *pFileList      = NULL;
static int          iFileCount     = 0;
static int          iFileCapacity  = 0;
static uint64_t    *puFileTokens   = NULL;
static char        *ppSzIncludes[kMaxNumPatterns] = { 0 };
static char        *ppSzExcludes[kMaxNumExcludes] = { 0 };
static int          iIncludeCount  = 0;
static int          iExcludeCount  = 0;
static int          iJsonOutput    = 0;  // Flag for JSON output
static int          iNoColor       = 0;  // Flag to disable color
static pthread_mutex_t mutexOutput = PTHREAD_MUTEX_INITIALIZER;

// Static function prototypes
static char *expandTilde( const char *const pszPath );
static char *getRelativePath( const char *const pszFilePath, const char *const pszBase );
static char *getRelPath( const char *const pszFilePath, const char *const pszDir );
static char *truncateString( const char *const pszStr, const int iMaxWidth );
static bool  isTextFile( const char *const pszFileName );
static void  addFileToList( const char *const pszFilePath );
static char **parseGitignore( const char *const pszPath, int *const piCount );
static bool  isExcluded( const char *const pszFilePath, const IgnorePatterns_t *const pPatterns );
static void  expandPattern( const char *const pszPattern, IgnorePatterns_t *pPatterns );
static uint64_t countTokens( const char *const pBuf, const size_t uSize, int *const piInToken );
static void  processFile( const int iIndex,
                           const char *const pszFileName,
                           char **const ppLocalBuffer,
                           size_t *const puBufferLength,
                           size_t *const puBufferCapacity );
static void *workerThread( void *pArg );
static void  parseArguments( const int iArgc, char *const pArgv[] );
static void  freePatterns( void );
static int   cmpTokenCount( const void *pA, const void *pB );

// Public functions
int main( const int iArgc, char *const pArgv[] );

// Static functions
static char *expandTilde( const char *const pszPath )
{
    char *pszResult = NULL;
    if ( pszPath[ 0 ] == '~' )
    {
        const char *pszHome = getenv( "HOME" );
        if ( pszHome == NULL )
        {
            pszResult = strdup( pszPath );
        }
        else
        {
            size_t uHomeLen = strlen( pszHome );
            size_t uPathLen = strlen( pszPath );
            pszResult = malloc( uHomeLen + uPathLen );
            if ( pszResult == NULL )
            {
                perror( "malloc" );
                exit( EXIT_FAILURE );
            }
            strcpy( pszResult, pszHome );
            strcat( pszResult, pszPath + 1 );
        }
    }
    else
    {
        pszResult = strdup( pszPath );
    }
    return pszResult;
}

static char *getRelativePath( const char *const pszFilePath, const char *const pszBase )
{
    size_t uBaseLen = strlen( pszBase );
    char *pszRel = NULL;
    if ( strncmp( pszFilePath, pszBase, uBaseLen ) == 0 )
    {
        const char *pszRest = pszFilePath + uBaseLen;
        if ( ( *pszRest == '/' ) == true )
        {
            pszRest++;
        }
        pszRel = strdup( pszRest );
    }
    else
    {
        pszRel = strdup( pszFilePath );
    }
    return pszRel;
}

static char *getRelPath( const char *const pszFilePath, const char *const pszDir )
{
    size_t uDirLen = strlen( pszDir );
    char *pszRel = NULL;
    if ( ( strncmp( pszFilePath, pszDir, uDirLen ) == 0 ) &&
         ( ( pszFilePath[ uDirLen ] == '/' ) || ( pszFilePath[ uDirLen ] == '\0' ) ) )
    {
        const char *pRel = pszFilePath + uDirLen;
        if ( ( *pRel == '/' ) == true )
        {
            pRel++;
        }
        pszRel = strdup( pRel );
    }
    return pszRel;
}

static char *truncateString( const char *const pszStr, const int iMaxWidth )
{
    int iLen = strlen( pszStr );
    char *pszTrunc = NULL;
    if ( iLen <= iMaxWidth )
    {
        pszTrunc = strdup( pszStr );
    }
    else
    {
        int iWidth = iMaxWidth;
        if ( iWidth < 4 )
        {
            iWidth = 4;
        }
        pszTrunc = malloc( iWidth + 1 );
        if ( pszTrunc == NULL )
        {
            perror( "malloc" );
            exit( EXIT_FAILURE );
        }
        strncpy( pszTrunc, pszStr, iWidth - 3 );
        pszTrunc[ iWidth - 3 ] = '\0';
        strcat( pszTrunc, "..." );
    }
    return pszTrunc;
}

static bool isTextFile( const char *const pszFileName )
{
    int iFd = open( pszFileName, O_RDONLY );
    if ( iFd < 0 )
    {
        return false; // Cannot open file
    }
    char pBuffer[ 512 ] = { 0 };
    ssize_t iBytes = read( iFd, pBuffer, sizeof( pBuffer ) );
    close( iFd );
    if ( iBytes <= 0 )
    {
        return true; // Empty file is text
    }
    for ( size_t uIdx = 0; uIdx < ( size_t ) iBytes; uIdx++ )
    {
        if ( pBuffer[ uIdx ] == '\0' )
        {
            return false; // Null byte indicates binary
        }
    }
    return true; // No null bytes, assume text
}

static void addFileToList( const char *const pszFilePath )
{
    if ( iFileCount >= iFileCapacity )
    {
        int iNewCap = ( iFileCapacity == 0 ) ? kInitialFileListCapacity : iFileCapacity * 2;
        FileEntry_t *pTemp = realloc( pFileList, iNewCap * sizeof( FileEntry_t ) );
        if ( pTemp == NULL )
        {
            perror( "realloc" );
            exit( EXIT_FAILURE );
        }
        pFileList = pTemp;
        iFileCapacity = iNewCap;
    }
    pFileList[ iFileCount ].pszPath = strdup( pszFilePath );
    pFileList[ iFileCount ].pszRelPath = getRelativePath( pszFilePath, pszBasePath );
    if ( pFileList[ iFileCount ].pszPath == NULL ||
         pFileList[ iFileCount ].pszRelPath == NULL )
    {
        perror( "strdup" );
        exit( EXIT_FAILURE );
    }
    iFileCount++;
}

static char **parseGitignore( const char *const pszPath, int *const piCount )
{
    FILE *pFile = fopen( pszPath, "r" );
    char **ppPatterns = NULL;
    if ( pFile == NULL )
    {
        *piCount = 0;
    }
    else
    {
        int iCapacity = 0;
        int iCount = 0;
        char pLine[ PATH_MAX ];
        while ( fgets( pLine, sizeof( pLine ), pFile ) != NULL )
        {
            char *pStart = pLine;
            while ( isspace( *pStart ) == true )
            {
                pStart++;
            }
            char *pEnd = pStart + strlen( pStart ) - 1;
            while ( ( pEnd > pStart ) && ( isspace( *pEnd ) == true ) )
            {
                *pEnd-- = '\0';
            }
            if ( ( *pStart == '\0' ) || ( *pStart == '#' ) || ( *pStart == '!' ) )
            {
                continue;
            }
            if ( iCount >= iCapacity )
            {
                iCapacity = ( iCapacity == 0 ) ? 8 : iCapacity * 2;
                char **pTemp = realloc( ppPatterns, iCapacity * sizeof( char * ) );
                if ( pTemp == NULL )
                {
                    perror( "realloc" );
                    exit( EXIT_FAILURE );
                }
                ppPatterns = pTemp;
            }
            ppPatterns[ iCount ] = strdup( pStart );
            if ( ppPatterns[ iCount ] == NULL )
            {
                perror( "strdup" );
                exit( EXIT_FAILURE );
            }
            iCount++;
        }
        fclose( pFile );
        *piCount = iCount;
    }
    return ppPatterns;
}

static bool isExcluded( const char *const pszFilePath, const IgnorePatterns_t *const pPatterns )
{
    bool bExcluded = false;
    for ( const IgnorePatterns_t *pCurr = pPatterns;
          ( pCurr != NULL ) && ( bExcluded == false );
          pCurr = pCurr->pParent )
    {
        char *pszRelPath = getRelPath( pszFilePath, pCurr->pszDir );
        if ( pszRelPath != NULL )
        {
            for ( int iIdx = 0; ( iIdx < pCurr->iPatternCount ) && ( bExcluded == false );
                  iIdx++ )
            {
                const char *pszPattern = pCurr->ppSzPatterns[ iIdx ];
                size_t uLen = strlen( pszPattern );
                if ( pszPattern[ uLen - 1 ] == '/' )
                {
                    char pModified[ PATH_MAX ];
                    snprintf( pModified, PATH_MAX, "%s*", pszPattern );
                    if ( fnmatch( pModified, pszRelPath, FNM_PATHNAME ) == 0 )
                    {
                        bExcluded = true;
                    }
                }
                else if ( strchr( pszPattern, '/' ) != NULL )
                {
                    if ( fnmatch( pszPattern, pszRelPath, FNM_PATHNAME ) == 0 )
                    {
                        bExcluded = true;
                    }
                }
                else
                {
                    char *pszBase = strrchr( pszRelPath, '/' );
                    pszBase = ( pszBase != NULL ) ? pszBase + 1 : pszRelPath;
                    if ( fnmatch( pszPattern, pszBase, 0 ) == 0 )
                    {
                        bExcluded = true;
                    }
                }
            }
            free( pszRelPath );
        }
    }
    if ( bExcluded == false )
    {
        for ( int iIdx = 0; ( iIdx < iExcludeCount ) && ( bExcluded == false ); iIdx++ )
        {
            if ( fnmatch( ppSzExcludes[ iIdx ], pszFilePath, 0 ) == 0 )
            {
                bExcluded = true;
            }
        }
    }
    return bExcluded;
}

static void expandPattern( const char *const pszPattern, IgnorePatterns_t *pPatterns )
{
    struct stat stStat;
    if ( stat( pszPattern, &stStat ) == 0 )
    {
        if ( S_ISREG( stStat.st_mode ) == true )
        {
            if ( isExcluded( pszPattern, pPatterns ) == false )
            {
                addFileToList( pszPattern );
            }
        }
        else if ( S_ISDIR( stStat.st_mode ) == true )
        {
            IgnorePatterns_t *pCurrPatterns = pPatterns;
            char pGitPath[ PATH_MAX ];
            snprintf( pGitPath, PATH_MAX, "%s/.gitignore", pszPattern );
            if ( access( pGitPath, F_OK ) == 0 )
            {
                int iNewCount = 0;
                char **ppSzNew = parseGitignore( pGitPath, &iNewCount );
                if ( iNewCount > 0 )
                {
                    IgnorePatterns_t *pNew = malloc( sizeof( IgnorePatterns_t ) );
                    if ( pNew == NULL )
                    {
                        perror( "malloc" );
                        exit( EXIT_FAILURE );
                    }
                    pNew->pszDir = strdup( pszPattern );
                    pNew->ppSzPatterns = ppSzNew;
                    pNew->iPatternCount = iNewCount;
                    pNew->pParent = pPatterns;
                    pCurrPatterns = pNew;
                }
            }
            DIR *pDir = opendir( pszPattern );
            if ( pDir != NULL )
            {
                struct dirent *pEntry;
                while ( ( pEntry = readdir( pDir ) ) != NULL )
                {
                    if ( ( strcmp( pEntry->d_name, "." ) == 0 ) ||
                         ( strcmp( pEntry->d_name, ".." ) == 0 ) )
                    {
                        continue;
                    }
                    if ( strcmp( pEntry->d_name, ".git" ) == 0 )
                    {
                        continue;
                    }
                    char pFullPath[ PATH_MAX ];
                    snprintf( pFullPath, PATH_MAX, "%s/%s", pszPattern, pEntry->d_name );
                    struct stat stChild;
                    if ( stat( pFullPath, &stChild ) == 0 )
                    {
                        if ( ( S_ISREG( stChild.st_mode ) == true ) &&
                             ( isExcluded( pFullPath, pCurrPatterns ) == false ) )
                        {
                            addFileToList( pFullPath );
                        }
                        else if ( S_ISDIR( stChild.st_mode ) == true )
                        {
                            expandPattern( pFullPath, pCurrPatterns );
                        }
                    }
                }
                closedir( pDir );
            }
            if ( pCurrPatterns != pPatterns )
            {
                free( pCurrPatterns->pszDir );
                for ( int iIdx = 0; iIdx < pCurrPatterns->iPatternCount; iIdx++ )
                {
                    free( pCurrPatterns->ppSzPatterns[ iIdx ] );
                }
                free( pCurrPatterns->ppSzPatterns );
                free( pCurrPatterns );
            }
        }
    }
    else
    {
        char pDir[ PATH_MAX ];
        char pFilePattern[ PATH_MAX ];
        const char *pszSlash = strrchr( pszPattern, '/' );
        if ( pszSlash != NULL )
        {
            size_t uLen = pszSlash - pszPattern;
            if ( uLen >= PATH_MAX )
            {
                uLen = PATH_MAX - 1;
            }
            strncpy( pDir, pszPattern, uLen );
            pDir[ uLen ] = '\0';
            snprintf( pFilePattern, PATH_MAX, "%s", pszSlash + 1 );
        }
        else
        {
            strcpy( pDir, "." );
            snprintf( pFilePattern, PATH_MAX, "%s", pszPattern );
        }
        IgnorePatterns_t *pCurrPatterns = pPatterns;
        char pGitPath[ PATH_MAX ];
        snprintf( pGitPath, PATH_MAX, "%s/.gitignore", pDir );
        if ( access( pGitPath, F_OK ) == 0 )
        {
            int iNewCount = 0;
            char **ppSzNew = parseGitignore( pGitPath, &iNewCount );
            if ( iNewCount > 0 )
            {
                IgnorePatterns_t *pNew = malloc( sizeof( IgnorePatterns_t ) );
                if ( pNew == NULL )
                {
                    perror( "malloc" );
                    exit( EXIT_FAILURE );
                }
                pNew->pszDir = strdup( pDir );
                pNew->ppSzPatterns = ppSzNew;
                pNew->iPatternCount = iNewCount;
                pNew->pParent = pPatterns;
                pCurrPatterns = pNew;
            }
        }
        DIR *pDirStream = opendir( pDir );
        if ( pDirStream != NULL )
        {
            struct dirent *pEntry;
            while ( ( pEntry = readdir( pDirStream ) ) != NULL )
            {
                if ( ( strcmp( pEntry->d_name, "." ) == 0 ) ||
                     ( strcmp( pEntry->d_name, ".." ) == 0 ) )
                {
                    continue;
                }
                if ( fnmatch( pFilePattern, pEntry->d_name, 0 ) == 0 )
                {
                    char pFullPath[ PATH_MAX ];
                    snprintf( pFullPath, PATH_MAX, "%s/%s", pDir, pEntry->d_name );
                    struct stat stChild;
                    if ( ( stat( pFullPath, &stChild ) == 0 ) &&
                         ( S_ISREG( stChild.st_mode ) == true ) &&
                         ( isExcluded( pFullPath, pCurrPatterns ) == false ) )
                    {
                        addFileToList( pFullPath );
                    }
                }
            }
            closedir( pDirStream );
        }
        if ( pCurrPatterns != pPatterns )
        {
            free( pCurrPatterns->pszDir );
            for ( int iIdx = 0; iIdx < pCurrPatterns->iPatternCount; iIdx++ )
            {
                free( pCurrPatterns->ppSzPatterns[ iIdx ] );
            }
            free( pCurrPatterns->ppSzPatterns );
            free( pCurrPatterns );
        }
    }
}

static uint64_t countTokens( const char *const pBuf, const size_t uSize, int *const piInToken )
{
    /*
       Count the total characters in the file and divide by 4.
       We use this method for simplicity and to avoid needing
       to bring in an actual tokenizer library and add overhead
       to the process.
    */
    ( void ) pBuf;      // Unused in this implementation.
    ( void ) piInToken; // Unused.
    return uSize / 4;
}


static void processFile( const int iIndex,
                         const char *const pszFileName,
                         char **const ppLocalBuffer,
                         size_t *const puBufferLength,
                         size_t *const puBufferCapacity )
{
    if ( pszFileName == NULL )
    {
        return;
    }

    uint64_t uLocalTokens = 0;

    if ( ! isTextFile( pszFileName ) )
    {
        pthread_mutex_lock( &mutexOutput );
        fprintf( stderr, "Skipping binary file: %s\n", pszFileName );
        pthread_mutex_unlock( &mutexOutput );
        puFileTokens[ iIndex ] = 0;
        return;
    }

#define APPEND_TO_LOCAL( pData, uLenData )                              \
    do                                                                  \
    {                                                                   \
        if ( *puBufferLength + ( uLenData ) + 1 > *puBufferCapacity )     \
        {                                                               \
            *puBufferCapacity = ( *puBufferLength + ( uLenData ) + 1 ) * 2; \
            char *pNewBuf = realloc( *ppLocalBuffer, *puBufferCapacity ); \
            if ( pNewBuf == NULL )                                      \
            {                                                           \
                perror( "realloc" );                                    \
                exit( EXIT_FAILURE );                                   \
            }                                                           \
            *ppLocalBuffer = pNewBuf;                                   \
        }                                                               \
        memcpy( *ppLocalBuffer + *puBufferLength, pData, uLenData );      \
        *puBufferLength += ( uLenData );                                  \
        *( *ppLocalBuffer + *puBufferLength ) = '\0';                     \
    }                                                                   \
    while ( 0 )

    {
        char *pszHeader = NULL;
        if ( asprintf( &pszHeader, "<FILE name=\"%s\">\n", pFileList[ iIndex ].pszRelPath ) >= 0 )
        {
            APPEND_TO_LOCAL( pszHeader, strlen( pszHeader ) );
            free( pszHeader );
        }
    }

    int iFd = open( pszFileName, O_RDONLY );
    if ( iFd < 0 )
    {
        pthread_mutex_lock( &mutexOutput );
        fprintf( stderr, "Error opening %s: %s\n", pszFileName, strerror( errno ) );
        pthread_mutex_unlock( &mutexOutput );
        puFileTokens[ iIndex ] = 0;
        return;
    }

    struct stat stStat;
    if ( fstat( iFd, &stStat ) < 0 )
    {
        pthread_mutex_lock( &mutexOutput );
        fprintf( stderr, "Error stating %s: %s\n", pszFileName, strerror( errno ) );
        pthread_mutex_unlock( &mutexOutput );
        close( iFd );
        puFileTokens[ iIndex ] = 0;
        return;
    }

    size_t uFileSize = ( size_t ) stStat.st_size;
    if ( uFileSize > 0 && uFileSize < 100 * 1024 * 1024 )
    {
        void *pMapped = mmap( NULL, uFileSize, PROT_READ, MAP_PRIVATE, iFd, 0 );
        if ( pMapped != MAP_FAILED )
        {
            int iInToken = 0;
            uLocalTokens = countTokens( pMapped, uFileSize, &iInToken );
            APPEND_TO_LOCAL( pMapped, uFileSize );
            munmap( pMapped, uFileSize );
        }
        else
        {
            char pBuf[kBufSize_bytes];
            ssize_t iBytes = 0;
            int iInToken = 0;
            while ( ( iBytes = read( iFd, pBuf, kBufSize_bytes ) ) > 0 )
            {
                uLocalTokens += countTokens( pBuf, iBytes, &iInToken );
                APPEND_TO_LOCAL( pBuf, iBytes );
            }
        }
    }
    else
    {
        char pBuf[kBufSize_bytes];
        ssize_t iBytes = 0;
        int iInToken = 0;
        while ( ( iBytes = read( iFd, pBuf, kBufSize_bytes ) ) > 0 )
        {
            uLocalTokens += countTokens( pBuf, iBytes, &iInToken );
            APPEND_TO_LOCAL( pBuf, iBytes );
        }
    }
    close( iFd );

    {
        const char *pszFooter = "\n</FILE>\n";
        APPEND_TO_LOCAL( pszFooter, strlen( pszFooter ) );
    }
    puFileTokens[ iIndex ] = uLocalTokens;

    if ( iVerbose == true )
    {
        pthread_mutex_lock( &mutexOutput );
        printf( "Processed: %s (%llu tokens)\n", pszFileName, uLocalTokens );
        pthread_mutex_unlock( &mutexOutput );
    }

#undef APPEND_TO_LOCAL
}

static void *workerThread( void *pArg )
{
    ThreadArgs_t *pArgs = ( ThreadArgs_t * ) pArg;
    pArgs->pszBuffer = malloc( kInitialBufferSize_bytes );
    if ( pArgs->pszBuffer == NULL )
    {
        perror( "malloc" );
        exit( EXIT_FAILURE );
    }
    pArgs->uBufferLength = 0;
    pArgs->uBufferCapacity = kInitialBufferSize_bytes;
    for ( int iIdx = pArgs->iStart; iIdx < pArgs->iEnd; iIdx++ )
    {
        processFile( iIdx, pFileList[ iIdx ].pszPath, &pArgs->pszBuffer,
                     &pArgs->uBufferLength, &pArgs->uBufferCapacity );
    }
    return NULL;
}

static void parseArguments( const int iArgc, char *const pArgv[] )
{
    int iOpt;
    while ( ( iOpt = getopt( iArgc, pArgv, "o:vjhn" ) ) != -1 )
    {
        switch ( iOpt )
        {
            case 'o':
            {
                pszOutputFile = strdup( optarg );
                break;
            }
            case 'v':
            {
                iVerbose = 1;
                break;
            }
            case 'j':
            {
                iJsonOutput = 1;
                break;
            }
            case 'n':
            {
                iNoColor = 1;
                break;
            }
            case 'h':
            default:
            {
                fprintf( stderr,
                         "Truckin version %s\nUsage: %s [-v] [-o output_file] [-j] [-n] [patterns...]\n",
                         TRUCKIN_VERSION,
                         pArgv[ 0 ] );
                exit( EXIT_SUCCESS );
            }
        }
    }
    for ( int iIdx = optind; iIdx < iArgc; iIdx++ )
    {
        ppSzIncludes[ iIncludeCount ] = strdup( pArgv[ iIdx ] );
        if ( ppSzIncludes[ iIncludeCount ] == NULL )
        {
            perror( "strdup" );
            exit( EXIT_FAILURE );
        }
        iIncludeCount++;
    }
    if ( iIncludeCount == 0 )
    {
        ppSzIncludes[ iIncludeCount ] = strdup( "*" );
        if ( ppSzIncludes[ 0 ] == NULL )
        {
            perror( "strdup" );
            exit( EXIT_FAILURE );
        }
        iIncludeCount++;
    }
    pszBasePath = expandTilde( ppSzIncludes[ 0 ] );
}

static void freePatterns( void )
{
    for ( int iIdx = 0; iIdx < iIncludeCount; iIdx++ )
    {
        free( ppSzIncludes[ iIdx ] );
    }
    for ( int iIdx = 0; iIdx < iExcludeCount; iIdx++ )
    {
        free( ppSzExcludes[ iIdx ] );
    }
}

static int cmpTokenCount( const void *pA, const void *pB )
{
    int iIdxA = *( const int * ) pA;
    int iIdxB = *( const int * ) pB;
    if ( puFileTokens[ iIdxA ] < puFileTokens[ iIdxB ] )
    {
        return 1;
    }
    else if ( puFileTokens[ iIdxA ] > puFileTokens[ iIdxB ] )
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

int main( const int iArgc, char *const pArgv[] )
{
    parseArguments( iArgc, pArgv );

    // Color Control Logic
    const char *no_color = getenv( "NO_COLOR" );
    const char *force_color = getenv( "FORCE_COLOR" );
    int iNoColorFlag = iNoColor;  // Start with command-line flag

    if ( no_color != NULL && *no_color != '\0' )
    {
        iNoColorFlag = 1;
    }
    else if ( force_color != NULL && *force_color != '\0' )
    {
        iNoColorFlag = 0;
    }
    else if ( iNoColorFlag == 0 && isatty( STDOUT_FILENO ) == 0 )
    {
        iNoColorFlag = 1;
    }

    if ( iNoColorFlag )
    {
        color_reset = "";
        color_header = "";
        color_total = "";
        color_filename = "";
        color_tokens = "";
        color_percent = "";
    }
    else
    {
        color_reset = "\033[0m";
        color_header = "\033[1;36m";
        color_total = "\033[1;33m";
        color_filename = "\033[1;34m";
        color_tokens = "\033[1;32m";
        color_percent = "\033[1;35m";
    }

    for ( int iIdx = 0; iIdx < iIncludeCount; iIdx++ )
    {
        expandPattern( ppSzIncludes[ iIdx ], NULL );
    }
    if ( iFileCount == 0 )
    {
        fprintf( stderr, "No files found.\n" );
        freePatterns();
        return EXIT_SUCCESS;
    }

    puFileTokens = calloc( iFileCount, sizeof( uint64_t ) );
    if ( puFileTokens == NULL )
    {
        perror( "calloc" );
        exit( EXIT_FAILURE );
    }

    int iNumThreads = 1;
    if ( iFileCount > kThreadThreshold_files )
    {
        long lCores = sysconf( _SC_NPROCESSORS_ONLN );
        iNumThreads = ( lCores > 0 ) ? ( int ) lCores : 1;
    }

    pthread_t *pThreads = malloc( iNumThreads * sizeof( pthread_t ) );
    ThreadArgs_t *pArgs = malloc( iNumThreads * sizeof( ThreadArgs_t ) );
    if ( pThreads == NULL || pArgs == NULL )
    {
        perror( "malloc" );
        freePatterns();
        exit( EXIT_FAILURE );
    }

    int iQuotient = iFileCount / iNumThreads;
    int iRemainder = iFileCount % iNumThreads;
    int iStart = 0;
    for ( int iIdx = 0; iIdx < iNumThreads; iIdx++ )
    {
        pArgs[ iIdx ].iStart = iStart;
        int iChunkSize = ( iIdx < iRemainder ) ? iQuotient + 1 : iQuotient;
        pArgs[ iIdx ].iEnd = iStart + iChunkSize;
        iStart += iChunkSize;
        if ( pthread_create( &pThreads[ iIdx ], NULL, workerThread, &pArgs[ iIdx ] ) != 0 )
        {
            perror( "pthread_create" );
        }
    }

    for ( int iIdx = 0; iIdx < iNumThreads; iIdx++ )
    {
        pthread_join( pThreads[ iIdx ], NULL );
    }

    uint64_t uTotalTokens = 0;
    for ( int iIdx = 0; iIdx < iFileCount; iIdx++ )
    {
        uTotalTokens += puFileTokens[ iIdx ];
    }

    size_t uTotalLength = 0;
    for ( int iIdx = 0; iIdx < iNumThreads; iIdx++ )
    {
        uTotalLength += pArgs[ iIdx ].uBufferLength;
    }

    char *pszClipboard = malloc( uTotalLength + 1 );
    if ( pszClipboard == NULL )
    {
        perror( "malloc" );
        exit( EXIT_FAILURE );
    }

    size_t uOffset = 0;
    for ( int iIdx = 0; iIdx < iNumThreads; iIdx++ )
    {
        memcpy( pszClipboard + uOffset, pArgs[ iIdx ].pszBuffer, pArgs[ iIdx ].uBufferLength );
        uOffset += pArgs[ iIdx ].uBufferLength;
    }
    pszClipboard[ uTotalLength ] = '\0';

    int *pIndices = malloc( iFileCount * sizeof( int ) );
    if ( pIndices == NULL )
    {
        perror( "malloc" );
        exit( EXIT_FAILURE );
    }
    for ( int iIdx = 0; iIdx < iFileCount; iIdx++ )
    {
        pIndices[ iIdx ] = iIdx;
    }
    qsort( pIndices, iFileCount, sizeof( int ), cmpTokenCount );

    int iMaxRelLen = 0;
    for ( int iIdx = 0; iIdx < iFileCount; iIdx++ )
    {
        int iLen = strlen( pFileList[ iIdx ].pszRelPath );
        if ( iLen > iMaxRelLen )
        {
            iMaxRelLen = iLen;
        }
    }
    int iColWidth = ( iMaxRelLen > kMaxFileColWidth_chars ) ? kMaxFileColWidth_chars : iMaxRelLen;

    pthread_mutex_lock( &mutexOutput );
    if ( iJsonOutput )
    {
        printf( "{\n" );
        printf( "    \"total_files\": %d,\n", iFileCount );
        printf( "    \"total_tokens\": %llu,\n", uTotalTokens );
        printf( "    \"files\": [\n" );
        for ( int iIdx = 0; iIdx < iFileCount; iIdx++ )
        {
            int iFileIdx = pIndices[ iIdx ];
            double dPercent = ( uTotalTokens > 0 ) ? ( 100.0 * puFileTokens[ iFileIdx ] / uTotalTokens ) : 0;
            if ( iIdx > 0 )
            {
                printf( ",\n" );
            }
            printf( "        {\"name\": \"%s\", \"tokens\": %llu, \"percent\": %.2f}",
                    pFileList[ iFileIdx ].pszRelPath, puFileTokens[ iFileIdx ], dPercent );
        }
        printf( "\n    ]\n" );
        printf( "}\n" );
    }
    else
    {
        printf( "\n%sSummary:%s\n", color_header, color_reset );
        printf( "%sTotal Files:%s %d\n", color_total, color_reset, iFileCount );
        printf( "%sTotal Tokens:%s %llu\n\n", color_total, color_reset, uTotalTokens );
        printf( "%s%-*s %-12s %-10s%s\n", color_header, iColWidth, "File", "Tokens", "Percent", color_reset );
        for ( int iIdx = 0; iIdx < iFileCount; iIdx++ )
        {
            int iFileIdx = pIndices[ iIdx ];
            double dPercent = ( uTotalTokens > 0 ) ? ( 100.0 * puFileTokens[ iFileIdx ] / uTotalTokens ) : 0;
            char *pszDisp = truncateString( pFileList[ iFileIdx ].pszRelPath, iColWidth );
            char pPercentStr[ 20 ];
            snprintf( pPercentStr, sizeof( pPercentStr ), "%6.2f%%", dPercent );
            printf( "%s%-*s%s %s%-12llu%s %s%-10s%s\n",
                    color_filename, iColWidth, pszDisp, color_reset,
                    color_tokens, puFileTokens[ iFileIdx ], color_reset,
                    color_percent, pPercentStr, color_reset );
            free( pszDisp );
        }
    }
    pthread_mutex_unlock( &mutexOutput );

    if ( pszOutputFile != NULL )
    {
        FILE *pFile = fopen( pszOutputFile, "w" );
        if ( pFile != NULL )
        {
            fputs( pszClipboard, pFile );
            fclose( pFile );
            if ( iVerbose == true )
            {
                printf( "Output written to %s\n", pszOutputFile );
            }
        }
        else
        {
            perror( "fopen output file" );
        }
    }
    else
    {
        FILE *pPipe = popen( "pbcopy", "w" );
        if ( pPipe != NULL )
        {
            fputs( pszClipboard, pPipe );
            pclose( pPipe );
        }
        else
        {
            perror( "popen pbcopy" );
        }
    }

    free( pThreads );
    for ( int iIdx = 0; iIdx < iNumThreads; iIdx++ )
    {
        free( pArgs[ iIdx ].pszBuffer );
    }
    free( pArgs );
    for ( int iIdx = 0; iIdx < iFileCount; iIdx++ )
    {
        free( pFileList[ iIdx ].pszPath );
        free( pFileList[ iIdx ].pszRelPath );
    }
    free( pFileList );
    free( puFileTokens );
    free( pszClipboard );
    free( pIndices );
    freePatterns();
    if ( pszOutputFile != NULL )
    {
        free( pszOutputFile );
    }
    if ( pszBasePath != NULL )
    {
        free( pszBasePath );
    }

    return EXIT_SUCCESS;
}
