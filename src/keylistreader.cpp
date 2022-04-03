#include "oesenc/keylistreader.h"
#include <iostream>
#include <tinyxml2.h>

using namespace oesenc;

KeyListReader::Status KeyListReader::load(const std::string &filename)
{
    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(filename.c_str()) != tinyxml2::XML_SUCCESS) {
        return Status::Failed;
    }

    const char *chartElementName = "Chart";
    const auto *rootElement = doc.RootElement();

    Status status = Status::Unknown;

    if (rootElement != nullptr) {
        auto *keyList = doc.FirstChildElement("keyList");

        if (!keyList) {
            return Status::Failed;
        }

        auto *chart = keyList->FirstChildElement(chartElementName);

        while (chart) {
            auto *fileNameElement = chart->FirstChildElement("FileName");
            auto *rInstallKeyElement = chart->FirstChildElement("RInstallKey");

            if (fileNameElement && rInstallKeyElement) {
                auto *nameText = fileNameElement->GetText();
                auto *rInstallText = rInstallKeyElement->GetText();

                if (nameText && rInstallText && strlen(nameText) > 0) {
                    m_chartKeys[nameText] = rInstallText;
                } else {
                    status = Status::Partial;
                }
            } else {
                status = Status::Partial;
            }

            chart = chart->NextSiblingElement(chartElementName);
        }
    }

    if (status == Status::Unknown) {
        status = Status::Success;
    }

    return status;
}

std::string KeyListReader::key(const std::string &chartName) const
{
    if (m_chartKeys.find(chartName) != m_chartKeys.end()) {
        return m_chartKeys.at(chartName);
    }
    return {};
}
