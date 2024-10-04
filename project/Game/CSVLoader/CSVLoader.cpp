#include "CSVLoader.h"

std::vector<std::vector<int>> CSVLoader::LoadMapFromCSV(const std::string& filename)
{
    std::vector<std::vector<int>> mapData;
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "ファイルを開くことができません: " << filename << std::endl;
        return mapData;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::vector<int> row;
        std::stringstream ss(line);
        std::string cell;

        while (std::getline(ss, cell, ',')) {
            row.push_back(std::stoi(cell));  // カンマで区切り、数値に変換して追加
        }

        mapData.push_back(row);  // 行データをマップに追加
    }

    file.close();
    return mapData;
}
