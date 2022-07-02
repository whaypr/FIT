#pragma once
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <cassert>
#include <functional>
using namespace std;
#include <iostream>


#define FILENAME_LEN_MAX    28
#define DIR_ENTRIES_MAX     128
#define OPEN_FILES_MAX      8
#define SECTOR_SIZE         16
#define DEVICE_SIZE_MAX     ( 1024 * 1024 * 1024 )
#define DEVICE_SIZE_MIN     ( 8 * 1024 * 1024 )

struct TFile {
  char            m_FileName[FILENAME_LEN_MAX + 1];
  size_t          m_FileSize;
};

struct TBlkDev {
  size_t           m_Sectors;
  function<size_t(size_t, void *, size_t )> m_Read;
  function<size_t(size_t, const void *, size_t )> m_Write;
};



class CFileSystem {
public:
	CFileSystem ( const TBlkDev d );
	~CFileSystem ();
	void clean ();

    static bool    CreateFs                                ( const TBlkDev   & dev );
    static CFileSystem * Mount                             ( const TBlkDev   & dev );
    bool           Umount                                  ( void );
    size_t         FileSize                                ( const char      * fileName );
    int            OpenFile                                ( const char      * fileName,
                                                             bool              writeMode );
    bool           CloseFile                               ( int               fd );
    size_t         ReadFile                                ( int               fd,
                                                             void            * data,
                                                             size_t            len );
    size_t         WriteFile                               ( int               fd,
                                                             const void      * data,
                                                             size_t            len );
    bool           DeleteFile                              ( const char      * fileName );
    bool           FindFirst                               ( TFile           & file );
    bool           FindNext                                ( TFile           & file );

private:
    const TBlkDev dev;

    int currID = -1;

    struct record {
    	TFile file;
    	int firstSector; // -1 = invalid, 0 = EOF
    };

    // < store_on_disk >
    int filesCount = 0;
    int usedSectorsCount = 0;
    record records[DIR_ENTRIES_MAX];
    int * freeSectors;
    int * fat;    
    // </ store_on_disk >

    record * openedFiles[OPEN_FILES_MAX];
    size_t readBytes[OPEN_FILES_MAX];
    int getFd ();
    int getSector ();
    void freeUsedSectors ( record * r );

    CFileSystem::record  * findFile ( const char * fileName );
    record * createFile ( const char * fileName );
    void truncateFile ( record * r );
};
