package lab01;

import java.util.Random;

/**
 * This class demonstrates array sorting and console output.
 * 
 * @author Sapronov Kirill 3311
 */
public class test {
	/**
     * Main method. Generates array, performs sorting, and displays results.
     *
     * @param args Command line arguments.
     */
    public static void main(String[] args) {
    	int[] array = new int[10];
        Random random = new Random();
        for (int i = 0; i < array.length; i++) {
            array[i] = random.nextInt(100);
        }
        System.out.println("Original array:");
        printArray(array);
        sortArray(array);
        System.out.println("Sorted array:");
        printArray(array);
    }
    /**
     * Method to sort the array using bubble sort.
     *
     * @param arr Array to be sorted.
     */
    private static void sortArray(int[] arr) {
        int n = arr.length;
        for (int i = 0; i < n - 1; i++) {
            for (int j = 0; j < n - i - 1; j++) {
                if (arr[j] > arr[j + 1]) {
                    int temp = arr[j];
                    arr[j] = arr[j + 1];
                    arr[j + 1] = temp;
                }
            }
        }
    }
    /**
     * Method for array output.
     *
     * @param arr Array to be printed.
     */
    private static void printArray(int[] arr) {
        for (int i : arr) {
            System.out.print(i + " ");
        }
        System.out.println();
    }
}
