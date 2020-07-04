#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "limace.h"

/*
* Representation d'un histogramme sous forme d'une image
* Entrees : histogramme et nombre de lignes (par exemple 256) de l'image resultat
* Sortie : image resultat
*/
Image Hist2Im(Matrix Hist, int NbLig)
{
    unsigned char **I;
    int *h,i,j,Max=0,NbCol=256;
    Image Resultat;
    
    if (MatType(Hist)!=Int) return NULL;
    Resultat=ImCAlloc(GrayLevel,NbLig,NbCol);
    h=*MatGetInt(Hist);
    for (j=0;j<NbCol;j++)
        if (h[j]>Max) Max=h[j];
    I=ImGetI(Resultat);
    for (j=0;j<256;j++)
        for (i=NbLig-1;i>=(NbLig-NbLig*h[j]/Max);i--)
            I[i][j]=255;
    return Resultat;    
}

void errorNullOne(Image image){
	if(image == NULL){
		fprintf(stderr, "Error null return for ImGet\n");
	}
}

void errorNullTwo(unsigned char **componentPixel){
	if(componentPixel == NULL){
		fprintf(stderr, "Error null return for ImGet\n");
	}
}

void errorZero(int linOrCol){
	if(linOrCol == 0){
		fprintf(stderr, "Error number of lines or columns\n");
	}
}

void rowCol(int* row, int* col, Image image){
	*row = ImNbRow(image);
	*col = ImNbCol(image);
	errorZero(*row);
	errorZero(*col);
}

/*
 * INPUT: colored image
 * OUTPUT: grey level image
 * */
Image exo1Seance1(const char* fileName){
	//Initialisation of variables
	Image colorImg = ImRead(fileName);
	int nbCol, nbRow;
	rowCol(&nbRow, &nbCol, colorImg);
	unsigned char **red = ImGetR(colorImg);
	unsigned char **green = ImGetG(colorImg);
	unsigned char **blue = ImGetB(colorImg);
	Image greyImg = ImAlloc(GrayLevel, nbRow, nbCol);
	unsigned char **greyLvImg = ImGetI(greyImg);
	//Verify that those variables have no errors
	errorNullOne(colorImg);
	errorNullTwo(red);
	errorNullTwo(green);
	errorNullTwo(blue);
	errorNullTwo(greyLvImg);
	//Compute new image
	for(int i = 0; i < nbRow; i++){
		for(int j = 0; j < nbCol; j++){
			greyLvImg[i][j] = round(0.299 * red[i][j] + 
				0.587 * green[i][j] + 0.114 * blue[i][j]);
		}
	}
	//Write new image in file
	ImWrite(greyImg, "chenille.pgm");
	//Free memory
	ImFree(&colorImg);
	
	return(greyImg);
}

/*
 * INPUT: grey level image(chenille.pgm)
 * OUTPUT: histogram of grey level image(chenille-hist.mx)
 * */
int** exo2Seance1(Image greyImage){
	//Initialisation of variables
	int nbRow, nbCol;
	rowCol(&nbRow, &nbCol, greyImage);
	unsigned char **greyLvImg = ImGetI(greyImage);
	Matrix matHisto = MatCAlloc(Int, 256, 1);
	int **matInt = MatGetInt(matHisto);
	//compute histogram
	for(int i = 0; i < nbRow; i++){
		for(int j = 0; j < nbCol; j++){
			matInt[greyLvImg[i][j]][0]++;
		}
	}
	//Print histogram in console and in file
	MatWriteAsc(matHisto,"chenille-hist.mx");
	//production of the image of the histogram
	Image outputHisto = Hist2Im(matHisto, 256);
	ImWrite(outputHisto, "histogramGrey");
	//Free memory
	ImFree(&outputHisto);
	return(matInt);
}

/*
 * INPUT: void
 * OUTPUT: binary image by threshold
 * */
