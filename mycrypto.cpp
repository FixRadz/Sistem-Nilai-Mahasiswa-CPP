#include "mycrypto.h"
#include <fstream>

using namespace std;

string getKeyFromIni() {
    ifstream file("config.ini");
    string line, section;
    while (getline(file, line)) {
        if (line == "[Security]") section = "sec";
        if (section == "sec" && line.find("EncryptionKey=") == 0) {
            return line.substr(14); // Ambil setelah sama dengan
        }
    }
    return "DEFAULTKEY";
}

string encryptDecrypt(string text) {
    string keyStr = getKeyFromIni(); // Ambil kunci dari .ini
    string output = text;
    for (size_t i = 0; i < text.size(); i++) {
        output[i] = text[i] ^ keyStr[i % keyStr.size()];
    }
    return output;
}