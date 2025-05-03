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
- oul    # spuštění programu
- ctest  # spuštění testů

(PRESET=debug nebo PRESET=release)

## používání programu
oul add [component] [group] [files...]        Přidá soubory [files] do skupiny [group] komponenty [component].
oul create [component]                        Vytvoří komponentu [component].
    -w=[location]                                 ve složce [location] (výchozí: [component])                                              
oul delete [component]                        Odstraní komponentu [component] z projektu.
oul exclude [component] [group] [files...]    Vyčlení soubory [files] ze skupiny [group] komponenty [component].
oul group add [component] [group]             Vytvoří skupinu [group] v komponentě [component].
oul group remove [component] [group]          Odstraní skupinu [group] z komponenty [component].
oul help                                      Vypíše text nápovědy.
oul init                                      Vytvoří konfiguraci projektu.
oul install [component]                       Stáhne komponentu [component] ze serveru.
    -as=[name]                                    uloží ji pod jménem [name] (výchozí: [component]) 
    -url=[url]                                    ze serveru daného [url] (výchozí: dle konfigurace projektu)
    -w=[location]                                 do složky [location] (výchozí: [component])                                              
oul list                                      Vypíše seznam komponent v projektu.
oul remove [component] [group] [files...]     Odebere soubory [files] ze skupiny [group] komponenty [component].
oul rename [previous name] [new name]         Přejmenuje komponantu [previous name] na [new name].
oul upload [component]                        Nahraje komponentu [component] na server.
    -as=[name]                                    nahraje ji pod jménem [name] (výchozí: [component])
    -url=[url]                                    na server daný [url] (výchozí: dle konfigurace projektu)

Příkazy add, remove, rename, create ani delete nevytváří, nemažou ani nepřejmenovávají sobory.
Pouze příkaz add kontroluje existenci přidávaných souborů.
Změna se projeví pouze v konfiguraci projektu.