#!/bin/bash

rm -rf ./bin
mkdir ./bin


javac -d ./bin -cp .:./test_toolchains/junit-4.13.2.jar:./test_toolchains/hamcrest-core-1.3.jar ./src/cryptoex/test/*.java ./src/cryptoex/logic/*.java
jar cfm tests.jar manifest.mf ./bin/*