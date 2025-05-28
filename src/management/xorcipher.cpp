#include "../../include/management/xorcipher.hpp"
using namespace std;
string xorcipher(const string& data, const string& key) {
  if (key.empty()) {
    return data;
  }

  string output = data;
  for (size_t i = 0; i < data.length(); ++i) {
    output[i] = data[i] ^ key[i % key.length()];
  }
  return output;
}
