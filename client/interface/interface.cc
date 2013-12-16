#include "interface.h"
#include <stdlib.h>

Interface::Interface()
{
  client = new Client();
}

Interface::~Interface()
{
  delete client;
}

void Interface::run()
{
  cout << "\t~~~~~~~~~ FreeShare ~~~~~~~~~" << endl;
  cout << "\t    by Roknus & Bliinky" << endl;
  cout << "\t         Version 1.0" << endl;

  int selectionMenu = 0;

  while(selectionMenu != 99)
    {
      cout << endl;
      cout << "\t 1 - Se connecter au serveur" << endl;
      cout << "\t 2 - Se déconnecter du serveur" << endl;
      cout << "\t 3 - Lancer le port d'écoute" << endl;
      cout << "\t 4 - Fermer le port d'écoute" << endl;
      cout << "\t 5 - Envoyer un fichier" << endl;
      cout << "\t 6 - Recuperer un fichier" << endl;
      cout << "\t 7 - Rafraichier liste des clients" << endl;
      cout << "\t 99 - Quitter" << endl;

      cin >> selectionMenu;
      
      switch(selectionMenu)
	{
	case 1:
	  connexionServeur();
	  break;
	case 2:
	  deconnexionServeur();
	  break;
	case 3:
	  lancerPortEcoute();
	  break;
	case 4:
	  fermeturePortEcoute();
	  break;
	case 5 :
	  cout << "Chemin du fichier ?" << endl;
	  char nomFichier[255];
	  cin >> nomFichier;
	  envoyerFichier(nomFichier);
	  break;
	case 6:
	  cout << "Nom du fichier ?" << endl;
	  char nomFichierRecup[255];
	  cin >> nomFichierRecup;
	  recupererFichier(nomFichierRecup);
	  break;
	case 7:
	  rafraichirClient();
	  break;
	case 99 :
	  cout << "A bientot :)" << endl;
	  break;
	default:
	  cout << "Entrée incorrecte !" << endl;
	  break;
	}  
    }
}

void Interface::lancerPortEcoute()
{
  client->lancerPortEcoute();
}

void Interface::fermeturePortEcoute()
{
  client->fermeturePortEcoute();
}

void Interface::connexionServeur()
{
  client->connexionServeur();
}

void Interface::deconnexionServeur()
{
  client->deconnexionServeur();
}

void Interface::rafraichirClient()
{
  client->rafraichirClient();
}

void Interface::envoyerFichier(char* nomFichier)
{
  client->envoyerFichier(nomFichier);
}

void Interface::recupererFichier(char* nomFichier)
{
  client->recupererFichier(nomFichier);
}
