1. (7p) Creați un program (Collect.exe) care să:
a. (1p) creeze folderul C:\Facultate\CSSO\Laboratoare\Week3\ProcessInfo
b. (2p) în folderul creat anterior, va crea un fișier cu numele “procese.txt” ce conține toate
procesele aflate pe sistem la momentul execuției Collect.exe
i. Fiecare linie va conține ParentProcessId, ProcessId, SzExeFile
c. (1p) în folderul creat anterior, va crea un fișier cu numele “fire.txt” ce conține toate firele
de execuție aflate pe sistem la momentul execuției Collect.exe (fiecare fir pe câte o linie)
i. Fiecare linie va conține threadId, OwnerProcessId
d. (2p) în folderul creat anterior, va crea un fișier cu numele “module_process.txt” ce conține
toate modulele încărcate de procesul curent (Collect.exe) (fiecare modul pe câte o linie)
i. Fiecare linie va conține moduleId, processId, szModule, szExePath
e. (2p) deschide un memory mapping cu numele “cssoh3basicsync” în care scrie numărul
total de iteme (procese, fire, module) pe care le-a scris în fișiere:
i. Exemplu:
1. “Module: 32”
2. “Procese: 120”
3. “Fire: 150”
2. (3.5p) Creați un program (Filter.exe) ce:
a. (1p) iși seteze privilegiul (SE_SYSTEM_PROFILE_NAME) pentru procesul curent
(Filter.exe)
b. (1p) conține o funcție ce parcurge un fișier primit ca parametru și un sir de caractere si
afișează pe ecran numele fișierului, urmat de numărul de linii din acel fișier ce contin sirul
de caractere specificat
c. (1p) lansează câte un thread pentru fiecare dintre fișierele create la 1b, 1c, 1d pe care va
rula funcția definită la punctul b; sirul de caractere specificat ca si parametru va fi un
nume de process, la alegere, dat de voi
d. (0.5p) implementeaza un sleep de 30 de minute; înainte de a porni sleep-ul, va afisa pe
ecran un mesaj specific
3. (2p) Creati un program (Killer.exe) ce:
a. (1p) Va identifica pid-ul procesului Filter.exe și îl va afișa în consolă
b. (1p) Va termina fortat procesul Filter.exe
4. (2.5p) Creați un program (Main.exe) ce:
a. (0.5p) creaza un memorymapping cu numele “cssoh3basicsync”
b. (0.5p) crează procesul Collect.exe și așteaptă ca acesta să-și termine execuția
c. (0.5p) citește din memory mapping datele scrise și le afișează pe ecran
d. (0.5p) lansează Filter.exe în execuție și fără a aștepta ca acesta să-și termine execuția
e. (0.5p) lansează în execuție programul Killer.exe

Punctajele de mai sus se vor acorda în totalitate sau 0, în funcție de corectitudinea implementării,
de existența verificărilor pentru posibile erori, clean code și închiderea handleurilor.