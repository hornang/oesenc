#include "oesenc/keylistreader.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <tinyxml2.h>

using namespace oesenc;
using namespace std;

namespace {

unordered_map<string, string> parseFile(const string &filename)
{
    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(filename.c_str()) != tinyxml2::XML_SUCCESS) {
        return {};
    }

    const char *chartElementName = "Chart";
    const auto *rootElement = doc.RootElement();

    if (rootElement == nullptr) {
        return {};
    }
    auto *keyList = doc.FirstChildElement("keyList");

    if (!keyList) {
        return {};
    }

    auto *chart = keyList->FirstChildElement(chartElementName);
    unordered_map<string, string> keys;

    while (chart) {
        auto *fileNameElement = chart->FirstChildElement("FileName");
        auto *rInstallKeyElement = chart->FirstChildElement("RInstallKey");

        if (fileNameElement && rInstallKeyElement) {
            auto *nameText = fileNameElement->GetText();
            auto *rInstallText = rInstallKeyElement->GetText();

            if (nameText && rInstallText && strlen(nameText) > 0) {
                keys[nameText] = rInstallText;
            } else {
                return keys;
            }
        } else {
            return keys;
        }

        chart = chart->NextSiblingElement(chartElementName);
    }

    return keys;
}
}

unordered_map<string, string> KeyListReader::readOesuKeys(string_view chartDir)
{
    for (auto &p : filesystem::directory_iterator(chartDir)) {
        if (p.path().extension() != ".XML" && p.path().extension() != ".xml") {
            continue;
        }
        auto chartKeys = parseFile(p.path().string());
        if (!chartKeys.empty()) {
            return chartKeys;
        }
    }
    return {};
}

string KeyListReader::readOesencKey(std::string_view chartDir)
{
    auto chartInfoFileName = filesystem::path(chartDir) / "Chartinfo.txt";

    if (!filesystem::exists(chartInfoFileName)) {
        return {};
    }

    ifstream file(chartInfoFileName);
    string line;

    const std::regex regex("^UserKey:(.+)$");

    while (std::getline(file, line)) {
        std::smatch match;

        if (std::regex_match(line, match, regex) && match.size() == 2) {
            try {
                return match[1].str();
            } catch (const std::invalid_argument &exception) {
                return {};
            }
        }
    }

    return {};
}
