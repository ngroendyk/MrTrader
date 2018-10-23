/****************************************************************************
 * File: CurlManager.cpp
 * Name: Nate Groendyk
 * Date: 20 Oct 2018
 * Purpose: Handle Curl stuff with C++. Wrappers exists, but I want mine
 * So I understand who things work with libcurl properly.
 ***************************************************************************/

#include "CurlManager.h"
#include <curl/curl.h>
#include <string>
#include <stdint.h>
#include <iostream>

using namespace std;

int CurlManager::mzTotalInstances = 0; //default value
bool CurlManager::mzCurlIsUp = false;

CurlManager::CurlManager() : mpEZCurlHandle(0),
                             mRcvdData()
{
    if (!mzTotalInstances)
    {
        //initialize curl. Must be done when only single thread or insure some
        //thread locking on static stuff
        mzCurlIsUp = !curl_global_init(CURL_GLOBAL_ALL);
    }
    mzTotalInstances++;
    if (!mzCurlIsUp)
    {
        return;
    }

    //Try to get an Easy Curl handle.
    mpEZCurlHandle = curl_easy_init();
}

CurlManager::~CurlManager()
{
    //release curl handles
    if (mpEZCurlHandle)
    {
        curl_easy_cleanup(mpEZCurlHandle);
        mpEZCurlHandle = 0; 
    }

    mzTotalInstances--;
    if (!mzTotalInstances)
    {
        //just released last instance of libCurl. Shut down its global stuff.
        //again must be either single thread, or uber thread locked to prevent
        //static data corruption issues.
        curl_global_cleanup();
    }
}

const bool CurlManager::isGood() const
{
    return mzCurlIsUp && mpEZCurlHandle;
}

bool CurlManager::DownloadURL(const string & Url)
{
    mRcvdData.clear(); //reset each time a new DL happens
    if (!mpEZCurlHandle)
    {
        return false;
    }

    curl_easy_setopt(mpEZCurlHandle, CURLOPT_URL, Url.c_str()); //get site

    //follow redirects
    curl_easy_setopt(mpEZCurlHandle, CURLOPT_FOLLOWLOCATION, 1L); 

    //Configure so that this curl handle uses our write callback, AND that
    //it gets our instance handle.
    curl_easy_setopt(mpEZCurlHandle, CURLOPT_WRITEFUNCTION, Handle_RcvData);
    curl_easy_setopt(mpEZCurlHandle, CURLOPT_WRITEDATA, 
        reinterpret_cast< void * >(this));

    //Turn off all the SSL auth crap (for now)

    //dont skip peer verify
    curl_easy_setopt(mpEZCurlHandle, CURLOPT_SSL_VERIFYPEER, 1L); 
    //dont skip host verify
    curl_easy_setopt(mpEZCurlHandle, CURLOPT_SSL_VERIFYHOST, 1L);

    //force http 2 if u can rther than 1.1. prevents hang
    curl_easy_setopt(mpEZCurlHandle, CURLOPT_HTTP_VERSION, 
                     CURL_HTTP_VERSION_2_0);
    
    #ifdef DEBUG
    curl_easy_setopt(mpEZCurlHandle, CURLOPT_VERBOSE, 1L); 
    #endif

    //set a User-agent, otherwise we get denied.
    curl_easy_setopt(mpEZCurlHandle, CURLOPT_USERAGENT, "curl/7.58.0");

    //Perform the DL
    CURLcode retval = curl_easy_perform(mpEZCurlHandle);
    if (retval != CURLE_OK)
    {
        cout << "libcurl error: " << curl_easy_strerror(retval) << endl;
        return false;
    }
    return true;
}


size_t CurlManager::Handle_RcvData(char * ptr, size_t size, size_t nmemb,
                                   void * UserData)
{
    if (!UserData)
    {
        return nmemb; ///bbbbb-bail!
    }

    CurlManager *pUs(reinterpret_cast< CurlManager* >(UserData));

    pUs->mRcvdData << string(ptr, nmemb); //create a string, add to instance.
    return nmemb; //we took care of all bytes, so simply tell libcurl so it
                  //doesnt crap out on us.
} 

const string CurlManager::getData() const
{
    return mRcvdData.str();
}


