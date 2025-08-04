# 进入上一级目录
cd ..

# 获取上一级目录的路径
root_dir=$(pwd)

# 源代码目录列表，使用空格分隔每个目录
src_dirs=(
    $(pwd)/src
)

for src_dir in "${src_dirs[@]}"; do
    # 使用clang-format格式化代码
    find "$src_dir" -type f \( -iname '*.h' -o -iname '*.c' -o -iname '*.cpp' -o -iname '*.hpp' \) | while read file
    do
        # 输出正在格式化的文件名
        echo "Formatting $file"
        
        # 执行clang-format命令格式化文件
        "$root_dir/tools/clang-format.exe" -i "$file"
    done
done

# 等待用户输入，超时设置为10秒，调试使用
#read -n 1 -t 10