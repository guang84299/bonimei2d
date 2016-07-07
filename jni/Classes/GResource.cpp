//
//  GResource.cpp
//  libguang
//
//  Created by ChunGuang Yan on 15/11/19.
//  Copyright © 2015年 guang. All rights reserved.
//

#include "GResource.h"
#ifdef MINIZIP_FROM_SYSTEM
#include <minizip/unzip.h>
#else // from our embedded sources
#include "unzip.h"
#endif



USING_NS_CC;

#define G_CODE_URL "http://localhost:63342/cocosfw/"

#define BUFFER_SIZE  8192
#define MAX_FILENAME  512

static GResource* _instanceGResource = nullptr;

GResource* GResource::getInstance()
{
    if(!_instanceGResource)
    {
        _instanceGResource = new GResource();
    }
    return _instanceGResource;
}

std::string GResource::getWritePath()
{
    if(this->_writePath.empty())
    {
        this->_writePath = FileUtils::getInstance()->getWritablePath() + "cocosfw/";
    }
    return this->_writePath;
}

std::string GResource::getCodeUrl()
{
    if(this->_codeUrl.empty())
    {
        this->_codeUrl = G_CODE_URL;
    }
    return this->_codeUrl;
}

std::string GResource::getResUrl()
{
    if(this->_resUrl.empty())
    {
        this->_resUrl = G_CODE_URL;
    }
    return this->_resUrl;
}

std::string GResource::getWriteCodePath()
{
    if(this->_writeCodePath.empty())
    {
        this->_writeCodePath = FileUtils::getInstance()->getWritablePath() + "cocosfw/src/";
    }
    return this->_writeCodePath;
}

std::string GResource::getWriteResPath()
{
    if(this->_writeResPath.empty())
    {
        this->_writeResPath = FileUtils::getInstance()->getWritablePath() + "cocosfw/res/";
    }
    return this->_writeResPath;
}

std::string GResource::getTempPath()
{
    if(this->_tempPath.empty())
    {
        this->_tempPath = FileUtils::getInstance()->getWritablePath() + "cocosfw/temp/";
    }
    return this->_tempPath;
}

std::string GResource::getCachePath()
{
    if(this->_cachePath.empty())
    {
        this->_cachePath = FileUtils::getInstance()->getWritablePath() + "cocosfw/cache/";
    }
    return this->_cachePath;
}

bool GResource::createDirForUrlFile(std::string file, bool isCode)
{
    std::string dir(file);
    std::string path = this->getWriteCodePath();
    if(!isCode)
    {
        path = this->getWriteResPath();
    }
    
    dir = path + dir.substr(0, dir.rfind('/')+1);
    if(!FileUtils::getInstance()->isDirectoryExist(dir))
    {
       return FileUtils::getInstance()->createDirectory(dir);
    }
    return false;
}

std::string GResource::baseDir(std::string file)
{
    size_t found = file.find_last_of("/\\");
    
    if (std::string::npos != found)
    {
        return file.substr(0, found);
    }
    else
    {
        return file;
    }
}
std::string GResource::baseFileName(std::string file)
{
    size_t found = file.find_last_of("/\\");
    
    if (std::string::npos != found)
    {
        return file.substr(found+1, file.length());
    }
    else
    {
        return file;
    }
}

bool GResource::isFile(std::string path)
{
    size_t found = path.find_last_of(".\\");
    
    if (std::string::npos == found)
    {
        return false;
    }

    return true;
}

bool GResource::copyFile(std::string file, std::string target)
{
    auto ft = FileUtils::getInstance();
    if(!ft->isFileExist(file))
    {
        log("GResource : %s file not exist!", file.c_str());
        return false;
    }
    std::string dir = "";
    std::string tarFile = "";
    if(this->isFile(target))
    {
        dir = this->baseDir(target);
        tarFile = this->baseFileName(target);
    }
    else
    {
        dir = target;
        tarFile = this->baseFileName(file);
    }
    
    if(!ft->isDirectoryExist(dir))
    {
        ft->createDirectory(dir);
    }
    
    if(dir[dir.length()-1] != '/')
    {
        dir += '/';
    }
    
    FILE *in = fopen(FileUtils::getInstance()->getSuitableFOpen(file).c_str(), "rb");
    if(!in)
    {
        log("GResource in:open %s file failure!", file.c_str());
        return false;
    }
    FILE *out = fopen(FileUtils::getInstance()->getSuitableFOpen(dir + tarFile).c_str(), "wb");
    if(!out)
    {
        log("GResource :open %s file failure!", (dir + tarFile).c_str());
        return false;
    }
    char data[BUFFER_SIZE];
    size_t size_in,size_out;
    while((size_in = fread(data, 1, BUFFER_SIZE, in)) > 0)
    {
        size_out = fwrite(data, 1, size_in, out);
        if ( size_in != size_out )
        {
            log("GResource :copy %s file failure!", file.c_str());
            return false;
        }
    }
    fclose(in);
    fclose(out);
    return true;
}

