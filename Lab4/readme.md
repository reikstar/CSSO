Vom implementa un sistem de management al stocurilor unui Magazin Alimentar după cum
urmează:
 Un proces se va ocupa de procesarea mărfii ce trebuie pusă pe rafturi, denumit –
deposit.exe
 Un proces se va ocupa de procesarea mărfii ce se aproprie de expirare ce trebuie scosă
de pe rafturi și donată – donation.exe
 Un proces se va ocupa de procesarea mărfii ce a fost vândută – sold.exe
 Un proces se va ocupa de validarea si gestiunea generală a mărfii – management.exe
Pe linkul https://drive.google.com/file/d/1upIr6LdYBjaHKS0YJbmRcmK3Q5QFoTl/view?usp=sharing găsiți o arhivă ce conține:
 Directorul ”deposit”:
o pentru fiecare zi conține un fișier cu formatul: yyyy.mm.dd ce conține mai multe
linii
o pe fiecare linie vom găsi 4 numere întregi separate prin virgula; primul număr
reprezintă un ID de produs, al doilea, numărul de zile de păstrare a produsului
(dacă numărul este depășit făra ca produsul să fie vândut acesta va fi donat), cel
de-al treilea, numărul raftului pe care trebuie pus, iar cel de-al patrulea prețul
produsului
o știm cu siguranță că id-urile sunt unice
 Directorul ”sold”:
o pentru fiecare zi conține un fișier cu formatul: yyyy.mm.dd ce conține mai multe
linii
o pe fiecare linie vom găsi un număr întreg, reprezentând id-ul raftului de la care sa vândut produsul
Activitatea fiecăruia dintre procesele denumite mai sus este descrisă în rândurile ce urmează.
 (2p) management.exe
o (0.25p) Crează directoarele:
 „C:\Facultate\CSSO\Week4\Reports”,
 „C:\Facultate\CSSO\Week4\Reports\Summary”
o (0.25p) Crează un fișier mapat in memorie „MarketShelves” ce va reține un array
de 10000 de valori de tipul DWORD;
o (0.25p) Crează un fișier mapat în memorie denumit „MarketValability” ce va
reține un array de 10000 de valori de tipul DWORD;
o (0.25p) Crează un fișier mapat în memorie denumit „ProductPrices” ce va reține
un array de 10000 de valori de tipul DWORD;
o (0.25p) Lansează simultan în execuție “deposit.exe”, “sold.exe”, „donate.exe” și
așteaptă terminarea lor; !!!Cele 3 procese vor rula în paralel!!!
o (0.25p) Va crea fișierul C:\Facultate\CSSO\Week4\Reports\Summary\sold.txt cu
valoare 0
o (0.25p) Va crea fișierul
C:\Facultate\CSSO\Week4\Reports\Summary\donations.txt cu valoarea 0
o (0.15p) Dacă fișierul C:\Facultate\CSSO\Week4\Reports\Summary\errors.txt
lipsește, afișează valorile numerice din:
 C:\Facultate\CSSO\Week4\Reports\Summary\sold.txt
 C:\Facultate\CSSO\Week4\Reports\Summary\donations.txt
o (0.1p) Dacă fișierul C:\Facultate\CSSO\Week4\Reports\Summary\errors.txt este
prezent afișează liniile din el.
 (1p) deposit.exe
o (0.25p) deschide fișierul mapat în memorie „MarketShelves” și arrayul (îl vom
denumi shelves)
o (0.25p) deschide fișierul mapat în memorie „MarketValability” și arrayul (îl vom
denumi valability)
o (0.25p) deschide fișierul mapat în memorie „ProductPrices” și arrayul (îl vom
denumi prices)
o (0.25p) parcurge fișierele din directorul „deposit” și pentru fiecare linie din
fișiere face următoarele etape:
 citește cele 4 numere (id_produs, expires_in , shelve_id, product_price)
 validează dacă valoarea de la shelves[shelve_id] este 0xFFFFFFFF; dacă
valoarea este diferită de 0xFFFFFFFF va deschide (sau crea dacă nu
există) fișierul C:\Facultate\CSSO\Week4\Reports\Summary\errors.txt,
va scrie o linie cu următorul conținut: ”S-a încercat adăugarea produsului
<id_produs> pe raftul <shelve_id> care este deja ocupat de
<valoarea_existentă>” și va continua să proceseze următoarele linii
 dacă valoarea de la shelves[shelve_id] este 0xFFFFFFFF:
 va actualiza acea valoare cu id_produs (shelves[shelve_id] =
id_produs)
 va actualiza valoarea de la valability[id_produs] cu valoarea
câmpului expires_in citit din linie (valability[id_produs] =
expires_in)
 va actualiza valoare de la prices[id_produs] cu valoarea câmpului
