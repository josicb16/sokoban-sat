# sokoban-sat

<h2>Sokoban</h2>

<img src = "http://www.matf.bg.ac.rs/p/files/126-sokoban.gif" width ="200" height ="200">
Sokoban je igra u kojoj je cilj da se svaka kutija, koja je na tabli, smesti u neku od datih kućica za kutije (broj kućica na tabli je jednak broju kutija). Igrač - Sokoban gura kutije sve dok ne ispuni cilj igre ili dok ne dođe do stanja u kom zasigurno više ne može da smesti sve kutije u kućice - tada je igra neuspešno završena.
<br>
Sokoban može da gurne kutiju za samo jedno polje gore, dole, levo ili desno, pri čemu ne može da gurne kutiju na željeno polje, ako je na tom polju zid ili druga kutija.

<h2>Opis programa</h2>

Od korisnika se najpre traži da unese početni izgled table u vidu niske, koja sadrži sledeće karaktere:
&#35; - označava zid;
o - označava kućicu za kutiju;
b - označava polje na kojem se nalazi kutija;
B - označava polje na kojem se nalazi kutija, a koje je ujedno i kućica za kutiju;
s - označava polje na kojem je Sokoban;
S - označava polje na kojem je Sokoban, koje je ujedno i kućica za kutiju;
razmak - označava prazno polje.

Primer:

<div>
    <p style="float: left;"><img src="http://www.matf.bg.ac.rs/p/files/1654460188-126-sokoban.png" height="200px" width="200px" border="1px"></p>
    <p>
           ##### 
         ###   # 
         #osb  # 
         ### bo# 
         #o##b # 
         # # o ##
         #b Bbbo#
         #   o  #
         ########
    </p>
</div>



<h2>Uputstvo za pokretanje</h2>
