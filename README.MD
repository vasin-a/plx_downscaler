# Downscaler

## Как собрать
1. Генерируем VS2017 проект`cmake -S . -B build-x64 -A x64 -G "Visual Studio 17"`
1. Собираем из студии или через `cmake --build build-x64 --config=Release`
1. Опционально копируем бинари в папку `bin` через `cmake --install build-x64 --prefix .`

## Как запустить

### Из Visual Studio
1. Выбираем таргет `host` активным для запуска.
1. В настройках таргета в качестве `Working Dir` указываем `$(SolutionDir)`
1. В аргументах командной строки таргета указываем `--src=../img --dst=./gauss --method=gauss --scale=0.1`

### Из консоли
1. Скачиваем готовый билд: https://github.com/vasin-a/plx_downscaler/releases
1. Распаковывем в любую удобную папку.
1. Запускаем любой из интерактивных bat скриптов.

### Доступные аргументы командной строки
- `--src=./img` путь до каталога с исходными изображениями. По-умолчанию `./img`.
- `--dst=./out` путь до целевого каталога с выходными изображениями. По-умолчанию `./out`.
- `--gamma=2.2` величина гамма-фактора при начальной линеаризации и финальной гамма-коррекции. По-умолчанию `2.2` (sRGB).
- `--scale=0.5` коэффициент масштабирования в интервале `(0..1]`.
- `--lod-bias=0.0` аналог [GL_TEXTURE_LOD_BIAS](https://registry.khronos.org/OpenGL-Refpages/gl4/html/glTexParameter.xhtml) для mip-based методов даунскейла (см. ниже). По-умолчанию `0`.
- `--src-alpha=ignore` режим конвертации RGB каналов после линеаризации исходного изображения (см. параметр `gamma`). По-умолчанию `ignore`. Доступные опции:
    - `i` / `ignore` / `leave` RGB каналы не изменяются.
    - `p` / `mult` / `premultiply` RGB каналы умножаются на альфу.
    - `u` / `div` / `unpremultiply` RGB каналы делятся на альфу.
- `--dst-alpha=ignore` режим конвертации RGB каналов перед гамма-коррекцией (см. параметр `gamma`). По-умолчанию `ignore`. Доступные опции такие же как в `--src-alpha`.
- `--method=mip` метод даунскейла. Доступные опции:
    - `gauss` обычное гауссово размытие с последющим [сабсэмплингом](https://maelfabien.github.io/computervision/cv_3/#i-image-sub-sampling).
    - `lanczos` то же что и `gauss`, только kernel свёртки [3-lobe Lanczos](https://en.wikipedia.org/wiki/Lanczos_resampling#Lanczos_kernel), что даёт чуть больше резкости.
    - `mip` аналог [GL_LINEAR_MIPMAP_LINEAR](https://registry.khronos.org/OpenGL-Refpages/es3.0/html/glTexParameter.xhtml). Классическая [трилинейная изотропная фильтрация](https://en.wikipedia.org/wiki/Mipmap). Mip-уровни строятся box-blur'ом.
    - `mip-floor` аналог [GL_LINEAR_MIPMAP_NEAREST](https://registry.khronos.org/OpenGL-Refpages/es3.0/html/glTexParameter.xhtml), только вместо округления (`round`) до ближайшего mip-уровня используется функция `floor`, то есть выбирается более детальный слой. Даёт больше резкости, но возможно проявление [алиасинга](https://svi.nl/AntiAliasing).