bool GResource::removeFile(std::string file)
{
   return FileUtils::getInstance()->removeFile(file);
}

bool GResource::decompress(std::string zip, bool isDecryption)
{
    // Find root path for zip file
    size_t pos = zip.find_last_of("/\\");
    if (pos == std::string::npos)
    {
        log("GResource : no root path specified for zip file %s\n", zip.c_str());
        return false;
    }
    const std::string rootPath = zip.substr(0, pos+1);
    
    // Open the zip file
    unzFile zipfile = unzOpen(FileUtils::getInstance()->getSuitableFOpen(zip).c_str());
    if (! zipfile)
    {
        log("GResource : can not open zip file %s\n", zip.c_str());
        return false;
    }
    
    // Get info about the zip file
    unz_global_info global_info;
    if (unzGetGlobalInfo(zipfile, &global_info) != UNZ_OK)
    {
        log("GResource : can not read file global info of %s\n", zip.c_str());
        unzClose(zipfile);
        return false;
    }
    
    // Buffer to hold data read from the zip file
    char readBuffer[BUFFER_SIZE];
    // Loop to extract all files.
    uLong i;
    for (i = 0; i < global_info.number_entry; ++i)
    {
        // Get info about current file.
        unz_file_info fileInfo;
        char fileName[MAX_FILENAME];
        if (unzGetCurrentFileInfo(zipfile,
                                  &fileInfo,
                                  fileName,
                                  MAX_FILENAME,
                                  NULL,
                                  0,
                                  NULL,
                                  0) != UNZ_OK)
        {
            log("GResource : can not read compressed file info\n");
            unzClose(zipfile);
            return false;
        }
        const std::string fullPath = rootPath + fileName;
        
        // Check if this entry is a directory or a file.
        const size_t filenameLength = strlen(fileName);
        if (fileName[filenameLength-1] == '/')
        {
            //There are not directory entry in some case.
            //So we need to create directory when decompressing file entry
            if ( !FileUtils::getInstance()->createDirectory(this->baseDir(fullPath)) )
            {
                // Failed to create directory
                log("GResource : can not create directory %s\n", fullPath.c_str());
                unzClose(zipfile);
                return false;
            }
        }
        else
        {
            // Entry is a file, so extract it.
            // Open current file.
            if (unzOpenCurrentFile(zipfile) != UNZ_OK)
            {
                log("GResource : can not extract file %s\n", fileName);
                unzClose(zipfile);
                return false;
            }
            
            // Create a file to store current file.
            std::string fullPath_dat = fullPath;
            fullPath_dat = fullPath_dat.replace(fullPath_dat.find(".dat"), 4, "");
           // log(fullPath_dat.c_str());
            FILE *out = fopen(FileUtils::getInstance()->getSuitableFOpen(fullPath_dat).c_str(), "wb");
            if (!out)
            {
                log("GResource : can not create decompress destination file %s\n", fullPath.c_str());
                unzCloseCurrentFile(zipfile);
                unzClose(zipfile);
                return false;
            }
            
            // Write current file content to destinate file.
            int error = UNZ_OK;
            do
            {
                error = unzReadCurrentFile(zipfile, readBuffer, BUFFER_SIZE);
                if (error < 0)
                {
                    log("GResource : can not read zip file %s, error code is %d\n", fileName, error);
                    fclose(out);
                    unzCloseCurrentFile(zipfile);
                    unzClose(zipfile);
                    return false;
                }
                
                if (error > 0)
                {
                    if(isDecryption)
                    this->decryption(readBuffer,error);
                    fwrite(readBuffer, error, 1, out);
                }
            } while(error > 0);
            
            fclose(out);
        }
        
        unzCloseCurrentFile(zipfile);
        
        // Goto next entry listed in the zip file.
        if ((i+1) < global_info.number_entry)
        {
            if (unzGoToNextFile(zipfile) != UNZ_OK)
            {
                log("GResource : can not read next file for decompressing\n");
                unzClose(zipfile);
                return false;
            }
        }
    }
    
    unzClose(zipfile);
    return true;
}

