Copyright (C) 2023 Cismaru Diana-Iuliana (321CA / 2022 - 2023)

# Tema 4 - Client Web - Comunicatie cu REST API

## Descrierea proiectului

Programul descrie functionalitatea unui client care interactioneaza cu un server
de tip REST. Implementarea mea din `client.cpp` accepta comenzi de la tastatura
intr-o bucla infinita (ce se opreste la primirea mesajului `exit`) si trimite
cereri catre server.

Proiectul are la baza scheletul [laboratorului 9](https://gitlab.cs.pub.ro/pcom/pcom-laboratoare-public/-/tree/master/lab9)
si rezolvarea acestuia din cadrul laboratorului propriu-zis.

Request-urile se afla in fisierul `requests.cpp` si sunt de 3 tipuri:
- *GET request*: interogare de resurse,
- *POST request*: adaugare de resurse,
- *DELETE request*: stergere de resurse; este regasit chiar in functia
corespunzatoare lui *GET*, deoarece implementarile se aseamana foarte tare, iar
crearea unei noi functii ar fi dus la mult cod duplicat.

## Parsarea JSON

Pentru parsarea raspunsurilor primite de la server, am utilizat biblioteca
`nlohmann`, sugerata in cerinta, deoarece am ales sa implementez tema in C++
si mi s-a parut usor de utilizat. Pentru serializarea JSON-urilor am utilizat
functia `dump()`, care mi-a oferit datele sub forma de string-uri. A fost utila
atunci cand am trimis catre server datele pentru logare/inregistrare, datele
pentru adaugarea unei noi carti dar si atunci cand am afisat datele despre carti.

## Comenzi implementate

In `commands.cpp` am implementat functiile corespunzatoare fiecarei comenzi
adresate serverului.

#### Inregistrarea unui cont (`register`)
- se inregistreaza datele introduse la STDIN legate de username si parola;
- se verifica ca datele introduse nu contin spatii; in caz contrar, se intoarce
o eroare;
- se efectueaza un `POST request` la url-ul */api/v1/tema/auth/register*;
- intoarce eroare daca username-ul este deja folosit;

#### Autentificare (`login`)
- se inregistreaza datele introduse la STDIN legate de username si parola;
- se verifica ca datele introduse nu contin spatii; in caz contrar, se intoarce
o eroare;
- se efectueaza un `POST request` la url-ul */api/v1/tema/auth/login*;
- daca exista deja un utilizator logat, cookie-ul de sesiune se suprascrie;
- intoarce eroare daca credentialele nu corespund unui user existent;

#### Cerere de acces in biblioteca (`enter_library`)
- se verifica cookie-ul de sesiune pentru a demonstra ca userul este logat;
- se efectueaza un `GET request` la url-ul */api/v1/tema/library/access*;
- retine un token JWT ce este folosit mai tarziu pentru a demonstra accesul
la biblioteca;

#### Vizualizarea informatiilor sumare despre toate cartile (`get_books`)
- se verifica atat ca un user este logat (prin existenta cookie-ului de sesiune),
cat si daca userul are acces la biblioteca (prin existenta token-ului JWT);
- se efectueaza un `GET request` la url-ul */api/v1/tema/library/books*;
- se afiseaza cartile in format JSON, parsand raspunsul serverului cu ajutorul
bibliotecii *nlohmann*;

#### Vizualizarea detaliilor despre o carte (`get_book`)
- se verifica atat ca un user este logat, cat si daca userul are acces la
biblioteca;
- se verifica validitatea ID-ului introdus: trebuie sa fie un numar si trebuie
sa corespunda unei carti din biblioteca;
- se efectueaza un `GET request` la url-ul *"/api/v1/tema/library/books/:bookid"*;
- se afiseaza cartea in format JSON, parsand raspunsul serverului cu ajutorul
bibliotecii *nlohmann*;

#### Adaugarea unei carti (`add_book`)
- se verifica atat ca un user este logat, cat si daca userul are acces la
biblioteca;
- se verifica corectitudinea datelor introduse la STDIN pentru cartea ce se
doreste a fi adaugata: trebuie ca `page_count` sa fie un numar si trebuie sa
ma asigur ca field-urile cerute nu au fost lasate goale;
- se efectueaza un `POST request` la url-ul */api/v1/tema/library/books*;

#### Stergerea unei carti (`delete_book`)
- se verifica atat ca un user este logat, cat si daca userul are acces la
biblioteca;
- se verifica validitatea ID-ului introdus: trebuie sa fie un numar si trebuie
sa corespunda unei carti din biblioteca;
- se efectueaza un `DELETE request` la url-ul */api/v1/tema/library/books/:bookid*;

#### Logout (`logout`)
- se efectueaza un `GET request` la url-ul */api/v1/tema/auth/logout*;
- se reseteaza cookie-ul de sesiune si token-ul JWT;
- in cazul in care nu era niciun utilizator logat, se intoarce o eroare;
