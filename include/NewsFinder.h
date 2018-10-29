/****************************************************************************
 * File: NewsFinder.h
 * Name: Nate Groendyk
 * Date: 20 Oct 2018
 * Purpose: Handles procesing news articles for links, and dumps links into 
 * a list to keep downloading.
 * THIS IS THE HEADER DEFS
 ***************************************************************************/
#ifndef NEWSFINDER_H
#define NEWSFINDER_H

#include "CurlManager.h" 
#include "gumbo.h"
#include <string>
#include <sstream>
#include <stdint.h>
#include <vector>
#include <set>
#include <list>

//#define DEBUG

class NewsFinder
{
  public:

    NewsFinder(); /*< Ctor */
    ~NewsFinder(); /*< Dtor */

    /**
     * Used to add links to initially parse out more biz links
     * 
     * @param NextLink a URL to look through.
     */
    void AddSeedLink(const std::string & NextLink);


    /**
     * Starts processing all the links, downloading new data, getting new links,
     * and parsing that data (via call-back)
     */
    void Process();

    /**
     * Used to determine a string is a reasonable shot at a URL
     * 
     * @param UrlToTest  The URL you want to know about
     * @return true - Yep, a URL. false - nope, not a URL
     */
    bool isValidUrl(const std::string & UrlToTest);


 
    //Add function hooks/callbacks into processing pages for stock trading
    //content....when we get a page, parse out its next links, we need to call
    //a  post-processing set, to process the actual page for stock insights.
    //think to keep multithreaded.

  private:

    /**
     * Process website page data for links. Adds it to the link list
     *
     * @param CurLink The link that is currently being looked through.
     * @param WebPAgeData The data that is to be processed.
     * @return true
     */
    bool ProcessForMoreSeedLinks(const std::string & CurLink,
                                 const std::string & WebPageData);

    /**
     * Overload for Processing a website. You only pass it the data. This
     * particular over uses the gumbo-parser to rip out links, which looks 
     * potentially more thought out then the other approach.
     *
     * @param WebPageData The data of the webpage.
     * @return true
     */
    bool ProcessForMoreSeedLinks(const std::string & WebPageData);

    
    /*< Helper func to parse/form better URLs */
    void FilterForSubLinks(const std::string & UnfilteredLink); 


    /*< Helper func to do recursive link finding using gumbo_parser */
    void search_for_links(GumboNode* node);


  private:
    std::list< std::string > mSeedLinks;
    std::set< std::string > mSeedLinkUniqs;  
    CurlManager mCurlDL; /*< Holds the curl instance for downloading stuff */
};

#endif
