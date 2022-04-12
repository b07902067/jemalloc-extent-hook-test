
# Record file
touch mmap_test_out
touch malloc_test_out

> mmap_test_out
> malloc_test_out
> malloc_test_no_mmap_out

# compile file and output executable
make

# run mmap test
for mmap_size in {2..16..2}; do
    echo "$mmap_size" >> mmap_test_out
    ./mem_test $mmap_size 8  | grep "Vm" >> mmap_test_out
    echo "" >> mmap_test_out
done

# run mallocx test
for malloc_size in {1..16}; do
    echo "$malloc_size" >> malloc_test_out
    ./mem_test 8 $malloc_size  | grep "Vm" >> malloc_test_out
    echo "" >> malloc_test_out

    echo "$malloc_size" >> malloc_test_no_mmap_out
    ./mem_test_no_mmap $malloc_size  | grep "Vm" >> malloc_test_no_mmap_out
    echo "" >> malloc_test_no_mmap_out
done

# visualize
python3 src/mem_test.py