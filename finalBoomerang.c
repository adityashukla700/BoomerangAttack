#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>


uint8_t sbox[]={99, 124, 119, 123, 242, 107, 111, 197,  48,   1, 103,  43, 254,
       215, 171, 118, 202, 130, 201, 125, 250,  89,  71, 240, 173, 212,
       162, 175, 156, 164, 114, 192, 183, 253, 147,  38,  54,  63, 247,
       204,  52, 165, 229, 241, 113, 216,  49,  21,   4, 199,  35, 195,
        24, 150,   5, 154,   7,  18, 128, 226, 235,  39, 178, 117,   9,
       131,  44,  26,  27, 110,  90, 160,  82,  59, 214, 179,  41, 227,
        47, 132,  83, 209,   0, 237,  32, 252, 177,  91, 106, 203, 190,
        57,  74,  76,  88, 207, 208, 239, 170, 251,  67,  77,  51, 133,
        69, 249,   2, 127,  80,  60, 159, 168,  81, 163,  64, 143, 146,
       157,  56, 245, 188, 182, 218,  33,  16, 255, 243, 210, 205,  12,
        19, 236,  95, 151,  68,  23, 196, 167, 126,  61, 100,  93,  25,
       115,  96, 129,  79, 220,  34,  42, 144, 136,  70, 238, 184,  20,
       222,  94,  11, 219, 224,  50,  58,  10,  73,   6,  36,  92, 194,
       211, 172,  98, 145, 149, 228, 121, 231, 200,  55, 109, 141, 213,
        78, 169, 108,  86, 244, 234, 101, 122, 174,   8, 186, 120,  37,
        46,  28, 166, 180, 198, 232, 221, 116,  31,  75, 189, 139, 138,
       112,  62, 181, 102,  72,   3, 246,  14,  97,  53,  87, 185, 134,
       193,  29, 158, 225, 248, 152,  17, 105, 217, 142, 148, 155,  30,
       135, 233, 206,  85,  40, 223, 140, 161, 137,  13, 191, 230,  66,
       104,  65, 153,  45,  15, 176,  84, 187,  22};


uint8_t sbox_inv[]={ 82, 9, 106, 213,  48,  54, 165,  56, 191,  64, 163, 158, 129,
       243, 215, 251, 124, 227,  57, 130, 155,  47, 255, 135,  52, 142,
        67,  68, 196, 222, 233, 203,  84, 123, 148,  50, 166, 194,  35,
        61, 238,  76, 149,  11,  66, 250, 195,  78,   8,  46, 161, 102,
        40, 217,  36, 178, 118,  91, 162,  73, 109, 139, 209,  37, 114,
       248, 246, 100, 134, 104, 152,  22, 212, 164,  92, 204,  93, 101,
       182, 146, 108, 112,  72,  80, 253, 237, 185, 218,  94,  21,  70,
        87, 167, 141, 157, 132, 144, 216, 171,   0, 140, 188, 211,  10,
       247, 228,  88,   5, 184, 179,  69,   6, 208,  44,  30, 143, 202,
        63,  15,   2, 193, 175, 189,   3,   1,  19, 138, 107,  58, 145,
        17,  65,  79, 103, 220, 234, 151, 242, 207, 206, 240, 180, 230,
       115, 150, 172, 116,  34, 231, 173,  53, 133, 226, 249,  55, 232,
        28, 117, 223, 110,  71, 241,  26, 113,  29,  41, 197, 137, 111,
       183,  98,  14, 170,  24, 190,  27, 252,  86,  62,  75, 198, 210,
       121,  32, 154, 219, 192, 254, 120, 205,  90, 244,  31, 221, 168,
        51, 136,   7, 199,  49, 177,  18,  16,  89,  39, 128, 236,  95,
        96,  81, 127, 169,  25, 181,  74,  13,  45, 229, 122, 159, 147,
       201, 156, 239, 160, 224,  59,  77, 174,  42, 245, 176, 200, 235,
       187,  60, 131,  83, 153,  97,  23,  43,   4, 126, 186, 119, 214,
        38, 225, 105,  20,  99,  85,  33,  12, 125};


