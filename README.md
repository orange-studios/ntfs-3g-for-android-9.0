移植ntfs-3g到Android 9.0

移植涉及到的相关目录：
external/ntfs-3g
system/vold

mmm external/ntfs-3g后生成的相关文件：
out/target/product/<project>/system/bin/fsck.ntfs
out/target/product/<project>/system/bin/mount.ntfs
out/target/product/<project>/system/bin/mkfs.ntfs
out/target/product/<project>/system/lib64/libntfs-3g.so
out/target/product/<project>/system/lib/libntfs-3g.so

重点关注代码：
system/vold/fs/Vfat.cpp
system/vold/model/PublicVolume.cpp

made by orangeyang @5/9/2020
