# spring-libretro launcher

Launch any command through retroarch

# Install

Copy `spring_libretro.so` to a unique name like `spring_htop_libretro.so` in your cores directory.

Also copy/create `spring_htop_libretro.info` and fill in the details including extensions for matching.

Add your command to execute. {path} will be replaced with the file chosen.

    command = "ls {path}"
