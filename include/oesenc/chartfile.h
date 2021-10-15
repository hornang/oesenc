#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

#include "deptharea.h"
#include "rect.h"
#include "s57.h"
#include "sounding.h"

#include "oesenc_export.h"

#pragma pack(push, 1)

typedef struct _OSENC_Record_Base
{
    uint16_t record_type;
    uint32_t record_length;
} OSENC_Record_Base;

typedef struct _OSENC_Record
{
    uint16_t record_type;
    uint32_t record_length;
    unsigned char payload;
} OSENC_Record;

typedef struct _OSENC_Feature_Identification_Record_Base
{
    uint16_t record_type;
    uint32_t record_length;
    uint16_t feature_type_code;
    uint16_t feature_ID;
    uint8_t feature_primitive;
} OSENC_Feature_Identification_Record_Base;

typedef struct _OSENC_Feature_Identification_Record_Payload
{
    uint16_t feature_type_code;
    uint16_t feature_ID;
    uint8_t feature_primitive;
} OSENC_Feature_Identification_Record_Payload;

typedef struct _OSENC_Attribute_Record_Base
{
    uint16_t record_type;
    uint32_t record_length;
    uint16_t attribute_type;
    unsigned char attribute_value_type;
} OSENC_Attribute_Record_Base;

typedef struct _OSENC_Attribute_Record
{
    uint16_t record_type;
    uint32_t record_length;
    uint16_t attribute_type;
    unsigned char attribute_value_type;
    void *payload;
} OSENC_Attribute_Record;

typedef struct _OSENC_Attribute_Record_Payload
{
    uint16_t attribute_type_code;
    unsigned char attribute_value_type;

    union {
        uint32_t attribute_value_int;
        double attribute_value_double;
        char *attribute_value_char_ptr;
    };
} OSENC_Attribute_Record_Payload;

typedef struct _OSENC_TXTDSCInfo_Record_Base
{
    uint16_t record_type;
    uint32_t record_length;
    uint32_t file_name_length;
    uint32_t content_length;
} OSENC_TXTDSCInfo_Record_Base;

typedef struct _OSENC_TXTDSCInfo_Record
{
    uint16_t record_type;
    uint32_t record_length;
    uint32_t file_name_length;
    uint32_t content_length;
    void *payload;
} OSENC_TXTDSCInfo_Record;

typedef struct _OSENC_TXTDSCInfo_Record_Payload
{
    uint32_t file_name_length;
    uint32_t content_length;
    char *data;
} OSENC_TXTDSCInfo_Record_Payload;

typedef struct _OSENC_PointGeometry_Record
{
    uint16_t record_type;
    uint32_t record_length;
    double lat;
    double lon;
} OSENC_PointGeometry_Record;

typedef struct _OSENC_PointGeometry_Record_Payload
{
    double lat;
    double lon;
} OSENC_PointGeometry_Record_Payload;

typedef struct _OSENC_MultipointGeometry_Record_Base
{
    uint16_t record_type;
    uint32_t record_length;
    double extent_s_lat;
    double extent_n_lat;
    double extent_w_lon;
    double extent_e_lon;
    uint32_t point_count;
} OSENC_MultipointGeometry_Record_Base;

typedef struct _OSENC_MultipointGeometry_Record_Payload
{
    double extent_s_lat;
    double extent_n_lat;
    double extent_w_lon;
    double extent_e_lon;
    uint32_t point_count;
    void *payLoad;
} OSENC_MultipointGeometry_Record_Payload;

typedef struct _OSENC_LineGeometry_Record_Base
{
    uint16_t record_type;
    uint32_t record_length;
    double extent_s_lat;
    double extent_n_lat;
    double extent_w_lon;
    double extent_e_lon;
    uint32_t edgeVector_count;
} OSENC_LineGeometry_Record_Base;

typedef struct _OSENC_LineGeometry_Record_Payload
{
    double extent_s_lat;
    double extent_n_lat;
    double extent_w_lon;
    double extent_e_lon;
    uint32_t edgeVector_count;
    void *payLoad;
} OSENC_LineGeometry_Record_Payload;

typedef struct _OSENC_AreaGeometry_Record_Base
{
    uint16_t record_type;
    uint32_t record_length;
    double extent_s_lat;
    double extent_n_lat;
    double extent_w_lon;
    double extent_e_lon;
    uint32_t contour_count;
    uint32_t triprim_count;
    uint32_t edgeVector_count;
} OSENC_AreaGeometry_Record_Base;

typedef struct _OSENC_AreaGeometry_Record_Payload
{
    double extent_s_lat;
    double extent_n_lat;
    double extent_w_lon;
    double extent_e_lon;
    uint32_t contour_count;
    uint32_t triprim_count;
    uint32_t edgeVector_count;
    void *payLoad;
} OSENC_AreaGeometry_Record_Payload;

typedef struct _OSENC_AreaGeometryExt_Record_Base
{
    uint16_t record_type;
    uint32_t record_length;
    double extent_s_lat;
    double extent_n_lat;
    double extent_w_lon;
    double extent_e_lon;
    uint32_t contour_count;
    uint32_t triprim_count;
    uint32_t edgeVector_count;
    double scaleFactor;
} OSENC_AreaGeometryExt_Record_Base;

typedef struct _OSENC_AreaGeometryExt_Record_Payload
{
    double extent_s_lat;
    double extent_n_lat;
    double extent_w_lon;
    double extent_e_lon;
    uint32_t contour_count;
    uint32_t triprim_count;
    uint32_t edgeVector_count;
    double scaleFactor;
    void *payLoad;
} OSENC_AreaGeometryExt_Record_Payload;

typedef struct _OSENC_VET_Record
{
    uint16_t record_type;
    uint32_t record_length;
    unsigned char payload;
} OSENC_VET_Record;

