# wspolbiezne_klient-serwer
Dwa programy: klient i serwer, które komunikują się przy pomocy kolejek FIFO.

1. Serwer tworzy kolejkę FIFO pod znaną z góry nazwą "s_fifo", nastepnie nasłuchuje na polecenia. Zadaniem serwera jest wykonywanie poleceń klienta i zwracanie ich wyników do kolejki klienta.

2. Klient generuje swoją kolejkę pod nazwą narzuconą przez użytkownika. W symulacji terminala użytkownik podaje tradycyjne unixowe polecenia (moga zawierać argumenty). Polecenia są wysyłane na kolejkę serwera i przez serwer wykonywane. 

Wpisanie w terminalu klienta słowa kluczowego "close" kończy pracę klienta. Wraz z utraceniem połączenia z ostatnim klientem server również zakańcza swój proces.