Image exo3Seance1(){
	//Initialisation of variables
	Image greyImg = exo1Seance1("chenille.ppm");
	int nbRow, nbCol;
	rowCol(&nbRow, &nbCol, greyImg);
	unsigned char **greyLvImg = ImGetI(greyImg);
	Image imgBlackWhite = ImAlloc(BitMap, nbRow, nbCol);
	unsigned char ** greyLvImgBW = ImGetI(imgBlackWhite);
	//Verification of variables
	errorNullTwo(greyLvImgBW);
	//Compute black and white image
	for(int i = 0; i < nbRow; i++){
		for(int j = 0; j < nbCol; j++){
			if(greyLvImg[i][j] < 22){
				greyLvImgBW[i][j] = 0;
			}
			else{
				greyLvImgBW[i][j] = 1;
			}
		}
	}
	//Write new image in file
	ImWrite(imgBlackWhite, "chenilleBW.pbm");
	//Free memory
	ImFree(&greyImg);
	return(imgBlackWhite);
}

/*
 * INPUT: colored image
 * OUTPUT: lsd image
 * */
Image exo4Seance1(const char* fileName){
	//Initialisation of variables
	Image colorImg = ImRead(fileName);
	int nbRowColor = ImNbRow(colorImg);
	int nbColColor = ImNbCol(colorImg);
	unsigned char **red = ImGetR(colorImg);
	unsigned char **green = ImGetG(colorImg);
	unsigned char **blue = ImGetB(colorImg);
	Image lsdImg = ImAlloc(Color, nbRowColor, nbColColor);
	unsigned char **redLsd = ImGetR(lsdImg);
	unsigned char **greenLsd = ImGetG(lsdImg);
	unsigned char **blueLsd = ImGetB(lsdImg);
	//Verification of variables
	errorNullOne(colorImg);
	errorZero(nbRowColor);
	errorZero(nbColColor);
	errorNullTwo(red);
	errorNullTwo(green);
	errorNullTwo(blue);
	errorNullOne(lsdImg);
	//Compute new image
	for(int i = 0; i < nbRowColor; i++){
		for(int j = 0; j < nbColColor; j++){
			if(red[i][j] < 128){
				redLsd[i][j] = 0;
			}
			else{
				redLsd[i][j] = 255;
			}
			if(green[i][j] < 128){
				greenLsd[i][j] = 0;
			}
			else{
				greenLsd[i][j] = 255;
			}
			if(blue[i][j] < 128){
				blueLsd[i][j] = 0;
			}
			else{
				blueLsd[i][j] = 255;
			}
		}
	}
	//Write new image in file
	ImWrite(lsdImg, "chenille-lsd.ppm");
	//Free memory
	ImFree(&lsdImg);
	return(lsdImg);
}

/*
 * INPUT: (grey image) X (file of ponctual transformation)
 * OUTPUT: image tranformed by ponctual transformation
 * */
Image exo1Seance2(char * fileOfPonctualTransformation, Image greyImg){
	//initialisation of variables
		//grey image
	int nbRow, nbCol;
	rowCol(&nbRow, &nbCol, greyImg);
	unsigned char **greyLvImg = ImGetI(greyImg);
		//matrix of the transformation
	Matrix matTransform = MatReadAsc(fileOfPonctualTransformation);;
	int **matIntTransform = MatGetInt(matTransform);
		//output image
	Image imgGreyTransform = ImAlloc(GrayLevel, nbRow, nbCol);
	unsigned char **imgGreyLvlTransform = ImGetI(imgGreyTransform);
	//computation for poncutal transformation
	for(int i = 0; i < nbRow; i++){
		for(int j = 0; j < nbCol; j++){
			imgGreyLvlTransform[i][j] = matIntTransform[greyLvImg[i][j]][0];
		}
	}
	//Results
	ImWrite(imgGreyTransform, "imagePonctualTransform");
	//Free memory
	ImFree(&greyImg);
	MatFree(&matTransform);
	return(imgGreyTransform);
}

