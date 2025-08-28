mkdir -p  ./Tests/results
./hazelc ./Tests/LocalFunctions/ --print-llvm > ./Tests/results/local_functions.txt
./hazelc ./Tests/orderOfOperations/ --print-llvm > ./Tests/results/order_of_op.txt
