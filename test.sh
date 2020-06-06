#!bin/sh
echo ""
echo "Process type BATCH"
time ./a.out 0&
echo ""
echo "Process type NORMAL"
time ./a.out 1 &
echo ""
echo "Process type INTERACTIVE"
time ./a.out 2 &
echo ""
