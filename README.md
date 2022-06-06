# sokoban-sat

<h2>Sokoban</h2>

<img src = "http://www.matf.bg.ac.rs/p/files/126-sokoban.gif" width ="200" height ="200">
Sokoban je igra u kojoj je cilj da se svaka kutija, koja je na tabli, smesti na neko od označenih polja (broj označenih polja na tabli je jednak broju kutija). Igrač - Sokoban gura kutije sve dok ne ispuni cilj igre ili dok ne dođe do stanja u kom zasigurno više ne može da smesti sve kutije na sva označena polja - tada je igra neuspešno završena.
<br>
Sokoban može da gurne kutiju za samo jedno mesto gore, dole, levo ili desno, pri čemu ne može da gurne kutiju na željeno polje, ako je na tom polju zid ili druga kutija.

<h2>Opis programa</h2>


Program za datu tablu igre i za dati broj koraka n, ispisuje niz od najviše n pokreta koje Sokoban treba da napravi, da bi igra bila uspešno završena; ako igra ne može da se uspešno završi u n koraka, tada program ispisuje odgovarajuću poruku o neuspehu. 

Igra Sokoban se u ovom slučaju posmatra kao problem planiranja, koji se onda svodi na SAT problem. Dakle, traženje niza poteza za uspešan završetak igre svodi se na ispitivanje zadovoljivosti odgovarajuće iskazne formule, koju generise ovaj program.


<br>
Od korisnika se najpre traži da unese početni izgled table u vidu niske, koja sadrži sledeće karaktere: <br>
&#35; - označava zid;
<br>
o - označava kućicu za kutiju;
<br>
b - označava polje na kojem se nalazi kutija;
<br>
B - označava polje na kojem se nalazi kutija, a koje je ujedno i kućica za kutiju;
<br>
s - označava polje na kojem je Sokoban;
<br>
S - označava polje na kojem je Sokoban, koje je ujedno i kućica za kutiju;
<br>
razmak - označava prazno polje.
<br>
<br>
A nakon toga korisnik unosi ceo broj n koji predstavlja broj koraka. 

<br>
<br>
<br>



<h2>Uputstvo za pokretanje</h2>
