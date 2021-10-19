#include <cstring>
#include <fstream>
#include <iostream>
#include <memory>
#include <unordered_map>

#include "oesenc/chartfile.h"
#include "oesenc/chartreader.h"
#include "oesenc/opencpn.h"
#include "oesenc/s57.h"

#define HEADER_SENC_VERSION 1
#define HEADER_CELL_NAME 2
#define HEADER_CELL_PUBLISHDATE 3
#define HEADER_CELL_EDITION 4
#define HEADER_CELL_UPDATEDATE 5
#define HEADER_CELL_UPDATE 6
#define HEADER_CELL_NATIVESCALE 7
#define HEADER_CELL_SENCCREATEDATE 8
#define HEADER_CELL_SOUNDINGDATUM 9

#define FEATURE_ID_RECORD 64
#define FEATURE_ATTRIBUTE_RECORD 65

#define FEATURE_GEOMETRY_RECORD_POINT 80
#define FEATURE_GEOMETRY_RECORD_LINE 81
#define FEATURE_GEOMETRY_RECORD_AREA 82
#define FEATURE_GEOMETRY_RECORD_MULTIPOINT 83
#define FEATURE_GEOMETRY_RECORD_AREA_EXT 84

#define VECTOR_EDGE_NODE_TABLE_EXT_RECORD 85
#define VECTOR_CONNECTED_NODE_TABLE_EXT_RECORD 86

#define VECTOR_EDGE_NODE_TABLE_RECORD 96
#define VECTOR_CONNECTED_NODE_TABLE_RECORD 97

#define CELL_COVR_RECORD 98
#define CELL_NOCOVR_RECORD 99
#define CELL_EXTENT_RECORD 100
#define CELL_TXTDSC_INFO_FILE_RECORD 101

ChartFile::ChartFile(const std::string &filename)
    : m_filename(filename)
{
}

ChartFile::~ChartFile()
{
    if (pBuffer) {
        free(pBuffer);
    }
}

bool ChartFile::readHeaders()
{
    std::unordered_map<int, S57::VectorEdge> vectorEdges;
    std::unordered_map<int, S57::ConnectedNode> connectedNodes;

    if (!ingest200(m_filename, m_s57, vectorEdges, connectedNodes, true)) {
        std::cerr << "Failed to read" << std::endl;
        return false;
    }
    return true;
}

bool ChartFile::read()
{
    std::unordered_map<int, S57::VectorEdge> vectorEdges;
    std::unordered_map<int, S57::ConnectedNode> connectedNodes;

    if (!ingest200(m_filename, m_s57, vectorEdges, connectedNodes)) {
        std::cerr << "Failed to read" << std::endl;
        return false;
    }

    for (S57 &obj : m_s57) {
        obj.buildGeometry(vectorEdges, connectedNodes);
    }
    return true;
}

