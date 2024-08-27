#Variables 

Les variables d'environnements sont des paires 'Identifiant=Valeur' héritées du système qui servent a l’exécution du shell (même si il peut fonctionner avec [[Variables minimales | un nombre minimal de ces variables]]), on les récupères grâce aux arguments dans le main `int main(int argc, char **argv, char **env)` dans un tableau de strings.

Les conventions bash pour nommer les variables:
	- Ne peuvent pas commencer par un chiffre
	- Ne peuvent contenir que des lettres majuscules ou minuscules et des underscores
	- Par convention, les variables en MAJ sont utiles à l'environnement alors que celles en minuscules sont plus souvent utilisées pour des scripts en local
	- EX: VARIABLE_NAME OK variable_name OK 1var KO

[[Variables Internes du Bash (Paramètres spéciaux)|Certaines variables d'environnement]] sont propres à Bash et masquées dans l'environnement car non considérées comme globales.