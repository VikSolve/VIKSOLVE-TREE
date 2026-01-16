# README

- [PL — Opis rozwiązania](#opis-rozwiazania-pl)
- [EN — Solution description](#solution-description-en)

# Opis rozwiązania (PL)

## Założenia
1) Prostokąt (x1, y1, x2, y2) rozumiemy jako prostokąt osiowo-równoległy, którego **lewy dolny** róg jest w punkcie (x1, y1), a **prawy górny** w (x2, y2).  
2) L <= x1, y1, x2, y2 <= R. Definiujemy M = (R - L + 1). Dla uproszczenia implementacji zakładamy, że M = 2^k dla pewnego k ∈ N.

## Problem
Chcemy online obsługiwać operacje:
1) **Update:** dodaj (lub odejmij) wartość `val` na prostokącie [x1..x2] × [y1..y2].  
2) **Query:** podaj **sumę** wartości na prostokącie [x1..x2] × [y1..y2].

## Złożoność
Rozwiązanie działa w złożoności czasowej i pamięciowej:
O(Q * (log2 M)^2),
gdzie Q to liczba operacji, czyli O((log2 M)^2) na pojedynczą operację.

## Idea
Utrzymujemy dynamiczne drzewo przedziałowe T na osi **X** dla zakresu [L, R].  
Każdy wierzchołek v drzewa T reprezentuje przedział [l, r] na osi X i przechowuje:

1) Dynamiczne drzewo przedziałowe **`sgtree`** na osi **Y**.  
   Przechowuje ono te aktualizacje, które podczas update’u na osi X **zatrzymały się** w v (tzn. [l, r] jest w całości w [x1, x2], więc nie schodzimy do dzieci).  
   Wtedy wykonujemy:
   - `sgtree.add(y1, y2, val)`.

2) Dynamiczne drzewo przedziałowe **`sum`** na osi **Y**.  
   Aktualizujemy je dla każdego odwiedzanego węzła v, gdy [l, r] przecina [x1, x2]. Dodajemy na [y1, y2] wartość:
   - `val * overlap`,
   gdzie:
   - `overlap = liczba elementów w części wspólnej [l, r] i [x1, x2]`
     (czyli overlap = max(0, min(r, x2) - max(l, x1) + 1)).

3) Wskaźnik na lewe dziecko (może nie istnieć).  
4) Wskaźnik na prawe dziecko (może nie istnieć).

---

## UPDATE (x1, y1, x2, y2, val)
Startujemy w korzeniu. Dla węzła v reprezentującego [l, r]:

1) Liczymy `overlap` i wykonujemy:
   - `v.sum.add(y1, y2, val * overlap)`.

2) Jeśli [l, r] jest w całości w [x1, x2], to:
   - `v.sgtree.add(y1, y2, val)`,
   - kończymy rekurencję w tym węźle.

3) W przeciwnym razie schodzimy do dzieci:
   - jeśli x1 <= floor((l+r)/2), wywołujemy update w lewym dziecku dla [l, floor((l+r)/2)],
   - jeśli floor((l+r)/2) < x2, wywołujemy update w prawym dziecku dla [floor((l+r)/2)+1, r].

---

## QUERY (x1, y1, x2, y2)
Startujemy w korzeniu. Dla węzła v reprezentującego [l, r]:

1) Jeśli [l, r] jest w całości w [x1, x2], zwracamy:
   - `v.sum.query(y1, y2)`.

2) W przeciwnym razie (częściowe pokrycie):
   - ustawiamy `result = 0`,
   - liczymy `overlap`,
   - dodajemy wkład z aktualizacji „zatrzymanych” w v:
     - `result += overlap * v.sgtree.query(y1, y2)`,
   - następnie dodajemy wyniki z dzieci (jeśli istnieją):
     - jeśli x1 <= floor((l+r)/2), schodzimy do lewego dziecka,
     - jeśli floor((l+r)/2) < x2, schodzimy do prawego dziecka,
   - zwracamy `result`.

---

# Solution description (EN)

## Assumptions
1) A rectangle (x1, y1, x2, y2) is an axis-aligned rectangle with the **bottom-left** corner at (x1, y1) and the **top-right** corner at (x2, y2).  
2) L <= x1, y1, x2, y2 <= R. Let M = (R - L + 1). For implementation convenience we assume M = 2^k for some k in N.

## Problem
We want to support online operations:
1) **Update:** add (or subtract) a value `val` on the rectangle [x1..x2] × [y1..y2].  
2) **Query:** return the **sum** on the rectangle [x1..x2] × [y1..y2].

## Complexity
Time and memory complexity:
O(Q * (log2 M)^2),
where Q is the number of operations, i.e. O((log2 M)^2) per operation.

## Idea
We maintain a dynamic segment tree T on the **X** axis over the range [L, R].  
Each node v of T represents an X-interval [l, r] and stores:

1) A dynamic segment tree **`sgtree`** on the **Y** axis.  
   It stores updates that, during the X-segtree update, **stop** at v (i.e. [l, r] is fully inside [x1, x2], so we do not go to v's children).  
   Then we perform:
   - `sgtree.add(y1, y2, val)`.

2) A dynamic segment tree **`sum`** on the **Y** axis.  
   We update it for every visited node v whenever [l, r] intersects [x1, x2]. We add on [y1, y2] the value:
   - `val * overlap`,
   where:
   - `overlap = the number of integers in the intersection of [l, r] and [x1, x2]`
     (i.e. overlap = max(0, min(r, x2) - max(l, x1) + 1)).

3) A pointer to the left child (may not exist).  
4) A pointer to the right child (may not exist).

---

## UPDATE (x1, y1, x2, y2, val)
We start at the root. For a node v representing [l, r]:

1) Compute `overlap` and do:
   - `v.sum.add(y1, y2, val * overlap)`.

2) If [l, r] is fully inside [x1, x2], then:
   - `v.sgtree.add(y1, y2, val)`,
   - stop recursion at this node.

3) Otherwise, recurse to children:
   - if x1 <= floor((l+r)/2), recurse to the left child for [l, floor((l+r)/2)],
   - if floor((l+r)/2) < x2, recurse to the right child for [floor((l+r)/2)+1, r].

---

## QUERY (x1, y1, x2, y2)
We start at the root. For a node v representing [l, r]:

1) If [l, r] is fully inside [x1, x2], return:
   - `v.sum.query(y1, y2)`.

2) Otherwise (partial overlap):
   - set `result = 0`,
   - compute `overlap`,
   - add the contribution from updates that stopped at v:
     - `result += overlap * v.sgtree.query(y1, y2)`,
   - then add results from children (if they exist):
     - if x1 <= floor((l+r)/2), go to the left child,
     - if floor((l+r)/2) < x2, go to the right child,
   - return `result`.
