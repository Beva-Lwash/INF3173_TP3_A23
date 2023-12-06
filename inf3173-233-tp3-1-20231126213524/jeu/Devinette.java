import java.util.Scanner;
import java.util.Random;

public class Devinette {
    public static void main(String[] args) {
        // On doit pouvoir définir un seed pour que le jeu soit déterministe
        long seed = 12345L;
	if (args.length > 0) {
            seed = Long.parseLong(args[0]);
	}
        Random random = new Random(seed);
        int numberToGuess = random.nextInt(101);
        Scanner scanner = new Scanner(System.in);
        int numberOfTries = 0;
        boolean hasGuessedCorrectly = false;

        System.out.println("Deviner un nombre entre 0 et 100:");

        while (!hasGuessedCorrectly) {
            int userGuess = scanner.nextInt();
            numberOfTries++;

            if (userGuess < 0 || userGuess > 100) {
                System.out.println("Erreur: saisir un nombre entre 0 et 100");
            } else if (userGuess < numberToGuess) {
                System.out.println("Trop bas essaie encore:");
            } else if (userGuess > numberToGuess) {
                System.out.println("Trop haut essaie encore:");
            } else {
                hasGuessedCorrectly = true;
                System.out.println("Gagné! Tu as trouvé en  " + numberOfTries + " essais.");
            }
        }

        scanner.close();
    }
}

