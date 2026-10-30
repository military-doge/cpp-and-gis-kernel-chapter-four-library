# cpp-and-gis-kernel-chapter-four-library

用 C++17 实现的 OGC 几何内核小库，并提供 Linux / Windows 下的静态库与动态库产物。

## 功能

参照 OGC Simple Features，实现三个基础几何要素：

| 类型 | 维度 | 主要能力 |
| --- | --- | --- |
| `cppgis::Point` | 0 维 | 坐标、两点距离、比较、WKT |
| `cppgis::LineString` | 1 维 | 点列、闭合/有效性判断、长度、WKT |
| `cppgis::Polygon` | 2 维 | 外环 + 内环(洞)、面积、周长、有效性、WKT |

- 面积使用鞋带公式，外环面积减去内环面积。
- WKT 输出遵循规范，例如 `POINT (3 4)`、`LINESTRING (0 0, 3 4)`、`POLYGON ((...), (...))`。

## 目录结构

```
.
├── include/                    头文件
│   ├── point.h
│   ├── line_string.h
│   └── polygon.h
├── src/                        实现
│   ├── point.cpp
│   ├── line_string.cpp
│   └── polygon.cpp
├── main.cpp                    调用示例
└── dist/                       预编译产物
    ├── linux-x86_64/
    │   ├── libcppgis.a         静态库
    │   └── libcppgis.so        动态库
    └── windows-x86_64/
        ├── libcppgis.lib       静态库
        ├── 生成动态库.lib       导入库
        ├── 生成动态库.dll       动态库
        └── include/            对外头文件
```

## 调用示例

本仓库根目录的 `main.cpp` 即调用示例，静态链接和动态链接共用同一份代码：

```cpp
#include <iostream>

#include "line_string.h"
#include "point.h"
#include "polygon.h"

int main()
{
    // Point
    const cppgis::Point a(0.0, 0.0);
    const cppgis::Point b(3.0, 4.0);
    std::cout << b.to_wkt() << '\n';
    std::cout << "  distance(a, b) = " << a.distance_to(b) << '\n';
    std::cout << "  a == b ? " << std::boolalpha << (a == b) << '\n';

    // LineString
    const cppgis::LineString line{{0.0, 0.0}, {3.0, 4.0}, {6.0, 0.0}};
    std::cout << line.to_wkt() << '\n';
    std::cout << "  points = " << line.num_points()
              << ", length = " << line.length()
              << ", closed = " << line.is_closed() << '\n';

    // Polygon：外环 + 一个洞
    const cppgis::Polygon polygon(
        cppgis::LineString{{0.0, 0.0}, {10.0, 0.0}, {10.0, 10.0},
                           {0.0, 10.0}, {0.0, 0.0}},
        {cppgis::LineString{{2.0, 2.0}, {2.0, 4.0}, {4.0, 4.0},
                            {4.0, 2.0}, {2.0, 2.0}}});
    std::cout << polygon.to_wkt() << '\n';
    std::cout << "  area = " << polygon.area()
              << ", perimeter = " << polygon.perimeter()
              << ", valid = " << polygon.is_valid() << '\n';

    return 0;
}
```

### Linux：静态链接

```bash
g++ -std=c++17 -Iinclude main.cpp dist/linux-x86_64/libcppgis.a -o demo_static
./demo_static
```

静态链接后库代码被拷进可执行文件，运行时不需要 `libcppgis.so`：

```bash
ldd demo_static | grep cppgis   # 无输出
```

### Linux：动态链接

```bash
g++ -std=c++17 -Iinclude main.cpp \
    -Ldist/linux-x86_64 -lcppgis \
    -Wl,-rpath,'$ORIGIN' -o demo_shared
cp dist/linux-x86_64/libcppgis.so .
./demo_shared
```

- `-Ldist/linux-x86_64`：库搜索目录
- `-lcppgis`：链接 `libcppgis.so`
- `-Wl,-rpath,'$ORIGIN'`：运行时到可执行文件所在目录找 `.so`

