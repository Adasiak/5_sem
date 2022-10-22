while getopts "a" option
do
case $option in
	a) echo file_num=$(ls -a  | grep -v '/$' | wc -l);exit;;
esac
done

echo file_num=$(ls  -1 | grep -v '/$' | wc -l);