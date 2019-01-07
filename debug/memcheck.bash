#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

INPUT=$1
LOGNO=${2:-0}
RAND=$RANDOM
LOGFILE="$DIR/logs/memcheck.log"
TMP_LOGFILE="$LOGFILE.$RAND"

valgrind \
  --leak-check=full \
  --log-file=$TMP_LOGFILE \
  $DIR/../bin/vm \
  "$INPUT"

echo -e "$(date)\n--------\nARGUMENTS: ${INPUT:-"NONE"}\n-------" >> $LOGFILE
cat $TMP_LOGFILE >> $LOGFILE
echo -e "" >> $LOGFILE

rm $TMP_LOGFILE
