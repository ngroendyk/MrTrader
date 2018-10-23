
//#include <curl/curl.h>
#include "NewsFinder.h"
#include <iostream>

using namespace std;
 
int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        cout << "Usage: " << argv[0] << " WEB_SITE" << endl;
        return 1;
    }

//    CurlManager SeedUrl;
//    cout << "Curl is: " << SeedUrl.isGood() << endl;

//    SeedUrl.DownloadURL(argv[1]);
    //cout << "Data is:" << SeedUrl.getData() << endl;

    //ProcessUrlPage(SeedUrl.getData());

    NewsFinder Aggregator;
    Aggregator.AddSeedLink(argv[1]);
    Aggregator.Process();

    cout << "DONE!" << endl;



    return 0;
}

