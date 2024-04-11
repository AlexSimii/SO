pentru a compila in dir SO se poate rula

>make clean

>make

in urma rulari o sa fie creat un executabil "filesaver.elf" in directorul SO


>./executabil dir1 dir2 ... 

path_cache_file_snapshoturi o sa fie setat pe FileSaverCache

sau

flags 

-d dir1, dir2 ...

-o path_cache_file_snapshoturi

ex: executabil -o path_cache_file_snapshoturi -d dir1 dir2 ...

    executabil -d dir1 dir2 ... -o path_cache_file_snapshoturi

    >./filesaver.elf -d test_dir -o FileSaverCache
    >./filesaver.elf -d /home/alex/Desktop/SO/test_dir -o FileSaverCache
    >./filesaver.elf -d /home/alex/Desktop/SO/test_dir/dir1 /home/alex/Desktop/SO/test_dir/dir2 /home/alex/Desktop/SO/test_dir/dir3 /home/alex/Desktop/SO/test_dir/dir4 -o FileSaverCache