也可以不用 rpath，改成运行时指定：

```bash
LD_LIBRARY_PATH=dist/linux-x86_64 ./demo_shared
```

确认运行时的依赖：

```bash
ldd demo_shared | grep cppgis
# libcppgis.so => .../dist/linux-x86_64/libcppgis.so
```

### 预期输出

Linux / Windows、静态 / 动态链接的输出都一致：

```
POINT (3 4)
  distance(a, b) = 5
  a == b ? false
LINESTRING (0 0, 3 4, 6 0)
  points = 3, length = 10, closed = false
POLYGON ((0 0, 10 0, 10 10, 0 10, 0 0), (2 2, 2 4, 4 4, 4 2, 2 2))
  area = 96, perimeter = 48, valid = true
```

### Windows (MSVC)：命令行

在 **x64 Native Tools Command Prompt for VS** 中执行。代码与 Linux 完全相同。
因为预编译产物是 Debug 版，客户端也要用 `/MDd` 保持运行库一致。

静态链接：

```bat
cl /nologo /std:c++17 /EHsc /MDd /I "dist\windows-x86_64\include" ^
   main.cpp "dist\windows-x86_64\libcppgis.lib" /Fe:demo_static.exe
demo_static.exe
```

动态链接：

```bat
cl /nologo /std:c++17 /EHsc /MDd /I "dist\windows-x86_64\include" ^
   main.cpp "dist\windows-x86_64\生成动态库.lib" /Fe:demo_shared.exe
copy /Y "dist\windows-x86_64\生成动态库.dll" .
demo_shared.exe
```

- `/I`：头文件目录
- `/MDd`：使用 Debug 版 C 运行库，与预编译库匹配
- `生成动态库.lib` 只是导入库，程序运行还需要 `生成动态库.dll`；放在 exe 同目录即可
- 可用 `dumpbin /dependents demo_shared.exe` 查看运行时依赖

### Windows (MSVC)：Visual Studio 项目

| 设置 | 静态链接 | 动态链接 |
| --- | --- | --- |
| 附加包含目录 | `dist\windows-x86_64\include` | `dist\windows-x86_64\include` |
| 链接器 → 附加依赖项 | `libcppgis.lib` | `生成动态库.lib` |
| 运行时文件 | 不需要 | `生成动态库.dll` 与 exe 同目录 |
| 配置 | Debug x64 | Debug x64 |

## 自行编译（Linux）

```bash
g++ -std=c++17 -Wall -Wextra -Iinclude -c src/point.cpp      -o point.o
g++ -std=c++17 -Wall -Wextra -Iinclude -c src/line_string.cpp -o line_string.o
g++ -std=c++17 -Wall -Wextra -Iinclude -c src/polygon.cpp     -o polygon.o
ar rcs libcppgis.a point.o line_string.o polygon.o

g++ -std=c++17 -Wall -Wextra -Iinclude -fPIC -c src/point.cpp       -o point.pic.o
g++ -std=c++17 -Wall -Wextra -Iinclude -fPIC -c src/line_string.cpp -o line_string.pic.o
g++ -std=c++17 -Wall -Wextra -Iinclude -fPIC -c src/polygon.cpp     -o polygon.pic.o
g++ -shared -o libcppgis.so point.pic.o line_string.pic.o polygon.pic.o
```

## 注意

- 当前 Windows 产物是 **x64 Debug**，运行依赖 `MSVCP140D.dll`、`VCRUNTIME140D.dll`、`ucrtbased.dll`，需要安装 Visual Studio 运行库。
- 导入库中记录的 DLL 文件名是 `生成动态库.dll`，运行时不要单独改名，否则会提示找不到模块。
- 预编译的 Windows 动态库已导出全部 public 符号，可直接供 MSVC 程序链接；如需从源码重建 DLL，请为接口添加 `__declspec(dllexport)`（或使用 `.def` 文件），否则生成的 DLL 不会导出符号。
