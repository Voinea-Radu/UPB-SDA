# Tema 3 SD

```
Echipa tema 3 SD:
andrei.coman1301
radu_mihai.voinea04
```

## Task 1 - Retea de Prietenie

Acest proiect implementează o retea de prietenie între utilizatori. Fiecare
utilizator poate adăuga sau elimina prieteni, afla distanta dintre ei, găsi
sugestii pentru noi prieteni și identifica prietenii comuni cu alti utilizatori.

## Comenzi disponibile

### 1. Adaugă prieten

```
add <nume-1> <nume-2>
```

Creează o legătură bidirectională între doi utilizatori. Ambii utilizatori se
vor adăuga reciproc ca prieteni.

### 2. Elimină prieten

```
remove <nume-1> <nume-2>
```

Elimină legătura dintre doi utilizatori.

### 3. Află distanta

```
distance <nume-1> <nume-2>
```

Calculează distanta între doi utilizatori. Distanta este considerată 1 pentru
fiecare prietenie.
Dacă nu există o legătură între cei doi utilizatori, se va afișa `There is no
way to get from <nume-1> to <nume-2>`.

### 4. Sugestii

```
suggestions <nume>
```

Afișează toti prietenii prietenilor utilizatorului, care nu sunt deja prieteni
cu el. Prietenii vor fi afișati crescător după id-ul utilizatorului.
Dacă nu există sugestii pentru un utilizator, se va afișa `There are no
suggestions for <nume>`.

### 5. Prieteni comuni

```
common <nume-1> <nume-2>
```

Găsește prietenii comuni ai doi utilizatori. Un prieten comun este o persoană
care îi are ca prieteni pe ambii utilizatori. Lista de prieteni va fi sortată
crescător după id-ul utilizatorului.

Exemplu:
Dacă nu există niciun prieten comun, se va afișa `No common friends for <nume-1>
and <nume-2>`.

### 6. Număr de prieteni

```
friends <nume>
```

Afișează numărul de conexiuni al unui utilizator.

### 7. Cel mai popular

```
popular <nume>
```

Afișează utilizatorul cu cele mai multe conexiuni dintre <nume> și prietenii
săi.
În caz de egalitate cu alti prieteni, se va prefera utilizatorul însuși ca cel
mai popular prieten. În caz de egalitate pentru prieteni, se va considera primul
după id.

## Task 2 - Postări și Reposturi

### 1. Creează o postare

```
create <nume> <titlu>
```

Utilizatorul `<nume>` va crea o postare cu titlul `<titlu>`. Fiecare postare va
avea un id unic, care va fi incrementat la fiecare postare nouă.

### 2. Repost

```
repost <nume> <post-id> [repost-id]
```

Creează un repost la o postare existentă. Dacă comanda repost are un repost id,
atunci se consideră că este un repost la un repost.

### 3. Primul repost comun

```
common-repost <post> <repost-id-1> <repost-id-2>
```

Găsește primul repost comun al două reposturi.

### 4. Like

```
like <nume> <post-id> [repost-id]
```

Adaugă un like la o postare sau repostare. Dacă se dă repost id, atunci se va
adăuga like-ul la repostare.

### 5. Ratio

```
ratio <post-id>
```

În jargonul internetului, ratio înseamnă că un răspuns la o postare are mai
multe like-uri decât postarea originală (ceea ce implică că opinia răspunsului
este mai acceptată decât cea a postării originale).

### 6. Șterge Postare/Repost

```
delete <post-id> [repost-id]
```

Șterge o postare sau un repost. Odată ce o postare sau un repost a fost șters,
toate reposturile care depind de aceasta vor fi șterse.

### 7. Obține like-uri

```
get-likes <post-id> [repost-id]
```

Afișează numărul de like-uri pentru o postare sau repostare.

### 8. Obține reposturi

```
get-reposts <post> [repost-id]
```

Afișează întreaga ierarhie de reposturi pentru o postare/repostare.

## Task 3 - Social Media

Acest task implementează funcționalități de bază ale unei platforme de
social media.

### 1. Feed

```
feed <nume> <feed-size>
```

Afișează cele mai recente `<feed-size>` postări ale unui utilizator și ale
prietenilor lui.

### 2. View profile

```
view-profile <nume>
```

Afișează toate postările și repostările unui utilizator.

### 3. Friends that reposted

```
friends-repost <nume> <post-id>
```

Afișează toți prietenii unui utilizator care au dat repost la postarea cu
`<post-id>`-ul primit.

### 4. Clică

```
common-group <nume>
```

Găsiți și afișați cel mai mare grup de prieteni care îl conține pe un anumit
utilizator. Un grup de prieteni se definește ca toți oamenii dintr-un grup care
sunt prieteni unii cu alții. Lista de nume afișată va fi afișată crescător după
id-ul utilizatorului.
