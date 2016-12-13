# blokus

## Specifikáció

### Játékmenet

Körökre osztott Blokus játék megvalósítása több játékos számára. A játékosok célja, hogy minél több különböző formájú elemet el tudjanak helyezni a játéktáblán úgy, hogy azok a lehető legnagyobb területet foglalják el.

A játék elején minden játékos kiválaszt egy különböző színt, majd a tábla egy-egy sarkában egymás után kezdenek. Minden játékosnak úgy kell elhelyeznie azonos színű elemeit, hogy azok egymás sarkával érintkezzenek. Azonos színű elemek élei nem érintkezhetnek, azonban különböző színű elemek élei igen.

Egy játékos számára akkor ér véget a játék, amikor már nem tud lerakni több elemet. Pontszáma a játéktáblán elhelyezett négyzetek számán alapszik. Amennyiben a játékos minden elemét el tudta helyezni a játéktáblán, 15 bónusz pontot kap, és ha a remete (1x1-es) elemet rakta le utoljára, további 5 bónusz pontot kap. A játék győztese a legmagasabb pontszámmal rendelkező játékos.

A játék végén kiírásra kerülnek a játékosok pontszámai és helyezései csökkenő sorrendben. A játékban elérhető elemek egy külső szövegfájlból töltődnek be.

### Irányítás

A játékosok egy körön belül fejenként legfeljebb egy elemet rakhatnak le az egér és a billentyűzet segítségével:

- **← (A), → (D)**: Elem kiválasztása (balra/jobbra navigálás az elhelyezhető elemek között)
- **↑ (W), ↓ (S)**: Kiválasztott elem tükrözése az X/Y tengelyre
- **Space**: Kiválasztott elem elforgatása 90°-kal
- **Egér mozgatása**: Kiválasztott elem mozgatása a játéktáblán
- **Bal egérgomb megnyomása**: Kiválasztott elem elhelyezése a játéktáblán, amennyiben lehetséges
- **P**: Passzolás

### Lehetőségek a program továbbfejlesztésére

Az alábbiakban a programmal kapcsolatos ötletek találhatók, amelyek megvalósítása opcionális.

- Menürendszer elkészítése
- 2 játékos mód

## Projekt futtatható fájljainak generálása

```
mkdir build
cd build
cmake ..
cmake --build .
```

## Dokumentáció generálása

```
doxygen
```
