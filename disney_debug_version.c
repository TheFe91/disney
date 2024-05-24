#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TITLE_LEN 60
#define MAX_Q_LEN 300
#define MAX_A_LEN 30
#define MAX_S_LEN 200
#define MAX_FILM 40
#define DEBUG

int punteggio = 0;
int j = 0;
int k;
int tmp;
int max;
char save;
char *already_ins[MAX_FILM]; //array di stringhe dove segno i film inseriti
FILE *gamesave;

void stampa_domande (int, char[], char[], FILE*, FILE*);
void stampa_credits (void);
void stampa_readme (void);
void acquisisci_risposta (int, char[], FILE*);
char livello (void);
void acquisisci_titolo (int);
void gioca (int, char[], FILE*, FILE*, char[], char[], char[]);
void gioca_senza_punti (int, char[], FILE*, FILE*, char[], char[], char[]);
void livello_finale (void);
void stampa_frase (char[], FILE*);
void risposta (FILE*);
void stampa_film_su_file (FILE*);
void stampa_elenco_film (void);
int main (void) {

	int scelta_menu, scelta_menu2, i;
	char level, points[4], temp[MAX_TITLE_LEN + 1];
	FILE *films;

		printf ("Benvenuto!\n");
		printf ("1. Gioca\n2. Credits\n3. ReadMe\n0. Esci\n\nScelta: ");
		scanf ("%d%*c", &scelta_menu);
		k = -1;

		while (scelta_menu != 0) {
		   switch (scelta_menu) {

		      case 1: gamesave = fopen ("gamesave/gamesave.txt", "r");
			      if (gamesave != NULL) {
			         printf ("Caricare un salvataggio precedente? (y, n) ");
			         scanf ("%c%*c", &save);
			         if (save == 'y') {

				    films = fopen ("gamesave/films.txt", "r");
				    for (i = 0; fgets (temp, MAX_TITLE_LEN, films) != NULL; i++) {
				       already_ins[i] = calloc (strlen (temp), sizeof (char));
		   		       strcpy (already_ins[i], temp);
				    }
				    k = i - 1;
				    fclose (films);

				    #if defined (DEBUG)
				       for (i = 0; already_ins[i] != NULL; i++)
					  printf ("already_ins[%d] = %s\nk = %d\n", i, already_ins[i], k);
				    #endif

				    fgets (points, 5, gamesave);
				    punteggio = atoi (points);
				    fclose (gamesave);
				    printf ("Ho caricato il salvataggio.\n");
			         }
			      }   

			      printf ("1. Livelli Standard\n2. Livello Finale\n\nScelta: ");
			      scanf ("%d%*c", &scelta_menu2);

			      switch (scelta_menu2) {
				 case 1:

			         level = livello();
			         switch (level) {
			            case 'F': max = 4;
				              acquisisci_titolo (1); break;

				    case 'M': max = 3;
					      acquisisci_titolo (2); break;

				    case 'D': max = 2;
					      acquisisci_titolo (3); break;
			         }
			      	 break;

			         case 2:

			         if (punteggio == 1200 || punteggio > 1200) {
		   		    system ("clear");
		   		    printf ("Hai totalizzato %d punti: hai quindi diritto di accedere alla Sfida Finale!!!\n", punteggio);
		   		    punteggio = 0;
		   		    livello_finale ();

		   		    if (punteggio == 24 || punteggio > 24) {
				       printf ("Le mie più sincere congratulazioni!\nGIOCO COMPLETATO!\n");
				       printf ("La password per aprire l'archivio con il tuo premio è Metempsicosi2012\nCiao! Al prossimo gioco!");
				       return 0;
				    }
				    else
				       printf ("Peccato! Sfida Finale persa!\n");
				       
				    
				 }
				 else
				    printf ("Hai totalizzato %d punti. Te ne servono 1200 per giocare la Sfida Finale!\n", punteggio);
				 break;
			      }

			      system ("clear");
			      printf ("1. Gioca\n2. Credits\n3. ReadMe\n0. Esci\n\nScelta: ");
			      scanf ("%d%*c", &scelta_menu);
		              break;

		      case 2: stampa_credits ();
			      system ("clear");
			      printf ("1. Gioca\n2. Credits\n3. ReadMe\n0. Esci\n\nScelta: ");
			      scanf ("%d%*c", &scelta_menu);
			      break;

		      case 3: stampa_readme ();
			      system ("clear");
			      printf ("1. Gioca\n2. Credits\n3. ReadMe\n0. Esci\n\nScelta: ");
			      scanf ("%d%*c", &scelta_menu);
			      break;

		   } //end switch
		} //end while

		stampa_film_su_file (films);

		return 0;

} //end main

