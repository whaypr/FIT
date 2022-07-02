#include "CFileSystem.hpp"

CFileSystem::CFileSystem ( const TBlkDev d ) : dev( d ) {
    for ( int i = 0; i < OPEN_FILES_MAX; ++i ) {
        openedFiles[i] = NULL;
        readBytes[i] = 0;
    }

    fat = new int[dev.m_Sectors];
    freeSectors = new int[dev.m_Sectors];
}

//-------------------------------------------------------------------------------------------
CFileSystem::~CFileSystem () {
    clean();
}

//-------------------------------------------------------------------------------------------
void CFileSystem::clean () {
    delete [] fat;
    delete [] freeSectors;    
}

//===========================================================================================
//===========================================================================================
bool CFileSystem::CreateFs ( const TBlkDev & dev ) {
    size_t size = (29+8+4+7) * DIR_ENTRIES_MAX;
    size_t rec_size = size / SECTOR_SIZE;
    size_t fsec_size = dev.m_Sectors / SECTOR_SIZE * sizeof(int);

    int data_offset = (rec_size + 1) + (fsec_size * 2);

    char * buff;
    if ( size >= dev.m_Sectors * sizeof(int) )
        buff = new char[size];
    else
        buff = new char[dev.m_Sectors * sizeof(int)];

    // reset filesCount and freeSectorsCount
    int frcnt = 0;
    memcpy( buff, &frcnt, sizeof(int) );

    int ussccnt = 0;
    memcpy( buff + sizeof(int), &ussccnt, sizeof(int) );

    dev.m_Write( 0, buff, 1 );

    // reset records
    record * rec = new record[DIR_ENTRIES_MAX];
    for ( int i = 0; i < DIR_ENTRIES_MAX; ++i )
        rec[i] = record{ {"dummy", 0}, -1 }; // make all records invalid
    memcpy( buff, rec, size );

    dev.m_Write( 1, buff, rec_size );

    // reset free sectors
    int * fsec = new int[dev.m_Sectors];
    for ( size_t i = 0; i < dev.m_Sectors - data_offset; ++i )
        fsec[i] = i + data_offset; // make all sectors free
    memcpy( buff, fsec, dev.m_Sectors*4 );

    dev.m_Write( rec_size + 1, buff, fsec_size );

    cout << "---FS CREATED---" << endl;

    delete [] buff;
    delete [] rec;
    delete [] fsec;
    return true;
}

//-------------------------------------------------------------------------------------------
CFileSystem * CFileSystem::Mount ( const TBlkDev & dev ) {
    CFileSystem * fs = new CFileSystem( dev );

    size_t size = (29+8+4+7) * DIR_ENTRIES_MAX; // filename+filesize+firstblock+padding
    size_t rec_size = size / SECTOR_SIZE;
    size_t fsec_size = dev.m_Sectors / SECTOR_SIZE * sizeof(int);

    char * buff;
    if ( size >= dev.m_Sectors * 4 )
        buff = new char[size];
    else
        buff = new char[dev.m_Sectors*4];

    // get filesCount and freeSectorsCount
    dev.m_Read( 0, buff, 1 );
    fs->filesCount = *reinterpret_cast<int*>(buff);
    fs->usedSectorsCount = *reinterpret_cast<int*>(buff + sizeof(int));

    // get records
    dev.m_Read( 1, buff, rec_size );
    memcpy( fs->records, buff, size );

    // get free blocks
    dev.m_Read( rec_size + 1, buff, fsec_size );
    memcpy( fs->freeSectors, buff, dev.m_Sectors*4 );

    // get fat
    dev.m_Read( rec_size + 1 + fsec_size, buff, fsec_size );
    memcpy( fs->fat, buff, dev.m_Sectors * 4 );

    cout << "FS MOUNTED ( files: " << fs->filesCount << " | used sectors: " << fs->usedSectorsCount << " )" << endl;

    delete [] buff;
    return fs;
}

//-------------------------------------------------------------------------------------------
bool CFileSystem::Umount () {
    size_t size = (29+8+4+7) * DIR_ENTRIES_MAX;
    size_t rec_size = size / SECTOR_SIZE;
    size_t fsec_size = dev.m_Sectors / SECTOR_SIZE * sizeof(int);

    char * buff;
    if ( size >= dev.m_Sectors * 4 )
        buff = new char[size];
    else
        buff = new char[dev.m_Sectors*4];

    // save filesCount and usedSectorsCount
    memcpy( buff, &filesCount, sizeof(int) );
    memcpy( buff + sizeof(int), &usedSectorsCount, sizeof(int) );
    dev.m_Write( 0, buff, 1 );

    // save records
    memcpy( buff, records, size );
    dev.m_Write( 1, buff, rec_size );

    // save freeSectors
    int * fsec = new int[dev.m_Sectors];
    fsec = freeSectors;
    memcpy( buff, fsec, dev.m_Sectors*4 );
    dev.m_Write( rec_size + 1, buff, fsec_size );

    // save fat
    int * tmpfat = new int[dev.m_Sectors];
    tmpfat = fat;
    memcpy( buff, tmpfat, dev.m_Sectors*4 );
    dev.m_Write( rec_size + 1 + fsec_size, buff, fsec_size );

    // close opened files
    for ( int fd = 0; fd < OPEN_FILES_MAX; ++fd ) {
        if ( ! openedFiles[fd] )
            continue;
        CloseFile( fd );
    }

    cout << "FS UNMOUNTED" << endl;

    delete [] buff;
    return true;
}

