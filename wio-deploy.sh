#!/bin/bash

if ! type "npm" > /dev/null; then
    echo "wio-deploy FAILED: npm not installed"
    exit 1;
fi

if ! type "wio" > /dev/null; then
    echo "wio-deploy FAILED: wio not installed"
    exit 1;
fi

if ! type "cmake" > /dev/null; then
    echo "wio-deploy FAILED: cmake not installed"
    exit 1;
fi

echo "wio-deploy INFO: initializing deploy package"

# Create wlib-wio
rm -rf wlib-wio
wio create pkg wlib-wio ALL --platform ALL

# Replace with template wio.yml
#rm wlib-wio/wio.yml
#cp wio-test.yml wlib-wio/
#mv wlib-wio/wio-test.yml wlib-wio/wio.yml

rm -rf wlib-wio/src
rm -rf wlib-wio/include
rm -rf wlib-wio/tests

mkdir wlib-wio/src
mkdir wlib-wio/include
mkdir wlib-wio/tests

# Copy over WLib files
echo "Copying WLib files into wio package"
cp -r lib/wlib/wlib wlib-wio/src/
cp -r lib/wlib/include/wlib wlib-wio/include

# Generate a dummy test
printf "#include <wlib/array_list>\n\nint main(int argc, char *argv[]) {\n    wlp::array_list<int> int_list(12);\n    int_list.push_back(12);\n}\n" > wlib-wio/tests/main.cpp

# Move all `src` headers to `include`
headers=$(find wlib-wio/src/ -type f -name "*.h")
relative=$(realpath --relative-to="wlib-wio/src" $headers)


IFS=$'\n' read -rd '' -a arr_headers <<<"$headers"
IFS=$'\n' read -rd '' -a arr_relative <<<"$relative"

len_headers=${#arr_headers[@]}
len_relative=${#arr_relative[@]}

for (( i = 0; i < ${len_headers}; i++ )); do
    mkdir --parents "wlib-wio/include/"$(dirname "${arr_relative[$i]}")
    mv ${arr_headers[$i]} "wlib-wio/include/"${arr_relative[$i]}
done

rm -rf wlib-stl/include
rm -rf wlib-stl/src

cp -r wlib-wio/include wlib-stl/include
cp -r wlib-wio/src wlib-stl/src

rm -rf wlib-wio

# Do a build test
#cd wlib-wio
#wio build --verbose
#wio clean
#cd ..

# Copy the deploy config
#rm -rf wlib-wio/wio.yml
#cp wio.yml wlib-wio/

# Do a deploy
#cd wlib-wio
#wio pac publish

