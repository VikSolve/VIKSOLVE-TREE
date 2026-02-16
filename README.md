# Dynamic 2D Rectangle Add / Rectangle Sum (Online, Sparse, O(log^2 U))

Implementacja struktury danych do operacji na bardzo dużej (rzadkiej) siatce 2D:

- Update: dodaj wartość `v` do wszystkich punktów w prostokącie `[x1, x2] x [y1, y2]`
- Query: oblicz sumę wartości w prostokącie `[x1, x2] x [y1, y2]`

Struktura działa online (operacje napływają dynamicznie), bez kompresji współrzędnych i bez alokacji pełnej tablicy `U x U`.

---

## 1) Model problemu

Utrzymujemy funkcję `A(x, y)` na dyskretnej domenie:
- `x, y` są całkowite i należą do `[L, R]`.

Operacje:

1. `ADD(x1, y1, x2, y2, v)`
   - dla każdego `(x, y)` z prostokąta `[x1, x2] x [y1, y2]`:
   - `A(x, y) = A(x, y) + v`

2. `SUM(x1, y1, x2, y2)`
   - zwraca:
   - `sum_{x=x1..x2} sum_{y=y1..y2} A(x, y)`

Zakładamy: `x1 <= x2`, `y1 <= y2`.

---

## 2) Idea struktury

### 2.1 Zewnętrzne drzewo segmentowe po osi X

Budujemy dynamiczne drzewo segmentowe po `x`.
Każdy węzeł `v` reprezentuje przedział `X_v = [l_v, r_v]`.

### 2.2 W każdym węźle X: dwa dynamiczne drzewa po osi Y

Dla każdego węzła `v` trzymamy dwie niezależne struktury 1D (range add / range sum) po `y`:

- `local[v]` — lokalne update'y zakotwiczone w `v`
- `agg[v]` — agregacja wkładu update'ów do CAŁEGO przedziału `X_v`

Wewnętrzna struktura 1D to klasyczny dynamiczny segment tree z lazy propagation:
- `range_add(y1, y2, delta)`
- `range_sum(y1, y2)`

---

## 3) Definicje formalne i niezmienniki

Niech `U_v` oznacza zbiór update'ów zakotwiczonych w węźle `v`
(tzn. update pokrywa `X_v` w całości, ale na poziomie rodzica nie było już pełnego pokrycia).

Dla każdego węzła `v` utrzymujemy:

### Niezmiennik I (znaczenie `local[v]`)

Dla dowolnego przedziału `Y = [a, b]`:

`local[v].sum(Y) = sum_{u in U_v} ( val(u) * |Y ∩ Y_u| )`

gdzie:
- `val(u)` = wartość update'u `u`
- `Y_u = [y1^u, y2^u]`

Interpretacja:
- to wkład "na jeden punkt x" należący do `X_v`.

### Niezmiennik II (znaczenie `agg[v]`)

Dla dowolnego `Y = [a, b]`:

`agg[v].sum(Y) = sum_{u in U_v} ( val(u) * |X_v| * |Y ∩ Y_u| )`

Interpretacja:
- to wkład update'ów zakotwiczonych w `v` do całego prostokąta `X_v x Y`.

### Niezmiennik III (dekompozycja zapytania częściowego)

Jeśli zapytanie po `x`, czyli `Qx = [x1, x2]`, przecina `X_v` częściowo, to:

`ans(v) = |Qx ∩ X_v| * local[v].sum(Qy) + ans(left(v)) + ans(right(v))`

Jeśli `X_v` jest w całości zawarte w `Qx`, to zwracamy:

`agg[v].sum(Qy)`

---

## 4) Operacja update: ADD

Dla `ADD(x1, y1, x2, y2, val)` w odwiedzanym węźle `v`, z `X_v = [l_v, r_v]`:

1. Policz:
   - `ov_x = |X_v ∩ [x1, x2]|`

2. Zawsze zaktualizuj:
   - `agg[v].range_add(y1, y2, val * ov_x)`