//-------------------------------------------------------------------------------------------
size_t CFileSystem::FileSize ( const char * fileName ) {
    record * r = findFile( fileName );
    return r ? r->file.m_FileSize : SIZE_MAX;
}

//-------------------------------------------------------------------------------------------
int CFileSystem::OpenFile ( const char * fileName, bool writeMode ) {
    int fd = getFd();
    if ( fd == -1 )
        throw "No file descriptor available";

    record * r = findFile( fileName );

    if ( writeMode && ! r ) {
        r = createFile( fileName );
        if ( ! r )
            return -1; // file not created
    } else if ( writeMode )
        truncateFile( r );
    else if ( ! writeMode && ! r )
        return -1; // no file to read

    openedFiles[fd] = r;
    readBytes[fd] = 0;

    cout << "\tOPEN " << fileName << " (FD " << fd << ")" << endl;
    return fd;
}

//-------------------------------------------------------------------------------------------
bool CFileSystem::CloseFile ( int fd ) {
    if ( fd < 0 || fd >= OPEN_FILES_MAX || ! openedFiles[fd] )
        return false;

    cout << "\tCLOSE " << openedFiles[fd]->file.m_FileName << " (FD " << fd << ")" << endl;
    openedFiles[fd] = NULL;

    return true;
}

//-------------------------------------------------------------------------------------------
size_t CFileSystem::ReadFile ( int fd, void * data, size_t len ) {
    size_t currentRead = 0;
    record * r = openedFiles[fd];

    // set correct read len
    size_t rem_len = r->file.m_FileSize - readBytes[fd];
    if (rem_len == 0)
        return 0; // everything has been read

    len = len > rem_len ? rem_len : len; // cap len if bigger than remaining size

    // find sector for reading 
    int sectors_read = readBytes[fd] / SECTOR_SIZE;
    int currentSector = r->firstSector;

    for ( int i = 0; i < sectors_read; ++i ) {
        currentSector = fat[currentSector];
    }

    // read currentSector
    char buff[SECTOR_SIZE];

    size_t readFromSector = readBytes[fd] % SECTOR_SIZE;
    size_t toRead = len > SECTOR_SIZE - readFromSector ? SECTOR_SIZE - readFromSector : len;

    dev.m_Read( currentSector, buff, 1 );
    memcpy( data, buff + readFromSector, toRead );
    cout << "\t\tREAD " << openedFiles[fd]->file.m_FileName << " ( sector " << currentSector << ", " << toRead << " bytes )" << endl;

    currentRead += toRead;

    // read the rest
    while ( currentRead < len ) {
        currentSector = fat[currentSector];

        toRead = len - currentRead > SECTOR_SIZE ? SECTOR_SIZE : len - currentRead;

        dev.m_Read( currentSector, buff, 1 );
        memcpy( (char*)data + currentRead, buff, toRead );
        cout << "\t\tREAD " << openedFiles[fd]->file.m_FileName << " ( sector " << currentSector << ", " << toRead << " bytes )" << endl;

        currentRead += toRead;
    }

    readBytes[fd] += currentRead;
    return currentRead;
}

