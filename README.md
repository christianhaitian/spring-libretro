# spring-libretro launcher

Launch any command through retroarch

# Install

do a `make` to generate the spring_libretro.so file.

Copy `spring_libretro.so` to a unique name like `spring_htop_libretro.so` in your cores directory.

Also copy/create `spring_htop_libretro.info` and fill in the details including extensions for matching.

Add your command to execute. %s will be replaced with the file chosen.

    command = "ls %s"

You can also just run `./build.sh` and it will generate a .so file for each .info file available in the directory
To quickly clean the directory of generate .so files, you can do `./build.sh clean`
