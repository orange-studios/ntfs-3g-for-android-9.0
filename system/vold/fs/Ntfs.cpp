/*
 * Copyright (C) 2008 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/mount.h>
#include <sys/wait.h>
#include <linux/fs.h>
#include <sys/ioctl.h>

#include <linux/kdev_t.h>

#include <android-base/logging.h>
#include <android-base/stringprintf.h>
#include <selinux/selinux.h>

#include <logwrap/logwrap.h>

#include "Ntfs.h"
#include "Utils.h"
#include "VoldUtil.h"

using android::base::StringPrintf;

namespace android {
namespace vold {
namespace ntfs {

static const char* kMkfsPath = "/system/bin/mkfs.ntfs";
static const char* kFsckPath = "/system/bin/fsck.ntfs";
static const char* kMtfsPath = "/system/bin/mount.ntfs";

bool IsSupported() {
    return access(kMkfsPath, X_OK) == 0
            && access(kFsckPath, X_OK) == 0;
//            && IsFilesystemSupported("ntfs");
}

status_t Check(const std::string& source) {
    int pass = 1;
    int rc = 0;
	const char* c_source = source.c_str();
    do {
        std::vector<std::string> cmd;
        cmd.push_back(kFsckPath);
        cmd.push_back("-n");
        cmd.push_back(c_source);

        rc = ForkExecvp(cmd);

        if (rc < 0) {
            LOG(ERROR) << "Filesystem check failed due to logwrap error";
            errno = EIO;
            return -1;
        }
		LOG(ERROR) << "orangeyang>>>>>> NTFS Filesystem check rc = " << rc << ")";
		
        switch(rc) {
        case 0:
            LOG(INFO) << "NTFS Filesystem check completed OK";
            return 0;

        case 1:
            LOG(ERROR) << "NTFS Filesystem check failed (not a NTFS filesystem)";
            errno = ENODATA;
            return -1;

        default:
            LOG(ERROR) << "NTFS Filesystem check failed (unknown exit code " << rc << ")";
            errno = EIO;
            return -1;
        }
    } while (0);

    return 0;
}

status_t Mount(const std::string& source, const std::string& target, bool ro,
        bool remount, bool executable, int ownerUid, int ownerGid, int permMask,
        bool createLost) {
	const char* c_source = source.c_str();
    const char* c_target = target.c_str();

    auto mountData = android::base::StringPrintf(
            "utf8,uid=%d,gid=%d,fmask=%o,dmask=%o,shortname=mixed",
            ownerUid, ownerGid, permMask, permMask);
	
	std::vector<std::string> cmd;
	cmd.push_back(kMtfsPath);
	cmd.push_back(c_source);
	cmd.push_back(c_target);
	cmd.push_back("-o");
	cmd.push_back(mountData);

	int rc = ForkExecvp(cmd);
	if (rc < 0) {
        LOG(ERROR) << "Filesystem format failed due to logwrap error";
        errno = EIO;
        return -1;
    }

    if (rc == 0) {
        LOG(INFO) << "NTFS Filesystem formatted OK";
        return 0;
    } else {
        LOG(ERROR) << "NTFS Format failed (unknown exit code " << rc << ")";
        errno = EIO;
        return -1;
    }

    return rc;
}

status_t Format(const std::string& source, unsigned long numSectors) {
    std::vector<std::string> cmd;
    cmd.push_back(kMkfsPath);
    cmd.push_back("-f");
    cmd.push_back("-L");
    cmd.push_back("android");

    if (numSectors) {
        cmd.push_back("-s");
        cmd.push_back(StringPrintf("%lu", numSectors));
    }

    cmd.push_back(source);

    int rc = ForkExecvp(cmd);
    if (rc < 0) {
        LOG(ERROR) << "Filesystem format failed due to logwrap error";
        errno = EIO;
        return -1;
    }

    if (rc == 0) {
        LOG(INFO) << "NTFS Filesystem formatted OK";
        return 0;
    } else {
        LOG(ERROR) << "NTFS Format failed (unknown exit code " << rc << ")";
        errno = EIO;
        return -1;
    }
    return 0;
}

}  // namespace ntfs
}  // namespace vold
}  // namespace android
