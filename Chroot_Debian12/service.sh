MODDIR=${0%/*}
source ${MODDIR}/config.prop

# 确保 /run/sshd 目录存在并且权限正确
chroot "${MOUNT_POINT}" /bin/bash -c 'mkdir -p /run/sshd && chown root:root /run/sshd && chmod 755 /run/sshd'

# 启动 SSH 服务
chroot "${MOUNT_POINT}" /bin/bash -c "/usr/sbin/sshd -p ${SSH_PORT}" &

while true; do
  if [ -d "/storage/emulated/0/Android/data" ]; then
    break
  fi
  sleep 3
done

if [ -n "$SDCARD" ]; then
   [ -d "${MOUNT_POINT}${SDCARD}" ] || mkdir -p ${MOUNT_POINT}${SDCARD}
   mount /storage/emulated/0 ${MOUNT_POINT}${SDCARD}
fi