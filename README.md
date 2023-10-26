# oesenc nautical chart reading library

A standalone C++ library to read OpenCPN's oesenc/oesu charts.

## Motivation

This software is shared to make nautical chart data available for research and experimentation. You can use this software to get such data until hydrographic offices starts to share this kind of data to the public. Please support o-charts.org by purchasing your charts there. Do not share unencrypted chart data with third-parties.

## Usage

The library is built with CMake and may be included in your project as a submodule and a `add_directory` CMake stateemnt. Or you can build and install it to a directory using CMake _install_. And then use `find_package` to link it to your project.

Actual decryption of chart data happens in an executable called `oexserverd` which is bundled with OpenCPN's "o-charts" plugin. This library expects to find `oexserverd` in the `PATH` at runtime. The executable is normally found at `C:\Users\Username\AppData\Local\opencpn\plugins\oexserverd.exe` on Windows or at `~/.local/bin/oexserverd` on Linux.

OpenCPN chart files come in two flavours: _oesenc_ and _oesu_ which is supersedes the former. They differ primarily in how the decryption key is provided. The decryption key is provided with the chart archive. This key plus information generated inside the proprietary `oexserverd` executable is what that decrypts the chart files.

For oesu files the oexserverd server requires a unique key fetched from the XML file in the chart directory. For oesenc files the key is the same for all chart files in the same chart archive and it is fetched from the `Chartinfo.txt` file in the chart archive.

The work of communicating with oexserverd and loading the correct key is handled by the library. The key loading part will vary a little bit depending on whether you are reading oesu or oesenc. Please use the examples below as a reference.

[Example reading of oesu file](examples/oesu_example.cpp):

```cpp
filesystem::path chartFileName = "C:/Path/to/chartdir/chartfile.oesu";

unordered_map<string, string> keys = KeyListReader::readOesuKeys(chartFileName.parent_path().string());

ServerControl serverControl;
if (!serverControl.waitUntilReady()) {
    cerr << "Failed to establish server connection\n";
}

unique_ptr<istream> inputStream = ServerReader::openOesu(serverControl.pipeName(),
                                                         chartFileName.string(),
                                                         keys[chartFileName.stem().string()]);
if (!inputStream) {
    cerr << "Failed to open " << chartFileName << "\n";
    return -1;
}

ChartFile chartFile(*inputStream);
chartFile.read();
vector<S57> s57Objects = chartFile.s57();
cout << "Num objects " << s57Objects.size() << "\n";
```

[Example reading oesenc file](examples/oesenc_example.cpp):

```cpp
filesystem::path chartFileName = "C:/Path/to/chartdir/chartfile.oesenc";

string key = KeyListReader::readOesencKey(chartFileName.parent_path().string());

ServerControl serverControl;
if (!serverControl.waitUntilReady()) {
    cerr << "Failed to establish server connection\n";
    return -1;
}

unique_ptr<istream> inputStream = ServerReader::openOesenc(serverControl.pipeName(),
                                                           chartFileName.string(),
                                                           key);
if (!inputStream) {
    cerr << "Failed to open " << chartFileName << "\n";
    return -1;
}

ChartFile chartFile(*inputStream);
chartFile.read();
vector<S57> s57Objects = chartFile.s57();
cout << "Num objects " << s57Objects.size() << "\n";
```

## S-57 decoding

Many of the most common S57 types and attributes are available to be read. See [s-57.com](http://www.s-57.com/) to lookup the types you see in the code. In addition to attributes each S57 object may have geometric properties associated with it (a position, multiple positions with a value, lines and polygons).

This library is not responsible for parsing the information into a strictly typed data model (classes with strictly typed fields and so on). This is often application specific and is up to the user for library.

## License

Parts of this code is derivative works of [OpenCPN](https://github.com/OpenCPN/OpenCPN) and [oesenc_pi](https://github.com/bdbcat/oesenc_pi). This code is licenced under GPLv3 or any later version.

