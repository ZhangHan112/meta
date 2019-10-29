make clean all 

echo ""
echo -n "跑幾個Iterations： "
read iter
echo -n "螞蟻數量： "
read num
echo -n "費洛蒙蒸發率： "
read rate
echo -n "要跑幾個Runs： "
read RUN

INDEX=1
while [ $INDEX -le $RUN ]
do
    
    echo -e "第 $INDEX run,   ACO\c"
    ./TSPACO $iter 10 0.98 1 1 > output1.dat #Iterations, AntNumbers, Evaporation, Runs, algo 
    echo -e "-->完成!,   MMAS-->\c"
    ./TSPACO $iter 10 0.98 1 2 > output2.dat #Iterations, AntNumbers, Evaporation, Runs, algo 
    echo "完成！"
    gnuplot pic.gp
    rm -f output1.dat output2.dat
    ((INDEX++))
done

