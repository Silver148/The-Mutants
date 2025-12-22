/*
THE MUTANT'S

Authors: Abel Ferrer(aka The_Light) and Juan Yaguaro(aka silverhacker)
File: update_system.c

THIS CODE WILL REMAIN CONFIDENTIAL UNTIL THE PROJECT IS COMPLETED. 
Anyone who leaks this code will be automatically kicked out of the 
group and will be considered a real gay.

Copyright 2025
*/

#include "update_system.h"

/* Progress globals (extern in header) */
volatile curl_off_t download_bytes = 0;
volatile curl_off_t download_total = 0;

void reset_download_progress(void)
{
    download_bytes = 0;
    download_total = 0;
}

/* libcurl xferinfo callback to update progress globals */
static int xferinfo(void *p, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow)
{
    (void)p;
    download_total = dltotal;
    download_bytes = dlnow;
    return 0; /* return non-zero to abort transfer */
}

static size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}

/* Return value: CURLcode (CURLE_OK == 0 on success) */
CURLcode download(const char *url, const char *filename) {
    CURL *curl;
    FILE *fp;
    CURLcode res;
    
    curl = curl_easy_init();
    if(curl) {
        fp = fopen(filename, "wb");
        if(fp == NULL) {
            printf("No se pudo abrir el archivo %s\n", filename);
            return 1;
        }
        
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);

        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10L);

        curl_easy_setopt(curl, CURLOPT_USERAGENT, "TheMutants-Updater/1.0");
        /* enable progress and set callback */
        curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, xferinfo);
        curl_easy_setopt(curl, CURLOPT_XFERINFODATA, NULL);
        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
        
        res = curl_easy_perform(curl);
        if(res != CURLE_OK) {
            fprintf(stderr, "Error curl_easy_perform(): %s\n", curl_easy_strerror(res));
        }

        char *effective_url = NULL;
        curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &effective_url);
        
        fclose(fp);
        curl_easy_cleanup(curl);
        
        return res;
    }
    return CURLE_FAILED_INIT;
}
