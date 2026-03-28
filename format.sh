#!/bin/bash

cd /c/Users/vasst/matfyz/ročníkový_projekt/oul
find src -type f -exec clang-format -style=file -i {} \;
find test -type f -exec clang-format -style=file -i {} \;
