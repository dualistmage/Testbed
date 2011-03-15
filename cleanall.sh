for dir in cpp java programming-contest script shell; do
    cd $dir
    find . -type d -exec make clean -C {} \;
    cd ../
done
    
