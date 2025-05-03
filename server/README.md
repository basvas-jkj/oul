# OUL server
Oul server je ukázkový server pro demonstraci fungování programu oul.

## použité nástroje
- npm             (verze 10.9.0)
- node.js         (verze 22.5.1)

## získání zdrojového kódu a sestavení
- git clone https://github.com/basvas-jkj/oul.git
- cd server
- npm run build                      # sestavení projektu
- npm run server                     # spuštění serveru na výchozím portu 8080
- node build/index.js -port=[port]   # spuštění serveru na portu [port]