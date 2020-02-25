#include <iostream>
#include <string>
#include <vector>
#include <dirent.h> 
#include <sys/statfs.h>  
#include <sys/stat.h> 
#include <unistd.h> 
#include <sys/types.h>
#include <errno.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "file_util.h"
//#include "log.h"

#include<sys/ioctl.h>

namespace file_util
{
	volatile bool manual_record_enable = false;

	int clearUpFile(const std::string& rootPath, std::string suffix,
				 unsigned int limitSize )
	{
		DIR *pDir;
		struct dirent *pDirent;
		pDir = opendir(rootPath.c_str());
		if (NULL == pDir)
		{
			printf("opendir failed. rootPath=%s\n", rootPath.c_str());
			return -1;
		}

		unsigned int removeCount = 0;
		char szTmpPath[1024] = { 0 };
		while ((pDirent = readdir(pDir)) != NULL)
		{
			if( ( strcmp(pDirent->d_name, ".") == 0 )
				|| ( strcmp(pDirent->d_name, "..") == 0 ) )
			{
				continue;
			}

			if( removeCount > limitSize )
			{
				break;
			}
			
			sprintf(szTmpPath, "%s/%s", rootPath.c_str(), pDirent->d_name);
			//printf("szTmpPath=%s, ", szTmpPath);

			// because the first file is the last write file, so we should not
			// remove it
			//if( 0 != removeCount )
			{
				(void)remove_file(szTmpPath);
			}
			removeCount++;
			
		}
		closedir(pDir);

		return removeCount;
		
	}

void StringReplace(std::string &strBase, std::string strSrc, std::string strDes)
{
    std::string::size_type pos = 0;
    std::string::size_type srcLen = strSrc.size();
    std::string::size_type desLen = strDes.size();
    pos = strBase.find(strSrc, pos);
    while ((pos != std::string::npos))
    {
        strBase.replace(pos, srcLen, strDes);
        pos = strBase.find(strSrc, (pos + desLen));
    }
    return;
}

/*****************************************************************************
 * ?? ?? ??  : file_util.normalizeDir
 * ?? ?? ??  : ??????
 * ????????  : 2016??7??8??
 * ????????  : ??"\\" ?滻? "/"
 * ????????  : std::string& str  ·??
 * ????????  : ??
 * ?? ?? ?  : 
 * ???ù??  : 
 * ??    ??  : 

*****************************************************************************/
void normalize_directory(std::string& str)
{
    if ('/' != str.at(str.length()-1)) str.push_back('/');
    StringReplace(str, "\\", "/");
}

/*****************************************************************************
 * ?? ?? ??  : file_util.is_directory_exist
 * ?? ?? ??  : ??????
 * ????????  : 2016??7??8??
 * ????????  : ?ж?·?????????
 * ????????  : const std::string& dirPath  ·??????
 * ????????  : ??
 * ?? ?? ?  : 
 * ???ù??  : 
 * ??    ??  : 

*****************************************************************************/
bool is_directory_exist(const std::string& dirPath)
{
    struct stat st;
    if (stat(dirPath.c_str(), &st) == 0)
    {
        return S_ISDIR(st.st_mode);
    }
    return false;
}

/*****************************************************************************
 * ?? ?? ??  : file_util.zFileManager.create_directory
 * ?? ?? ??  : ??????
 * ????????  : 2016??7??8??
 * ????????  : ??????
 * ????????  : const std::string& path  ??????
 * ????????  : ??
 * ?? ?? ?  : bool
 * ???ù??  : 
 * ??    ??  : 

*****************************************************************************/
bool create_directory(const std::string& path)
{
    if (is_directory_exist(path))
    {
        return true;
    }

    // Split the path
    size_t start = 0;
    size_t found = path.find_first_of("/\\", start);
    std::string subpath;
    std::vector<std::string> dirs;

    if (found != std::string::npos)
    {
        while (true)
        {
            subpath = path.substr(start, found - start + 1);
            if (!subpath.empty())
                dirs.push_back(subpath);
            start = found + 1;
            found = path.find_first_of("/\\", start);
            if (found == std::string::npos)
            {
                if (start < path.length())
                {
                    dirs.push_back(path.substr(start));
                }
                break;
            }
        }
    }

    DIR *dir = NULL;

    // Create path recursively
    subpath = "";
    for (uint32_t i = 0; i < dirs.size(); ++i)
    {
        subpath += dirs[i];
        dir = opendir(subpath.c_str());

        if (!dir)
        {
            // directory doesn't exist, should create a new one

            int ret = mkdir(subpath.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);
            if (ret != 0 && (errno != EEXIST))
            {
                // current directory can not be created, sub directories can not be created too
                // should return
                return false;
            }
        }
        else
        {
            // directory exists, should close opened dir
            closedir(dir);
        }
    }
    return true;
}

/*****************************************************************************
 * ?? ?? ??  : file_util.append_directory
 * ?? ?? ??  : ??????
 * ????????  : 2016??7??8??
 * ????????  : ??????￡?????????
 * ????????  : const std::string& dirPath  ??·??
               const std::string& subDir   ??·??
 * ????????  : ??
 * ?? ?? ?  : 
 * ???ù??  : 
 * ??    ??  : 

*****************************************************************************/
bool append_directory(
    const std::string& dirPath, const std::string& subDir)
{
    std::string tmpDir = dirPath;
    normalize_directory(tmpDir);
    if (!is_directory_exist(tmpDir))
    {
        return false;
    }

    tmpDir += subDir;
    normalize_directory(tmpDir);
    if ( !is_directory_exist(tmpDir) )
    { 
        if (!create_directory(tmpDir))
        {
            return false;
        }
    }
    return true;
}

/*****************************************************************************
 * ?? ?? ??  : file_util.is_file_exist
 * ?? ?? ??  : ??????
 * ????????  : 2016??7??8??
 * ????????  : ?ж???????????
 * ????????  : const std::string& filename  ?·???????
 * ????????  : ??
 * ?? ?? ?  : 
 * ???ù??  : 
 * ??    ??  : 

*****************************************************************************/
bool is_file_exist(const std::string& filename)
{
    if (filename.empty())
    {
        return false;
    }

    struct stat sts;
    return (stat(filename.c_str(), &sts) != -1) ? true : false;
}

/*****************************************************************************
 * ?? ?? ??  : file_util.remove_file
 * ?? ?? ??  : ??????
 * ????????  : 2016??7??8??
 * ????????  : ??????
 * ????????  : const std::string &path  ?·???????
 * ????????  : ??
 * ?? ?? ?  : 
 * ???ù??  : 
 * ??    ??  : 

*****************************************************************************/
bool remove_file(const std::string &path)
{
    if (remove(path.c_str())) 
    {
        printf("remove fail. path=%s\n", path.c_str());
        return false;
    }
    return true;
}

/*****************************************************************************
 * ?? ?? ??  : file_util.getTimeString
 * ?? ?? ??  : ??????
 * ????????  : 2016??7??8??
 * ????????  : ????????????????????? ??_??_??_?_??_??
 * ????????  : ??
 * ????????  : ??
 * ?? ?? ?  : std
 * ???ù??  : 
 * ??    ??  : 

*****************************************************************************/
std::string getTimeString()
{
    time_t tt = time(NULL);
    struct tm* ptm = localtime(&tt);
    char date[60] = { 0 };
    sprintf(date, "%d-%02d-%02d-%02d-%02d-%02d",
        (int)ptm->tm_year + 1900, (int)ptm->tm_mon + 1, (int)ptm->tm_mday,
        (int)ptm->tm_hour, (int)ptm->tm_min, (int)ptm->tm_sec);
    return std::string(date);
}

std::string getTimeString( unsigned long long timeStamp )
{
    time_t tt = timeStamp;
    struct tm* ptm = localtime(&tt);
    char date[60] = { 0 };
    sprintf(date, "%d-%02d-%02d-%02d-%02d-%02d",
        (int)ptm->tm_year + 1900, (int)ptm->tm_mon + 1, (int)ptm->tm_mday,
        (int)ptm->tm_hour, (int)ptm->tm_min, (int)ptm->tm_sec);
    return std::string(date);
}

/*****************************************************************************
 * ?? ?? ??  : zBaseUtil.getMicroSecond
 * ?? ?? ??  : ??????
 * ????????  : 2015??11??12??
 * ????????  : ??????? -- 1/1000000??
 * ????????  : void
 * ????????  : ??
 * ?? ?? ?  : std
 * ???ù??  : 
 * ??    ??  : 

*****************************************************************************/
long long getMicroSecond(void)
{
    struct timespec ts = {0, 0};   
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (long long)ts.tv_sec * 1000000 + ts.tv_nsec/1000;
}

/*****************************************************************************
 * ?? ?? ??  : zBaseUtil.getMilliSecond
 * ?? ?? ??  : ??????
 * ????????  : 2015??12??03??
 * ????????  : ???????? -- 1/1000??
 * ????????  : void
 * ????????  : ??
 * ?? ?? ?  : std
 * ???ù??  : 
 * ??    ??  : 

*****************************************************************************/
long long getMilliSecond(void)
{
    struct timespec ts = {0, 0};
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (long long)ts.tv_sec * 1000 + ts.tv_nsec/1000000;
}

/*****************************************************************************
 * ?? ?? ??  : zBaseUtil.getSecond
 * ?? ?? ??  : ??????
 * ????????  : 2015??12??03??
 * ????????  : ???????????????? -- ??λ:??
 * ????????  : void
 * ????????  : ??
 * ?? ?? ?  : std
 * ???ù??  : 
 * ??    ??  : 

*****************************************************************************/
long long getSecond(void)
{
    struct timespec ts = {0, 0};
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (long long)ts.tv_sec;
}

/*****************************************************************************
 * ?? ?? ??  : file_util.to_string_fix
 * ?? ?? ??  : ??????
 * ????????  : 2016??7??8??
 * ????????  : ???? ?  ???????????6λ??????
 * ????????  : int val  ?????
 * ????????  : ??
 * ?? ?? ?  : std::string
 * ???ù??  : 
 * ??    ??  : 

*****************************************************************************/
std::string to_string_fix(uint32_t val)
{
    char buf[32];
    sprintf(buf, "%06u", val);
    return buf;
}

/*****************************************************************************
 * ?? ?? ??  : file_util.zFileManager.get_file_size
 * ?? ?? ??  : ??????
 * ????????  : 2016??7??10??
 * ????????  : ????????С
 * ????????  : const std::string& filePath  ????·??????
 * ????????  : ??
 * ?? ?? ?  : long
 * ???ù??  : 
 * ??    ??  : 

*****************************************************************************/
long get_file_size(const std::string& filePath)
{
    if (filePath.empty())
    {
        printf("Invalid file path!");
        return -1;
    }

    struct stat info;
    int result = stat(filePath.c_str(), &info);
    return (0 == result) ? (long)(info.st_size) : -1;
}

/*****************************************************************************
 * ?? ?? ??  : file_util.EnumDirFiles
 * ?? ?? ??  : ??????
 * ????????  : 2016??7??10??
 * ????????  : ??????·???μ?????б?
 * ????????  : const std::string& rootPath   ·??????
               std::list<std::string>& list  ????????
               std::string suffix            ?????
               int limit_size                ?????С
 * ????????  : ??
 * ?? ?? ?  : 
 * ???ù??  : 
 * ??    ??  : 

*****************************************************************************/
int EnumDirFiles(std::list<std::string> &list,
    const std::string &rootPath, std::string suffix, int limit_size)
{
    std::string strFind;

    DIR *pDir;
    struct dirent *pDirent;
    pDir = opendir(rootPath.c_str());
    if (NULL == pDir)
    {
        printf("opendir failed. rootPath=%s\n", rootPath.c_str());
        return -1;
    }

    while ((pDirent = readdir(pDir)) != NULL)
    {
        // if ((pDirent->d_name == '.') || pDirent->d_name[0] == '..')
//		printf("szTmpPath=%s, pDirent->d_type= %d\n", pDirent->d_name, pDirent->d_type);
		if( pDirent->d_type != DT_DIR )
		{
			continue;
		}
        if (strcmp(pDirent->d_name, ".") == 0 || strcmp(pDirent->d_name, "..") == 0)
        {
            continue;
        }

       char szTmpPath[1024] = { 0 };
        sprintf(szTmpPath, "%s/%s", rootPath.c_str(), pDirent->d_name);
        strFind = pDirent->d_name;

        if (    suffix.empty()
            || ((strFind.length() - suffix.length()) == strFind.rfind(suffix.c_str())))
        {
            if (0 == limit_size)
            {
                list.push_back(pDirent->d_name);
            }
            else
            {
                int file_size = get_file_size(szTmpPath);
                if (file_size >= limit_size)
                {
                    list.push_back(pDirent->d_name);
                }
                else
                {
                    (void)remove_file(szTmpPath);
                }
            }
        }
    }

    closedir(pDir);
    return list.size();
}

/*****************************************************************************
 * ?? ?? ??  : file_util.EnumDirFiles
 * ?? ?? ??  : ??????
 * ????????  : 2016??7??10??
 * ????????  : ??????·???μ??·????????б?
 * ????????  : const std::string& rootPath   ·??????
               std::list<std::string>& list  ????????
               std::string suffix            ?????
               int limit_size                ?????С
 * ????????  : ??
 * ?? ?? ?  : 
 * ???ù??  : 
 * ??    ??  : 

*****************************************************************************/
int EnumDirFiles_FullPath(std::list<std::string> &list,
    const std::string &rootPath, std::string suffix, int limit_size)
{
    std::string strFind;

    DIR *pDir;
    struct dirent *pDirent;
    pDir = opendir(rootPath.c_str());
    if (NULL == pDir)
    {
        printf("opendir failed. rootPath=%s\n", rootPath.c_str());
        return -1;
    }

    while ((pDirent = readdir(pDir)) != NULL)
    {
        // if ((pDirent->d_name == '.') || pDirent->d_name[0] == '..')
        if (strcmp(pDirent->d_name, ".") == 0 || strcmp(pDirent->d_name, "..") == 0)
        {
            continue;
        }

        char szTmpPath[1024] = { 0 };
        sprintf(szTmpPath, "%s/%s", rootPath.c_str(), pDirent->d_name);
        strFind = pDirent->d_name;
        //printf("szTmpPath=%s, pDirent->d_name= %s\n", szTmpPath, pDirent->d_name);

        if (    suffix.empty()
            || ((strFind.length() - suffix.length()) == strFind.rfind(suffix.c_str())))
        {
            if (0 == limit_size)
            {
                list.push_back(szTmpPath);
            }
            else
            {
                int file_size = get_file_size(szTmpPath);
                if (file_size >= limit_size)
                {
                    list.push_back(szTmpPath);
                }
                else
                {
                    (void)remove_file(szTmpPath);
                }
            }
        }
    }

    closedir(pDir);
    return list.size();
}

	
unsigned long long getDiskFreeSize(
    const std::string& path, unsigned long long *total_size)
{
    std::string diskName = path;
    unsigned long long result = 0;

    diskName = path;
    struct statfs diskInfo;
    statfs(diskName.c_str(), &diskInfo);
    unsigned long long block_size = diskInfo.f_bsize;
    unsigned long long freeDisk = diskInfo.f_bfree * block_size;

    /*
        LOG_D("f_blocks=%lld, f_bfree=%lld, f_bavail=%lld, f_bsize=%lld\n", 
            diskInfo.f_blocks, diskInfo.f_bfree, diskInfo.f_bavail, diskInfo.f_bsize);
     */

    if (NULL != total_size)
    {
        *total_size = diskInfo.f_blocks * block_size;
    }

    result = freeDisk;
    #if 0
    printf("%s block_size == %lld, diskInfo.f_bfree == %lld, freeDisk == %lld \n", 
        diskName.c_str(), block_size, diskInfo.f_bfree, freeDisk);
    #endif
    return result;
}

/*****************************************************************************
 * ?? ?? ??  : file_util.strip_filename
 * ?? ?? ??  : ??????
 * ????????  : 2016??7??18??
 * ????????  : ???·??????????????
 * ????????  : const std::string &full_filename  ?·???????
 * ????????  : ??
 * ?? ?? ?  : std
 * ???ù??  : 
 * ??    ??  : 

*****************************************************************************/
std::string strip_filename(const std::string &full_filename)
{
    std::size_t found = full_filename.find_last_of("/\\");
    if (found != std::string::npos)
    {
        return full_filename.substr(found + 1);
    }

    return full_filename;
}

/*****************************************************************************
 * ?? ?? ??  : file_util.strip_path
 * ?? ?? ??  : ??????
 * ????????  : 2016??7??26??
 * ????????  : ???????????????·??
 * ????????  : const std::string &full_filename  ?·???????
 * ????????  : ??
 * ?? ?? ?  : std::string: ·??
 * ???ù??  : 
 * ??    ??  : 

*****************************************************************************/
std::string strip_path(const std::string &full_filename)
{
    std::size_t found = full_filename.find_last_of("/\\");
    if (found != std::string::npos)
    {
        return full_filename.substr(0, found + 1);
    }
    return "";
}

/*****************************************************************************
 * ?? ?? ??  : file_util.get_mountpoint
 * ?? ?? ??  : ??????
 * ????????  : 2016??7??26??
 * ????????  : ???U??????
 * ????????  : const char **mount_point  U??????
 * ????????  : ??
 * ?? ?? ?  : bool: true: U?????false: ??U??
 * ???ù??  : 
 * ??    ??  : 

*****************************************************************************/
bool get_mountpoint(const char **mount_point)
{
    static const char mount_proc[] = "/proc/mounts";
    static const char mount_path[] = "/mnt/extsd";
    //static const char mount_path[] = "/mnt/usbhost1";

    int fd = open(mount_proc, O_RDONLY | O_NONBLOCK);
    if (fd < 0)
    {
        return false;
    }

    char content[2048];
    int len = read(fd, content, 2048);
    if (len <= 0)
    {
        close(fd);
        return false;
    }

    close(fd);
    content[len] = '\0';
    if (NULL == strstr(content, mount_path))
    {
        return false;
    }

    *mount_point = mount_path;
    return true;
}

bool is_sdcard_mounted()
{
	static const char mount_proc[] = "/proc/mounts";
    static const char mount_path[] = "/mnt/extsd";
	
	int fd = open(mount_proc,  O_RDONLY | O_NONBLOCK);
	if (fd < 0)
    {
        return false;
    }
	
	char content[2048] = {0};
    int len = read(fd, content, sizeof(content));
    if (len <= 0)
    {
        close(fd);
        return false;
    }

    close(fd);
    if (NULL == strstr(content, mount_path))
    {
        return false;
    }
    return true;
}

bool open_record_switch(int &fd)
{
    fd = open("/dev/record_switch", O_RDONLY, 0);
    if(fd >= 0)
    {
        return true;
    }
    return false;
}

void close_record_switch(int fd)
{
    close(fd);
}

bool read_record_switch(int fd)
{
    fd_set rfds;
    struct timeval tv;

    FD_ZERO(&rfds);
    FD_SET(fd, &rfds);
    tv.tv_usec = 0;
    tv.tv_sec = 2;

    // n<0: error. n=0: timeout. n>0: pulse.
    int n = select(fd + 1, &rfds, NULL, NULL, &tv);
    if (n > 0)
    {
        printf("read_record_switch: start to record!!\n");
    }
    return (n > 0);
}

bool get_manual_record_enable()
{
    return manual_record_enable;
}

bool set_manual_record_enable(bool v)
{
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

    (void)pthread_mutex_lock(&mutex);
    manual_record_enable = v;
    (void)pthread_mutex_unlock(&mutex);

    //printf("set_manual_record_enable: v=%d\n", v);
    return v;
}


static const char * alarm_index_file_path = "/mnt/extsd/normal_raw_images/alarm_index.txt";
int append_alarm_index(int alarmType, int imageIndex)
{
	int fd = open(alarm_index_file_path, O_CREAT|O_WRONLY|O_APPEND, 00666);
	if(fd == -1){
		printf("open alarm index file fail");
		return -1;
	}

	char alarm_index_str[64] = {0};
	static const char * alarmStr[8] = 
        {"none", "careful", "forward", "danger", "fatigue", "phoning", "nolifebelt", "smoking"};
	snprintf(alarm_index_str, sizeof(alarm_index_str), "%06d\t%s\r\n", imageIndex, alarmStr[alarmType]);
	//printf("alarm index = %s", alarm_index_str);

	if(write(fd, alarm_index_str, strlen(alarm_index_str)) < 0){
		printf("save alarm index fail");
		close(fd);
		return -1;
	}

	close(fd);
	return 0;
}

void remove_dir(const std::string& dir_path)
{
	char cmd[128] = {0};
	snprintf(cmd, sizeof(cmd), "rm -rf %s", dir_path.c_str());
	system(cmd);
	return ;
}

int is_file_read_write_ok(const std::string& path)
{
	int fd = open(path.c_str(), O_RDWR);
	if(fd < 0){
		return 0;
	}

	printf("check read");
#if 1
	char buff[20] = {0};
	int n_read = 0;
	n_read = read(fd, buff, 1);
	printf("n_read = %d", n_read);
#else
	fd_set ReadFdList;
	FD_ZERO(&ReadFdList);      
	FD_SET(fd , &ReadFdList);
	
	struct timeval waitTime = { 0, 1000000 };

	int nRet = select(fd + 1, &ReadFdList, NULL, NULL, &waitTime);
	printf("nRet = %d", nRet);
	if ( (nRet <= 0) ||	(0 == FD_ISSET(fd, &ReadFdList) ) )
	{
		printf("unreadable");
		close(fd);
		return 0;
	}
#endif
	printf("readable");
	close(fd);
	return 1;
}

}