bool GResource::decryption(char* bytes,int len)
{
    // 解密
    // [0-9a-f][0-9a-f]
    // 0x\0,
    static std::vector<unsigned char> table = {
        0x8a, 0x67, 0xf8, 0xe5, 0xec, 0x27, 0x2c, 0x00, 0x92, 0x2e, 0xfa, 0x43, 0xa8, 0xc9, 0x0a, 0xf8,
        0xb1, 0x8e, 0x87, 0x67, 0xe3, 0xdf, 0x8b, 0x93, 0x35, 0x14, 0xf9, 0x91, 0x18, 0x25, 0x36, 0xe9,
        0xd4, 0x6e, 0x0d, 0xab, 0x27, 0xd5, 0xc0, 0x75, 0xa6, 0xb1, 0x10, 0xc8, 0x67, 0x20, 0xe2, 0x45,
        0xa3, 0x6d, 0xda, 0x8f, 0x49, 0x79, 0x07, 0xb2, 0x6d, 0x27, 0x55, 0x35, 0xba, 0x4e, 0x90, 0xef,
        0x27, 0x2c, 0xf7, 0x56, 0x80, 0x08, 0xf6, 0xf0, 0xca, 0xa5, 0x8f, 0xd6, 0x7e, 0x1e, 0xdc, 0x1c,
        0x62, 0xb5, 0x33, 0x7e, 0xea, 0x9c, 0x5e, 0xd4, 0x0f, 0x56, 0x51, 0x34, 0x38, 0xc6, 0x1c, 0x0e,
        0x49, 0x47, 0xbf, 0x91, 0x3d, 0x55, 0x85, 0x97, 0x4e, 0x97, 0x80, 0x39, 0xde, 0x53, 0x4e, 0xb6,
        0x86, 0xee, 0x15, 0x53, 0x07, 0xa2, 0x8d, 0x12, 0x11, 0x98, 0x13, 0x9e, 0x7e, 0xf9, 0x37, 0x96,
        0x03, 0x9a, 0x8e, 0xe4, 0x9c, 0x93, 0xeb, 0xb7, 0x69, 0x33, 0x3f, 0x8c, 0xb5, 0x20, 0x83, 0x9d,
        0xd7, 0xa5, 0xb0, 0x13, 0xf2, 0x01, 0x54, 0x41, 0xb9, 0xb9, 0xdb, 0xf1, 0x47, 0x3e, 0x0f, 0xa7,
        0x72, 0x11, 0x24, 0xac, 0x3c, 0x78, 0xc2, 0x32, 0xa3, 0xf7, 0x01, 0x99, 0x60, 0x8f, 0x9d, 0x41,
        0xc1, 0x6a, 0xd6, 0x2f, 0x5a, 0xc5, 0x9d, 0x4f, 0x88, 0xf3, 0x70, 0xba, 0xfd, 0x0c, 0xad, 0xb7,
        0x7f, 0xc0, 0xd9, 0xb9, 0xe1, 0xf2, 0x32, 0x9b, 0x4d, 0x75, 0x27, 0xac, 0xa0, 0x7d, 0xa1, 0x34,
        0x24, 0xca, 0x65, 0x35, 0x0d, 0xdb, 0x3d, 0x5c, 0xcb, 0x4f, 0x52, 0x49, 0x52, 0xc6, 0xe1, 0x07,
        0xdb, 0x89, 0x2c, 0x7b, 0x5a, 0x6f, 0x33, 0xf6, 0x36, 0xb8, 0x21, 0x77, 0x88, 0x55, 0x0c, 0x67,
        0x22, 0xf4, 0x59, 0x80, 0x21, 0x4a, 0x51, 0x58, 0xe6, 0xd0, 0x4e, 0x69, 0xf4, 0xb7, 0x87, 0xbe,
    };
    
    
    for (auto i = 0; i < len; ++i)
        bytes[i] ^= table[i % 256];
    return true;
}