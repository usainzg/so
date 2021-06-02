rm -rf ./binaries/*

for file in ./files/*
do
    ./binarize $file
    echo $file " => binarized." 
done

mv *.elf ./binaries/