char livello (void) {

	char level;

		printf ("Scegli il livello di difficolta' del gioco (F, M, D) ");
		scanf ("%c%*c", &level);

		level = toupper (level);

		return level;
}

void acquisisci_titolo (int level) {

	char percorso_d[MAX_TITLE_LEN + 1] = "levels/", percorso_r[MAX_TITLE_LEN + 1] = "levels/", percorso_s[MAX_TITLE_LEN + 1] = "levels/";
	char titolo[MAX_TITLE_LEN + 1];
	char check[MAX_TITLE_LEN + 1];
	char replay;
	int i, already_exists = 1;
	FILE *d, *r;

		system ("clear");

		printf ("Inserisci il titolo del film di cui vuoi le domande: ");
		gets (titolo);

		do {

		for (i = 0; titolo[i] != '\0'; i++) {

		   titolo[i] = tolower (titolo[i]);
		   if (titolo[i] == ' ')
		      titolo[i] = '-';
		}

		strcat (percorso_d, titolo);
		strcat (percorso_r, titolo);
		strcat (percorso_s, titolo);
		strcat (percorso_d,"/domande.txt");
		strcat (percorso_r, "/risposte.txt");
		strcat (percorso_s, "/suggerimenti.txt");

		d = fopen (percorso_d, "r");
		r = fopen (percorso_r, "r");

		if (d == NULL || r == NULL) {
		   printf ("Non ho trovato il file\n");
		   system ("PAUSE");
		   return;
		}
		else
		   printf ("\n%s: ho aperto i file.\n\n", titolo);

		i = 0;

		   for ( ; already_ins[i] != NULL && already_exists != 0; i++) {
		      if (already_ins[i][strlen (already_ins[i]) - 1] == '\n') /*elimino il carattere '\n' alla fine che sballa tutto*/
		         already_ins[i][strlen(already_ins[i]) - 1] = '\0';    /****************************************************/
		      already_exists = strcmp (already_ins[i], titolo);
		      #if defined (DEBUG)
			 printf ("already_ins[%d] = %s\ntitolo = %s\nalready_exists = %d\n\n", i, already_ins[i], titolo, already_exists);
		      #endif
		   }
		
		if (already_exists != 0) { //se non ho trovato il titolo nell'elenco di quelli inseriti...
		   k++; //la prima volta k va a 0 (k = -1; k++)
		   already_ins[k] = calloc (strlen (titolo), sizeof (char)); //alloco uno spazio di memoria sufficiente per contenere il titolo del film
		   strcpy (already_ins[k], titolo); //...lo inserisco
		   #if defined (DEBUG)
		      printf ("already_ins[%d] = %s\n\n", i, already_ins[i]);
		   #endif
		   gioca (level, titolo, d, r, percorso_d, percorso_r, percorso_s); //inizia il gioco a punti
		}

		else { //se il titolo è già presente in quelli inseriti --> l'utente vuole rigiocare uno stesso livello
		   gioca_senza_punti (level, titolo, d, r, percorso_d, percorso_r, percorso_s);
		}

		#if defined (DEBUG)
		   printf ("%s\n", titolo);
		#endif

		printf ("Un'altra partita? (y, n) ");
		scanf ("%c%*c", &replay);

		if (replay == 'y') { //se l'utente vuole giocare un'altra partita...
		   already_exists = 1;
		   j = 0;
		   system ("clear");
		   printf ("Inserisci il titolo del film di cui vuoi le domande: ");
		   gets (titolo);
		   strcpy (percorso_d, "levels/");
		   strcpy (percorso_r, "levels/");
		   strcpy (percorso_s, "levels/");
	   	} //end if
		else if (replay != 'y' && replay != 'n') {
		   printf ("Scelta non valida.\n");
		   return;
		}

		} while (replay != 'n');

		return;
}	

