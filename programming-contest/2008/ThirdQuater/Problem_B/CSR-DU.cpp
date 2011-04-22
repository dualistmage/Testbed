#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char *argv[])
{
    // ----------------------------------- [ check execution parameters ]
    if (argc != 4) {
        cout << "USAGE : CSR <matrix_file> <x_vector_file> <y_vector_file>" << endl;
        return -1;
    } // end - if

    //ifstream fpin;
    FILE *fpin;
    FILE *fpout;

    int i, j;
    int nrow, ncol;
    int* row_ptr;
    int* col_ind;

    int row_index;
    int col_index;
    int count;
    int row,col;
    
    float num;
    float* values;
    float** sparse;

    float* x;
    float* y;
    int vectorSize;


    // ----------------------------------- [ read input files ]
    fpin = fopen(argv[2], "r");

    fscanf(fpin, "%d", &vectorSize);

    x = new float[vectorSize + 1];
    
    for(i = 0; i < vectorSize; i++)
        fscanf(fpin, " %f ", &x[i]);

    fclose(fpin);
    
    fpin = fopen(argv[1], "r");
    row_index = 0;
    col_index = 0;
    count = 0;

    fscanf(fpin, "%d %d", &nrow, &ncol);
    
    sparse = new float*[nrow];
    for(i = 0; i < nrow; i++) {
        sparse[i] = new float[ncol];
        memset(sparse[i], 0, sizeof(float)*ncol);
    }
 
    col_ind = new int[nrow*ncol];
    row_ptr = new int[nrow + 1];
    values  = new float[nrow*ncol];

   
    while( !feof(fpin))
    {
        fscanf(fpin, "%d,%d,%f", &row, &col, &num);
        sparse[row][col] = num;
    }

    // ----------------------------------- [ change sparse matrix ]


    for(i = 0; i < nrow; i++) {
        row_ptr[i] = count;
        for(j = 0; j < ncol; j++) {
            num = sparse[i][j];
            if ( num != 0 ) {
                col_ind[col_index] = j;
                values[col_index] = num;
                col_index++;
                count++;
            }
        }
    }

    row_index = nrow;
    row_ptr[row_index] = count;

    // ----------------------------------- [ calculate fomula ]

    y = new float[vectorSize + 1];
    memset(y, 0, sizeof(float)*(vectorSize+1));


    for(i = 0; i < vectorSize; i++) {
        for(j = row_ptr[i]; j < row_ptr[i + 1]; j++)
        {
            y[i] += values[j] * x[col_ind[j]];
        }
    }


    string out = argv[3];
    fpout = fopen(argv[3], "w");

    fprintf(fpout, "%d\n", vectorSize);


    for(i = 0; i < vectorSize; i++) {
        fprintf(fpout, "%f\n", y[i]);
    }
        
	return 0;

}
