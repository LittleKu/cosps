#ifndef __COMMON_UTILS_H__
#define __COMMON_UTILS_H__

#define USER_AGENT_IE8	"Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 5.1; Trident/4.0; .NET CLR 1.1.4322; .NET CLR 2.0.50727; .NET CLR 3.0.4506.2152; .NET CLR 3.5.30729; InfoPath.2; MS-RTC LM 8; FDM)"

int file_get_contents(const char* file_name, char* & content, int& length);
int file_download(const char* url, char* & content, int& length, const char* proxy = NULL);
int file_download(const char* url, const char* out_file_name, const char* proxy = NULL);

size_t ThrowAwayCallback(void *ptr, size_t size, size_t nmemb, void *data);
size_t WriteFileCallback(void *ptr, size_t size, size_t nmemb, void *data);

#endif