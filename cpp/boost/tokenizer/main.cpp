///
/// @brief test code of boost::unordered_map
/// @author Dohyun Yun ( dualistmage@gmail.com )
/// @date 2010.06.21
///

#include <iostream>
#include <boost/tokenizer.hpp>

int main(int argc, char* argv[])
{
    { // Use tokenizer with delimiter.
        using namespace std;
        std::string inputStr = "To construct a language model, we let x = (x1 , . . . , xk ) be a vector of latent random variables representing the thematic content of a document.";

        typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
        const string WORD_DELIMITER_STRING = " `~!@#$%^&*()_-+={}[]\\|\"':;?/>.<,\n\r";
        boost::char_separator<char> sep( WORD_DELIMITER_STRING.c_str() );
        tokenizer tokens( inputStr, sep );
        cout << "Input : \"" << inputStr << "\"" << endl;
        cout << "------------------------------------------------------------------------" << endl;
        for(tokenizer::iterator tok_iter = tokens.begin(); tok_iter != tokens.end(); tok_iter++)
            cout << "Token : " << *tok_iter << endl;
    } 
}