void gioca (int level, char titolo[], FILE *d, FILE *r, char percorso_d[], char percorso_r[], char percorso_s[]) {

	char domanda[MAX_Q_LEN + 1];

		#if defined (DEBUG)
		   printf ("siamo in gioca\n");
		#endif

		#if defined (DEBUG)
		   printf ("percorso_d: %s\npercorso_r: %s\n", percorso_d, percorso_r);
		#endif

		while(fgets (domanda, MAX_Q_LEN, d) != NULL)
		   stampa_domande (level, domanda, percorso_s, d, r);

		printf ("Hai totalizzato %d punti.\n", punteggio);

		if (punteggio == 30 || punteggio > 30) {
		   fclose (d);
		   printf ("Complimenti!! Livello superato!\n");
		   gamesave = fopen ("gamesave/gamesave.txt", "w");
		   fprintf (gamesave, "%d", punteggio);
		   fclose (gamesave);
		   printf ("Ho salvato il punteggio\n");
		}
		else {
		   printf ("Peccato! Livello fallito!\n");
		   fclose (d);
		}		   

		return;
}

void gioca_senza_punti (int level, char titolo[], FILE *d, FILE *r, char percorso_d[], char percorso_r[], char percorso_s[]) {

	char domanda[MAX_Q_LEN + 1];


		#if defined (DEBUG)
		   printf ("siamo in gioca senza punti\n");
		#endif

		while(fgets (domanda, MAX_Q_LEN, d) != NULL)
		   stampa_domande (level, domanda, percorso_s, d, r);

		printf ("Hai totalizzato %d punti.\n", punteggio);

		if (punteggio == 30 || punteggio > 30) {
		   fclose (d);
		   printf ("Complimenti!! Livello superato!\n");
		}
		else {
		   printf ("Peccato! Livello fallito!\n");

		   fclose (d);
		}		   

		return;
}

void stampa_domande (int level, char domanda[], char percorso_s[], FILE *d, FILE *r) {

		printf ("%s ", domanda);
		j++;
		acquisisci_risposta (level, percorso_s, r);

		return;
}

void acquisisci_risposta (int level, char percorso_s[], FILE *r) {

	char risp_es[MAX_A_LEN + 1], risposta[MAX_A_LEN + 1], help[MAX_TITLE_LEN + 1];
	int check, i;
	FILE *s;

	tmp = j;

	gets (risposta);

	#if defined (DEBUG)
	   printf ("risposta: %s\n", risposta);
	#endif
	for (i = 0; risposta[i] != '\0'; i++) {
	   risposta[i] = tolower (risposta[i]);
	   if (risposta[i] == ' ')
	      risposta[i] = '-';
	}

	if ((strcmp (risposta, "h")) == 0) {

	   if (max == 0) {
	      printf ("Non hai più aiuti disponibili\n");
	      gets (risposta);
	   }
	   else {
	      s = fopen (percorso_s, "r");
		  #if defined (DEBUG)
		     if (s == NULL) {
		     printf ("Errore in apertura del file dei suggerimenti\n");
		     return; }
		  #endif

	      for (; j != 0; j--)
	         fgets (help, MAX_S_LEN, s);

	      fclose (s);
	      printf ("%s\n", help);
	      gets (risposta);
	      j = tmp;
	      max--;
	   }
	}

	fscanf (r, "%s", risp_es);

	#if defined (DEBUG)
	   printf ("risposta esatta: %s\n", risp_es);
	#endif

	check = strcmp (risp_es, risposta);

	switch (level) {
	   case 1:
	      if (check == 0) {
	         punteggio += 10;
	         #if defined (DEBUG)
	            printf ("punteggio: %d\n", punteggio);
	         #endif
	         return;
	      }
	      else {
		 #if defined (DEBUG)
	            printf ("punteggio: %d\n", punteggio);
	         #endif
	         return; } break;
	   case 2:
              if (check == 0) {
	         punteggio += 8;
	         #if defined (DEBUG)
	            printf ("punteggio: %d\n", punteggio);
	         #endif
	         return;
	      }
	      else {
	         punteggio -= 1;
		 #if defined (DEBUG)
	            printf ("punteggio: %d\n", punteggio);
	         #endif
	         return; } break;

	   case 3:
	      if (check == 0) {
	         punteggio += 8;
	         #if defined (DEBUG)
	            printf ("punteggio: %d\n", punteggio);
	         #endif
	         return;
	      }
	      else {
	         punteggio -= 2;
		 #if defined (DEBUG)
	            printf ("punteggio: %d\n", punteggio);
	         #endif
	         return; } break;
	}
}

