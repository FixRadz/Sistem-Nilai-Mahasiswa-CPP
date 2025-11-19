#ifndef MYCRYPTO_H
#define MYCRYPTO_H

#include <string>
using namespace std;

#ifdef BUILD_DLL
    #define DLL_API __declspec(dllexport)
#else
    #define DLL_API __declspec(dllimport)
#endif

extern "C" {
    DLL_API string encryptDecrypt(string text); 
}

#endif