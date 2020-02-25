#ifndef __RW_FILE_UTIL_H__
#define __RW_FILE_UTIL_H__

#include <string>
#include <list>
#include <pthread.h>

namespace file_util
{
extern volatile bool manual_record_enable;

void StringReplace(std::string &strBase, std::string strSrc, std::string strDes);
void normalize_directory(std::string& str);
bool is_directory_exist(const std::string& dirPath);
bool create_directory(const std::string& path);
bool append_directory(const std::string& dirPath, const std::string& subDir);

bool is_file_exist(const std::string& filename);
bool remove_file(const std::string &path);

std::string getTimeString();
 std::string getTimeString( unsigned long long timeStamp );
long long getMicroSecond(void);
long long getMilliSecond(void);
long long getSecond(void);

std::string to_string_fix(uint32_t val);
long get_file_size(const std::string& filePath);
int EnumDirFiles(std::list<std::string>& list,
     const std::string& rootPath, std::string suffix, int limit_size);
int EnumDirFiles_FullPath(std::list<std::string> &list,
    const std::string &rootPath, std::string suffix, int limit_size);

 int clearUpFile(const std::string& rootPath, std::string suffix,
				 unsigned int limit_size );
unsigned long long getDiskFreeSize(
    const std::string& path, unsigned long long *total_size = NULL);
std::string strip_filename(const std::string &full_filename);
std::string strip_path(const std::string &full_filename);

bool get_mountpoint(const char **mount_point);

bool open_record_switch(int &fd);
void close_record_switch(int fd);
bool read_record_switch(int fd);
bool get_manual_record_enable();
bool set_manual_record_enable(bool v);
int append_alarm_index(int alarmType, int imageIndex);
void remove_dir(const std::string& dir_path);
int is_file_read_write_ok(const std::string& path);
bool is_sdcard_mounted();

}

#endif
