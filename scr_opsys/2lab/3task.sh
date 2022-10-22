for arg in $@; 
do
    file=$(find /home/ -type f -name "$arg*")
    dic=$(find /home/ -name "$arg*")
    # echo $file
    if test $file ;
    then 
        echo "Plik Istnieje" 
    elif test $dic;
    then
        file_num=$(ls  "$dic" | grep -v '/$' | wc -l);
        if test $file_num > 0;
        then
            echo $file_num;
        else
            echo "Katalog jest pusty"
        fi  
    else
        echo "Plik nie istnieje"
    
    fi
done

