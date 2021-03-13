# Elevators_system

Cel projektu :
  Implementacja systemu zarządzającego windami.

Plan rozwoju projektu :
  1. Zdefiniowanie wymagań projektowych
  2. Zbudowanie algorytmu zarządzającego kolejnością obsługi pasażerów
  3. Rozważenie projektowania pod kątem przeniesienia aplikacji na platformę embedded
  4. Implementacja rozwiązań
  5. Testy

Zaimplementowane rozwiązania :
  - winda aktualizuje swoją drogę na każdym piętrze
  - winda jeździ w jednym kierunku, aż do dojechania do maksymalnego lub minimalnego piętra, na które któryś z pasażerów zdecydował się jechać, a następnie zawraca jeśli ktoś chciałby jechać w przeciwnym kierunku lub czeka na kolejne zawołanie
  - winda po drodze zabiera każdego pasażera(o ile jest jeszcze miejsce w srodku)

Możliwości rozwoju projektu :
  - optymalizacja algorytmu poprzez synchronizację wind, tak aby najbliższa winda zbierała potencjalnego pasażera
