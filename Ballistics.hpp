/*
	Ballistics: Composant de foncteur de trajectoire.


	Dependance: Entity


	Selon la valeur énumérée Type, procède à ReturnMove()

	Pour l'instant, on n'appliquera ça qu'aut entités non controlées par IA ni Joueur.
	Mais il serait possible d'étendre le modèle, en fournissant la direction souhaitée ( 9 valeurs possibles)
	et en "modulant" le mouvement.

	pour les 9 valeurs: Cf le pavé numérique: 1 <-> bas gauche, 5 <-> mouvement nul, 8 <-> haut ...

*/

// NOTA syntaxe: Pass: void foo(float (*pt2Func)(float, float));
//		 Decl: int (TMyClass::*pt2ConstMember)(float, char, char) const = NULL;     // C++
// TMyClass instance1;
// int result3 = (instance1.*pt2Member)(12, 'a', 'b');   // C++
// int result4 = (*this.*pt2Member)(12, 'a', 'b');       // C++ if this-pointer can be used	

/*
//------------------------------------------------------------------------------------
// 2.7 How to Return a Function Pointer
//     'Plus' and 'Minus' are defined above. They return a float and take two float


// Direct solution: Function takes a char and returns a pointer to a
// function which is taking two floats and returns a float. <opCode>
// specifies which function to return
float (*GetPtr1(const char opCode))(float, float)
{
   if(opCode == '+')
      return &Plus;
   else
      return &Minus; // default if invalid operator was passed
}


// Solution using a typedef: Define a pointer to a function which is taking
// two floats and returns a float
typedef float(*pt2Func)(float, float);

// Function takes a char and returns a function pointer which is defined
// with the typedef above. <opCode> specifies which function to return
pt2Func GetPtr2(const char opCode)
{
   if(opCode == '+')
      return &Plus;
   else
      return &Minus; // default if invalid operator was passed
}


// Execute example code
void Return_A_Function_Pointer()
{
   cout << endl << "Executing 'Return_A_Function_Pointer'" << endl;

   // define a function pointer and initialize it to NULL
   float (*pt2Function)(float, float) = NULL;

   pt2Function=GetPtr1('+');   // get function pointer from function 'GetPtr1'
   cout << (*pt2Function)(2, 4) << endl;   // call function using the pointer


   pt2Function=GetPtr2('-');   // get function pointer from function 'GetPtr2'
   cout << (*pt2Function)(2, 4) << endl;   // call function using the pointer
}
*/
class Ballistics : public Entity {

	typedef struct {float dx; float dy;} Coord;
	enum Direction
	{
		TL, TT, TR,	//Top
		LL, XX, RR,	//Middle  -> XX:= Neutral.
		BL, BB, BR,	//Bottom
	};

	enum Type
	{
	        LINEAR,	// Mouvement linéaire

		CIRCLE, // Trajectoire décrivant un cercle (fournir rayon et coord du centre à la création)

		SQUARE, // Trajectoire polynome du second degré (fournir a, b, et c à la création - origine implicite)

		SINE,   // Trajectoire sinusoidale (fournir l'amplitude du mouvement et origine à la création)

		NONE	// Immobilise l'objet
	};
	
	// Typedef des pointeurs de fonctions à 1 argument Type (Templatable mais inutile?)
	// -> Alléger syntaxe des appels
	typedef float(*CallBackSingle(Type);

	// Renvoie l'adresse de la methode de mouvement demandée
	CallBackSingle GetMoveMethod(const Type type);

	// Renvoie les coordonnées avec lesquelles incrémenter.
	Coord& ReturnMove (Direction dir);





}
