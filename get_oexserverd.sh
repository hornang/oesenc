#!/bin/bash

set -e

export LC_ALL=en_US.utf8

case "$OSTYPE" in
  linux*)
	PLUGIN_REGEXP="(?<=<tarball-url>).*o-charts.*-debian-x86_64.*?(?=</tarball-url>)"
  ;;
  msys*|cygwin)
	PLUGIN_REGEXP="(?<=<tarball-url>).*o-charts.*msvc.*win32.*?(?=</tarball-url>)"
  ;;
  *)
	echo Unknown system $OSTYPE >&2
	exit 1
  ;;
esac

PLUGIN_XML_FILE="ocpn-plugins.xml"
curl --fail https://raw.githubusercontent.com/OpenCPN/plugins/master/ocpn-plugins.xml > $PLUGIN_XML_FILE
TARBALL_URL=$(grep -m 1 -o -P "$PLUGIN_REGEXP" $PLUGIN_XML_FILE)
curl --fail $TARBALL_URL -o tarball.tar.gz
tar xf tarball.tar.gz
EXTRACTED_DIR=$(find -type d -iname o-charts*)
OEXSERVERD_PATH=$(find $EXTRACTED_DIR -type f -iname "oexserverd*")
echo "Location of executable: $OEXSERVERD_PATH"
PATHFILE=./oexserverd_path.txt
echo "$(pwd)/$(dirname $OEXSERVERD_PATH)" > $PATHFILE
echo "PATH written to: $PATHFILE "
