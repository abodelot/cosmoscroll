#!/bin/bash

clang-format $(find src -type f -name "*.cpp") --dry-run --Werror
