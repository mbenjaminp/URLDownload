//works on Windows


#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <urlmon.h>    //needs urlmon.lib or liburlmon.a (MinGW)!



/*
This Programm takes a Path to an existing ANSI-Textfile,
which contains URLs to downloadable files and stores them
in the same directory where the Programm is executed
*/

int main(int argc, char* argv[])
{
    std::ifstream readURL;
    std::ofstream writeaccesstest;
    std::string actualURL;
    std::string filename;
    int unnamedfilecounter = 0;


    //Tests if the Path from the URLfile is given as Parameter
    if(argc != 2)
    {
        std::cout << "Use: URLDownload.exe Path_from_the_file_with_URLs";
        exit(1);
    }

    //Tests if the URLfile can be opened
    readURL.open(argv[1]);
    if(!readURL.is_open())
    {
        std::cout << "Error opening the given URLfile Path.";
        exit(1);
    }

    //Tests if writing is allowed on the Path where this Programm is executed
    writeaccesstest.open("Writeaccesstest.test");
    if(!writeaccesstest.is_open())
    {
        std::cout << "Error no writing allowed on current Path";
        writeaccesstest.close();
        std::remove("Writeaccesstest.test");
        exit(1);
    }

    //removes testfile
    writeaccesstest.close();
    std::remove("Writeaccesstest.test");

    //reading URLs
    while(readURL.good())
    {
        //reads URLs, one per line
        std::getline(readURL, actualURL);
        //reads the Filename from the URL (it´s supposed that the filename is given after the last "/" in the URL)
        filename = actualURL.substr(actualURL.find_last_of("/") + 1);
        //prevents empty filenames, when the filename can´t be read with the method above
        if(filename == "")
        {
            filename = "Unnamed" + std::to_string(unnamedfilecounter);
            unnamedfilecounter++;
        }
        //trying to download file from given actualURL
        if(URLDownloadToFileA(NULL, actualURL.c_str(), filename.c_str(), 0, NULL) != S_OK)
        {
            std::cout << "Error at downloading " << filename << " from URL: " << actualURL << "\n\n";
        }
    }

    //checking, if while ended because of an error, or the Fileend
    if(!readURL.eof())
    {
        std::cout << "File reading error. Some files may not have been downloaded.";
    }

    //closing ifstream
    readURL.close();

    return 0;
}
