/****************************************************************************
 * File: CurlManager.h
 * Name: Nate Groendyk
 * Date: 20 Oct 2018
 * Purpose: Handle Curl stuff with C++. Wrappers exists, but I want mine
 * So I understand who things work with libcurl properly.
 * THIS IS THE HEADER DEFS
 ***************************************************************************/
#ifndef CURLMANAGER_H
#define CURLMANAGER_H

#include <curl/curl.h>
#include <string>
#include <sstream>
#include <stdint.h>


//#define DEBUG


class CurlManager
{
  public:

    CurlManager(); /*< Ctor */
    ~CurlManager(); /*< Dtor */

    const bool isGood() const; /*< returns if curl instance is good to use */
    
    /**
     * Downloads a website.
     * @param Url The website to download.
     * @return true - downloaded. false - nope.
     */
    bool DownloadURL(const std::string & Url);

    /**
     * Getter for the received data string
     */
    const std::string getData() const;

  private:
    
    /**
     * Callback to handle when lib curl receives data.
     *
     * @param ptr       Pointer to the received data
     * @param size      always 1 (in chars/bytes I guess)
     * @param nmemb     Size (in chars/bytes I guess) of rcv'd data.
     * @param UserData  This will receive a pointer to the related CurlManager
     *                  class, so the function knows what to operate on.
     */
    static size_t Handle_RcvData(char * ptr, size_t size, size_t nmemb,
                                 void * UserData);

  private:

    static int mzTotalInstances; /*<Tracks if we need to build/tear down curl*/
    static bool mzCurlIsUp; /*< Keeps track if Curl is OK to use */

    CURL * mpEZCurlHandle; /*< Handle of the easy curl object */
    std::ostringstream mRcvdData; /*< Holds received data */ 

};

#endif
