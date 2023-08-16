#include <filesystem>
#include <fstream>

#include <oesenc/chartfile.h>
#include <oesenc/keylistreader.h>
#include <oesenc/servercontrol.h>
#include <oesenc/serverreader.h>

using namespace std;
using namespace oesenc;

int main(int argc, char *argv[])
{
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

    return 0;
}
