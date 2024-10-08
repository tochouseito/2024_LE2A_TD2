#pragma once

#include<vector>
#include<string>
#include<iostream>
#include<fstream>
#include<sstream>

class CSVLoader
{
public:
    // マップを読み込んで2次元ベクトルとして返す
   static std::vector<std::vector<int>> LoadMapFromCSV(const std::string& filename);
};