product_price citit din linie (prices[id_produs] = product_price)
 va scrie în fișierul “C:\Facultate\CSSO\Week4\Reports\logs.txt”
„Am adăugat pe raftul <shelve_id> produsul <id_produs> ce are o
valabilitate de <expires_in> zile și un preț de <product_price>.”
 (1p) sold.exe
o (0.25p) deschide fișierul mapat în memorie „MarketShelves” și arrayul (îl vom
denumi shelves)
o (0.25p) deschide fișierul mapat în memorie „MarketValability” și arrayul (îl vom
denumi valability)
o (0.25p) deschide fișierul mapat în memorie „ProductPrices” și arrayul (îl vom
denumi prices)
o (0.25p) parcurge fișierele din directorul „sold” și pentru fiecare linie din fișiere
face următoarele etape:
 citește id-ul raftului (shelve_id)
 verifică dacă valoarea de la shelve_id din shelves (shelves[shelve_id])
este diferită de 0xFFFFFFFF; dacă valoarea este 0xFFFFFFFF va deschide
(sau crea dacă nu există) fișierul
C:\Facultate\CSSO\Week4\Reports\Summary\errors.txt, va scrie o linie
cu următorul conținut: ”S-a încercat vânzarea unui produs de pe un raft
<shelve_id> ce nu conține produs” și va continua să proceseze
următoarele linii
 dacă valoarea este diferită de 0xFFFFFFFF:
 daca valability[id_produs] != 0:
o va lua valoarea din shelves[shelve_id] si o va considera
id_produs
o va scrie în fișierul
“C:\Facultate\CSSO\Week4\Reports\logs.txt” „S-a vândut
produsul <id_produs> de pe raftul <shelve_id> cu
<valability[id_produs]> zile înainte de a fi donat și cu
prețul de prices[id_produs]”
o va actualiza valoarea din
C:\Facultate\CSSO\Week4\Reports\Summary\sold.txt
(new_value = old_value + prices[id_produs])
o va seta valability[id_produs] = 0xFFFFFFFF
o va seta prices[id_produs] = 0xFFFFFFFF
o va seta shelves[shelve_id] = 0xFFFFFFFF
 (1p) donation.exe
o (0.25p) deschide fișierul mapat în memorie „MarketShelves” și arrayul (îl vom
denumi shelves)
o (0.25p) deschide fișierul mapat în memorie „MarketValability” și arrayul (îl vom
denumi valability)
o (0.25p) deschide fișierul mapat în memorie „ProductPrices” și arrayul (îl vom
denumi prices)
o (0.25p) parcurge array-ul valability, și:
 dacă valoarea e 0xFFFFFFFF nu face nimic
 dacă valoarea e 0 va:
 va actualiza valoarea din
C:\Facultate\CSSO\Week4\Reports\Summary\donations.txt
(new_value = old_value + prices[id_produs])
 va scrie în fișierul “C:\Facultate\CSSO\Week4\Reports\logs.txt”
„Produsul <id_produs> a fost donat”
 va seta valability[id_produs] = 0xFFFFFFFF
 va seta proces[id_produs] = 0xFFFFFFFF
 va enumera id-urile din shelves, si acolo unde shelves[shelve_id]
== id_produs vom seta shelves[shelve_id] = 0xFFFFFFFF
 dacă valoarea e > 0 și < 0xFFFFFFFF va decrementa acea valoare
De implementat:
 (0.5p) Procesele “deposit.exe”, “sold.exe” și ”donation.exe” vor avea un timeout de 60
de secunde
 (0.5p) Implementând un mecanism de sincronizare ne vom asigura ca toate procesele au
terminat de procesat o zi înainte de a trece la următoarea zi (de exemplu, atat
deposit.exe, sold.exe și donation.exe ar trebui sa termine de procesat ziua 2023.11.12
înainte de a trece la ziua 2023.11.13; vor putea trece la ziua 2023.11.14 după ce au
terminat toate de procesat ziua 2023.11.13)
 (3p * 3) Accesul la fișierul fizic “C:\Facultate\CSSO\Week3\Reports\logs.txt”, și cele
mapate în memorie MarketShelves, MarketValability, ProductPrices trebuie realizat în
manieră sincronizată.
Alte precizări:
 Punctajul maxim pentru temele complete, dar fără mecanismul de sincronizare: 5p
 Trebuie implementate minim 3 mecanisme de sincronizare diferite!
 Se poate lucra în echipă de 2 persoane dacă se va implementa interfața bonus; daca
nu se implementeaza bonusul, tema este individuală
Bonus (5p):
 Implementarea unei interfațe minimale ce va permite selectarea celor 2 directoare de
input (deposit, sold), va avea un buton ce va permite rularea procesului management (în
cazul implementării interfeței nu va mai afișa la consola continutul fisierelor errors /
sold / donations ci le va afișa în interfață) 