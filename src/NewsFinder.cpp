/****************************************************************************
 * File: NewsFinder.cpp
 * Name: Nate Groendyk
 * Date: 20 Oct 2018
 * Purpose: Handles procesing news articles for links, and dumps links into 
 * a list to keep downloading.
 ***************************************************************************/

#include "NewsFinder.h"
#include "CurlManager.h" 
#include "gumbo.h"

#include <string>
#include <stdint.h>
#include <iostream>
#include <vector>
#include <list>
#include <set>
#include <boost/algorithm/string.hpp>

using namespace std;
using namespace boost;


NewsFinder::NewsFinder() : mSeedLinks(),
                           mSeedLinkUniqs(),
                           mCurlDL() 
{
}

NewsFinder::~NewsFinder()
{
}

void NewsFinder::AddSeedLink(const string & NextLink)
{
    //add it to the set/see if we can find it in the set.
    if (mSeedLinkUniqs.insert(NextLink).second)
    {
        //was added to set, push into vector
        mSeedLinks.push_back(NextLink);
        cout << "UNIQ LINK: " << NextLink << endl;
    }
}

void NewsFinder::Process()
{
    //iterate through all the possible links, parsing and getting more links!
    for (list < string >::const_iterator iter = mSeedLinks.begin();
         iter != mSeedLinks.end(); ++iter)
    {
        mCurlDL.DownloadURL(*iter);
        string PageData(mCurlDL.getData());
//        ProcessForMoreSeedLinks(*iter, PageData);
        ProcessForMoreSeedLinks(PageData);
 
    }
}

void NewsFinder::search_for_links(GumboNode* node)
{
    if (node->type != GUMBO_NODE_ELEMENT) 
    {
        return;
    }
    GumboAttribute* href;
    if (node->v.element.tag == GUMBO_TAG_A &&
        (href = gumbo_get_attribute(&node->v.element.attributes, "href"))) 
    {
        string NewLink(href->value);
        //Check if its a legit link
        if (isValidUrl(NewLink))
        {
            AddSeedLink(NewLink);
        }
    }
    //Handle the kids.
    GumboVector* children = &node->v.element.children;
    for (unsigned int i = 0; i < children->length; ++i)
    {
        search_for_links(static_cast<GumboNode*>(children->data[i]));
    }
}


/** This overload uses gumbo_parser to get its links. **/
bool NewsFinder::ProcessForMoreSeedLinks( const string & WebPageData)
{
    //Send to gumbo parser.
    GumboOutput* output = gumbo_parse(WebPageData.c_str());
    search_for_links(output->root);
    gumbo_destroy_output(&kGumboDefaultOptions, output);
    return true;
}

//Original Link crawler. Differrence is we just do a bad ripping job.
bool NewsFinder::ProcessForMoreSeedLinks(const string & CurLink,
                                         const string & WebPageData)
{
    //go through the current page.
    //find links.
    //check links against current list. only add if its not in there.
    for (size_t CurPos = 0; ; )
    {
        size_t retval = WebPageData.find("http", CurPos);
        if (retval == string::npos)
        {
            break; //done this website.
        }

        //Found the position to start grabbing chars from
        //go till a trailing \"
        size_t endval = WebPageData.find("\"", retval);
        if (endval == string::npos)
        {
            break; //didnt get complete link info.
        }
        
        //extract string.
        string NewLink(WebPageData.substr(retval, endval - retval));
        FilterForSubLinks(NewLink);

        //update next path.
        CurPos = endval;
    }    
    return true;
}

void NewsFinder::FilterForSubLinks(const string & UnfilteredLink)
{
    //the string might have a vlid url. it might have several valid. it might
    //have crap. If its valid, try to rip it out and add. try to extract the
    //legit stuff

    //Lets try to use boost to split some strings, using the new lines, spaces,
    // ect as the delimitting info.

    //useful link: https://www.boost.org/doc/libs/1_68_0/doc/html/string_algo/usage.html
    vector< string > ResultStrs;

    //For now adding & and ;, but adding & def seems IFFY. meaning there are
    //circumstances to specifically look for.
    // (complicating logic) such as "&quote -> bad URL
    //split(ResultStrs, UnfilteredLink, is_any_of("\n\r >'"), token_compress_on);
    split(ResultStrs, UnfilteredLink, is_any_of("\n\r >';&"), token_compress_on);


    //iterate through the resultant split vector. Figure out if a legit URL
    for (vector< string >::iterator iter = ResultStrs.begin();
        iter != ResultStrs.end(); ++iter)
    {
        trim(*iter);
        size_t CurPos = iter->find("http");
        if (CurPos == string::npos)
        {
            continue;
        } 
        else if (CurPos != 0) 
        {
            ProcessForMoreSeedLinks("", *iter);
            continue;
        }

        if (isValidUrl(*iter))
        {
            AddSeedLink(*iter);
        }
    }
}


bool NewsFinder::isValidUrl(const std::string & UrlToTest)
{

    //basically try to find some common stuff that a valid URL would have.
    //and if theres stuff it wouldnt have. we assume that invalidates it.
    
    //Must have a "http"
    if (UrlToTest.find("http") == string::npos)
    {
        return false;
    }

    //Must have a "://"
    if (UrlToTest.find("://") == string::npos)
    {
        return false;
    }

    //Must have a "."
    if (UrlToTest.find(".") == string::npos)
    {
        return false;
    }

    return true;
}



