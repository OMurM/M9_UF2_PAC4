# M9_UF2_PAC4

# Servei de gestió de comandes am multiples clients

# Aquesta aplicació hauria de permetre gestionar les comandes de manera simultanea. El servidor escolta les connexions entrants.

# Flow 

Server 
	- Escolta connexions al port 9100
	- Rep la comanda del client
	- Assigna un identificador unic a la comanda ( format - ORD-00X )
	- Desa la comanda al fitxer comandes.txt
	- Retorna l'identificatiu al client

Client
	- Es connecta al servidor pel port 9100
	- Envia una comanda escrita desde el terminal
	- Rep i mostra l'identificador únic assignat pel servidor


WorkFlow

1. Server
2. Client

