if g++ src/*.cpp -I include -Ofast -std=c++17 -o main -fopenmp; then
    bash run.sh
else
    echo "Did not compile";
fi
