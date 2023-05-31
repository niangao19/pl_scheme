lcov -c -i -d ./ -o init.info
./testpl.out
lcov -c  -d ./ -o cover.info
lcov -a init.info -a cover.info -o total.info
lcov --remove total.info '/src/include/*' '/user/bin/*' -o result.info
genhtml -o results result.info

