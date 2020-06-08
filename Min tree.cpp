struct MINIMA{

    fixed_point_t F_MIN;
    fixed_point_t S_MIN;

};


void RCNMS_V2_fixed(fixed_point_t L_q[][row_length_origin], fixed_point_t L_r[][bit_number], int B[][30], fixed_point_t L_Q[], fixed_point_t L_c[]){

    int l, m, n, t, k;
    float part_pdt = 1;
    float min_num;
    int flag;
    int start_index, end_index;

    fixed_point_t L_q_appx[bit_number][row_length_origin];
    int i,j;
    int min_idx;
    float true_min;

    fixed_point_t sign, temp;
    fixed_point_t L_q_appx[bit_number][row_length_origin];

    vector<fixed_point_t> Candidates;
    vector<int> IDX;
    fixed_point_t fmin, smin;
    MINIMA minimums;



    for(l=0; l<Layer_length; l++){
        start_index = l * M_l;
        end_index = (l+1) * M_l;

        for(m=start_index; m<end_index; m++){ 
            for(t=0; t<30; t++){
                if(B[m][t] == -1)
                    break;
                else
                    L_q[B[m][t]][m] = L_Q[B[m][t]] - L_r[m][B[m][t]];
            }
        }// VN Processing
        
        for(m=start_index; m<end_index; m++){

            t=0;
            while(B[m][t] != -1){
                Candidates.push_back(abs(L_q[B[m][t]][m]));
                t++;
            }

            minimums = RC_Min_tree(Candidates, IDX);

            smin = minimums.S_MIN;

            for(i=0; i<IN.size(); i++)
                if(IDX[i] == 1) fmin = IN[i]; //finding true first min with idx generated from RC mintree == MUXing

            t=0;
            while(B[m][t] != -1){
                if(IDX[t] == 0)
                    L_r[m][B[m][t]] = mulfix(fmin, normalize_factor);
                else
                    L_r[m][B[m][t]] = mulfix(smin, normalize_factor);
                t++;
            } 
        }// CN Processing --- simulation 해봐야함

        
        for(m=start_index; m<end_index; m++){
            for(t=0; t<30; t++){
                if(B[m][t] == -1)
                    break;
                else
                    L_Q[B[m][t]] = L_q[B[m][t]][m] + L_r[m][B[m][t]];
            }
        }// AP update
    }
}

MINIMA RC_Min_tree(vector<fixed_point_t> IN, vector<int> IDX){

    MINIMA minimums;
    fixed_point_t F_MIN, S_MIN;
    vector<fixed_point_t> temp(IN);

    int size = IN.size()

    for(int i=0; i<size; i++){
        temp[i] << F_bit_2;
        temp[i] >> (F_bit_2 + B_bit_2);
    }

    vector<fixed_point_t> temp_idx(temp);
    sort(temp.begin(), temp.end());
    minimums.F_MIN = temp[0] << B_bit_2;
    minimums.S_MIN = temp[1] << B_bit_2;

    for(int i=0; i<size; i++){
        if(temp_idx[i] != minimums.F_MIN) IDX.push_back(0);
        else IDX.push_back(1);
    }

    return minimums;
}

















