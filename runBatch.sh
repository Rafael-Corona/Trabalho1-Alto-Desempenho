
MaxThreads=16
targetName="./studentspar-"
end="t"
for (( i=2 ; i<=$MaxThreads ; i = i * 2)); 
do
    for n in {1..50}; 
    do
        print=$targetName$i$end
        $print
    done

done
