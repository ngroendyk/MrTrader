#!/bin/bash
# Written By: Nate Groendyk.
# Date: 20 Oct 2018
# Purpose: Wipe out cmake generated stuff and build stuff.

#--------------------#
DeleteAllOf () {
    #Arg $1 is the file name to delete
    #Arg $2->end is an exclusion list.


    list=`find ./ -name "$1"`
    if [[ $list == "" ]]; then
        return
    fi

    # Iterate through all the found files.
    while read -r line; do

        #Check if the current file/path string combo is in the exclusion list
        InExList=""
        for ExListIter in "${@:2}"; do
            Exclude=`echo "$line" | grep "$ExListIter"`
            TrimmedSpace="$(echo -e "${Exclude}" | sed -e 's/^[[:space:]]*//' -e 's/[[:space:]]*$//')"
            if [[ $TrimmedSpace != "" ]]; then
                #echo "$line is in exclusion list: $ExListIter"
                InExList="$ExListIter"
                break
            fi
        done

        #Exclude=`echo "$line" | grep "$2"`
        if [[ $InExList == "" ]]; then
            echo "Deleting: $line ..."
            rm -rf "$line"
        else
            echo "NOT Deleting: $line, it is in exclusion list: $InExList"
        fi
    done <<< "$list"
}

#--------------------#

#Because it appends it as new lines the (\n), we need to use the $''
#see: https://superuser.com/questions/284187/bash-iterating-over-lines-in-a-variable
#ExclusionList=$'gumbo_parser\n'
#ExclusionList+=$'src'

#Make sure intermediate exclusions have space at end.
ExclusionList="gumbo_parser "
#ExclusionList+="src "

#Find all the cmade files, Make files and Wipe the slate clean of them.
DeleteAllOf cmake_install.cmake $ExclusionList
DeleteAllOf Makefile $ExclusionList
DeleteAllOf "*.exc" $ExclusionList
DeleteAllOf CMakeCache.txt $ExclusionList
DeleteAllOf CMakeFiles $ExclusionList
 



#rm cmake_install.cmake
#rm -rf ./CMakeFiles
#rm CMakeCache.txt
#
#rm *.exc
#rm Makefile
# 
