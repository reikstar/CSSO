<a name="br1"></a> 

Se cere realizarea unui program care să interacționeze cu un server aflat la adresa:

[**http://cssohw.herokuapp.com/**](http://cssohw.herokuapp.com/)[.](http://cssohw.herokuapp.com/)[ ](http://cssohw.herokuapp.com/)**AGENTUL requesturilor trebuie să conțină numărul**

**vostru matricol! Altfel tema va fi punctată cu 0 puncte.**

1\. (3p) Veți implementa un GET Request pe

[**http://cssohw.herokuapp.com/assignhomework/<matricol**](http://cssohw.herokuapp.com/assignhomework/%3cmatricol)[>**](http://cssohw.herokuapp.com/assignhomework/%3cmatricol)[** ](http://cssohw.herokuapp.com/assignhomework/%3cmatricol)În urma apelării acestui

serviciu se va obține un fișier cu linii de text. Salvați fișierul pe disk în

"C:\Facultate\CSSO\Week5\myconfig.txt"

2\. (9p) Citiți linie cu linie din fișierul salvat anterior, "myconfig.txt".

Liniile vor avea formatul: **<method>:<link>**

·

·

<method>: **GET sau POST**

<link>: linkul pe care veți apela metoda <method>; linkul poate fi unul dintre

următoarele:

o

(3p) [http://cssohw.herokuapp.com/dohomework/<valoare>](http://cssohw.herokuapp.com/dohomework/%3cvaloare%3e)[,](http://cssohw.herokuapp.com/dohomework/%3cvaloare%3e)[ ](http://cssohw.herokuapp.com/dohomework/%3cvaloare%3e)exemplu:

<http://cssohw.herokuapp.com/dohomework/123>

.

Dacă method este GET, dupa apelul linkului, salvați răspunsul într-un

fișier cu numele <valoare> în locația

“C:\Facultate\CSSO\Week5\Downloads\”. Dacă fișierul există deja,

apendați răspunsul primit, pe o nouă linie în fișierul respectiv

Dacă method este POST, trimeți trimiteți răspunsul primit la ultimul

GET realizat, în format “application/x-www-form-urlencoded” ce

respectă modelul: "id=<nr\_matricol>&value=<raspunsul\_primit>"

.

o (6p) [http://cssohw.herokuapp.com/dohomework_additional/<valoare](http://cssohw.herokuapp.com/dohomework_additional/%3cvaloare)[>](http://cssohw.herokuapp.com/dohomework_additional/%3cvaloare),

exemplu: <http://cssohw.herokuapp.com/dohomework_additional/123>

.

Metoda poate fi doar GET

.

După apelul linkului, salvați răspunsul într-un fișier cu numele

“<valoare>\_aditional.txt” în locația

“C:\Facultate\CSSO\Week5\Downloads\”. Dacă fișierul există deja,

apendați răspunsul pe o nouă linie în fișierul respectiv

.

Cu valoarea răspunsului obținut veți face un nou request

[http://cssohw.herokuapp.com/dohomework_additional/<valoare_](http://cssohw.herokuapp.com/dohomework_additional/%3cvaloare)raspu

ns>; se vor repeta requesturile către

[http://cssohw.herokuapp.com/dohomework_additional/<valoare_raspu](http://cssohw.herokuapp.com/dohomework_additional/%3cvaloare_raspuns)

[ns](http://cssohw.herokuapp.com/dohomework_additional/%3cvaloare_raspuns)[>](http://cssohw.herokuapp.com/dohomework_additional/%3cvaloare_raspuns)[ ](http://cssohw.herokuapp.com/dohomework_additional/%3cvaloare_raspuns)până când valoarea răspunsului obținut este 0. Toate

răspunsurile obținute vor fi la rândul lor salvate în fișierul creat

anterior („<valoare>\_aditional.txt”).



<a name="br2"></a> 

3\. (3p) La final, implementați un POST request pe

<http://cssohw.herokuapp.com/endhomework>[ ](http://cssohw.herokuapp.com/endhomework)în care să scrieți în format “application/x-

www-form-urlencoded” numărul vostru matricol, numărul total de requesturi realizate la

punctul 2, numărul de requesturi GET, numărul de requesturi POST precum și size-ul

total al fișierelor aflate in “C:\Facultate\CSSO\Week5\Downloads\”

("id=<nr\_matricol>&total=<total>&get=<get>&post=<post>&size=<size>")


Bonus (5p):

Implementarea unei interfețe minimale ce va permite introducerea linkului de la

·

punctul 1, numărul matricol și care va avea un buton ce va permite rularea programului.

Vor fi afișate în fereastră requesturile ce se fac și răspunsurile primite.


