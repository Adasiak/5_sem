trap "echo trapped" {1..15}

while true 
do
    echo -n "$$ "
    date
    sleep 1
done
