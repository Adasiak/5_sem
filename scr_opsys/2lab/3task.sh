for arg in $@; 
do
    dic=$(find /home/ -type d -name "$arg*");
    file=$(find /home/ -type f -name "$arg*");
    
    if test $dic;
    then
        file_num=$(ls  "$dic" | grep -v '/$' | wc -l);
        if test $file_num > 0;
        then
                echo "Katalog ma:" $file_num "elementów";
        else
                echo "Katalog jest pusty"
        fi 
    elif test $file ;
    then 
        echo "Plik Istnieje" 
    else
        echo "Katalog/plik nie istnieje"
    
    fi
done

