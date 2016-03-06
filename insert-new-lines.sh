#!/bin/bash

#adds new lines to all source files
printf "Adding new lines to all source files...\n"
find . -name "*.cc" -print0 | xargs -0 -I {} sed -i '$a\' {}
printf "Done.\n\n"

printf "Adding new lines to all header files...\n"
#adds new lines to all header files
find . -name "*.h" -print0 | xargs -0 -I {} sed -i '$a\' {}
printf "Done.\n"
