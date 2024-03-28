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
- `--gamma=2.2` величина гамма-фактора при гамма коррекции. По-умолчанию `2.2` (sRGB).
- `--method=lanczos` метод даунскейла. Доступные варианты:
    - `gauss` обычное гауссово размытие с последющим [сабсэмплингом](https://maelfabien.github.io/computervision/cv_3/#i-image-sub-sampling).
    - `lanczos` то же что и `gauss`, только kernel свёртки [3-lobe Lanczos](https://en.wikipedia.org/wiki/Lanczos_resampling#Lanczos_kernel), что даёт чуть больше резкости.
    - `mip` классическая [трилинейная интерполяция по mip-уровням](https://en.wikipedia.org/wiki/Mipmap), она же [пирамида Гаусса](https://en.wikipedia.org/wiki/Pyramid_(image_processing)#Gaussian_pyramid). Mip уровни строятся box blur'ом вместо гаусса.
- `scale=0.5` коэффициент масштабирования в интервале (0..1].
