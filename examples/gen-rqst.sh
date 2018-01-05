#!/bin/dash

# The script has one argument - timeout( in sec. )
i=0

if [ $# -eq 0 ]; then
  timeout=1
else
  timeout=$1
fi

while [ $i -lt "100" ]
do
  echo $i
  sleep $timeout
  ./client
  i=`expr $i + 1`
done
