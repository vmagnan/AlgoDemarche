#include <stdio.h>
#include <stdlib.h>
#include "libISEN/BmpLib.h"
#include "matrice.h"
#include "analyse.h"
#include "toolbox.h"


void chargeImage(char nomFichier[11],DonneesImageRGB **img)	// Charge une image grâce au nom de celle-ci
{
	*img = lisBMPRGB(nomFichier);
	if((*img == NULL))
	{
		fprintf(stderr, "erreur l'image %s n'a pas pu être charger", nomFichier);
	}
}

image rgbToHsv(troimat t) //Pour 1 photo
{
	image tp; //tableau de h s v
	determ det;

	for(int i = 0; i < HAUTEUR; i++)
	{
		
		for(int j = 0; j < LARGEUR; j++)
		{
			determineMinColor(t.b[i][j],t.v[i][j],t.r[i][j],&det);	// Determine le max et le min pour effectuer les calculs de H S et V
			determineMaxColor(t.b[i][j],t.v[i][j],t.r[i][j],&det);

			tp.h[i][j]=calculH(det,t.r[i][j],t.v[i][j],t.b[i][j]); // Rentre les valeurs de H S et V dans un tableau de pixel(une image)
			tp.s[i][j]=calculS(det);
			tp.v[i][j]=(det.max.v*100)/255;		
		}
	}
	//~ for(int k=0;k<600;k++)
			//~ {
				//~ printf("\ntp.h[%d][608]= %d\n",k,tp.h[k][608]);
				//~ printf("tp.s[%d][608]= %d\n",k,tp.s[k][608]);
				//~ printf("tp.v[%d][608]= %d\n",k,tp.v[k][608]);
				
			//~ }
	return tp;
}

void determineMinColor(int bleu,int vert,int rouge,determ *det)
{

	if(bleu<=vert && bleu <=rouge)
	{
		det->min.c=1;
		det->min.v=bleu;
	}
	else if(vert<=bleu && vert<=rouge)
	{
		det->min.c=2;
		det->min.v=vert;
	}
	else if(rouge<=vert && rouge <=bleu)
	{
		det->min.c=3;
		det->min.v=rouge;
	}
}

void determineMaxColor(int bleu,int vert,int rouge,determ *det)
{

	if(bleu>=vert && bleu >=rouge)
	{
		det->max.c=1;
		det->max.v=bleu;
	}
	else if(vert>=bleu && vert>=rouge)
	{
		det->max.c=2;
		det->max.v=vert;
	}
	else if(rouge>=vert && rouge >=bleu)
	{
		det->max.c=3;
		det->max.v=rouge;
	}
}

int calculS(determ det)
{
	if(det.max.v==0)
	return 0;
	else
	return 1-(det.min.v/det.max.v);

}

int calculH(determ det,int r,int g,int b)
{
	int offset=10;
	if(abs(r-g)<=offset && abs(r-b)<=offset &&  abs(g-b)<=offset)
	{
		return -100;
	}
	if(det.max.v==det.min.v)
	{
		return 0;
	}
	else if(det.max.c==3) // r
	{
		return (int)(60*((float)(g-b)/(det.max.v-det.min.v))+360)%360;
	}
	else if(det.max.c==2) // v
	{
		return 60*((float)(b-r)/(det.max.v-det.min.v))+120;
	}
	else if(det.max.c==1) // b
	{
		return 60*((float)(r-g)/(det.max.v-det.min.v))+240;
	}
	else
	return -1;
}




void ChangePixCouleurImg(image t,int color[HAUTEUR][LARGEUR])
{
	int offset = 1;
	int target = 180;
	for(int i = 0; i < HAUTEUR; i++)
		{

			for(int j = 0; j < LARGEUR; j++)
			{
				//~ if((t.h[i][j] < target + offset && t.h[i][j] > target - offset) && t.s[i][j] > 20)
				if((t.h[i][j] < target + offset && t.h[i][j] > target - offset ) /*&& t.s[i][j] == 100 && t.v[i][j] == 100 */)
				{
				color[i][j]= 0; // On va le mettre en blanc
				//~ printf("Rouge\n");
				}
				else 
				{
				color[i][j]= -1; // On va le mettre en noir
				//~ printf("autre\n");
				}
			}
		}	
}

jointure identifieColor(image tp,DonneesImageRGB *img)
{
		jointure joint; // Position de chaques point appartenant à la couleur de certaines jointures
		int color[HAUTEUR][LARGEUR];	// Couleur détectée
		int a=0;	// Variable d'incrémentation pour les position des points
		initialiseTabPoint(&joint); // Initialise les cases du tableau à 0
		printf("hauteurimage=%d\n",img->hauteurImage);
		ChangePixCouleurImg(tp,color);
		for(int i = 0; i < HAUTEUR; i++)
		{
			for(int j = 0; j < LARGEUR; j++)
			{
				if (color[i][j] == 0) // Correspond à la couleur verte
				{
					joint.j[0].position[a].y=i;	// Ce 1 correspond aussi à la couleur Rouge	
					joint.j[0].position[a].x=j;
					a++;
					joint.j[0].nb++;
				}
				else if (color[i][j] == -1) 
				{
					
				}
			}
			
			
		}
		
		return joint;
}

void initialiseTabPoint(jointure *t)
{
	for(int i=0;i<JOINT;i++)
	{
		t->j[i].nb=0;
		for(int j=0;j<MAXPIXJOINT;j++)
		{
			t->j[i].position[j].x=0;
			t->j[i].position[j].y=0;
		}
	}
}

void sommePointJoint(jointure *t) // Trouve les coordonnées du centre des cercle (jointure)
{
	int i;
	int sommation_x=0;
	int sommation_y=0;
	
	for(i=0;i<t->j[0].nb;i++)
	{
		sommation_x+=t->j[0].position[i].x;
		sommation_y+=t->j[0].position[i].y;
	}
	if(t->j[0].nb != 0)
	{
		t->j[0].centre.x=sommation_x/t->j[0].nb;
		t->j[0].centre.y=sommation_y/t->j[0].nb;
	}
}
