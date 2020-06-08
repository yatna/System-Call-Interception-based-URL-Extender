#!bin/bash
gcc -o urlextend -w pt2.c
URL='http://yatna.github.io'
#URL=' https://www.cs.stonybrook.edu'
GREEN='\033[0;32m'
NC='\033[0m' # No Color


echo "------------------------------------------------------------------------------------------------------"
echo "   TC 1   "
echo "------------------------------------------------------------------------------------------------------"
echo "urlextend wc" $URL
echo "$GREEN"
./urlextend wc $URL
echo "${NC}"


echo "------------------------------------------------------------------------------------------------------"
echo "   TC 2   "
echo "------------------------------------------------------------------------------------------------------"
echo "urlextend head" $URL
echo "${GREEN}"
./urlextend head $URL
echo "${NC}"



echo "------------------------------------------------------------------------------------------------------"
echo "   TC 3   "
echo "------------------------------------------------------------------------------------------------------"
echo "urlextend tail" $URL
echo "${GREEN}"
./urlextend tail $URL
echo "${NC}"


echo "------------------------------------------------------------------------------------------------------"
echo "   TC 4  "
echo "------------------------------------------------------------------------------------------------------"
echo "urlextend cp" $URL "_copy_url"
echo "${GREEN}"
rm -f _copy_url
./urlextend cp $URL _copy_url
echo "A copy named _copy_url must have been made in the current directory"
echo "${NC}"



echo "------------------------------------------------------------------------------------------------------"
echo "   TC 5  "
echo "------------------------------------------------------------------------------------------------------"
echo "urlextend cat | head -20" $URL
echo "${GREEN}"
./urlextend cat $URL | head -20
echo "${NC}"


echo "------------------------------------------------------------------------------------------------------"
echo "   TC 6   "
echo "------------------------------------------------------------------------------------------------------"
echo "urlextend stat" $URL
echo "${GREEN}"
./urlextend stat $URL
echo "${NC}"



echo "------------------------------------------------------------------------------------------------------"
echo "   TC 7   			Works fine with non-url files as well"
echo "------------------------------------------------------------------------------------------------------"
echo "urlextend cat" /etc/ssh/ssh_config "| head"
echo "${GREEN}"
./urlextend cat /etc/ssh/ssh_config | head
echo "${NC}"



echo "------------------------------------------------------------------------------------------------------"
echo "   TC 8  "
echo "------------------------------------------------------------------------------------------------------"
echo "urlextend grep meta" $URL 
echo "${GREEN}"
./urlextend grep meta $URL
echo "${NC}"




echo "------------------------------------------------------------------------------------------------------"
echo "   TC 9  "
echo "------------------------------------------------------------------------------------------------------"
echo "urlextend sed 's/meta/_________REPLACED_META_WITH_PETA/'" $URL "| head -20"
echo "${GREEN}"
./urlextend sed 's/meta/_________REPLACED_META_WITH_PETA/' $URL | head -20
echo "${NC}"



echo "------------------------------------------------------------------------------------------------------"
echo "   TC 10  "
echo "------------------------------------------------------------------------------------------------------"
echo "urlextend ln " $URL "link_file"
echo "${GREEN}"
rm -f link_file
./urlextend ln $URL link_file
echo "${NC}"




echo "------------------------------------------------------------------------------------------------------"
echo "   TC 10  "
echo "------------------------------------------------------------------------------------------------------"
echo "urlextend ln --symbolic" $URL "symlink_file"
echo "${GREEN}"
echo "Since after creating symbolic link to temp file the tempfile is deleted, symlink_file will have no content. However, you can stat it to see that it is a symbolic link."
rm -f symlink_file
./urlextend ln --symbolic $URL symlink_file
echo "${NC}"
