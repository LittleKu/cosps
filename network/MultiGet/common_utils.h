#ifndef __COMMON_UTILS_H__
#define __COMMON_UTILS_H__

int file_get_contents(const char* file_name, char* & content, int& length);
int file_download(const char* url, char* & content, int& length, const char* proxy = NULL);
int file_download(const char* url, const char* out_file_name, const char* proxy = NULL);

size_t ThrowAwayCallback(void *ptr, size_t size, size_t nmemb, void *data);
size_t WriteFileCallback(void *ptr, size_t size, size_t nmemb, void *data);

#endif