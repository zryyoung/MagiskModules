MODDIR=${0%/*}

# 如果已经在后台运行，先杀掉再重启
pid=$(pgrep -f "$MODDIR/CCleaner")
if [ -n "$pid" ]; then
    echo "[*] Found running CCleaner (PID: $pid), killing..."
    kill "$pid"
fi
echo "[*] Running CCleaner ..."
# 直接调用 service.sh
sh "$MODDIR/service.sh" &

echo "[*] ✅ Success."
exit 0