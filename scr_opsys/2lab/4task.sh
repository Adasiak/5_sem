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

tar -cjf $nazwatar "find . -type f -name "$maska" -mtime -$n"