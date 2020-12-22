#include <stdio.h>
#include <stdint.h>
#include <time.h>

/* global variable declaration */
int16_t M_row = 0;
int16_t M_col = 0;


void matrix_vector_multiplication(const int8_t *M, const int16_t *V, int16_t *R){
  for (int16_t i = 0; i < M_row; i++) {
      R[i] = 0;
      for (int16_t j=0; j < M_col; j++) {
          int32_t tmp = (M[i +M_row*j] * V[j]) >> 10;
          if(tmp >0x001F){
              R[i] +=  0x7FFF;
              break;
          }else if (tmp < -0x20 ){
              R[i] +=  -0x8000;
              break;
          }else{
              R[i] +=  (M[i +M_row*j] * V[j]);
          }
      }
  }
}


int main(){



    int16_t V [] = {-5120,2,3,4,5,6,7,8};
    int8_t M [] = {10,2,3,4,5,6,7,8,
    			         1,2,3,4,5,6,7,8,
				           1,2,3,4,5,6,7,8,
				           1,2,3,4,5,6,7,8,
    			         1,2,3,4,5,6,7,8,
        		       1,2,3,4,5,6,7,8,
    			         1,2,3,4,5,6,7,8,
    			         1,2,3,4,5,6,7,8};

    M_col = sizeof(V)/sizeof(V[0]);
    M_row = sizeof (M)/M_col;

    int16_t R [M_row];

    matrix_vector_multiplication(M,V,R);

    for (int16_t i = 0; i < M_row; i++){
        printf(" %d \n", R[i]);
    }
    printf("M_row: %d M_col: %d\n", M_row, M_col);
}
