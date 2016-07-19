# Client-Serveur-EFTP
Application client-serveur EFTP écrite en C

# Bugs connus
* Les commandes downl et upld ne peuvent pas être lancées deux fois de suite sans attendre un petit laps de temps. Cela vient du fait que ces commandes utilisent d'autres sockets créés/détruits à la volée (sera corrigé bientôt)
* Pas de multithread/multiprocessus
