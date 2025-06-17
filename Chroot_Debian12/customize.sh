SKIPUNZIP=1

unzip -o "$ZIPFILE" 'rootfs.tar.gz' -d "$TMPDIR"
unzip -o "$ZIPFILE" 'config.prop' -d "$MODPATH"
unzip -o "$ZIPFILE" 'module.prop' -d "$MODPATH"
unzip -o "$ZIPFILE" 'post-fs-data.sh' -d "$MODPATH"
unzip -o "$ZIPFILE" 'service.sh' -d "$MODPATH"
unzip -o "$ZIPFILE" 'uninstall.sh' -d "$MODPATH"

source "$MODPATH/config.prop"

if [ -d "${MOUNT_POINT}/etc" ]; then
   umount ${MOUNT_POINT}/sys
   umount ${MOUNT_POINT}/proc
   umount ${MOUNT_POINT}/tmp
   umount ${MOUNT_POINT}/dev
   umount ${MOUNT_POINT}/dev/shm
   umount ${MOUNT_POINT}/dev/pts
   mv -f ${MOUNT_POINT} ${MOUNT_POINT}_old 2>/dev/null
fi

[ -d "$MOUNT_POINT" ] || mkdir -p $MOUNT_POINT
tar xzf $TMPDIR/rootfs.tar.gz -C $MOUNT_POINT
