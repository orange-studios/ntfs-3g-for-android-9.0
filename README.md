移植ntfs-3g到Android 9.0


移植涉及到的相关目录：
external/ntfs-3g
system/vold


mmm external/ntfs-3g后生成的相关文件:
out/target/product/<project>/system/bin/fsck.ntfs
out/target/product/<project>/system/bin/mount.ntfs
out/target/product/<project>/system/bin/mkfs.ntfs
out/target/product/<project>/system/lib64/libntfs-3g.so
out/target/product/<project>/system/lib/libntfs-3g.so


重点关注代码：
system/vold/fs/Vfat.cpp
system/vold/model/PublicVolume.cpp


修改记录：
Changes not staged for commit:
  (use "git add <file>..." to update what will be committed)
  (use "git checkout -- <file>..." to discard changes in working directory)

	modified:   Android.bp
	modified:   main.cpp
	modified:   model/PublicVolume.cpp

Untracked files:
  (use "git add <file>..." to include in what will be committed)

	fs/Ntfs.cpp
	fs/Ntfs.h


注意：
在项目mk文件中添加PRODUCT_PACKAGES += \
 fsck.ntfs \
 libntfs-3g \
 mount.ntfs \
 mkfs.ntfs



made by orangeyang @5/9/2020
