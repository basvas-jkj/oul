# OUL
Oul je nástroj podporující kompozitní vývoj projektu. Umožňuje v projektu vytvářet komponenty, 
nahrávat je na server nebo je naopak stahovat. Díky tomu je nsadné stejnou komponentu používat
a uprovovat ve více různých projektech.

## použité nástroje a knihovny
- CMake                                             (verze 3.30.2)
- Ninja                                             (verze 1.12.1)
- clang                                             (verze 20.1.2)
- vcpkg                                             (verze 2025-02-11-bec4296bf5289dc9ce83b4f5095943e44162f9c2) 
- wildcards                                         (verze 1.4.0)
- yaml-cpp                                          (verze 0.8.0)
- nlohmann-json                                     (verze 3.11.3)
- boost-algorithm                                   (verze 1.86.0)
- boost-filesystem                                  (verze 1.86.0)
- boost-process                                     (verze 1.86.0)
- boost-test                                        (verze 1.86.0)

## získání zdrojového kódu a sestavení
- git clone https://github.com/basvas-jkj/oul.git
- cmake --preset [PRESET]
- cd  build/[PRESET]
- cmake --build .
- oul # spuštění programu
- ctest # spuštění testů

(PRESET=debug nebo PRESET=release)