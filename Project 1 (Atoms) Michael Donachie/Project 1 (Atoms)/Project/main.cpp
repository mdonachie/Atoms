/*
 * Name: Michael Donachie
 * Project 1: Atoms
 * Course: CSI108 (Spring 2025)
 * Date: April 12, 2025
 * Description: Enter/alter number of subatomic particles for a set of
 *				atoms and determine the type of element, charge, etc.
 *				based on those subatomic particles.
 */

#include <iostream>
#include <string>
#include <cctype>  // for tolower()

using namespace std;

// Atom class definition.
class Atom
{
public:
	// Initialize to hydrogen.
	Atom();

	// Initialize based on how many of each subatomic particle.
	Atom(int initProtons, int initNeutrons, int initElectrons);
	
	// Output symbol and how many of each subatomic particle.
	void display() const;

	// Return charge based on how many protons and electrons.
	int charge() const;

	//Some getters 
	int getProtons() const;
	int getNeutrons() const;
	int getElectrons() const;

	//Mutator to set the number of neutrons... could produce an isotope. 
	void setNeutrons(int newtrons);

	//Return atomic symbol from array based off of protons. 
	string getSymbol(int protons) const;

	// Declaration for our elements table for 118 elements.
	static const string elementTable[118];

	// Declaration for static member func to return t/f if 2 atoms are different isotopes.
	static bool isotopeChecker(const Atom& atom1, const Atom& atom2);

	//Overloaded operator function declarations.. is the right term protoype or declaration?
	Atom& operator ++();

	friend Atom operator --(Atom& atom);

	//The three necessary requirements for dynamic array allocation: Copy constructor, Assignment, Destructor. 
	Atom(const Atom& other);
	Atom& operator=(const Atom& other);
	~Atom();

private:
	// How many of each subatomic particle.
	int protons;
	int neutrons;
	int electrons;
};

// Prototypes for functions dealing with atoms.

//Equality operator declared globally 
bool operator ==(const Atom& atom1, const Atom& atom2);

int main()
{
	//Array of atom objects to process.
	//const int NUM_ATOMS = 4;
	//Atom atoms[NUM_ATOMS];
	Atom* atoms;
	int numAtoms;

	cout << "Please enter the amount of atoms to create: " << endl;
	cin >> numAtoms;

	while (numAtoms < 1 || numAtoms > 118) 
	{
		cerr << "Invalid input, enter at least 1 atom and no more than 118. " << endl;
		cin >> numAtoms;
	}

	//Dynamically allocated array 
	atoms = new Atom[numAtoms];          
	  
// Repeatedly allow selecting an atom and performing
// an atomic operation until user chooses to quit.
while (true)
{
	// Display current state of atoms.
	cout << "\nAtoms:" << endl;
	for (int i = 0; i < numAtoms; i++)
	{
		cout << (i + 1) << ") ";
		atoms[i].display();
		cout << " [" << showpos << atoms[i].charge() << noshowpos << "]";
		cout << endl;
	}

	// Prompt user to enter atom # for operation.
	// Note: User enters 1-based indices although
	// array indices are zero-based.
	int atomNum;
	cout << "Operate on which atom (0 = quit)> ";
	cin >> atomNum;

	// Done with atoms?
	if (atomNum == 0)
		break;

	// Validate that atom # is in range. If not,
	// require the user to re-enter.
	if (atomNum < 1 || atomNum > numAtoms)
	{
		cerr << "Atom # must be 1 to " << numAtoms << endl;
		continue;
	}

	// Store reference to atom on which to operate.
	Atom& opAtom = atoms[atomNum - 1];

	// Allow user to choose atomic operation.
	cout << "\nAtomic operation:" << endl;
	cout << "enter a)tom properties" << endl;
	cout << "adjust n)eutrons" << endl;
	cout << "adjust c)harge" << endl;
	cout << "different i)sotopes" << endl;
	cout << "Operation> ";
	char operation;
	cin >> operation;

	// Values entered by user to set or alter atom.
	int numProtons;
	int numNeutrons;
	int numElectrons;

	// Process requested atomic operation (case insensitive).
	switch (tolower(operation))
	{
	case 'a':
		cout << "Enter # protons: ";
		cin >> numProtons;
		cout << "Enter # neutrons: ";
		cin >> numNeutrons;
		cout << "Enter # electrons: ";
		cin >> numElectrons;
		opAtom = Atom(numProtons, numNeutrons, numElectrons);
		break;

	case 'n':
		cout << "Enter new # neutrons: " << endl;
		cin >> numNeutrons;
		opAtom.setNeutrons(numNeutrons);  
		//User input gets passed into function -> becomes new # of neutrons

		break;

	case 'c':
	{
		//Read in user input, then use a nested switch statement to process it 
		//Uses our pre-increment and pre-decrement operator
		//Documentation: https://en.cppreference.com/w/cpp/language/operator_incdec 
		char dir;
		cout << "Provide a symbol to change charge direction (+/-): " << endl;
		cin >> dir;

		switch (dir)
		{
		case '+':
			++opAtom;
			cout << "Increased charge to: " << opAtom.charge() << endl;
			break;

		case '-':
			--opAtom;
			cout << "Decreased charge to: " << opAtom.charge() << endl;
			break;
		default:
			cerr << "Invalid symbol. Use + or - " << endl;
			break;
		}
	}
		break;

	case 'i':
	{
		//If # of protons in an element are not equal to # neutrons in the same element, an isotope is created.
		//Documentation: https://www.iaea.org/newscenter/news/what-are-isotopes
		//For fun since some isotopes are unstable and can be radioactive
		//I added a small text color change to green while displaying isotopes 
		//Using ANSI escape color codes
		//Documentation: https://gist.github.com/Kielx/2917687bc30f567d45e15a4577772b02
		cout << "Isotopes of current atom: " << endl;

		bool isotopes = false;

		//Step through all atoms. Check if others are isotopes using our function
		for (int i = 0; i < numAtoms; i++)
		{
			if (i != (atomNum - 1) && Atom::isotopeChecker(opAtom, atoms[i]))
			{
				cout << "\033[32m"; //Green text
				cout << "Atom " << (i + 1) << ": ";
				atoms[i].display();
				cout << endl;
				cout << "\033[0m"; //Revert text 
				isotopes = true;
			}
		}
		if (!isotopes)
		{
			cout << "No other isotopes found. " << endl;
		}
	}
		break;

	default:
		cerr << "Unknown atomic operation: " << operation << endl;
		break;
	}
}

//Free memory, and then prevent dangling ptr
delete[] atoms;
atoms = NULL;

return 0;
}