void livello_finale (void) {

	FILE *d, *r;
	char frase[MAX_Q_LEN + 1];

		d = fopen ("levels/livello-finale/domande.txt", "r");
		r = fopen ("levels/livello-finale/risposte.txt", "r");

		while(fgets (frase, MAX_Q_LEN, d) != NULL)
		   stampa_frase (frase, r);

		fclose (d);

		printf ("Hai totalizzato %d punti\n", punteggio);
		

		return;
}

void stampa_frase (char domanda[], FILE *r) {

	char frase[MAX_Q_LEN + 1];

		printf ("%s ", domanda);
		risposta (r);
		return;
}

void risposta (FILE *r) {

	int i, check;
	char risposta[MAX_A_LEN + 1], risp_es[MAX_A_LEN + 1];

		gets (risposta);

		for (i = 0; risposta[i] != '\0'; i++) {
	   	   risposta[i] = tolower (risposta[i]);
	   	   if (risposta[i] == ' ')
	      	      risposta[i] = '-';
	    	}

		fscanf (r, "%s", risp_es);

		#if defined (DEBUG)
		   printf ("risposta utente: %s\n", risposta);
	   	   printf ("risposta esatta: %s\n", risp_es);
		#endif

		check = strcmp (risp_es, risposta);

		if (check == 0) {
		   punteggio++;
		   printf ("ESATTO! Punteggio = %d\n", punteggio);
		   return;
		}

		else {
		   printf ("ERRATO! Punteggio = %d\n", punteggio);
		   return;
		}
}

void stampa_credits (void) {

	FILE *c;
	char linea[MAX_Q_LEN + 1];
	
	system ("clear");

	c = fopen ("credits.txt", "r");

	while ((fgets (linea, MAX_Q_LEN, c)) != NULL)
	   printf ("%s", linea);

	putchar ('\n');
	system ("PAUSE");

	return;
}

void stampa_readme (void) {

	FILE *c;
	char linea[MAX_Q_LEN + 1];
	
	system ("clear");

	c = fopen ("readme.txt", "r");

	while ((fgets (linea, MAX_Q_LEN, c)) != NULL)
	   printf ("%s", linea);

	putchar ('\n');
	system ("PAUSE");

	return;
}

void stampa_film_su_file (FILE *films) {

	int i;

		films = fopen ("gamesave/films.txt", "w");

		for (i = 0; already_ins[i] != NULL; i++)
		   fprintf (films, "%s\n", already_ins[i]);

		fclose (films);
	
		return;
}

void stampa_elenco_film (void) {

	FILE *films;
	char film[MAX_TITLE_LEN + 1];

		films = fopen ("titoli.txt", "r");

		while(fgets (film, MAX_TITLE_LEN, films) != NULL)
		   printf ("%s\n", film);

		system ("PAUSE");
	
		return;
}
