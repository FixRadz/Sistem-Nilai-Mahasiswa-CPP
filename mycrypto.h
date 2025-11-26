#ifndef MYCRYPTO_H
#define MYCRYPTO_H

#include <string>

#ifdef BUILD_DLL
    #define DLL_API __declspec(dllexport)
#else
    #define DLL_API __declspec(dllimport)
#endif

extern "C" {
    DLL_API std::string encryptDecrypt(std::string text); 
}

#endif