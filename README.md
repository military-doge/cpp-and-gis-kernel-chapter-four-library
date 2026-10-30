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

## 使用

### Linux

静态链接：

```bash
g++ -std=c++17 -Iinclude main.cpp dist/linux-x86_64/libcppgis.a -o app
```

动态链接：

```bash
g++ -std=c++17 -Iinclude main.cpp \
    -Ldist/linux-x86_64 -lcppgis -Wl,-rpath,'$ORIGIN' -o app
```

运行时 `libcppgis.so` 需要放在可执行文件同目录（或加入 `LD_LIBRARY_PATH`）。

### Windows (MSVC)

- 附加包含目录：`dist/windows-x86_64/include`
- 静态链接：链接器 → 附加依赖项填 `libcppgis.lib`
- 动态链接：链接器 → 附加依赖项填 `生成动态库.lib`，并把 `生成动态库.dll` 放到 exe 同目录

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
