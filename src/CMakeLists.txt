# Build all the related targets.
add_executable(test.exc 
    test.cpp 
    CurlManager.cpp 
    NewsFinder.cpp)

target_link_libraries(test.exc curl gumbo WN)


