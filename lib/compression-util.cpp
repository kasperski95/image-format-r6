#include <cmath>
#include "compression-util.h"


std::string rice_encode(int M, int val_to_encode){

    //val_to_encode = left*M + right
    int left = val_to_encode / M;
    int right = val_to_encode % M;

    // unary representation of left part
    std::string left_str = "0";
    for(int i=0; i<left; i++)
        left_str = "1" + left_str;

    // binary represenation of right
    std::string right_str = "";
    while(right > 0){
        if( right % 2 > 0 )
            right_str = "1" + right_str;
        else
            right_str = "0" + right_str;

        right /= 2;
    }

    while(right_str.length() < log2(M))
        right_str = "0" + right_str;

    std::string result = left_str + right_str;

    return result;
}

int rice_decode(int M, std::string val_to_decode){
    std::string left_str =
            val_to_decode.substr( 0, val_to_decode.find('0') );

    std::string right_str =
            val_to_decode.substr( val_to_decode.find('0')+1 );

    int left = left_str.length();

    int right = 0;
    for(int i = 0; i<right_str.length(); i++){
        right = right +
                ( right_str[ right_str.length()-i-1 ] - '0' ) * pow(2,i);
    }

    int result = left*M + right;

    return result;
}
