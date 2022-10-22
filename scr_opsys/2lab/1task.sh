export LC_ALL

dzien=$(date +%w)

if test $dzien -gt 5 ; then
    echo "Weekend"
else
    echo "Dzien roboczy"
fi
