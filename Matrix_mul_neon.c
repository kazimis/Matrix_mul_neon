#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <stdlib.h>
#include <arm_neon.h>

/* global variable declaration */
int16_t M_row = 0;
int16_t M_col = 0;

// both vector and matrix have column major layout in memory
// We also assume that M is 8x8 or multiples of 8
void matrix_vector_multiplication(const int8_t *M, const int16_t *V, int16_t *R){

	// Cols of a 8x8 sub matrix of M (64 bit registers) (Temporary registers)
	int8x8_t TMP0;
	int8x8_t TMP1;
	int8x8_t TMP2;
	int8x8_t TMP3;
	int8x8_t TMP4;
	int8x8_t TMP5;
	int8x8_t TMP6;
	int8x8_t TMP7;

	//Cols of a 8x8 sub matrix of M (128 bit registers)
	int16x8_t M0;
	int16x8_t M1;
	int16x8_t M2;
	int16x8_t M3;
	int16x8_t M4;
	int16x8_t M5;
	int16x8_t M6;
	int16x8_t M7;

	//column of a 8x1 sub matrix of V
	int16x8_t V0;

	//Cols of a 8x1 sub matrix of R
	int16x8_t R0;

	int M_idx;

	//8x8 loop unrolling
	for(int16_t i = 0; i < M_row; i+=8){
		// mov zeros to registers for R
		R0 = vmovq_n_s16(0);
		for (int16_t j = 0; j < M_col; j+=8) {
			M_idx = i + M_row*j;

			// Load 8 8-bit numbers from column 0, 2 ,..,7
			TMP0 = vld1_s8(M+M_idx);
			TMP1 = vld1_s8(M+M_idx+M_row);
			TMP2 = vld1_s8(M+M_idx+2*M_row);
			TMP3 = vld1_s8(M+M_idx+3*M_row);
			TMP4 = vld1_s8(M+M_idx+4*M_row);
			TMP5 = vld1_s8(M+M_idx+5*M_row);
			TMP6 = vld1_s8(M+M_idx+6*M_row);
			TMP7 = vld1_s8(M+M_idx+7*M_row);

			// mov TMPs to 8 16-bit M0, ..., M7 registers (sign extended)
			M0 = vmovl_s8(TMP0);
			M1 = vmovl_s8(TMP1);
			M2 = vmovl_s8(TMP2);
			M3 = vmovl_s8(TMP3);
			M4 = vmovl_s8(TMP4);
			M5 = vmovl_s8(TMP5);
			M6 = vmovl_s8(TMP6);
			M7 = vmovl_s8(TMP7);

			// multiply and accumulate
			V0 = vld1q_s16(V+j);
			R0 = vqrdmlahq_laneq_s16(R0, M0, V0, 0);
			R0 = vqrdmlahq_laneq_s16(R0, M1, V0, 1);
			R0 = vqrdmlahq_laneq_s16(R0, M2, V0, 2);
			R0 = vqrdmlahq_laneq_s16(R0, M3, V0, 3);
			R0 = vqrdmlahq_laneq_s16(R0, M4, V0, 4);
			R0 = vqrdmlahq_laneq_s16(R0, M5, V0, 5);
			R0 = vqrdmlahq_laneq_s16(R0, M6, V0, 6);
			R0 = vqrdmlahq_laneq_s16(R0, M7, V0, 7);

		}

		//STR R0 to register
		vst1q_s16(R+i, R0);
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