void rowRotation(uint8_t *l,int n){
    for(int i=0;i<n;i++){
        uint8_t a=l[0];
        for(int j=0;j<3;j++){
            l[j]=l[j+1];
        }
        l[3]=a;
    }
}
void shiftRows(uint8_t state_array[4][4]) {
    for (int i = 1; i < 4; i++) {
        rowRotation(state_array[i], i);
    }
}


void rowRotationRight(uint8_t l[], int n) {
    for (int i = 0; i < n; i++) {
        uint8_t a = l[3];
        for (int j = 3; j > 0; j--) {
            l[j] = l[j - 1];
        }
        l[0] = a;
    }
}

void invShiftRows(uint8_t state_array[][4]) {
    for (int i = 1; i < 4; i++) {
        rowRotationRight(state_array[i], i);
    }
}


uint8_t xtime(uint8_t x) {
    return (x << 1) ^ (((x >> 7) & 0x01) * 0x1b);
}

uint8_t gmult(uint8_t x, uint8_t y) {
    uint8_t result = 0;
    for (int i = 0; i < 8; i++) {
        if (y & 0x01) {
            result ^= x;
        }
        x = xtime(x);
        y >>= 1;
    }
    return result;
}

void mixColumn(uint8_t state[4][4]) {
    for (int col = 0; col < 4; col++) {
        uint8_t a = state[0][col];
        uint8_t b = state[1][col];
        uint8_t c = state[2][col];
        uint8_t d = state[3][col];

        state[0][col] = gmult(a, 2) ^ gmult(b, 3) ^ c ^ d;
        state[1][col] = a ^ gmult(b, 2) ^ gmult(c, 3) ^ d;
        state[2][col] = a ^ b ^ gmult(c, 2) ^ gmult(d, 3);
        state[3][col] = gmult(a, 3) ^ b ^ c ^ gmult(d, 2);
    }
}

void invMixColumn(uint8_t state[4][4]){
    for (int col = 0; col < 4; col++) {
        uint8_t a = state[0][col];
        uint8_t b = state[1][col];
        uint8_t c = state[2][col];
        uint8_t d = state[3][col];

        state[0][col] = gmult(a, 14) ^ gmult(b, 11) ^ gmult(c,13) ^ gmult(d,9);
        state[1][col] = gmult(a,9) ^ gmult(b, 14) ^ gmult(c, 11) ^ gmult(d,13);
        state[2][col] = gmult(a,13) ^ gmult(b,9) ^ gmult(c, 14) ^ gmult(d, 11);
        state[3][col] = gmult(a, 11) ^ gmult(b,13) ^ gmult(c,9) ^ gmult(d, 14);
    }
}

void addRoundKey(uint8_t state_array[4][4], uint8_t key[4][4]) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            state_array[i][j] ^= key[i][j];
        }
    }
}

void substituteBytes(uint8_t state_array[4][4], uint8_t sbox[]) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            state_array[i][j] = sbox[state_array[i][j]];
        }
    }
}

void invSubstituteBytes(uint8_t state_array[4][4],uint8_t sbox_inv[]){
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            state_array[i][j] = sbox_inv[state_array[i][j]];
        }
    }
}

uint8_t rcon[11] = {
    0x00, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1B, 0x36
};
void rotateLeft(uint8_t word[4]) {
    uint8_t temp = word[0];
    for (int i = 0; i < 3; i++) {
        word[i] = word[i + 1];
    }
    word[3] = temp;
}

// Key expansion function
void expandKey(uint8_t key[4][4], uint8_t keys[11][4][4]) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            keys[0][i][j] = key[i][j];  
        }
    }

    for (int round = 1; round <= 10; round++) {
        
        uint8_t temp[4];
        for (int i = 0; i < 4; i++) {
            temp[i] = keys[round - 1][i][3];
        }

        // Perform the rotate left operation
        rotateLeft(temp);

        // Perform the SubBytes transformation
        for (int i = 0; i < 4; i++) {
            temp[i] = sbox[temp[i]];
        }

        // XOR with the round constant
        temp[0] ^= rcon[round];

        // Generate the new key
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                if (j == 0) {
                    keys[round][i][j] = keys[round - 1][i][j] ^ temp[i];
                } else {
                    keys[round][i][j] = keys[round][i][j - 1] ^ keys[round - 1][i][j];
                }
            }
        }
    }
}



