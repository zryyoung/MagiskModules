MODDIR=${0%/*}

# 修复执行权限
chmod 755 "$MODDIR/CCleaner"
chmod 755 "$MODDIR/service.sh"
chmod 755 "$MODDIR/action.sh"

# 启动清理服务（假如你是循环执行 ELF）
# "$MODDIR/CCleaner"
# "$MODDIR/CCleaner" &

# 启动守护进程
exec $MODDIR/CCleaner &