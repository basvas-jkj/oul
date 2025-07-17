# OUL
Oul je nástroj podporující kompozitní vývoj projektu. Umožňuje v projektu vytvářet komponenty, 
nahrávat je na server nebo je naopak stahovat. Díky tomu je snadné stejnou komponentu používat
a upravovat ve více různých projektech.

## použité nástroje a knihovny
Nástroje:
- CMake                                             (verze 3.30.2)
- Ninja                                             (verze 1.12.1)
- clang                                             (verze 20.1.2)
- vcpkg                                             (tag 2025-02-14)

Knihovny:
- yaml-cpp                                          (verze 0.8.0)
- nlohmann-json                                     (verze 3.11.3)
- boost-algorithm                                   (verze 1.86.0)
- boost-filesystem                                  (verze 1.86.0)
- boost-process                                     (verze 1.86.0)
- boost-test                                        (verze 1.86.0)
- boost-url                                         (verze 1.86.0)
- boost-dll                                         (verze 1.86.0)

Knihovny není nutné instalovat manuálně, jejich stažení a překlad zajišťuje CMake automaticky.
Je však nutné nastavit proměnnou prostředí VCPKG_ROOT tak, aby ukládala cestu ke kořenové složce programu vcpkg.

## získání zdrojového kódu a sestavení
- git clone https://github.com/basvas-jkj/oul.git
- cmake --preset [PRESET]
- cd  build/[PRESET]
- cmake --build .
- oul    # spuštění programu
- ctest  # spuštění testů

(PRESET=debug nebo PRESET=release)

## používání programu
<dl>
<dt>oul add [component] [group] [files...]</dt>
<dd>Přidá soubory [files] do skupiny [group] komponenty [component].</dd>

<dt> oul create [component]</dt>
<dd>Vytvoří komponentu [component].
    <dl>
    <dt><small>-w=[location]</small></dt>
    <dd>ve složce [location] (výchozí: [component])</dd>
    </dl>
</dd>

<dt>oul delete [component]</dt>
<dd>Odstraní komponentu [component] z projektu.</dd>

<dt>oul exclude [component] [group] [files...]</dt>
<dd>Vyčlení soubory [files] ze skupiny [group] komponenty [component].</dd>

<dt>oul group add [component] [group]</dt>
<dd>Vytvoří skupinu [group] v komponentě [component].</dd>

<dt>oul group remove [component] [group]</dt>
<dd>Odstraní skupinu [group] z komponenty [component].</dd>

<dt>oul help</dt>
<dd>Vypíše text nápovědy.</dd>

<dt>oul init</dt>
<dd>Vytvoří konfiguraci projektu.</dd>

<dt>oul install [component]</dt>
<dd>Stáhne komponentu [component] ze serveru.
    <dl>
    <dt><small>-as=[name]</small></dt>
    <dd>uloží ji pod jménem [name] (výchozí: [component])</dd>
    <dt><small>-url=[url]</small></dt>
    <dd>ze serveru daného [url] (výchozí: dle konfigurace projektu)</dd>
    <dt><small>-w=[location]</small></dt>
    <dd>do složky [location] (výchozí: [component])</dd>
    </dl>
</dd>

<dt>oul list</dt>
<dd>Vypíše seznam komponent v projektu.</dd>

<dt>oul list -url=[url]</dt>
<dd>Stáhne a vypíše seznam komponent ze serveru [url].</dd>

<dt> oul remove [component] [group] [files...]</dt>
<dd>Odebere soubory [files] ze skupiny [group] komponenty [component].</dd>

<dt>oul rename [previous name] [new name]</dt>
<dd>Přejmenuje komponantu [previous name] na [new name].</dd>

<dt>oul upload [component]</dt>
<dd>Nahraje komponentu [component] na server.
    <dl>
    <dt><small>-as=[name]</small></dt>
    <dd>nahraje ji pod jménem [name] (výchozí: [component])</dd>
    <dt><small>-url=[url]</small></dt>
    <dd>na server daný [url] (výchozí: dle konfigurace projektu)</dd>
    </dl>
</dd>
</dl>
Příkazy add, remove, rename, create ani delete nevytváří, nemažou ani nepřejmenovávají sobory
ani nekontrolují jejich existenci.
Změna se projeví pouze v konfiguraci projektu.
