MODDIR=${0%/*}

# 如果已经在后台运行，先杀掉再重启
pid=$(pgrep -f "$MODDIR/CCleaner")
if [ -n "$pid" ]; then
    echo "[*] Found running CCleaner (PID: $pid), killing..."
    kill "$pid"
fi
echo "[*] Running CCleaner ..."
# 直接调用 service.sh
"$MODDIR/service.sh" &

echo "[*] ✅ Success."
echo "[*] CCleaner1"
echo "[*] 选择清理空文件，文件夹以及应用缓存"
"$MODDIR/CCleaner1"
sleep 1
exit 0