3. Jeśli `X_v` jest całe w `[x1, x2]`, to dodatkowo:
   - `local[v].range_add(y1, y2, val)`
   - i STOP (nie schodzimy do dzieci)

4. W przeciwnym razie:
   - schodzimy rekurencyjnie do przecinających dzieci.

Dlaczego w kroku 2 jest mnożnik `ov_x`?
- Bo `agg[v]` przechowuje sumę po wszystkich `x` z `X_v`, a nie "na pojedynczy x".

---

## 5) Operacja query: SUM

Dla `SUM(x1, y1, x2, y2)` w węźle `v`:

1. Jeśli brak przecięcia z `[x1, x2]`:
   - zwróć `0`

2. Jeśli `X_v` jest całe w `[x1, x2]`:
   - zwróć `agg[v].sum(y1, y2)`

3. W przeciwnym razie:
   - `ov_x = |X_v ∩ [x1, x2]|`
   - `here = ov_x * local[v].sum(y1, y2)`
   - zwróć `here + query(left) + query(right)`

To dokładnie realizuje Niezmiennik III.

---

## 6) Szkic dowodu poprawności

Dowód przez indukcję po:
- strukturze drzewa `x`
- liczbie wykonanych operacji.

### Krok bazowy
Bez update'ów wszystkie struktury są puste, więc wszystkie sumy = 0.
Niezmienniki I–III zachodzą trywialnie.

### Krok indukcyjny: update
Dla nowego update'u `U`:
- w każdym odwiedzonym `v`, `agg[v]` dostaje dokładnie `val * |X_v ∩ X_U|` na odpowiednim przedziale `y`,
  więc wkład do całego `X_v x Y` jest poprawny (Niezmiennik II),
- jeśli `X_v` jest całe w `X_U`, update kotwiczy się w `v`, więc `local[v]` dostaje `val` na `Y_U`
  (Niezmiennik I),
- przy niepełnym pokryciu update schodzi do dzieci; rozłączność podprzedziałów dzieci
  eliminuje podwójne liczenie.

### Krok indukcyjny: query
- pełne pokrycie: `agg[v]` zwraca dokładną sumę po `X_v x Qy` (Niezmiennik II),
- częściowe pokrycie:
  - wkład lokalny to `|Qx ∩ X_v| * local[v].sum(Qy)`,
  - resztę dają dzieci (Niezmiennik III).
Suma daje dokładny wynik.

---

## 7) Złożoność

Niech:
- `U = R - L + 1` (rozmiar osi),
- `Q` = liczba operacji.

Mamy:
- wysokość dynamicznego drzewa po `x`: `O(log U)`,
- w każdym odwiedzonym węźle jedna operacja 1D po `y`: `O(log U)`.

Stąd:
- czas `ADD`: `O(log^2 U)`
- czas `SUM`: `O(log^2 U)`

Pamięć:
- zależy od liczby faktycznie utworzonych węzłów (sparse),
- typowo do `O(Q log^2 U)` w pesymistycznym przypadku.

---

## 8) Wymagania implementacyjne i pułapki

1. Clamp przecięcia:
   - licz długość przecięcia jako:
   - `ov([a,b],[c,d]) = max(0, min(b,d) - max(a,c) + 1)`

2. Duże liczby:
   - używaj co najmniej 128-bitów (`__int128`) dla sum i iloczynów typu:
   - `val * ov_x * ov_y`

3. Spójne indeksowanie:
   - trzymaj domenę domkniętą `[L, R]`
   - długość segmentu: `len = r - l + 1`

4. Wczesne wyjścia:
   - brak przecięcia kończy rekurencję natychmiast.

---

## 9) Kiedy używać tej struktury

Używaj, gdy:
- zakres współrzędnych jest bardzo duży (np. do `2^31` albo więcej),
- dane są rzadkie (nie chcesz alokować pełnej macierzy),
- operacje są online (naprzemienne update/query),
- potrzebujesz dokładnych sum prostokątów po prostokątnych modyfikacjach.