//-------------------------------------------------------------------------------------------
size_t CFileSystem::WriteFile ( int fd, const void * data, size_t len ) {
    size_t writtenBytes = 0;
    record * r = openedFiles[fd];

    // find last sector with data
    int lastSector = r->firstSector;
    int tmp = lastSector;
    while ( tmp ) {
        lastSector = tmp;
        tmp = fat[tmp];
    }

    int lastSector_dataLen = r->file.m_FileSize % SECTOR_SIZE;

    // new sector if file is empty
    if ( r->firstSector == 0 ) {
        lastSector = getSector();
        cout << "\t\t\tALLOC " << lastSector << " FOR " << openedFiles[fd]->file.m_FileName << endl;
        r->firstSector = lastSector;
        fat[lastSector] = 0; // EOF
    // new sector if the last is full
    } else if ( lastSector_dataLen == 0 ) {
        int s = getSector();
        cout << "\t\t\tALLOC " << s << " FOR " << openedFiles[fd]->file.m_FileName << endl;
        fat[lastSector] = s;
        fat[s] = 0; // EOF

        lastSector = s;
    }

    char buff[SECTOR_SIZE];

    // write on lastSector
    if ( lastSector_dataLen != 0 ) {
        dev.m_Read( lastSector, buff, 1 ); // read lastSector data
        cout << "\t\t\treading last sector before writing (" << lastSector_dataLen << ")" << endl;
    }


    writtenBytes = SECTOR_SIZE - lastSector_dataLen;
    writtenBytes = writtenBytes > len ? len : writtenBytes; // cap value

    memcpy( buff + lastSector_dataLen, data, writtenBytes ); // fill the rest of the buffer with data
    dev.m_Write( lastSector, buff, 1 );
    cout << "\t\tWRITE " << openedFiles[fd]->file.m_FileName << " ( sector " << lastSector << ", " << writtenBytes << " bytes )" << endl;

    int new_bytes = writtenBytes;

    // write the rest
    while ( writtenBytes < len ) {
        int s = getSector();
        cout << "\t\t\tALLOC " << s << " FOR " << openedFiles[fd]->file.m_FileName << endl;
        fat[lastSector] = s;
        fat[s] = 0; // EOF

        lastSector = s;

        new_bytes = len - writtenBytes > SECTOR_SIZE ? SECTOR_SIZE : len - writtenBytes;

        memcpy( buff, (char *)data + writtenBytes, new_bytes );
        dev.m_Write( lastSector, buff, 1 );
        cout << "\t\tWRITE " << openedFiles[fd]->file.m_FileName << " ( sector " << lastSector << ", " << new_bytes << " bytes )" << endl;

        writtenBytes += new_bytes;
    }

    r->file.m_FileSize += writtenBytes;
    return writtenBytes;
}

//-------------------------------------------------------------------------------------------
bool CFileSystem::DeleteFile ( const char * fileName ) {
    record * r = findFile( fileName );

    if ( ! r )
        return false; // file does not exist

    freeUsedSectors( r );
    r->firstSector = -1; // invalidate record
    filesCount--;

    cout << "DELETE " << fileName << endl;
    return true;
}

//-------------------------------------------------------------------------------------------
bool CFileSystem::FindFirst ( TFile & file ) {
    currID++;

    for ( ; currID < DIR_ENTRIES_MAX; ++currID )
        if ( records[currID].firstSector != -1 ) {
            file = records[currID].file;
            cout << "FOUND " << file.m_FileName << endl;
            return true;
        }

    currID = -1;
    return false;
}

//-------------------------------------------------------------------------------------------
bool CFileSystem::FindNext ( TFile & file ) { 
    return FindFirst( file );
}

//===========================================================================================
//===========================================================================================
int CFileSystem::getFd () {
    for ( int i = 0; i < OPEN_FILES_MAX; ++i )
        if ( ! openedFiles[i] )
            return i;

    return -1;
}

//-------------------------------------------------------------------------------------------
int CFileSystem::getSector () {
    int sector = freeSectors[usedSectorsCount++]; // get first free sector and increment counter
    return sector ? sector : -1;
}

//-------------------------------------------------------------------------------------------
void CFileSystem::freeUsedSectors ( record * r ) {
    int sector = r->firstSector;
    r->firstSector = 0;

    while ( sector ) {
        cout << "\t\t\tFREE " << sector << " FROM " << r->file.m_FileName << endl;
        usedSectorsCount--; // can never be < 0
        freeSectors[usedSectorsCount] = sector;
        sector = fat[sector];
    }
}

//-------------------------------------------------------------------------------------------
CFileSystem::record * CFileSystem::createFile ( const char * fileName ) {
    if ( filesCount >= DIR_ENTRIES_MAX )
        return NULL; // cannot create another file

    record * rec = NULL;

    // add record
    for ( auto & r : records ) {
        if ( r.firstSector == -1 ) {
            strncpy(r.file.m_FileName, fileName, FILENAME_LEN_MAX );
            r.file.m_FileName[FILENAME_LEN_MAX] = 0;
            r.file.m_FileSize = 0;
            r.firstSector = 0;

            rec = &r;
            break;
        }
    }
    filesCount++;

    cout << "CREATE " << fileName << endl;
    return rec;
}

//-------------------------------------------------------------------------------------------
CFileSystem::record * CFileSystem::findFile ( const char * fileName ) {
    for ( auto & r : records )
        if ( ! strcmp( r.file.m_FileName, fileName ) )
            return &r;

    return NULL;
}

//-------------------------------------------------------------------------------------------
void CFileSystem::truncateFile ( record * r ) {
    freeUsedSectors( r );
    r->file.m_FileSize = 0;
}
