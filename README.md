# oesenc

A standalone C++ library to read OpenCPN's oesenc chart.

## Motivation

This software is shared to make nautical chart data available for research and experimentation. You can use this software to get such data until hydrological offices starts to share this kind of data to the public.

oesenc charts must be decrypted first using: [oesenc-export](https://github.com/hornang/oesenc-export)

Please support o-charts.org by purchasing your charts there. Do not share unencrypted chart data with third-parties.

## Features

For now just the basic data is read from the file: land areas, depth areas, and soundings. Land and depth area is polygon data and also the most complicated data to extract so adding support for new data less complicated.

## Design

The library is intended to have few dependencies. C++ was chosen for this project so it could be easily integrated to other software and also deployed to most platforms.

## License

Parts of this code is derivative works of [OpenCPN](https://github.com/OpenCPN/OpenCPN) and [oesenc_pi](https://github.com/bdbcat/oesenc_pi). This code is licenced under GPLv3 or any later version.

