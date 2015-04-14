/***************************************************************************
 *                                  _   _ ____  _
 *  Project                     ___| | | |  _ \| |
 *                             / __| | | | |_) | |
 *                            | (__| |_| |  _ <| |___
 *                             \___|\___/|_| \_\_____|
 *
 * Copyright (C) 1998 - 2011, Daniel Stenberg, <daniel@haxx.se>, et al.
 *
 * This software is licensed as described in the file COPYING, which
 * you should have received as part of this distribution. The terms
 * are also available at http://curl.haxx.se/docs/copyright.html.
 *
 * You may opt to use, copy, modify, merge, publish, distribute and/or sell
 * copies of the Software, and permit persons to whom the Software is
 * furnished to do so, under the terms of the COPYING file.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 ***************************************************************************/
#include <stdio.h>
#include <unistd.h>
#include <curl/curl.h>

#include <string>

#include "./rev.h"

struct userdata
{
  std::string s;
};

size_t write_tostring(char *ptr, size_t size, size_t nmemb, void *userdata)
{
  struct userdata * ud = static_cast<struct userdata *>(userdata);

  ud->s.append(ptr, (size * nmemb));

  // and log to stdout
  return fwrite(ptr, size, nmemb, stdout);
}


int main(void)
{
  CURL *curl;
  CURLcode res;
  FILE * pf;

  curl_global_init(CURL_GLOBAL_ALL);

  pf = fopen("d1", "r");

  curl = curl_easy_init();
  if(curl) {
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
    curl_easy_setopt(curl, CURLOPT_HEADER, 1L);

    /* get the first document */
    curl_easy_setopt(curl, CURLOPT_URL, "http://127.0.0.1:15986/-/user/org.couchdb.user:user");
    curl_easy_setopt(curl, CURLOPT_PUT, 1L);
    curl_easy_setopt(curl, CURLOPT_READDATA, pf);

    /* Perform the request, res will get the return code */
    res = curl_easy_perform(curl);
    /* Check for errors */
    if(res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));

    /* rewind */
    fseek(pf, 0, SEEK_SET);

    /* Perform the request again, res will get the return code */
    res = curl_easy_perform(curl);
    /* Check for errors */
    if(res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));


    struct userdata get_result;

    curl_easy_setopt(curl, CURLOPT_PUT, 0L);
    curl_easy_setopt(curl, CURLOPT_READDATA, 0);
    curl_easy_setopt(curl, CURLOPT_URL, "http://127.0.0.1:15986/-/user/org.couchdb.user:user?write=true");
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &get_result);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_tostring);
    
    /* Perform the request, res will get the return code */
    res = curl_easy_perform(curl);
    /* Check for errors */
    if(res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));

      
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, stdout);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);

    // do something that gets the name
    // "_rev":"1-9a1b90d9570c2c6183949f2b6fd8fd79"
    std::string rev = extract_rev(get_result.s);
    std::string newRev = "http://127.0.0.1:15986/-/user/org.couchdb.user:user/-rev/";
    newRev += rev;

    curl_easy_setopt(curl, CURLOPT_URL, newRev.c_str());
    curl_easy_setopt(curl, CURLOPT_PUT, 1L);
    curl_easy_setopt(curl, CURLOPT_READDATA, pf);

    fseek(pf, 0, SEEK_SET);

    /* Perform the request, res will get the return code */
    res = curl_easy_perform(curl);
    /* Check for errors */
    if(res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));

    /* always cleanup */
    fclose(pf);
    curl_easy_cleanup(curl);
  }

  return 0;
}
