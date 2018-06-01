#!/bin/sh

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

echo "wio-deploy INFO: initializing deploy folder"


