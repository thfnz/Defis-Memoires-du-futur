// Manipulation directe des ports
#define pON(port, pin) (port |= bit(pin)) // port OR bit(pin) - Passage à l'état HAUT
#define pOFF(port, pin) (port &= ~ bit(pin)) // port AND NOT bit(pin) - Passage à l'état BAS
#define lect(port, pin) (port &= bit(pin)) // port AND bit(pin) - Lecture

// Pins à brancher.
const int WB = 0;
const int SA = 1;
const int SL = 2;
const int WL = 3;
const int PRE = 4;
const int BL = 5;
const int SET_PARA = 6;
const int CLOCK = 7;
const int SC_IN = 8;
const int SC_OUT = 9;
const int SC_SEL_ZERO = 10;
const int SC_SEL_UN = 11;

// Définition des fonctions de base du programme (cf chronogrammes)
void clk(){
	// Simule un coup de clock.
	// Il est important de noter ici que la carte à pointe prend en compte le changement d'état de ses entrée..
	// uniquement quand elle reçoit un front montant sur le pin CLOCK.
	// Le rapport cyclique du signal créneau envoyé n'a donc aucune importance.
	pON(PORTD, CLOCK);
	delayMicroseconds(1);
	pOFF(PORTD, CLOCK);
}

void scWLSL(int ligne){
	// Permet la sélection de la ligne sur laquelle les opérations de lecture et d'écriture vont être effectuées.

	// Amélioration possible : usage tableaux dynamiques entrée utilisateur pour selection plusieurs lignes
	// (et donc stockage potentiel de nombres codés sur + que 128 bits)

	// Sélection de la bonne Scan Chain (cf 4.4.2.1 de la thèse de Mr Francois)
	pON(PORTB, SC_SEL_ZERO);
	pOFF(PORTB, SC_SEL_UN);


	for(int i = 0; i < 128; i++){
		if(i == ligne){ // Si la i-ième ligne est la ligne choisie
			pON(PORTB, SC_IN);
			clk();
			pOFF(PORTB, SC_IN);
		} else clk();
	}
}

int scOut(){
	// Permet de lire la valeur stockée dans la ligne préalablement choisie par l'appel de la fonction scWLSL.

	// Sélection de la bonne Scan Chain (cf 4.4.2.1 de la thèse de Mr Francois)
	pOFF(PORTB, SC_SEL_ZERO);
	pOFF(PORTB, SC_SEL_UN);

	int resultat = 0; // Valeur stockée dans la ligne choisie

	for(int i = 0; i < 128; i++){ // Lecture des 128 blocs d'une ligne
		clk();

		// A VERIFIER SUR CARTE -----------------------------------------------
		if (lect(PORTB, SC_OUT) != 0){ // la i-ième cellule contient un 1
			resultat += pow(2, i);
		}
	}

	return resultat;
}

void scBL(int pos){
	// Fonction semblable à scWLSL. Utile si implémentation de l'écriture unitaire.
	pOFF(PORTB, SC_SEL_ZERO);
	pON(PORTB, SC_SEL_UN);
	for(int i = 0; i < 128; i++){
		if(i == pos){
			pON(PORTB, SC_IN);
			clk();
			pOFF(PORTB, SC_IN);
		} else clk();
	}
}

void zeroPara(int ligne){
	// Programmation parallèle d'un 0 sur toute la ligne en argument.

	// Sélection de la ligne
	scWLSL(ligne);

	// Copie du chronogramme (cf 4.4.1.1 de la thèse de mr Francois)
	pON(PORTD, PRE);
	clk();
	pON(PORTD, WL);
	clk();
	clk();
	pOFF(PORTD, PRE);
	clk();
	pON(PORTD, SL);
	clk();
	clk();
	pOFF(PORTD, SL);
	clk();
	pON(PORTD, PRE);
	clk();
	clk();
	pOFF(PORTD, WL);
	clk();
	pOFF(PORTD, PRE);
	clk();
}

void unPara(int ligne){
	// Programmation parallèle d'un 1 sur toute la ligne en argument.

	// Sélection de la ligne
	scWLSL(ligne);

	// Copie du chronogramme (cf 4.4.1.1 de la thèse de mr Francois)
	pON(PORTD, PRE);
	clk();
	pON(PORTD, WL);
	clk();
	clk();
	pOFF(PORTD, PRE);
	clk();
	pON(PORTD, BL);
	clk();
	clk();
	pON(PORTD, PRE);
	clk();
	clk();
	pOFF(PORTD, WL);
	clk();
	pOFF(PORTD, PRE);
	clk();
}

void lecture(int ligne, int writeBack){
	// Lecture du contenu d'une ligne.
	// writeBack == 1 active le Write-Back. L'opération de lecture étant normalement destructive,
	// ce paramètre doit être activé si l'on souhaite réutiliser plus tard le contenu de la ligne.

	// Sélection de la ligne
	scWLSL(ligne);

	// Copie du chronogramme (cf 4.4.1.1 de la thèse de mr Francois)
	pON(PORTD, SET_PARA);
	clk();
	pON(PORTD, PRE);
	clk();
	pON(PORTD, WL);
	clk();
	clk();
	pOFF(PORTD, PRE);
	clk();
	pON(PORTD, SL);
	clk();
	pON(PORTD, SA);
	clk();
	clk();
	clk();
	clk();
	pOFF(PORTD, SL);
	pOFF(PORTD, SA);
	if(writeBack == 1){
		pON(PORTD, WB);
	}
	clk();
	pON(PORTD, PRE);
	pOFF(PORTD, WB);
	clk();
	clk();
	pOFF(PORTD, WL);
	clk();
	pOFF(PORTD, PRE);
	clk();
	pOFF(PORTD, SET_PARA);
	clk();
}

void setup(){
	pinMode(WB, OUTPUT);
	pinMode(SA, OUTPUT);
	pinMode(SL, OUTPUT);
	pinMode(WL, OUTPUT);
	pinMode(PRE, OUTPUT);
	pinMode(BL, OUTPUT);
	pinMode(CLOCK, OUTPUT);
	pinMode(SC_IN, OUTPUT);
	pinMode(SC_SEL_ZERO, OUTPUT);
	pinMode(SC_SEL_UN, OUTPUT);

	pinMode(SC_OUT, INPUT);
}

void loop(){
	
}