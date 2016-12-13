# Tesztelési dokumentáció

Ez a dokumentum útmutatóként szolgál a Blokus alkalmazás teszteléséhez. Az alább leírt lépések sorrendje a program állapotátmenetein alapul, így ezen útmutatót követve az esetlegesen előforduló programhibák többsége felfedezhető.

## Formák beolvasása fájlból

Amennyiben létezik a formákat tartalmazó fájl, feltételezhetjük, hogy helyes a formátuma. Egyéb esetben a programnak ki kell lépnie. A formák listába történő beolvasásának helyessége ellenőrizhető grafikus úton vagy breakpointok használatával.

## Játékmenet

Egy játék létrehozásakor figyelni kell az automatikusan létrehozott játékosok helyes inicializációjára, valamint arra, hogy a grafikus elemek - a játéktábla és az eredményjelző - helyesen jelenjenek meg. Alapállapotban a játéktábla összes cellája szürke színű, az eredményjelzőn pedig tetszőleges sorrendben kell megjelennie ugyanazon pontszámoknak.

### Elem kiválasztása

A játékosok a még elhelyezhető elemeik közül szabadon válogathatnak. Új elem kiválasztásakor meg kell változnia a játéktábla áttetsző rétegének, valamint bizonyos esetekben az egérmutatónak is. A játéktábla áttetsző rétege szimbolizálja a kiválasztott elemet annak elhelyezése előtt, az egérmutató pedig visszajelzést ad egy elem elhelyezhetősége felől.

### Kiválasztott elem tükrözése/forgatása

A kiválasztott elem tükrözési/forgatási mechanizmusa matematikai alapokon nyugszik, és szemrevételezéssel ellenőrizhető.

### Kiválasztott elem mozgatása a játéktáblán

A kiválasztott elemnek követnie kell a kurzort, miközben celláinak igazodniuk kell a játéktábla celláihoz. Az elemek cellái nem vághatók le: amennyiben a kurzor a játéktáblán kívülre kerül, a kiválasztott elem érintheti a játéktábla szélét, de azon nem haladhat túl.

### Kiválasztott elem elhelyezése a játéktáblán

A kiválasztott elem csak a játékszabályt követő módon helyezhető el a játéktáblán, azaz az elem legalább egy sarkának érintkeznie kell egy azonos színű cellával anélkül, hogy azonos színű elemek élei érintkeznének. Minden játékos elsőként elhelyezett elemének érintkeznie kell a játéktábla sarkainak egyikével.

### Passzolás

Amennyiben egy játékos egyetlen elemet sem tud elhelyezni a játéktáblán, lehetősége van passzolni. Ekkor az őt soron követő játékostól kell folytatódnia a játéknak, amíg nem minden játékos passzol egymást követően. Ha egy játékos az összes elemét elhelyezte a játéktáblán, automatikusan passzol a játék végéig.

## Pontozás

A játékosok pontszámai folyamatosan követhetők a játéktábla melletti eredményjelzőn. A pontszámoknak csökkenő sorrendben kell megjelenniük.
