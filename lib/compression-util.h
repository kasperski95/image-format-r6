#ifndef COMPRESSION_UTIL_H
#define COMPRESSION_UTIL_H

#include <string>

/** Encode given value with Rice's encoding.
  * Binary representation of result is converted to string consising od '0's and '1's.
  **/
std::string rice_encode(int M, int val_to_encode);

/** reversed function rice_encode(..)
 **/
int rice_decode(int M, std::string val_to_decode);



#endif //COMPRESSION_UTIL_H
