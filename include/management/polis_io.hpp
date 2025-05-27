#pragma once
#include "polis.hpp"
#include <vector>
#include <string>

void loadPolisFromCSV(const std::string& filename, std::vector<Polis>& daftarPolis);
void savePolisToCSV(const std::string& filename, const std::vector<Polis>& daftarPolis);
