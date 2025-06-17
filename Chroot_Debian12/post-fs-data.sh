MODDIR=${0%/*}
source ${MODDIR}/config.prop

# 以下mount规则取自Linux Deploy   --致敬
mountfs(){
mount -t sysfs sys ${MOUNT_POINT}/sys
mount -t proc proc ${MOUNT_POINT}/proc
mount -t tmpfs tmpfs ${MOUNT_POINT}/tmp
mount -o bind /dev ${MOUNT_POINT}/dev

[ -d "/dev/shm" ] || mkdir -p /dev/shm
[ -d "/dev/pts" ] || mkdir -p /dev/pts
[ -d "/dev/net" ] || mkdir -p /dev/net

mount -o rw,nosuid,nodev,mode=1777 -t tmpfs tmpfs /dev/shm
mount -o bind /dev/shm "${MOUNT_POINT}/dev/shm"
mount -o rw,nosuid,noexec,gid=5,mode=620,ptmxmode=000 -t devpts devpts /dev/pts
mount -o bind /dev/pts "${MOUNT_POINT}/dev/pts"

[ -e "/dev/fd" ] || ln -s /proc/self/fd /dev/
[ -e "/dev/stdin" ] || ln -s /proc/self/fd/0 /dev/stdin
[ -e "/dev/stdout" ] || ln -s /proc/self/fd/1 /dev/stdout
[ -e "/dev/stderr" ] || ln -s /proc/self/fd/2 /dev/stderr
[ -e "/dev/tty0" ] || ln -s /dev/null /dev/tty0
[ -e "/dev/net/tun" ] || mknod /dev/net/tun c 10 200
}

mountfs

#清除更新前的旧rootfs
rm -rf ${MOUNT_POINT}_old 2>/dev/null