#include<iostream>
#include<fstream>
#include<bitset>
#include<string>
#include<vector>
#include<numeric>



std::string xor_bitwise(std::string  & a, std::string & b )
{   
	std::string output_str{""};
	for(int i =0; i < a.length();i++ )
	{	
	std::string temp_bit_str = (std::bitset<8>(a[i]) ^ std::bitset<8>(b[i])).to_string();
	output_str += temp_bit_str;
	}
	return output_str;
}

int hamming_distance(std::string & compare_str1, std::string & compare_str2)
{ int count = 0;
    std::string temp_bin_str{xor_bitwise(compare_str1,compare_str2)};
     for(char ch : temp_bin_str)
   {
       if(ch == '1')
            count++;
   }
   return count;
}
// std::vector<std::string> b64_4char_groups(std::string & fullb64txt)
// {

// }

std::string bin_of_b64(std::string & b64_str)
{ std::string temp_str{""};
    for (int i = 0; i < b64_str.length(); i++)
    {  if(b64_str[i] == '=')
        {
            break;

        }
        else
        {
            std::string current_bit_str = std::bitset<6> (b64_str[i]).to_string();
            temp_str += current_bit_str;

        }    
    }
    return temp_str;
}

std::string bin_to_ascii(std::string & bin_str) // binary to ascii conversion
{
	std::string output_str{""};
	for(size_t i =0;i<bin_str.length();i+=8)
	{
		std::string temp_str = bin_str.substr(i,8);
		char ch = std::stoul(temp_str,nullptr,2);
		
		output_str += ch;
	}
	return output_str;
}

void avg_dist(std::vector<float>& avg_vec_of_keylengths, std::string ascii_ciphertxt, unsigned int keylength)
{
    float avg_hamm = 0.0;
    std::vector<std::string> vec_keylength_groups;
    std::vector<float>vec_norm_dist;

    for (int i=0;i < ascii_ciphertxt.length();i+=keylength)
    {
       std::string temp_str {""};
       temp_str = ascii_ciphertxt.substr(i,keylength);
       vec_keylength_groups.push_back(temp_str);
    }
    for (int i = 0; i < vec_keylength_groups.size();i++)
    {   if(i+1 == vec_keylength_groups.size())
        {
            break;
        }
      int hamm_dst{hamming_distance(vec_keylength_groups[i],vec_keylength_groups[i+1])};
      float normal = float(hamm_dst)/float(keylength);
      vec_norm_dist.push_back(normal);
      //std::cout << normal << std::endl;
      avg_hamm = float(std::accumulate(vec_norm_dist.begin(),vec_norm_dist.end(),0)) / float(vec_norm_dist.size());
      //printf("adding avg distance of keylength %d: %f", keylength, avg_hamm);
      
    }
    avg_vec_of_keylengths.push_back(avg_hamm);
}
std::string xor_multi_key(std::string & a, std::string key ) // a hhere is the input string ins ascii to xor bitwise
{
	int i = 0, k = 0;
	std::string output_str{""};
	while (i < a.size())
	{		
		std::string temp_str = (std::bitset<8>(a[i]) ^ std::bitset<8>(key[k])).to_string();
				output_str += temp_str;
		
		i++;
		k++;
		if(k > 28)
		{
			k = 0;
		}
	}
	return output_str;
}

std::string xor_key(std::string & a, int  key )
{
	
	std::string output_str{""};
	for (char ch : a)
	{
		
		std::string temp_str = (std::bitset<8>(ch) ^ std::bitset<8>(key)).to_string();
		output_str += temp_str;
	}
	return output_str;
}

int main()
{
    std::string full_b4_str{""};

    std::ifstream b64file("base 64 encrypted.txt");
	
	if(b64file.is_open())
	{
		std::string line;
		int count = 1;
		
		while(b64file >> line)
		{	//printf("line %d string is: ",count);
			//std::cout << line << "\n\n";
			full_b4_str += line;
			
			count++;
			
		}
		b64file.close();
	}
    std::cout << "number of byt3s in full base 64 string is: " << full_b4_str.size() << std::endl;
   
    std::string bin_full_b64_str{bin_of_b64(full_b4_str)};
    
    std::string ascii_fullcipher{bin_to_ascii(bin_full_b64_str)};
    
    std::vector<float> vec_avg_hamm_keylengths;
    for (unsigned int keySize = 2; keySize < 41; keySize++)
    {
    avg_dist(vec_avg_hamm_keylengths,ascii_fullcipher,keySize);
    } 
    
    int tried_KeySize = 2;
    for (int i = 0; i < vec_avg_hamm_keylengths.size(); i++)
    {       
       printf("average hamming distance with key size %d: %f\n", tried_KeySize,vec_avg_hamm_keylengths[i]);
       tried_KeySize += 1;
    }
    // std::cout << "\n\n" << vec_avg_hamm_keylengths[25] << '\n' 
    //           << vec_avg_hamm_keylengths[27] << std::endl;

    // std::string key{"TerminatortX: Bring the noise"};
    
    
    // std::string bin_xor{xor_multi_key(ascii_fullcipher,key)};
    // // std::cout << bin_xor << std::endl;    
    // std::string decrypted_str{bin_to_ascii(bin_xor)};
    // std::cout << decrypted_str << std::endl;
    std::vector<std::string> vec_key_blocks27;
    for(int i = 0; i <  27; i++ )
    {
        vec_key_blocks27.push_back(std::string(1,ascii_fullcipher[i])); 
        // this std::string(1,ascii_fullcipher[i]) converst character to a string where the 1 tells
        //the constructor  how many copies to put in the string pf ascii_fullcipher[i] 
        //which he only want the 1 character as a string
       
    }

    // std::cout <<"\n" << vec_key_blocks27[0] << " " << vec_key_blocks27.size() <<std::endl;
    int i,k;
    for( i = 27, k = 0; i < ascii_fullcipher.length();i++,k++ )
    {
        if(k == 27)
        {
            k =0;
        }
        vec_key_blocks27[k].push_back(ascii_fullcipher[i]);
    }
    // std::cout << "\n" << vec_key_blocks27[0] << std::endl;

    std::vector<std::string> vec_key_blocks29;
    for(int i = 0; i <  29; i++ )
    {
         vec_key_blocks29.push_back(std::string(1,ascii_fullcipher[i]));
       
    }

      for( i = 29, k = 0; i < ascii_fullcipher.length();i++,k++ )
    {
        if(k == 29)
        {
            k =0;
        }
        vec_key_blocks29[k].push_back(ascii_fullcipher[i]);
    }
    std::cout <<"\n\n" << vec_key_blocks29.size() << "\n" << vec_key_blocks29[0] << std::endl; 
    std::vector<std::vector<std::string>> determine_key(29);
    std::cout << determine_key.size() << std::endl;
    for(int i=0; i < 256; i++)
        {
            for (int j = 0; j < vec_key_blocks29.size(); j++)
            {
                std::string temp_bin {xor_key(vec_key_blocks29[j],i)};
                std::string temp_ascii{bin_to_ascii(temp_bin)};
                determine_key[j].push_back(temp_ascii); 
            }
        }
        std::cout << "******************** determin key of the blocks **********************************" << "\n\n";
    // std::cout << determine_key[0][1] << std::endl;
    for (unsigned int i=0; i< 29; i++) 
    {
        for(unsigned int j = 0; j < 256; j++)
        {
            printf("Block message in block %d with key %d: ",i,j);
            std::cout << determine_key[i][j] << "\n\n";
        }
    }
    return 0;
}
