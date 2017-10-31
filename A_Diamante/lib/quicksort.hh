#include <vector>

using namespace std;
/*
 * Copyright (C) 2009 Ronny Yabar Aizcorbe <ronnycontacto@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify it
 * unright the terms and conditions of the GNU Lesser General Public License,
 * version 2.1, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
 */

// Función para dividir el array y hacer los intercambios
int divide(vector<int> &array,vector<int> &cols,vector<int> &nodes, int start, int end) {
    int left;
    int right;
    int pivot;
    int temp;
    int tempCols;
    int tempweidth;


    pivot = array[start];
    left = start;
    right = end;

    // Mientras no se cruzen los índices
    while (left < right) {
        while (array[right] > pivot) {
            right--;
        }

        while ((left < right) && (array[left] <= pivot)) {
            left++;
        }

        // Si todavía no se cruzan los indices seguimos intercambiando
        if (left < right) {

            temp = array[left];
	        tempCols = cols[left];
	        tempweidth = nodes[left];

            array[left] = array[right];
            array[right] = temp;

	        cols[left] = cols[right];
            cols[right] = tempCols;

     	    nodes[left] = nodes[right];
            nodes[right] = tempweidth;

	
        }
    }

    // Los índices ya se han cruzado, ponemos el pivot en el lugar que le corresponde
    temp = array[right];
    tempCols = cols[right];
    tempweidth = nodes[right];

    array[right] = array[start];
    array[start] = temp;
    
	
    cols[right] = cols[start];
    cols[start] = tempCols;

    nodes[right] = nodes[start];
    nodes[start] = tempweidth;


    // La nueva posición del pivot
    return right;
}

// Función recursiva para hacer el ordenamiento
void quicksort(vector<int> &array,vector<int> &cols,vector<int> &nodes, int start, int end)
{
    int pivot;

    if (start < end) {
        pivot = divide(array,cols,nodes, start, end);

        // Ordeno la lista de los menores
        quicksort(array,cols,nodes, start, pivot - 1);

        // Ordeno la lista de los mayores
        quicksort(array, cols,nodes,pivot + 1, end);
    }
}

