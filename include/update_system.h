#ifndef UPDATE_SYSTEM_H
#define UPDATE_SYSTEM_H

#include <SDL2/SDL.h>
#include <curl/curl.h>
/* download() returns a libcurl CURLcode (CURLE_OK on success). */
CURLcode download(const char *url, const char *filename);

/* Blocking download that uses libcurl multi API. Intended to be run in a
 * dedicated thread so the transfer is non-blocking for the main/UI thread.
 * Returns a CURLcode (CURLE_OK on success).
 */
CURLcode download_multi_blocking(const char *url, const char *filename);

/* Progress reporting (updated by download):
 * - `download_bytes` : bytes downloaded so far
 * - `download_total` : total bytes expected (0 if unknown)
 */
extern volatile curl_off_t download_bytes;
extern volatile curl_off_t download_total;

/* Reset progress counters (optional) */
void reset_download_progress(void);

#endif