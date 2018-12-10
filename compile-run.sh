if g++ src/*.cpp -I include -std=c++14 -o main -fopenmp; then
    bash run.sh
else
    echo "Did not compile";
fi