/*
 * INPUT: histogram grey level
 * OUTPUT: cumulated histogram
 * */
int** exo2Seance2(int** histogram){
	//variables
	Matrix matHistoCumul = MatCAlloc(Int, 256, 1);
	int **matIntHistoCumul = MatGetInt(matHistoCumul);
	//compute cumulated histogram 
	for(int i = 0; i < 256; i++){
		if(i == 0){
			matIntHistoCumul[0][0] = histogram[0][0];
		}
		else{
			matIntHistoCumul[i][0] = matIntHistoCumul[i - 1][0] + histogram[i][0];
		}
	}
	//write cumulated histo to file
	MatWriteAsc(matHistoCumul,"vaisseaux-cumhist.mx");
	return(matIntHistoCumul);
}

/* NEED TO BE SIMPLIFIED!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * INPUT: grey level image (vaisseaux.pgm) 
 * OUTPUT: (punctual transformation in file vaisseaux-eq1.mx) X (image in file vaisseaux-eq1.pgm) 
 * X (visualise histo of vaisseaux.pgm) X (visualise histo of vaisseaux-eq1.pgm)
 * */
void exo3Seance2(Image greyImage){
	int nbRow = ImNbRow(greyImage);
	int nbColumn = ImNbCol(greyImage);
	unsigned char **greyLvImg = ImGetI(greyImage);
	Matrix matHisto = MatCAlloc(Double, 256, 1);
	double **matIntHisto = MatGetDouble(matHisto);
	Matrix matHistoCumul = MatCAlloc(Double, 256, 1);
	double **matIntHistoCumul = MatGetDouble(matHistoCumul);
	Matrix matHistoCumulNorm = MatCAlloc(Double, 256, 1);
	double **matIntHistoCumulNorm = MatGetDouble(matHistoCumulNorm);
	double nbTotPix = nbRow * nbColumn;
	Matrix matF = MatCAlloc(Int, 256, 1);
	int **matDoubF = MatGetInt(matF);
	//compute histo
	for(int i = 0; i < nbRow; i++){
		for(int j = 0; j < nbColumn; j++){
			matIntHisto[greyLvImg[i][j]][0]++;
		}
	}
	//compute histo cumul
	for(int i = 0; i < 256; i++){
		if(i == 0){
			matIntHistoCumul[0][0] = matIntHisto[0][0];
		}
		else{
			matIntHistoCumul[i][0] = matIntHistoCumul[i - 1][0] + matIntHisto[i][0];
		}
	}
	//compute histo cumul norm
	for (int i = 0; i < 256; i++){
		matIntHistoCumulNorm[i][0] = matIntHistoCumul[i][0] /  nbTotPix;//divide each value of cumulated histo by n(number of pixels of the image
	}
	//compute trans ponct 	//use the formula f(i)=[255hc(i)] with hc histo cumul normalized
	for (int i = 0; i < 256; i++){
		matDoubF[i][0] = round(255 * matIntHistoCumulNorm[i][0]);
	}
	MatWriteAsc(matF, "vaisseaux-eq1.mx");
	for(int i = 0; i < nbRow; i++){
		for(int j = 0; j < nbColumn; j++){
			greyLvImg[i][j] = matDoubF[greyLvImg[i][j]][0];
		}
	}
}

int main(){
	exo1Seance1("chenille.ppm"); OK
	//~ exo2Seance1(exo1Seance1("chenille.ppm")); OK
	//~ exo3Seance1(); OK
	//~ exo4Seance1("chenille.ppm"); OK
	//~ exo1Seance2("lut-inv.mx", exo1Seance1("chenille.ppm")); OK
	//~ exo2Seance2(exo2Seance1(exo1Seance1("chenille.ppm")));   OK
	//exo3Seance2(ImRead("vaisseaux.pgm")); //NOT OK!!!!!!!!!!!!!
	return(0);
}

//~ int argc, char* argv[]
//~ printf("TEST");
