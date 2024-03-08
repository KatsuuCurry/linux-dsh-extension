Theofilia Jessica 894476

Per il mio progetto, ho scelto di implementare la terza estensione, ovvero set ed echo.

SET

set PATH NEW_PATH
set VAR_NAME VAR_VALUE

Attraverso il comando set, è possibile gestire variabili, modificarle o eliminarle, oppure modificare il percorso (path).

L’utente può salvare una variabile con un nome di massimo 32 caratteri. Il nome della variabile e il valore associato vengono memorizzati all’interno di un array globale (che può contenere un massimo di 256 variabili). 
Viene lanciato un messaggio di errore nel caso in cui la lunghezza del percorso o del nome della variabile superi il limite, e l’esecuzione del programma viene interrotta se la funzione riceve parametri nulli. Una volta verificata la validità dei parametri, si controlla l’eventuale esistenza di tale variabile all’interno dell’array globale. In caso positivo, si dealloca la memoria e si alloca lo spazio necessario al nuovo valore, sostituendolo. Altrimenti, si alloca lo spazio per il nome della nuova variabile e per il suo valore associato, e si salva. Nel caso in cui la funzione riceva il nome di una variabile valido senza specificare il valore (NULL), essa la elimina dall’array, deallocando lo spazio allocato.

L’utente può anche salvare un nuovo percorso nella variabile globale, inserendo come nome della variabile PATH, la cui lunghezza massima è di 512 caratteri.

ECHO

echo $PATH
echo $VAR

Attraverso il comando “echo”, è possibile stampare il valore di una variabile esistente o il percorso.

La funzione recupera il valore del percorso o della variabile salvati nelle variabili globali e lo stampa a schermo. Viene lanciato un messaggio di errore nel caso in cui il nome della variabile non sia preceduto dal simbolo “$”, oppure se è presente solo tale simbolo. Inoltre, l’esecuzione del programma viene interrotta se la funzione riceve parametri nulli o nomi di variabili non dichiarate.