int main(){

    clock_t start=clock();
    time_t epoch=time(NULL);
    srand(time(&epoch));
    
    int alpha=5;
    int beta=8;
    printf("beta: %d\n",beta);
    // uint8_t beta1[4][4]={0};
    // beta1[0][0]=beta;
    // mixColumn(beta1);
    // printf("\nbeta to be added: \n");
    // for(int i=0;i<4;i++){
    //     for(int j=0;j<4;j++){
    //         printf("%d ",beta1[i][j]);
    //     }
    //     printf("\n");
    // }

    // delta addition
    int gamma=127;
    printf("gamma: %d\n",gamma);
    int deltaS=177;
    printf("delta: %d\n",deltaS);
    uint8_t delta[4][4]={0};
    delta[0][0]=deltaS;
    mixColumn(delta);
    printf("\ndelta to be added: \n");
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            printf("%d ",delta[i][j]);
        }
        printf("\n");
    }


// Round function
    int satisfied=0;
    int count=0;
    for(int i=0;i<(1<<24);i++){
        uint8_t state1[4][4]={0};
        uint8_t state2[4][4]={0};
        for(int i=0;i<4;i++){
            for(int j=0;j<4;j++){
                state1[i][j]=rand()%256;
                state2[i][j]=state1[i][j];
            }
        }
        state2[0][0]=state2[0][0]^alpha;

        // keys
        uint8_t key[4][4] = {0};
        for(int i=0;i<4;i++){
            for(int j=0;j<4;j++){
                key[i][j]=rand()%256;
            }
        }
        uint8_t keys[11][4][4];

        expandKey(key, keys);


        // first round aes
        substituteBytes(state1,sbox);
        shiftRows(state1);
        mixColumn(state1);
        addRoundKey(state1,keys[0]);


        substituteBytes(state2,sbox);
        shiftRows(state2);
        mixColumn(state2);
        addRoundKey(state2,keys[0]);

        // second round aes

        substituteBytes(state1,sbox);
        shiftRows(state1);
        mixColumn(state1);
        addRoundKey(state1,keys[1]);


        substituteBytes(state2,sbox);
        shiftRows(state2);
        mixColumn(state2);
        addRoundKey(state2,keys[1]);

        // delta addition
        for(int i=0;i<4;i++){
            for(int j=0;j<4;j++){
                state1[i][j]=state1[i][j]^delta[i][j];
                state2[i][j]=state2[i][j]^delta[i][j];
            }
        }

        // inverse aes second

        addRoundKey(state1,keys[1]);
        invMixColumn(state1);
        invShiftRows(state1);
        invSubstituteBytes(state1,sbox_inv);

        addRoundKey(state2,keys[1]);
        invMixColumn(state2);
        invShiftRows(state2);
        invSubstituteBytes(state2,sbox_inv);

        // inverse aes first

        addRoundKey(state1,keys[0]);
        invMixColumn(state1);
        invShiftRows(state1);
        invSubstituteBytes(state1,sbox_inv);

        addRoundKey(state2,keys[0]);
        invMixColumn(state2);
        invShiftRows(state2);
        invSubstituteBytes(state2,sbox_inv);

        // checking

        int flag=0;
        for(int i=0;i<4;i++){
            for(int j=0;j<4;j++){
                if(i!=0 || j!=0){
                    int intermediate=state1[i][j]^state2[i][j];
                    if(intermediate!=0) {
                        flag=1;
                        break;
                    }
                }
            }
        }
        
        if(flag==0){
            count++;
            int xor=state1[0][0]^state2[0][0];
            // if(xor<10)
            //     printf("xor: %d\n",xor);
            if(xor==alpha){
                satisfied++;
            }
        }

    }
    
    printf("satisfied: %d\n",satisfied);
    printf("count: %d\n",count);
    
    clock_t end=clock();
    double time=(double)(end-start)/CLOCKS_PER_SEC;
    printf("Execution time is: %f seconds",time);
    return 0;

}