typedef struct _OSENC_VET_Record_Base
{
    uint16_t record_type;
    uint32_t record_length;
} OSENC_VET_Record_Base;

typedef struct _OSENC_VET_RecordExt
{
    uint16_t record_type;
    uint32_t record_length;
    double scaleFactor;
    unsigned char payload;
} OSENC_VET_RecordExt;

typedef struct _OSENC_VET_RecordExt_Base
{
    uint16_t record_type;
    uint32_t record_length;
    double scaleFactor;
} OSENC_VET_RecordExt_Base;

typedef struct _OSENC_VET_RecordExt_Payload
{
    double scaleFactor;
    void *payLoad;
} OSENC_VET_RecordExt_Payload;

typedef struct _OSENC_VCT_Record
{
    uint16_t record_type;
    uint32_t record_length;
    unsigned char payload;
} OSENC_VCT_Record;

typedef struct _OSENC_VCT_Record_Base
{
    uint16_t record_type;
    uint32_t record_length;
} OSENC_VCT_Record_Base;

typedef struct _OSENC_VCT_RecordExt
{
    uint16_t record_type;
    uint32_t record_length;
    double scaleFactor;
    unsigned char payload;
} OSENC_VCT_RecordExt;

typedef struct _OSENC_VCT_RecordExt_Base
{
    uint16_t record_type;
    uint32_t record_length;
    double scaleFactor;
} OSENC_VCT_RecordExt_Base;

typedef struct _OSENC_VCT_RecordExt_Payload
{
    double scaleFactor;
    void *payLoad;
} OSENC_VCT_RecordExt_Payload;

typedef struct _OSENC_COVR_Record
{
    uint16_t record_type;
    uint32_t record_length;
    unsigned char payload;
} _OSENC_COVR_Record;

typedef struct _OSENC_COVR_Record_Base
{
    uint16_t record_type;
    uint32_t record_length;
} _OSENC_COVR_Record_Base;

typedef struct _OSENC_COVR_Record_Payload
{
    uint32_t point_count;
    float point_array;
} _OSENC_COVR_Record_Payload;

typedef struct _OSENC_NOCOVR_Record
{
    uint16_t record_type;
    uint32_t record_length;
    unsigned char payload;
} _OSENC_NOCOVR_Record;

typedef struct _OSENC_NOCOVR_Record_Base
{
    uint16_t record_type;
    uint32_t record_length;
} _OSENC_NOCOVR_Record_Base;

typedef struct _OSENC_NOCOVR_Record_Payload
{
    uint32_t point_count;
    float point_array;
} _OSENC_NOCOVR_Record_Payload;

typedef struct _OSENC_EXTENT_Record
{
    uint16_t record_type;
    uint32_t record_length;
    double extent_sw_lat;
    double extent_sw_lon;
    double extent_nw_lat;
    double extent_nw_lon;
    double extent_ne_lat;
    double extent_ne_lon;
    double extent_se_lat;
    double extent_se_lon;
} _OSENC_EXTENT_Record;

typedef struct _OSENC_EXTENT_Record_Payload
{
    double extent_sw_lat;
    double extent_sw_lon;
    double extent_nw_lat;
    double extent_nw_lon;
    double extent_ne_lat;
    double extent_ne_lon;
    double extent_se_lat;
    double extent_se_lon;
} _OSENC_EXTENT_Record_Payload;

#pragma pack(pop)

class ChartFile
{
public:
    OESENC_EXPORT ChartFile();
    OESENC_EXPORT ~ChartFile();
    OESENC_EXPORT bool read(const std::string &file);
    OESENC_EXPORT int getReadVersion() { return m_senc_file_read_version; }
    OESENC_EXPORT int getSencReadVersion() { return m_senc_file_read_version; }
    OESENC_EXPORT int getSENCReadLastUpdate() { return m_read_last_applied_update; }
    OESENC_EXPORT int getSENCReadScale() { return m_Chart_Scale; }
    OESENC_EXPORT std::string getUpdateDate() { return m_LastUpdateDate; }
    OESENC_EXPORT std::string getBaseDate() { return m_sdate000; }
    OESENC_EXPORT std::string getReadName() { return m_Name; }
    OESENC_EXPORT std::string getSoundingsDatumString() { return m_SoundingDatum; }

    OESENC_EXPORT Rect &getReadExtent() { return m_extent; }
    OESENC_EXPORT Rect extent() const { return m_extent; }

    OESENC_EXPORT void InitializePersistentBuffer(void);
    OESENC_EXPORT unsigned char *getBuffer(size_t length);
    OESENC_EXPORT const std::vector<std::shared_ptr<S57>> &s57() const { return m_s57; }

private:
    bool ingest200(const std::string &senc_file_name,
                   std::vector<std::shared_ptr<S57>> &s57Vector,
                   std::unordered_map<int, std::shared_ptr<S57::VectorEdge>> &vectorEdges,
                   std::unordered_map<int, S57::ConnectedNode> &connectedNodes);
    uint8_t *skipTessellationData(_OSENC_AreaGeometry_Record_Payload *record);
    std::string m_Name;
    std::string m_sdate000;
    std::string m_LastUpdateDate;
    std::string m_key;
    std::string m_SoundingDatum;

    int m_Chart_Scale = 0;
    int m_senc_file_read_version = 0;
    int m_senc_file_create_version = 0;
    int m_nGeoRecords = 0;
    int m_last_applied_update = 0;
    int m_read_last_applied_update = 0;
    int m_native_scale = 0;
    int m_read_base = 0;

    unsigned char *pBuffer = nullptr;
    size_t bufferSize = 0;
    Rect m_extent;
    std::vector<std::shared_ptr<S57>> m_s57;
};
