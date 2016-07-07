//
//  GResource.h
//  libguang
//
//  Created by ChunGuang Yan on 15/11/19.
//  Copyright © 2015年 guang. All rights reserved.
//

#ifndef GResource_h
#define GResource_h

#include "cocos2d.h"

class GResource
{
public:
    static GResource* getInstance();
    //可写路径
    std::string getWritePath();
    //代码URL
    std::string getCodeUrl();
    //资源URL
    std::string getResUrl();
    //可写代码路径
    std::string getWriteCodePath();
    //可写资源路径
    std::string getWriteResPath();
    //临时路径
    std::string getTempPath();
    //缓存路径
    std::string getCachePath();
    //根据下载文件创建文件夹
    bool createDirForUrlFile(std::string file,bool isCode);
    //返回一个文件的根目录
    std::string baseDir(std::string file);
    //返回一个文件的文件名
    std::string baseFileName(std::string file);
    //判断一个字符串是否为文件
    bool isFile(std::string path);
    //拷贝一个文件
    bool copyFile(std::string file, std::string target);
    //删除
    bool removeFile(std::string file);
    //解压
    bool decompress(std::string zip, bool isDecryption);
    //解密
    bool decryption(char* bytes,int len);
    
private:
    std::string _writePath;
    std::string _codeUrl;
    std::string _resUrl;
    std::string _writeCodePath;
    std::string _writeResPath;
    std::string _tempPath;
    std::string _cachePath;
};

#endif /* GResource_h */
