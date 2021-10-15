# oesenc

A standalone C++ library to read OpenCPN's oesenc chart.

## Motivation

This software is shared to make nautical chart data available for research and experimentation. You can use this software to get such data until hydrological offices starts to share this kind of data to the public.

oesenc charts must be decrypted first using: [oesenc-export](https://github.com/hornang/oesenc-export)

Please support o-charts.org by purchasing your charts there. Do not share unencrypted chart data with third-parties.

## Features

Many of the most common S57 types and attributes are available to be read. See [s-57.com](http://www.s-57.com/) to lookup the types you see in the code. In addition to attributes each S57 object may have geometric properties associated with it (a position, multiple positions with a value, lines and polygons). There are getters to fetch these geometric properties, but be aware that only a few object types have S57::buildGeometry() activated.

This library is not responsible for parsing the information into a strictly typed data model (classes with strictly typed fields and so on). This is often application specific and is up to the user for library.

## Design

The library is intended to have few dependencies. C++ was chosen for this project so it could be easily integrated to other software and also deployed to most platforms.

## License

Parts of this code is derivative works of [OpenCPN](https://github.com/OpenCPN/OpenCPN) and [oesenc_pi](https://github.com/bdbcat/oesenc_pi). This code is licenced under GPLv3 or any later version.

