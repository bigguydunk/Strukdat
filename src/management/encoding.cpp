#include "../../include/management/encoding.hpp"
#include <sstream>
#include <iomanip>
using namespace std;

string to_hex(const string& input) {
  stringstream ss;
  ss << hex << setfill('0');
  for (unsigned char c : input) {
    ss << setw(2) << static_cast<int>(c);
  }
  return ss.str();
}

string from_hex(const string& hex) {
  string res;
  for (size_t i = 0; i < hex.length(); i += 2) {
    string byteString = hex.substr(i, 2);
    char byte = static_cast<char>(stoi(byteString, nullptr, 16));
    res.push_back(byte);
  }
  return res;
}