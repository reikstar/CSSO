Punctaj: 15
Termen pentru submisia temei pe formular:
2023/10/22 (23:59 UTC)
Termen de prezentare:
2023/10/23 -> 2023/10/24
Scrieți un program care să îndeplinească următoarele cerințe:
(2p) – L1
Să creeze următoarea ierarhie de directorare:
 C:\Facultate\CSSO\Laboratoare\Week2\InstalledSoftware
 C:\Facultate\CSSO\Laboratoare\Week2\Rezultate
(3p) – L1/L2
În cadrul directorului C:\Facultate\CSSO\Laboratoare\Week2\Rezultate, se vor
crea 3 fișiere:
 HKLM.txt
 HKCC.txt
 HKCU.txt
În fiecare dintre cele 3 fisiere de mai sus (specifice unui hive) se vor afisa, pe câte o
linie, următoarele informații:
 lpcSubKeys
 lpcMaxSubKeyLen
 lpftLastWriteTime (aici valoarea va fi scrisă în format human readable –
yyyy/mm/dd hh:mm:ss)
(1p) – L1
În cadrul directorului C:\Facultate\CSSO\Laboratoare\Week2\Rezultate se va
crea un fișier cu numele sumar.txt (ce va conține 3 linii (corespunzătoare fișierelor de
mai sus), pe fiecare linie aflându-se pathul întreg spre fisier și sizeul acestuia)
(3p) – L1/L2
În cadrul directorului
C:\Facultate\CSSO\Laboratoare\Week3\InstalledSoftware se va crea cate un fisier
corespunzătoare cheilor găsite in HKCU\Software
(3p) – L1/L2
La final, creați în cheia de registru (HKCU\Software\CSSO\Week2) o valoare de
tipul REG_SZ ce va contine pathul întreg spre directorul
„C:\Facultate\CSSO\Laboratoare\Week3\InstalledSoftware” și o valoare REG_DWORD
ce va avea ca și valoare numarul de fișiere create.
Punctajele de mai sus pot fi acordate parțial sau în totalitate.
Punctajele de mai joi vor fi acordate doar dacă este bifată în totalitate cerinta.
(1p) Tratarea cazurilor de eroare pentru apelurile de WindowsAPI.
(1p) Toate handle-urile obținute sunt închise.
(1p) Cod ușor de citit (variabile denumite sugestiv, lipsa codului duplicat (creare de
funcții acolo unde este necesar), etc.