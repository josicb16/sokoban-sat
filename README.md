# sokoban-sat

<h2>Sokoban</h2>

<img src = "http://www.matf.bg.ac.rs/p/files/126-sokoban.gif" width ="200" height ="200">
Sokoban je igra u kojoj je cilj da se svaka kutija, koja je na tabli, smesti na neko od označenih polja (broj označenih polja na tabli je jednak broju kutija). Igrač - Sokoban gura kutije sve dok ne ispuni cilj igre ili dok ne dođe do stanja u kom zasigurno više ne može da smesti sve kutije na sva označena polja - tada je igra neuspešno završena.
<br>
Sokoban može da gurne kutiju za samo jedno mesto gore, dole, levo ili desno, pri čemu ne može da gurne kutiju na željeno polje, ako je na tom polju zid ili druga kutija.

<h2>Opis programa</h2>


Program za datu tablu igre i za dati broj koraka n, ispisuje niz od najviše n poteza koje Sokoban treba da napravi, da bi igra bila uspešno završena; ako igra ne može da se uspešno završi u n koraka, tada program ispisuje odgovarajuću poruku o neuspehu. 

Igra Sokoban se u ovom slučaju posmatra kao problem planiranja, koji se onda svodi na SAT problem. Dakle, traženje niza poteza za uspešan završetak igre svodi se na ispitivanje zadovoljivosti iskazne formule kojom je predstavljen dati problem.


<br>
Od korisnika se najpre traži da unese početni izgled table u vidu niske, koja sadrži sledeće karaktere: <br>
&#35; - označava zid;
<br>
o - označava polje na koje treba smestiti kutiju;
<br>
b - označava polje na kojem se nalazi kutija;
<br>
B - označava polje na kojem se nalazi kutija, a koje je ujedno i polje na koje treba smestiti kutiju;
<br>
s - označava polje na kojem je Sokoban;
<br>
S - označava polje na kojem je Sokoban, koje je ujedno i polje na koje treba smestiti kutiju;
<br>
razmak - označava prazno polje.
<br>
<br>
Nakon toga korisnik unosi još i ceo broj n koji predstavlja broj koraka. 

<br>
<br>
<br>


<h2>Pokretanje programa</h2>

Za rad programa je potrebno preuzeti minisat rešavač i u terminalu iskucati sledeće komande: 


1) g++ GenerateDimacs.cpp Sokoban.cpp Formula.cpp -std=c++11
2) ./a.out
3) minisat dimacs.txt result.txt -no-luby -rinc=1.5 -phase-saving=0 -rnd-freq=0.02
4) g++ GetPlan.cpp -std=c++11
5) ./a.out
