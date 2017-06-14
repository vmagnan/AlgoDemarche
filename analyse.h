#ifndef ANALYSE_H
#define ANALYSE_H
#include "matrice.h"
#define JOINT 5	// Jointures des articulation Main Coude Pied Epaule Genoux
#define MAXPIXJOINT 200000	// Jointures des articulation

typedef struct image{
	int h[HAUTEUR][LARGEUR],s[HAUTEUR][LARGEUR],v[HAUTEUR][LARGEUR]; // Repere HSV	H: Hue	S: Saturation	V: Value
}image;

typedef struct infocoul{
	int c; // 1=Bleu	2=Vert	3=Rouge
	int v; // Valeur
}infocoul;

typedef struct determ{
	infocoul min,max; // Valeur Min et Max + Info sur la couleur correspondante
}determ;

typedef struct posi{
	int x,y;
}posi;

typedef struct posf{
	float x,y;
}posf;

typedef struct info{
	posi position[MAXPIXJOINT]; // y puis x
	int nb;
	posi centre;
}info;

typedef struct jointure{
	info j[JOINT];
	
}jointure;


image rgbToHsv(troimat t);

void chargeImage(char nomFichier[11],DonneesImageRGB **img);

void determineMaxColor(int bleu,int vert,int rouge,determ *det);
void determineMinColor(int bleu,int vert,int rouge,determ *det);

int calculS(determ det);
int calculH(determ det,int r,int g,int b);


void ChangePixCouleurImg(image p,int color[HAUTEUR][LARGEUR]);
void identifieColor(image tp,jointure pic[NBIMAGE],int z);
void initialiseTabPoint(jointure pic[NBIMAGE],int z);
void sommePointJoint(jointure *pic);

#endif
