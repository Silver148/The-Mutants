#ifndef UPDATE_SYSTEM_H
#define UPDATE_SYSTEM_H

#include <curl/curl.h>
/* download() returns 0 on success, non-zero on failure */
int download(const char *url, const char *filename);

/* Progress reporting (updated by download):
 * - `download_bytes` : bytes downloaded so far
 * - `download_total` : total bytes expected (0 if unknown)
 */
extern volatile curl_off_t download_bytes;
extern volatile curl_off_t download_total;

/* Reset progress counters (optional) */
void reset_download_progress(void);

#endif