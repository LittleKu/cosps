#include "stdafx.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#include <curl/curl.h>
#include "common_utils.h"
#include "string_utils.h"

typedef struct body_data 
{
	char* content;
	size_t size;
	int offset;
	int status;
}body_data;

typedef struct head_data
{
	body_data* body_ptr;
}head_data;

static size_t cb_header(void *ptr, size_t size, size_t nmemb, void *userp)
{
	size_t realsize = size * nmemb;

	std::string line((char*)ptr, realsize);
	std::string content_length;
	if(find_between(line, "Content-Length:", "\r\n", content_length) == 0)
	{
		return realsize;
	}
	trim(content_length);
	int len = atoi(content_length.c_str());
	if(len <= 0)
	{
		return realsize;
	}

	head_data* header_ptr = (head_data*)userp;
	header_ptr->body_ptr->status = 1;
	header_ptr->body_ptr->size = len;

	return realsize;
}

static size_t cb_body(void *ptr, size_t size, size_t nmemb, void *userp)
{
	size_t realsize = size * nmemb;

	body_data* body_ptr = (body_data*)userp;
	if(body_ptr->status == 1 && body_ptr->size > 0)
	{
		if(body_ptr->content == NULL)
		{
			body_ptr->content = (char*)malloc(body_ptr->size);
		}
		memcpy(&(body_ptr->content[body_ptr->offset]), ptr, realsize);
		body_ptr->offset += realsize;
	}
	else
	{
		body_ptr->content = (char*)realloc(body_ptr->content, body_ptr->size + realsize);
		if (body_ptr->content == NULL) 
		{
			body_ptr->status = -1;
			/* out of memory! */
			printf("not enough memory (realloc returned NULL)\n");
			return -1;
		}
		memcpy(&(body_ptr->content[body_ptr->offset]), ptr, realsize);
		body_ptr->offset += realsize;
		body_ptr->size += realsize;
	}

	return realsize;
}

int file_get_contents(const char* file_name, char* & content, int& len)
{
	content = NULL;
	len = 0;

	struct stat file_info;
	/* get the file size of the local file */
	if(stat(file_name, &file_info)) 
	{
		printf("Couldnt open '%s': %s\n", file_name, strerror(errno));
		return 1;
	}
	FILE* fp = fopen(file_name, "rb");
	if(fp == NULL)
	{
		printf("Failed to open file '%s'\n", file_name);
		return 2;
	}

	len = file_info.st_size;
	content = (char*)malloc(len + 1);
	

	int offset = 0;
	char buf[4096];
	size_t nread;
	while( (nread = fread(buf, 1, 4096, fp)) > 0)
	{
		memcpy(content + offset, buf, nread);
		offset += nread;
	}
	content[len] = '\0';
	
	fclose(fp);

	return 0;
}

int file_download(const char* url, char*& content, int& length, const char* proxy)
{
	CURL *curl_handle;

	/* init the curl session */
	curl_handle = curl_easy_init();
	
	/* specify URL to get */
	curl_easy_setopt(curl_handle, CURLOPT_URL, url);
	curl_easy_setopt(curl_handle, CURLOPT_NOPROGRESS, 1L);
	curl_easy_setopt(curl_handle, CURLOPT_FOLLOWLOCATION, 1);
	/* some servers don't like requests that are made without a user-agent
	field, so we provide one */
	curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, USER_AGENT_IE8);
	if(proxy)
	{
		curl_easy_setopt(curl_handle, CURLOPT_PROXY, proxy);
	}
	
	body_data body;
	body.content = NULL;
	body.size = 0;
	body.offset = 0;
	body.status = 0;
	
	head_data head;
	head.body_ptr = &body;

	curl_easy_setopt(curl_handle, CURLOPT_HEADERFUNCTION, cb_header);
	curl_easy_setopt(curl_handle, CURLOPT_WRITEHEADER, &head);

	/* send all data to this function  */
	curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, cb_body);
	/* we pass our 'chunk' struct to the callback function */
	curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &body);
	
	curl_easy_perform(curl_handle);
	/* cleanup curl stuff */
	curl_easy_cleanup(curl_handle);

	content = body.content;
	length = body.size;

	return body.status;
}

int file_download(const char* url, const char* out_file_name, const char* proxy)
{
	CURL *curl_handle;

	FILE* fp_out = fopen(out_file_name, "wb");
	if(fp_out == NULL)
	{
		printf("Failed to open file '%s'\n", out_file_name);
		return 0;
	}

	/* init the curl session */
	curl_handle = curl_easy_init();
	
	/* specify URL to get */
	curl_easy_setopt(curl_handle, CURLOPT_URL, url);
	curl_easy_setopt(curl_handle, CURLOPT_NOPROGRESS, 0L);
	curl_easy_setopt(curl_handle, CURLOPT_FOLLOWLOCATION, 1);
	/* some servers don't like requests that are made without a user-agent
	field, so we provide one */
	curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, USER_AGENT_IE8);
	if(proxy)
	{
		curl_easy_setopt(curl_handle, CURLOPT_PROXY, proxy);
	}

	curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, fp_out);
	
	curl_easy_perform(curl_handle);
	fclose(fp_out);

	/* cleanup curl stuff */
	curl_easy_cleanup(curl_handle);

	return 1;
}


size_t ThrowAwayCallback(void *ptr, size_t size, size_t nmemb, void *data)
{
	(void)ptr;
	(void)data;

	return (size_t)(size * nmemb);
}
size_t WriteFileCallback(void *ptr, size_t size, size_t nmemb, void *data)
{
	FILE* fp = (FILE*)data;

	fwrite(ptr, size, nmemb, fp);
	
	return (size_t)(size * nmemb);
}