//Dynamic allocation definitions

//Copy constructor, creates a copy of Atom initialized with values from source
Atom::Atom(const Atom& other)
	: protons(other.protons), neutrons(other.neutrons), electrons(other.electrons)
{}

//Assignment operator, avoids self assignment
Atom& Atom::operator =(const Atom& other)
{
	if (this != &other)
	{
		protons = other.protons;
		neutrons = other.neutrons;
		electrons = other.electrons;
	}
	return *this;
}

//Destructor
Atom::~Atom()
{
	cout << "Destructor called (debug)";
}

// Definitions for functions dealing with atoms.

//Compare # of protons. Same element is decided by # of protons... Does not check if atoms are isotopes or ions.
bool operator ==(const Atom& atom1, const Atom& atom2)
{
	return atom1.getProtons() == atom2.getProtons();
}

//Check if atoms are the same using operator ==. Then, check for diff neutron counts. atoms with same protons but diff neutrons are isotopes
bool Atom::isotopeChecker(const Atom& atom1, const Atom& atom2)  
{
	if (atom1 == atom2)
	{
		return (atom1.getNeutrons() != atom2.getNeutrons());
	}
	return false;
}
    

//Member function operator++ definition, increases charge of atom and returns a reference to object (not a copy)
//Documentation: charge = protons - electrons. electrons-- increases the charge of the atom.
//https://youtu.be/6-gQ0LVcDW0?si=HGci8GrIcLaliyxH&t=36
Atom& Atom::operator ++() 
{
	if (electrons > 0) { // Avoid negative electrons
		electrons--;
	}
	return *this;
}

//Get # of electrons from class, -1 to them, return objext not copy. 
Atom operator --(Atom& atom)
{
	atom.electrons++;
	return atom;
}

// Atom member definitions.

//Definition for our new elementsTable array, listing all 118 element shorthands in rows of (mostly) 10. 
//Documentation: I used Google's Periodic table to see all the atomic numbers 
const string Atom::elementTable[118] = 
{
	"H", "He", "Li", "Be", "B", "C", "N", "O", "F", "Ne",
	"Na", "Mg", "Al", "Si", "P", "S", "Cl", "Ar", "K", "Ca",
	"Sc", "Ti", "V", "Cr", "Mn", "Fe", "Co", "Ni", "Cu", "Zn",
	"Ga", "As", "Se", "Br", "Kr", "Rb", "Sr", "Y", "Zr", "Nb", 
	"Mo", "Tc", "Ru", "Rh", "Pd", "Ag", "Cd", "In", "Sn", "Sb", 
	"Te", "I", "Xe", "Cs", "Ba", "La", "Ce", "Pr", "Nd", "Pm",
	"Sm", "Eu", "Gd", "Tb", "Dy", "Ho", "Er", "Tm", "Yb", "Lu",
	"Hf", "Ta", "W", "Re", "Os", "Ir", "Pt", "Au", "Hg", "Tl",
	"Pb", "Bi", "Po", "At", "Rn", "Fr", "Ra", "Ac", "Th", "Pa",
	"U", "Np", "Pu", "Am", "Cm", "Bk", "Cf", "Es", "Fm", "Md",
	"No", "Lr", "Rf", "Db", "Sg", "Bh", "Hs", "Mt", "Ds", "Rg",
	"Cn", "Nh", "Fl", "Mc", "Lv", "Ts", "Og"
};

// Initialize to hydrogen by using constructor delegation -> calls our constructor with parameters.
Atom::Atom()
	: Atom(1, 0, 1)
{
}

// Accessor to return atomic symbol by using elementTable, including selection to handle out of bounds input
// Documentation: My chemist friend who let me know #protons = atomic number
string Atom::getSymbol(int protons) const
{
	if (protons < 1 || protons > 118)
	{
		return "Invalid # of protons. Please select between 1 and 118.";
	}
	return elementTable[protons - 1];
}

// Initialize based on how many of each subatomic particle using initializer list 
Atom::Atom(int initProtons, int initNeutrons, int initElectrons)
	: protons(initProtons), neutrons(initNeutrons), electrons(initElectrons) 
{
}

int Atom::getProtons() const
{
	return protons;
}

int Atom::getNeutrons() const
{
	return neutrons;
}

//I made this getter for the bonus but didnt complete it
//Keeping it for consistency with other atom components
int Atom::getElectrons() const
{
	return electrons;
}

// Call this function and pass in desired number of neutrons.
void Atom::setNeutrons(int newtrons)
{
	neutrons = newtrons;
}

// Output symbol and how many of each subatomic particle
void Atom::display() const
{
	cout << getSymbol(protons) << ": "
		 << protons << "p+, "
		 << neutrons << "n, "
		 << electrons << "e- ";
}

// Return charge based on how many protons and electrons.
int Atom::charge() const
{
	return protons - electrons;
}