bool ChartFile::ingest200(const std::string &senc_file_name,
                          std::vector<S57> &s57Vector,
                          std::unordered_map<int, S57::VectorEdge> &vectorEdges,
                          std::unordered_map<int, S57::ConnectedNode> &connectedNodes,
                          bool headersOnly)
{
    ChartReader fpx;

    if (!fpx.Open(senc_file_name)) {
        return false;
    }

    S57 *s57 = nullptr;

    while (true) {
        OSENC_Record_Base record;
        if (!fpx.Read(&record, sizeof(OSENC_Record_Base))) {
            break;
        }

        switch (record.record_type) {
        case HEADER_SENC_VERSION: {
            unsigned char *buf = getBuffer(record.record_length - sizeof(OSENC_Record_Base));
            if (!fpx.Read(buf, record.record_length - sizeof(OSENC_Record_Base))) {
                return false;
            }
            uint16_t *pint = (uint16_t *)buf;
            m_senc_file_read_version = *pint;

            if (m_senc_file_read_version < 201) {
                std::cerr << "Libray not compatible with versions under 201" << std::endl;
            } else if (m_senc_file_read_version > 201) {
                std::cerr << "Libray has not been tested with newer versions" << std::endl;
            }
            break;
        }
        case HEADER_CELL_NAME: {
            unsigned char *buf = getBuffer(record.record_length - sizeof(OSENC_Record_Base));
            if (!fpx.Read(buf, record.record_length - sizeof(OSENC_Record_Base))) {
                return false;
            }
            m_Name = reinterpret_cast<const char *>(buf);
            break;
        }
        case HEADER_CELL_PUBLISHDATE: {
            unsigned char *buf = getBuffer(record.record_length - sizeof(OSENC_Record_Base));
            if (!fpx.Read(buf, record.record_length - sizeof(OSENC_Record_Base))) {
                return false;
            }
            m_sdate000 = reinterpret_cast<const char *>(buf);
            break;
        }

        case HEADER_CELL_EDITION: {
            unsigned char *buf = getBuffer(record.record_length - sizeof(OSENC_Record_Base));
            if (!fpx.Read(buf, record.record_length - sizeof(OSENC_Record_Base))) {
                return false;
            }
            uint16_t *pint = (uint16_t *)buf;
            m_read_base = *pint;
            break;
        }

        case HEADER_CELL_UPDATEDATE: {
            unsigned char *buf = getBuffer(record.record_length - sizeof(OSENC_Record_Base));
            if (!fpx.Read(buf, record.record_length - sizeof(OSENC_Record_Base))) {
                return false;
            }
            m_LastUpdateDate = reinterpret_cast<const char *>(buf);
            break;
        }

        case HEADER_CELL_UPDATE: {
            unsigned char *buf = getBuffer(record.record_length - sizeof(OSENC_Record_Base));
            if (!fpx.Read(buf, record.record_length - sizeof(OSENC_Record_Base))) {
                return false;
            }
            uint16_t *pint = (uint16_t *)buf;
            m_read_last_applied_update = *pint;
            break;
        }

        case HEADER_CELL_NATIVESCALE: {
            unsigned char *buf = getBuffer(record.record_length - sizeof(OSENC_Record_Base));
            if (!fpx.Read(buf, record.record_length - sizeof(OSENC_Record_Base))) {
                return false;
            }
            uint32_t *pint = (uint32_t *)buf;
            m_nativeScale = *pint;
            break;
        }

        case HEADER_CELL_SOUNDINGDATUM: {
            unsigned char *buf = getBuffer(record.record_length - sizeof(OSENC_Record_Base));
            if (!fpx.Read(buf, record.record_length - sizeof(OSENC_Record_Base))) {
                return false;
            }
            m_SoundingDatum = reinterpret_cast<const char *>(buf);
            break;
        }

        case HEADER_CELL_SENCCREATEDATE: {
            unsigned char *buf = getBuffer(record.record_length - sizeof(OSENC_Record_Base));
            if (!fpx.Read(buf, record.record_length - sizeof(OSENC_Record_Base))) {
                return false;
            }
            break;
        }

        case CELL_EXTENT_RECORD: {
            unsigned char *buf = getBuffer(record.record_length - sizeof(OSENC_Record_Base));
            if (!fpx.Read(buf, record.record_length - sizeof(OSENC_Record_Base))) {
                return false;
            }
            _OSENC_EXTENT_Record_Payload *pPayload = (_OSENC_EXTENT_Record_Payload *)buf;
            const Position upperLeft(pPayload->extent_nw_lat, pPayload->extent_nw_lon);
            const Position lowerRight(pPayload->extent_se_lat, pPayload->extent_se_lon);
            m_extent = Rect(upperLeft, lowerRight);
            break;
        }

        case CELL_COVR_RECORD: {
            unsigned char *buf = getBuffer(record.record_length - sizeof(OSENC_Record_Base));
            if (!fpx.Read(buf, record.record_length - sizeof(OSENC_Record_Base))) {
                return false;
            }
            break;
        }

        case CELL_NOCOVR_RECORD: {
            unsigned char *buf = getBuffer(record.record_length - sizeof(OSENC_Record_Base));
            if (!fpx.Read(buf, record.record_length - sizeof(OSENC_Record_Base))) {
                return false;
            }
            break;
        }

        case FEATURE_ID_RECORD: {
            if (headersOnly) {
                fpx.close();
                return true;
            }
            unsigned char *buf = getBuffer(record.record_length - sizeof(OSENC_Record_Base));
            if (!fpx.Read(buf, record.record_length - sizeof(OSENC_Record_Base))) {
                return false;
            }
            _OSENC_Feature_Identification_Record_Payload *pPayload = (_OSENC_Feature_Identification_Record_Payload *)buf;
            S57::Type typeCode = S57::fromTypeCode(pPayload->feature_type_code);
            s57Vector.push_back(S57(typeCode));
            s57 = &(*std::prev(s57Vector.end()));
            break;
        }

        case FEATURE_ATTRIBUTE_RECORD: {
            unsigned char *buf = getBuffer(record.record_length - sizeof(OSENC_Record_Base));
            if (!fpx.Read(buf, record.record_length - sizeof(OSENC_Record_Base))) {
                return false;
            }
            OSENC_Attribute_Record_Payload *pPayload = (OSENC_Attribute_Record_Payload *)buf;

            int attributeValueType = pPayload->attribute_value_type;

            S57::Attribute attribute = S57::attributeFromTypeCode(pPayload->attribute_type_code);

            if (attribute != S57::Attribute::Unknown) {
                switch (attributeValueType) {
                case 0: {
                    const uint32_t val = pPayload->attribute_value_int;
                    if (s57) {
                        s57->setAttribute(attribute, static_cast<uint32_t>(val));
                    }
                    break;
                }

                case 1: // Integer list
                {
                    // Calculate the number of elements from the record size
                    //int nCount = (record.record_length - sizeof(_OSENC_Attribute_Record)) ;

                    break;
                }
                case 2: // Single double precision real
                {
                    const double val = pPayload->attribute_value_double;
                    if (s57) {
                        s57->setAttribute(attribute, static_cast<float>(val));
                    }
                    break;
                }

                case 3: // List of double precision real
                {
                    //TODO
                    break;
                }

                case 4: // Ascii String
                {
                    const char *val = (char *)&pPayload->attribute_value_char_ptr;
                    if (s57) {
                        s57->setAttribute(attribute, std::string(val));
                    }
                    break;
                }

                default:
                    break;
                }
            }

            break;
        }

        case FEATURE_GEOMETRY_RECORD_POINT: {
            unsigned char *buf = getBuffer(record.record_length - sizeof(OSENC_Record_Base));
            if (!fpx.Read(buf, record.record_length - sizeof(OSENC_Record_Base))) {
                return false;
            }

            _OSENC_PointGeometry_Record_Payload *pPayload = (_OSENC_PointGeometry_Record_Payload *)buf;

            if (s57) {
                s57->setPointGeometry(Position(pPayload->lat, pPayload->lon));
            }
            break;
        }

        case FEATURE_GEOMETRY_RECORD_AREA: {
            unsigned char *buf = getBuffer(record.record_length - sizeof(OSENC_Record_Base));
            if (!fpx.Read(buf, record.record_length - sizeof(OSENC_Record_Base))) {
                return false;
            }
            _OSENC_AreaGeometry_Record_Payload *pPayload = (_OSENC_AreaGeometry_Record_Payload *)buf;

            if (s57 != nullptr) {
                unsigned char *next_byte = nullptr;
                next_byte = skipTessellationData(pPayload);
                s57->setLineGeometry(reinterpret_cast<S57::LineElement *>(next_byte),
                                     pPayload->edgeVector_count);
            }

            break;
        }

        case FEATURE_GEOMETRY_RECORD_AREA_EXT: {
            unsigned char *buf = getBuffer(record.record_length - sizeof(OSENC_Record_Base));
            if (!fpx.Read(buf, record.record_length - sizeof(OSENC_Record_Base))) {
                return false;
            }
            break;
        }

        case FEATURE_GEOMETRY_RECORD_LINE: {
            unsigned char *buf = getBuffer(record.record_length - sizeof(OSENC_Record_Base));
            if (!fpx.Read(buf, record.record_length - sizeof(OSENC_Record_Base))) {
                return false;
            }
            _OSENC_LineGeometry_Record_Payload *pPayload = (_OSENC_LineGeometry_Record_Payload *)buf;
            if (s57 != nullptr) {
                s57->setLineGeometry(reinterpret_cast<S57::LineElement *>(&pPayload->payLoad),
                                     pPayload->edgeVector_count);
            }
            break;
        }

        case FEATURE_GEOMETRY_RECORD_MULTIPOINT: {
            unsigned char *buf = getBuffer(record.record_length - sizeof(OSENC_Record_Base));
            if (!fpx.Read(buf, record.record_length - sizeof(OSENC_Record_Base))) {
                return false;
            }
            OSENC_MultipointGeometry_Record_Payload *pPayload = (OSENC_MultipointGeometry_Record_Payload *)buf;
            std::vector<S57::PointGeometry> multiPointGeometry;

            float *pointTable = reinterpret_cast<float *>(&pPayload->payLoad);

            for (unsigned int i = 0; i < pPayload->point_count; i++) {
                double easting = pointTable[i * 3 + 0];
                double northing = pointTable[i * 3 + 1];
                double depth = pointTable[i * 3 + 2];
                const Position position = OpenCPN::fromSimpleMercator(easting,
                                                                      northing,
                                                                      m_extent.center());
                multiPointGeometry.push_back(S57::PointGeometry { position, depth });
            }
            if (s57 != nullptr) {
                s57->setMultiPointGeometry(multiPointGeometry);
            }
            break;
        }

        case VECTOR_EDGE_NODE_TABLE_RECORD: {
            unsigned char *buf = getBuffer(record.record_length - sizeof(OSENC_Record_Base));
            if (!fpx.Read(buf, record.record_length - sizeof(OSENC_Record_Base))) {
                return false;
            }
            uint8_t *pRun = (uint8_t *)buf;
            int nCount = *(int *)pRun;
            pRun += sizeof(int);

            for (int i = 0; i < nCount; i++) {
                unsigned int featureIndex = *(int *)pRun;
                pRun += sizeof(int);

                int pointCount = *(int *)pRun;
                pRun += sizeof(int);

                float *pPoints = NULL;
                if (pointCount) {
                    pPoints = (float *)malloc(pointCount * 2 * sizeof(float));
                    std::memcpy(pPoints, pRun, pointCount * 2 * sizeof(float));
                }
                pRun += pointCount * 2 * sizeof(float);
                S57::VectorEdge vectorEdge;
                std::vector<Position> positions;
                Position ref = m_extent.center();
                for (int i = 0; i < pointCount; i++) {
                    positions.push_back(OpenCPN::fromSimpleMercator(pPoints[i * 2],
                                                                    pPoints[i * 2 + 1],
                                                                    ref));
                }
                free(pPoints);
                vectorEdge.setPositions(positions);
                // The index should be representable by a positive 32 bit integer
                if (featureIndex > 0x7FFFFFFF) {
                    std::cerr << "Found bad vector edge index: " << featureIndex << std::endl;
                } else {
                    vectorEdges[featureIndex] = std::move(vectorEdge);
                }
            }
            break;
        }

        case VECTOR_EDGE_NODE_TABLE_EXT_RECORD: {
            unsigned char *buf = getBuffer(record.record_length - sizeof(OSENC_Record_Base));
            if (!fpx.Read(buf, record.record_length - sizeof(OSENC_Record_Base))) {
                return false;
            }
            break;
        }

        case VECTOR_CONNECTED_NODE_TABLE_RECORD: {
            long unsigned int buf_len = record.record_length - sizeof(OSENC_Record_Base);
            unsigned char *buf = getBuffer(buf_len);
            if (!fpx.Read(buf, buf_len)) {
                return false;
            }

            uint8_t *pRun = (uint8_t *)buf;
            int nCount = *(int *)pRun;
            pRun += sizeof(int);

            for (int i = 0; i < nCount; i++) {
                unsigned int featureIndex = *(int *)pRun;

                pRun += sizeof(int);
                float *pt = (float *)pRun;
                pt++;

                float *pPoint = (float *)malloc(2 * sizeof(float));
                memcpy(pPoint, pRun, 2 * sizeof(float));
                pRun += 2 * sizeof(float);

                // Could be simplified to just Position now
                S57::ConnectedNode connectedNode(OpenCPN::fromSimpleMercator(pPoint[0],
                                                                             pPoint[1],
                                                                             m_extent.center()));
                free(pPoint); // Fix this in a proper way
                connectedNodes[featureIndex] = connectedNode;
            }
            break;
        }

        case CELL_TXTDSC_INFO_FILE_RECORD: {
            unsigned char *buf = getBuffer(record.record_length - sizeof(OSENC_Record_Base));
            if (!fpx.Read(buf, record.record_length - sizeof(OSENC_Record_Base))) {
                return false;
            }
            break;
        }

        default:
            if (record.record_type == 0) {
                return true;
            } else {
                std::cerr << "Unrecognized Record: " << record.record_type << std::endl;
                return false;
            }
            break;
        }
    }
    fpx.close();
    return true;
}

uint8_t *ChartFile::skipTessellationData(_OSENC_AreaGeometry_Record_Payload *record)
{
    unsigned int n_TriPrim = record->triprim_count;
    int nContours = record->contour_count;

    //  Get a pointer to the payload
    void *payLoad = &record->payLoad;

    //  The point count array is the first element in the payload, length is known
    int *contour_pointcount_array_run = (int *)payLoad;
    contour_pointcount_array_run += nContours;

    uint8_t *pPayloadRun = (uint8_t *)contour_pointcount_array_run; //Points to the start of the triangle primitives

    for (unsigned int i = 0; i < n_TriPrim; i++) {
        pPayloadRun++;
        const int nvert = *(uint32_t *)pPayloadRun;
        const long unsigned int byte_size = nvert * 2 * sizeof(float);
        pPayloadRun += sizeof(uint32_t);
        pPayloadRun += 4 * sizeof(double);
        pPayloadRun += byte_size;
    }

    return pPayloadRun;
}

unsigned char *ChartFile::getBuffer(size_t length)
{
    if (length > bufferSize) {
        pBuffer = (unsigned char *)realloc(pBuffer, length * 2);
        bufferSize = length * 2;
    }
    return pBuffer;
}
