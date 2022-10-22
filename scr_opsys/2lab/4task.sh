while getopts "n:m:a:" option
do 
case $option in
	n) n=$optarg;;
	m) maska=$optarg;;
	a) nazwatar=$optarg;;
	*) wcho "Usage: $0 -n timesincemodification -m mask -a archname"; exit 1;;
esac
done

if test -z "$n";
then
	echo "Brakuje liczby dni od modyfikacji. Podaj liczbd dni"
	read n
fi

if test -z "$maska";
then
	echo 'Brakuje maski. Podaj maske np("*.?")'
	read maska
fi

if test -z "$nazwatar";
then
	echo 'Brakuje nazwy archiwum. Podaj nazwe (np.xxx.tar)'
	read nazwatar
fi

file=$(find /home/ -type f -name "*.$maska" -mtime -$n)
file_count=$(find /home/ -type f -name "*.$maska" -mtime -$n | wc -l)

if test $file_count >0;
then
	tar -cvf $nazwatar $file
else
	echo "Brak pliku"
fi